#define _POSIX_C_SOURCE 200112L
#include "chat/client.h"
#include "chat/utils.h"
#include "chat/constants.h"

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
		port = DEFAULT_PORT;
	}

	// define hints
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;			// IPv4
	hints.ai_socktype = SOCK_STREAM;	// TCP

	// get address info useing the ip
	if (getaddrinfo(ip, port, &hints, &res)) {
		print_error("Error(getaddrinfo): Could not get address info.");
		return -1;
	}

	// create socket
	int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd == -1) {
		print_error("Error(socket): Could not create socket.");
		freeaddrinfo(res);
		return -1;
	}

	// connect to the server socket
	if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
		print_error("Error(connect): Could not connect to server.");
		close(sockfd);
		freeaddrinfo(res);
		return -1;
	}
	freeaddrinfo(res);
	return sockfd;
}

void chat_run_client(int server_fd)
{
	char username[UNAME_SIZE];
	char peername[UNAME_SIZE];
	char buffer[2048];

	// get username
	if ( !get_user_name(username, sizeof(username)) ) {
		print_error("Failed to get username. exiting.");
		return;
	}
	exchange_user_names(server_fd, username, peername);

	// start chat
	print_info("Chat started!");
	print_chat_top_box();
	while (1) {
		// get input message
		do {
			print_chat_prompt(username);
			if ( !fgets(buffer, sizeof(buffer), stdin) ) {
				print_error("Error(stdin): System error while reading from stdin.");
				break;
			}
			buffer[strcspn(buffer, "\n")] = 0; // remove new line
		} while (strlen(buffer) == 0);

		// trap client exit message
		if (chat_exit_messege(buffer)) {
			break;
		}

		// send message
		if (send(server_fd, buffer, strlen(buffer), 0) == -1) {
			print_error("Error(send): Failed to send message.");
			break;
		}

		// wait for new message
		int bytes = recv(server_fd, buffer, sizeof(buffer) - 1, 0);
		if (bytes <= 0) {
			print_info("Server disconnected.");
			break;
		}

		// terminate new line
		buffer[bytes] = '\0';
		print_chat_message(peername, buffer);

		// trap server exit message
		if (chat_exit_messege(buffer)) {
			break;
		}
	}
	print_chat_bottom_box();
	print_info("Chat ended.");
}

