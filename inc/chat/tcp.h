#ifndef CHAT_TCP_H
#define CHAT_TCP_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#include <stdbool.h>

/**
 * @brief Create socket and bind to port
 * @param port to setup on
 * @return the socketid or -1 on error
 * */
int chat_tcp_bind(const char* port);

/**
 * @brief waits for client connection and accept it
 * @param listen_sockfd
 */
int chat_tcp_accept(int listen_sockfd);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !CHAT_TCP_H

