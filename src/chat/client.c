#define _POSIX_C_SOURCE 200112L
#include "chat/client.h"
#include "chat/utils.h"
#include "chat/constants.h"
#include "chat/chat_utils.h"

#include <sys/socket.h>		// socket(), bind(), listen(), accept(), setsockopt()
#include <netdb.h>			// getaddrinfo(), struct addrinfo
#include <string.h>			// memset()
#include <stdbool.h>		// bool
#include <unistd.h>			// close()
#include <stdio.h>			// fgets(), stdin

int chat_client_setup(const char* ip, const char* port)
{
	struct addrinfo hints;
	struct addrinfo* res;

	if (port == NULL) {
		port = CHAT_DEFAULT_PORT;
	}

	// define hints
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;			// IPv4
	hints.ai_socktype = SOCK_STREAM;	// TCP

	// get address info useing the ip
	if (getaddrinfo(ip, port, &hints, &res)) {
		log_error("Error(getaddrinfo): Could not get address info.");
		return -1;
	}

	// create socket
	int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd == -1) {
		log_error("Error(socket): Could not create socket.");
		freeaddrinfo(res);
		return -1;
	}

	// connect to the server socket
	if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
		log_error("Error(connect): Could not connect to server.");
		close(sockfd);
		freeaddrinfo(res);
		return -1;
	}
	freeaddrinfo(res);
	return sockfd;
}

void chat_run_client(int server_fd)
{
	char username[CHAT_USER_NAME_SIZE];
	char peername[CHAT_USER_NAME_SIZE];
	char buffer[CHAT_MSG_BUFFER_SIZE];

	// TODO: outiside ofthis
	// get username
	if (get_user_name(username, sizeof(username))) {
		log_error("Failed to get username. exiting.");
		return;
	}
	exchange_user_names(server_fd, username, peername);

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
		if (chat_send_all(server_fd, buffer)) {
			break;
		}

		// recive message
		chat_recv_all(server_fd, buffer);
		chat_broadcast_message(peername, buffer);
		print_chat_message(peername, buffer);
	}

	print_chat_bottom_box();
	log_info("Chat ended.");
}

