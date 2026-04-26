#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

/* get user info, connect to server and send handshake */
int chat_client_setup(const char* ip, const char* port);

/* format and print chat message then show prompt */
void handle_chat_message(char* payload);

/* parse and handle server notification code */
void handle_notify_message(char* payload);

/* start main client loop for io and server events */
void chat_run_client(int server_fd);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !CHAT_CLIENT_H

