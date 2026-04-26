#define _POSIX_C_SOURCE 200112L
#include "tcp.h"
#include "utils.h"

#include <errno.h>			// errno
#include <string.h>			// memset()
#include <unistd.h>			// close()
#include <netdb.h>			// getaddrinfo(), struct addrinfo
#include <fcntl.h>			// fcntl()

/* set non non-blocking flag to fd */
static int set_nonblocking(int fd);

static int set_nonblocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		return -1;
	}

	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        return -1;
    }

	return 0;
}

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

		sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sockfd == -1) {
			continue; // try next ai
		}

		if (set_nonblocking(sockfd) == -1) {
			log_warn("Warning: Failed to set non-blocking."); // non-fatal
		}

		const int enable = 1;
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1) {
			log_warn("Warning(socket): Port reuse failed."); // non-fatal
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
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return -2; // no client wating
		}
		log_error("Error(accept): Failed to accept connection.");
		return -1;
	}

	if (set_nonblocking(client_fd) == -1) {
		log_warn("Warning: Failed to set non-blocking.");
	}

	return client_fd;
}

int chat_tcp_connect(const char *host, const char *port)
{
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host, port, &hints, &res)) {
        log_error("Error(getaddrinfo): Could not get address info.");
        return -1;
    }

    int sockfd = -1;
    for (struct addrinfo *rp = res; rp; rp = rp->ai_next) {
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd == -1) {
			continue; // try next ai
		}
        if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) == 0) {
			break; // success
		}
        close(sockfd);
        sockfd = -1;
    }

    if (sockfd == -1) {
		log_error("Error(connect): Could not connect.");
	}

	freeaddrinfo(res);
    return sockfd;
}
