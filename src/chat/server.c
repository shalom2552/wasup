#define _POSIX_C_SOURCE 200112L
#include "chat/server.h"
#include "chat/utils.h"
#include "chat/constants.h"

#include <sys/socket.h>		// socket(), bind(), listen(), accept(), setsockopt()
#include <netdb.h>			// getaddrinfo(), struct addrinfo
#include <string.h>			// memset()
#include <stdbool.h>		// bool
#include <unistd.h>			// close()
#include <stdio.h>			// fgets(), stdin

int chat_server_setup(const char* port)
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
	hints.ai_flags = AI_PASSIVE;		// local ip

	// get address info
	if (getaddrinfo(NULL, port, &hints, &res)) {
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

	// allow address reuse
	int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        print_error("Warning: Port reuse failed."); // non-fatal error
    }

	// bind
	if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
		print_error("Error(bind): Could not bind socket to port.");
		close(sockfd);
		freeaddrinfo(res);
		return -1;
	}

	freeaddrinfo(res);

	// listen
	if (listen(sockfd, 1) == -1) {
		print_error("Error(listen): Could not listen on socket.");
		close(sockfd);
		return -1;
	}

	return sockfd;
}

int chat_accept_client(int listen_sockfd)
{
	struct sockaddr_storage remote_addr;
	socklen_t add_size = sizeof(remote_addr);

	int client_fd = accept(listen_sockfd, (struct sockaddr*)&remote_addr, &add_size);
	if (client_fd == -1) {
		print_error("Error(accept): Failed to accept connection.");
		return -1;
	}
	return client_fd;
}

void chat_run_server(int client_fd)
{
	char username[UNAME_SIZE];
	char peername[UNAME_SIZE];
	char buffer[2048];

	// get username
	if ( !get_user_name(username, sizeof(username)) ) {
		print_error("Failed to get username. exiting.");
		return;
	}
	exchange_user_names(client_fd, username, peername);

	// start chat
	print_info("Chat started!");
	print_chat_top_box();
	while (1) {

		// recive from client
		int bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
		if (bytes <= 0) { // error or client close connection
			print_info("Client disconnected.");
			break;
		}

		// trap client exit message
		if (chat_exit_messege(buffer)) {
			print_info("Client (client_fd) exit the chat.");
			break;
		}

		// terminate with end string
		buffer[bytes] = '\0';
		print_chat_message(peername, buffer);

		// get input message
		do {
			print_chat_prompt(username);
			if ( !fgets(buffer, sizeof(buffer), stdin) ) {
				print_error("Error(stdin): System error while reading from stdin.");
				break;
			}

			buffer[strcspn(buffer, "\n")] = 0; // remove new line
		} while (strlen(buffer) == 0);

		// trap server exit message
		if (chat_exit_messege(buffer)) {
			print_info("Server exit the chat.");
			break;
		}
		send(client_fd, buffer, strlen(buffer), 0);
	}
	print_chat_bottom_box();
	print_info("Chat ended.");
}

