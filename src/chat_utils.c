#include "chat_utils.h"
#include "constants.h"
#include "utils.h"

#include <errno.h>			// errno
#include <sys/socket.h>		// send(), recv()
#include <string.h>			// strlen(), strcspn()
#include <stdio.h>			// fgets(), stdin
#include <stdlib.h>			// atoi()
#include <unistd.h>			// close()
#include <stddef.h>			// size_t
#include <sys/types.h>		// ssize_t

/* send exactly len bytes to fd */
static int send_bytes(int fd, const char* buffer, size_t len);

int chat_get_input_message(char *buffer)
{
	if ( !fgets(buffer, CHAT_MSG_BUFFER_SIZE, stdin) ) {
		log_error("Error(stdin): System error while reading from stdin.");
		return 1;
	}
	buffer[strcspn(buffer, "\n")] = 0;
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

int chat_send_all(const int fd, const char* buffer, size_t len)
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

int chat_trap_exit_message(const char* msg)
{
	if (strstr(msg, "/exit") || strstr(msg, "/quit") || strstr(msg, "/q"))
	{
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

int chat_get_input_username(char *out, size_t size)
{
    return get_user_input("Enter user name", out, size, "Anonymous");
}

int chat_get_input_room(char *out, size_t size)
{
    if (get_user_input("Enter room number", out, size, "0")) {
        return 1;
    }
    int room = atoi(out);
    if (room < 0 || room > CHAT_MAX_ROOMS) {
		room = 0;
	}
    snprintf(out, size, "%d", room);
    return 0;
}
