#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

/**
 * @brief Set up server connection
 * @param ip of the server
 * @param port To setup on
 * @return the socketid or -1 on error
 */
int chat_client_setup(const char* ip, const char* port);

/* read the server message if its a system notification handle it else return 0 */
void handle_recived_message(char* buffer);

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

