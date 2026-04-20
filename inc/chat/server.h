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

/**
 * @brief Run chat loop.
 * recive -> display -> read -> send
 * @param client_fd The active socket fd
 */
void chat_run_server(int client_fd);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !CHAT_SERVER_H

