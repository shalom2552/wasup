#include "chat_utils.h"
#include "log.h"
#include "constants.h"

#include <errno.h>			// errno
#include <sys/socket.h>		// send(), recv()
#include <string.h>			// strlen(), strcspn()
#include <stdio.h>			// fgets(), stdin
#include <stdlib.h>			// atoi()
#include <unistd.h>			// close()
#include <stddef.h>			// size_t
#include <sys/types.h>		// ssize_t

int send_bytes(int fd, const char* buffer, size_t len)
{
	size_t sent = 0;
	while (sent < len) {
		ssize_t n = send(fd, buffer + sent, len - sent, MSG_NOSIGNAL);
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

int chat_send_all(const int fd, const char* buffer, size_t len)
{
	if (send_bytes(fd, buffer, len) < 0) {
		return -1;
	}

	// new line framing rule
	return send_bytes(fd, "\n", 1);
}

int chat_recv_all(const int fd, char* buffer, size_t size)
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

void chat_disconnect(int fd)
{
	if (fd != -1) {
		close(fd);
	}
}

int validate_room_input(char* input)
{
    int room = atoi(input);
    if (room < 0 || room > CHAT_MAX_ROOMS) {
		room = 0;
	}
    return room;
}

int chat_trap_exit_message(const char* msg)
{
	if (strcmp(msg, "/exit") == 0
        || strcmp(msg, "/quit") == 0
        || strcmp(msg, "/q") == 0)
	{
		return 1;
	}
	return 0;
}
