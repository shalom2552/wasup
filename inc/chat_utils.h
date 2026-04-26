#ifndef CHAT_CHAT_UTILS_H
#define CHAT_CHAT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#include <stddef.h>		// size_t

/* prompt a user for a message and save to buffer */
int chat_get_input_message(char* buffer);

/* send a message to fd, uses new line as framing rule.
 * return 0 on success or -1 on error */
int chat_send_all(const int fd, const char* buffer, size_t len);

/* recive a message from fd, uses new line as framing rule.
 * return read size, 0 on peer close or -1 on error */
int chat_recv_all(const int fd, char* buffer, size_t size);

/* return 1 if msg is [/exit|/quit|/q] or 0 otherwise */
int chat_trap_exit_message(const char* msg);

/* terminate fd connection */
void chat_disconnect(int fd);

/* get username return 0 on failure */
int chat_get_input_username(char *out, size_t size);

/* checks if the input room is in the valid range, sets to 0 if not */
int validate_room_input(char* input);

/* get chat room return 0 on failure */
int chat_get_input_room(char *out, size_t size);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !CHAT_CHAT_UTILS_H

