#include "chat/chat_utils.h"
#include "chat/constants.h"
#include "chat/utils.h"

#include <sys/socket.h>		// send(), recv()
#include <string.h>			// strlen(), strcspn()
#include <stdio.h>			// fgets(), stdin
#include <unistd.h>			// close()

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

int chat_send_all(const int fd, char* buffer)
{
	// BUG: needs a loop
	// send message
	send(fd, buffer, strlen(buffer), 0);
	return 0;
}

int chat_recv_all(const int fd, char *buffer)
{
	// recive from client
	// BUG: needs a loop
	int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0) { // error or client close connection
		log_info("Client disconnected.");
		return 1;
	}

	// terminate with end string
	buffer[bytes] = '\0';
	return 0;
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

