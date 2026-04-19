#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#include <stdbool.h>

/**
 * @brief Set up socket
 * @param port to setup on
 *  If port is NULL use DEFAULT_PORT="1313"
 * @return the socketid or -1 on error
 * */
int chat_server_setup(const char* port);

/**
 * @brief waits for client connection
 * @param listen_sockfd
 */
int chat_accept_client(int listen_sockfd);

/* TODO: */
void chat_run_server(int client_fd);

/**
 * @brief terminate connection
 * @param fd file descriptor
 */
void chat_disconnect(int fd);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !CHAT_SERVER_H

