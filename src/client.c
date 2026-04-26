#define _POSIX_C_SOURCE 200112L
#include "client.h"
#include "ui.h"
#include "log.h"
#include "tcp.h"
#include "constants.h"
#include "chat_utils.h"

#include <stdio.h>
#include <sys/socket.h>		// socket(), bind(), listen(), accept(), setsockopt()
#include <netdb.h>			// getaddrinfo(), struct addrinfo
#include <string.h>			// memset()
#include <stdbool.h>		// bool
#include <unistd.h>			// close()
#include <poll.h>			// poll(), POLLIN, POLLHUP, POLLERR
#include <errno.h>			// errno

static struct {
	int fd;
	char name[CHAT_USER_NAME_SIZE];
} Client;

int chat_client_setup(const char* ip, const char* port)
{
	// get user info
	char room[CHAT_ROOM_SIZE];
	if (chat_get_input_username(Client.name, sizeof(Client.name)) ||
        chat_get_input_room(room, sizeof(room)))
    {
		log_error("Could not get client info. Exiting.");
		return -1;
	}

	// connect
	Client.fd = chat_tcp_connect(ip, port);
	if (Client.fd < 0) {
		log_error("Error(setup): Could not set up conection.");
		return -1;
	}

	// handshake
	char handshake[CHAT_USER_NAME_SIZE + CHAT_ROOM_SIZE + 1];
	snprintf(handshake, sizeof(handshake), "%s:%s", Client.name, room);
	if (chat_send_all(Client.fd, handshake, strlen(handshake))) {
		close(Client.fd);
		return -1;
	}
	return Client.fd;
}

int handle_server_notification(char* buffer)
{
    // TODO: genrralize server notifications
    char* NOTIFY_PREFIX = "SERVER_NOTIFY:";
    int code = -1;
    int count = -1;

    if (strncmp(buffer, NOTIFY_PREFIX, strlen(NOTIFY_PREFIX)) == 0) {
        char* notification = buffer + strlen(NOTIFY_PREFIX);
        if (sscanf(notification, "%d:%d", &code, &count) == 2) {
            print_room_count(count);
        }
        return 1;
    }

    return 0; // not a server notification
}

void chat_run_client(int server_fd)
{
	char buffer[CHAT_MSG_BUFFER_SIZE];

    struct pollfd pfds[2];
    pfds[0].fd = STDIN_FILENO;
    pfds[0].events = POLLIN;
    pfds[1].fd = server_fd;
    pfds[1].events = POLLIN;

	print_chat_top_box();
    print_chat_message_prompt(Client.name);

    while (1) {
        int ready = poll(pfds, 2, -1);
        if (ready < 0) {
            if (errno == EINTR) {
				continue;
			}
            log_error("Error(poll): failed.");
            break;
        }

        // recive message
        if (pfds[1].revents & (POLLIN | POLLHUP | POLLERR)) {
            int n = chat_recv_all(server_fd, buffer, sizeof(buffer));
            if (n <= 0) {
				printf("\n");
                log_error("Server disconnected.");
                break;
            }
            // TODO: handle server notification
            if ( !handle_server_notification(buffer) ) {
                printf(ANSI_CLEAR_LINE);
                print_chat_message(buffer);
                print_chat_message_prompt(Client.name);
            }
        }

        // send message
        if (pfds[0].revents & POLLIN) {
            if (chat_get_input_message(buffer)) {
				break;
			}
            if (strlen(buffer) == 0) {
				print_chat_message_prompt(Client.name);
                continue;  // empty message - dont send
            }
            if (chat_trap_exit_message(buffer)) {
				break;
			}
            if (chat_send_all(server_fd, buffer, strlen(buffer)) < 0) {
				break;
			}
			print_chat_message_prompt(Client.name);
        }
    }

    print_chat_bottom_box();
    chat_disconnect(server_fd);
    log_info("Chat ended.");
}
