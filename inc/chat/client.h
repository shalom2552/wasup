#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

/**
 * @brief Set up socket
 * @param ip The
 * @param port To setup on
 * @return the socketid or -1 on error
 * */
int chat_client_setup(const char* ip, const char* port);

/**
 * @brief Run chat loop.
 * recive -> display -> read -> send
 * @param server_fd The server socket fd
 */
void chat_run_client(int server_fd);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !CHAT_CLIENT_H

