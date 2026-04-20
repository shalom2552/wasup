#define _POSIX_C_SOURCE 200112L
#include "chat/tcp.h"
#include "chat/utils.h"

#include <string.h>			// memset()
#include <unistd.h>			// close()
#include <netdb.h>			// getaddrinfo(), struct addrinfo

int chat_tcp_bind(const char* port)
{
	struct addrinfo hints;
	struct addrinfo* res;

	// define hints
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;			// IPv4
	hints.ai_socktype = SOCK_STREAM;	// TCP
	hints.ai_flags = AI_PASSIVE;		// local ip

	// get address info
	if (getaddrinfo(NULL, port, &hints, &res)) {
		log_error("Error(getaddrinfo): Could not get address info.");
		return -1;
	}

	int sockfd = -1;
	for (struct addrinfo* rp = res; rp != NULL; rp = rp->ai_next) {

		// TODO: set non blocking soccket: SOCK_STREAM | SOCK_NONBLOCK
		// int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
		sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sockfd == -1) {
			continue; // try next ai
		}

		const int enable = 1;
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1) {
			log_warn("Warning(socket): Port reuse failed."); // non-fatal error
		}

		if (bind(sockfd, rp->ai_addr, rp->ai_addrlen) == 0) {
			break; // success
		}

		close(sockfd);
		sockfd = -1;
	}

	if (sockfd == -1) {
		log_error("Error(bind): Could not bind socket to port.");
	}

	// free res
	freeaddrinfo(res);

	return sockfd;
}

int chat_tcp_accept(int listen_sockfd)
{
	struct sockaddr_storage remote_addr;
	socklen_t add_size = sizeof(remote_addr);

	int client_fd = accept(listen_sockfd, (struct sockaddr*)&remote_addr, &add_size);
	if (client_fd == -1) {
		log_error("Error(accept): Failed to accept connection.");
		return -1;
	}

	return client_fd;
}
