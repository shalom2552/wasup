#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include "constants.h"

/**
 * @brief Set up Server and listen
 * @param port to setup on
 * @return the socketid or -1 on error
 * */
int chat_server_setup(const char* port);

/**
 * @brief Run chat loop.
 * recive -> display -> read -> send
 * @param client_fd The active socket fd
 */
void chat_run_server(const int listen_fd);

/* add new client to chat room */
void handle_new_connection(const int listen_fd);

/* get user name and room number */
int handle_handshake(const int fd, const int idx);

/* handle a message from an existing client */
void handle_client_message(const int idx);

/* broadcast a new message to all other clients in the room */
void broadcast(const int from_idx, const char* msg);

/* send a notification to all clients in a room */
void chat_notify_room(int to_room, NotifyCode code, const char* data);

/* send room user count notification to room */
void notify_room_users_count(int room, int count);

/* send user join notification to room */
void notify_room_user_join(int room, char* username);

/* send user left notification to room */
void notify_room_user_left(int room, char* username);

/* remove client from the list and close his fd */
void remove_client(const int idx);

#endif // !CHAT_SERVER_H
