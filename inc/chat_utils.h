#ifndef CHAT_CHAT_UTILS_H
#define CHAT_CHAT_UTILS_H

#include "constants.h"
#include <stddef.h>		// size_t

/* send raw bytes until len is reached */
int send_bytes(int fd, const char* buffer, size_t len);

/* send message with newline framing */
int chat_send_all(const int fd, const char* buffer, size_t len);

/* receive message until newline, return bytes read */
int chat_recv_all(const int fd, char* buffer, size_t size);

/* close connection if fd is valid */
void chat_disconnect(int fd);

/* parse room number and check bounds */
int validate_room_input(char* input);

/* check if message is exit command */
int chat_trap_exit_message(const char* msg);

/* send server notification code and data to client */
int chat_notify_client(int fd, NotifyCode code, const char* data);

#endif // !CHAT_CHAT_UTILS_H
