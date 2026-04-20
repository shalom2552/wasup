#ifndef CHAT_CHAT_UTILS_H
#define CHAT_CHAT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

/* TODO: Document */
int chat_get_input_message(char* buffer);

/* TODO: Document */
int chat_send_all(const int fd, char* buffer);

/* TODO: Document */
int chat_recv_all(const int fd, char* buffer);

/* TODO: Document */
void chat_broadcast_message(const char* peername, char* msg);

/* return 1 if msg is [/exit|/quit|/q] or 0 otherwise */
int chat_trap_exit_message(const char* msg);

/* terminate fd connection */
void chat_disconnect(int fd);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !CHAT_CHAT_UTILS_H

