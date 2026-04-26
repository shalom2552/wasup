#ifndef CHAT_CHAT_UTILS_H
#define CHAT_CHAT_UTILS_H

#include <stddef.h>		// size_t

/* send exactly len bytes to fd */
int send_bytes(int fd, const char* buffer, size_t len);

/* send a message to fd, uses new line as framing rule.
 * return 0 on success or -1 on error */
int chat_send_all(const int fd, const char* buffer, size_t len);

/* recive a message from fd, uses new line as framing rule.
 * return read size, 0 on peer close or -1 on error */
int chat_recv_all(const int fd, char* buffer, size_t size);

/* terminate fd connection */
void chat_disconnect(int fd);

/* checks if the input room is in the valid range, sets to 0 if not */
int validate_room_input(char* input);

/* return 1 if msg is [/exit|/quit|/q] or 0 otherwise */
int chat_trap_exit_message(const char* msg);

#endif // !CHAT_CHAT_UTILS_H
