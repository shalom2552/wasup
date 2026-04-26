#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include "constants.h"
#include <poll.h>

/* bind socket to port and start listening */
int chat_server_setup(const char* port);

/* start main server loop to manage connections and messages */
void chat_run_server(const int listen_fd);

/* handle polling events for new connections and client messages */
void handle_server_events(struct pollfd* pfds, int listen_fd);

/* accept new connection and perform handshake */
void handle_new_connection(const int listen_fd);

/* read and parse username and room from new client */
int handle_handshake(const int fd, const int idx);

/* receive message from client and broadcast to room */
void handle_client_message(const int idx);

/* close client connection and remove from active list */
void remove_client(const int idx);

/* send server notification to all clients in a room */
void notify_room(int room, int exclude_idx, NotifyCode code, const char* data);

/* broadcast formatted chat message to room members */
void notify_room_new_msg(const int from_idx, const char* msg);

/* update all room members with current user count */
void notify_room_users_count(const int room, const int count);

/* notify room about user joining or leaving */
void notify_room_user_event(const int room, const int idx, const NotifyCode code);

#endif // !CHAT_SERVER_H
