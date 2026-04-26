#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#include <stdbool.h>

/**
 * @brief Set up Server and listen
 * @param port to setup on
 * @return the socketid or -1 on error
 * */
int chat_server_setup(const char* port);

/* remove client from the list and close his fd */
void remove_client(const int idx);

/* broadcast a new message to all other clients in the room */
void broadcast(const int from_idx, const char *msg);

/* send a notification to the client program */
void notify_room(int room, int update);

/* add new client to chat room */
void handle_new_connection(int listen_fd);

/* get user name and room number */
int handle_handshake(const int fd, const int idx);

/**
 * @brief Run chat loop.
 * recive -> display -> read -> send
 * @param client_fd The active socket fd
 */
void chat_run_server(const int listen_fd);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !CHAT_SERVER_H

