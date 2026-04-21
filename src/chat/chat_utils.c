#include "chat/chat_utils.h"
#include "chat/constants.h"
#include "chat/utils.h"

#include <errno.h>			// errno
#include <sys/socket.h>		// send(), recv()
#include <string.h>			// strlen(), strcspn()
#include <stdio.h>			// fgets(), stdin
#include <unistd.h>			// close()
#include <stddef.h>			// siez_t
#include <sys/types.h>		// ssize_t

/* send exactly len bytes to fd */
static int send_bytes(int fd, const char* buffer, size_t len);

int chat_get_input_message(char *buffer)
{
	do { // repeat on empty input
		if ( !fgets(buffer, CHAT_MSG_BUFFER_SIZE, stdin) ) {
			log_error("Error(stdin): System error while reading from stdin.");
			return 1;
		}
		buffer[strcspn(buffer, "\n")] = 0; // remove new line
	} while (strlen(buffer) == 0);
	return 0;
}

static int send_bytes(int fd, const char* buffer, size_t len)
{
	size_t sent = 0;
	while (sent < len) {
		ssize_t n = send(fd, buffer + sent, len - sent, 0);
		if (n < 0) {
			if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
				continue; // non-blocking
			}
			return -1;
		}
		sent += (size_t)n;
	}
	return 0;
}

int chat_send_all(const int fd, char* buffer, size_t len)
{
	if (send_bytes(fd, buffer, len) < 0) {
		return -1;
	}

	// new line framing rule
	return send_bytes(fd, "\n", 1);
}

int chat_recv_all(const int fd, char *buffer, size_t size)
{
	size_t total = 0;
	while (total < size - 1) {
		ssize_t n = recv(fd, buffer + total, 1, 0);
		if (n < 0) {
			if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
				continue; // non-blocking
			}
			return -1;
		}
		if (n == 0) {
			return 0; // peer closed
		}
		if (buffer[total] == '\n') {
			break; // framing rule
		}
		++total;
	}

	buffer[total] = '\0'; // terminate with end string
	return (int)total;
}

void chat_broadcast_message(const char* peername, char* msg)
{
	// TODO: broad cast to all peers
	print_chat_message(peername, msg);
}

int chat_trap_exit_message(const char* msg)
{
	if (strstr(msg, "/exit") || strstr(msg, "/quit") || strstr(msg, "/q"))
	{
		log_info("Server exit the chat.");
		return 1;
	}
	return 0;
}

void chat_disconnect(int fd)
{
	if (fd != -1) {
		close(fd);
	}
}
