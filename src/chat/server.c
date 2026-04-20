#include "chat/server.h"
#include "chat/utils.h"
#include "chat/chat_utils.h"
#include "chat/constants.h"
#include "chat/tcp.h"

#include <sys/socket.h>		// listen()
#include <unistd.h>			// close()

int chat_server_setup(const char* port)
{
	int sockfd = chat_tcp_bind(port);
	if (sockfd == -1) {
		return -1;
	}

	// listen
	if (listen(sockfd, 1) == -1) {
		log_error("Error(listen): Could not listen on socket.");
		close(sockfd);
		return -1;
	}

	return sockfd;
}

void chat_run_server(int client_fd)
{
	char buffer[CHAT_MSG_BUFFER_SIZE];

	// TODO: make this a list of struct clients
	// get username
	char username[CHAT_USER_NAME_SIZE];
	char peername[CHAT_USER_NAME_SIZE];
	if (get_user_name(username, sizeof(username))) {
		return;
	}
	exchange_user_names(client_fd, username, peername);

	// start chat
	log_info("Chat started!");
	print_chat_top_box();
	while (1) {

		// get input message
		if (chat_get_input_message(buffer)) {
			break;
		}

		// trap exit
		if (chat_trap_exit_message(buffer)) {
			break;
		}

		// send message
		if (chat_send_all(client_fd, buffer)) {
			break;
		}

		// recive message
		chat_recv_all(client_fd, buffer);
		chat_broadcast_message(peername, buffer);
		print_chat_message(peername, buffer);
	}

	print_chat_bottom_box();
	log_info("Chat ended.");
}

