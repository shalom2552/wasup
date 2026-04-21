#include <stdio.h>
#define _POSIX_C_SOURCE 200112L
#include "chat/client.h"
#include "chat/utils.h"
#include "chat/tcp.h"
#include "chat/constants.h"
#include "chat/chat_utils.h"

#include <sys/socket.h>		// socket(), bind(), listen(), accept(), setsockopt()
#include <netdb.h>			// getaddrinfo(), struct addrinfo
#include <string.h>			// memset()
#include <stdbool.h>		// bool
#include <unistd.h>			// close()
#include <poll.h>			// poll(), POLLIN, POLLHUP, POLLERR
#include <errno.h>			// errno

// TODO: Do we need Client struct? (used outside of setup?)
static struct {
	int fd;
	char name[CHAT_USER_NAME_SIZE];
} Client;

int chat_client_setup(const char* ip, const char* port)
{
	// connect
	Client.fd = chat_tcp_connect(ip, port);
	if (Client.fd < 0) {
		log_error("Error(setup): Could not set up client conection.");
		return -1;
	}

	// get user info
	char room[CHAT_ROOM_SIZE];
	if (chat_get_input_username(Client.name, sizeof(Client.name)) &&
		chat_get_input_room(room, sizeof(room))) {
		log_error("Could not get client info. Exiting.");
		close(Client.fd);
		return -1;
	}

	// handshake
	char handshake[CHAT_USER_NAME_SIZE + CHAT_ROOM_SIZE];
	snprintf(handshake, sizeof(handshake), "%s:%s", Client.name, room);
	if (chat_send_all(Client.fd, handshake, strlen(handshake))) {
		close(Client.fd);
		return -1;
	}
	return Client.fd;
}

void chat_run_client(int server_fd)
{
	char buffer[CHAT_MSG_BUFFER_SIZE];

    struct pollfd pfds[2];
    pfds[0].fd = STDIN_FILENO;
    pfds[0].events = POLLIN;
    pfds[1].fd = server_fd;
    pfds[1].events = POLLIN;

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
                log_info("Server disconnected.");
                break;
            }
			printf(ANSI_CLEAR_LINE);
            print_chat_message(buffer);
            print_chat_prompt(Client.name);
        }

        // send message
        if (pfds[0].revents & POLLIN) {
            if (chat_get_input_message(buffer)) {
				break;
			}
            if (chat_trap_exit_message(buffer)) {
				break;
			}
            if (chat_send_all(server_fd, buffer, strlen(buffer)) < 0) {
				break;
			}
        }
    }

    log_info("Chat ended.");
}

