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

int chat_client_setup(const char* ip, const char* port)
{
	return chat_tcp_connect(ip, port);
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
            print_chat_message("", buffer);
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

    print_chat_bottom_box();
    log_info("Chat ended.");
}

