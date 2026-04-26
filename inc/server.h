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

/* remove client from the list and close his fd */
void remove_client(const int idx);

/* send a notification to all clients in a room, excluding exclude_idx (-1 = send to all) */
void notify_room(int room, int exclude_idx, NotifyCode code, const char* data);

/* broadcast a new message to all other clients in the room */
void notify_room_new_msg(const int from_idx, const char* msg);

/* send room user count notification to room */
void notify_room_users_count(const int room, const int count);

/* send user join or left notification to all others in room */
void notify_room_user_event(const int room, const int idx, const NotifyCode code);

#endif // !CHAT_SERVER_H
