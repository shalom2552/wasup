#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

/* get user info, connect to server and send handshake */
int client_setup(const char* ip, const char* port);

/* start main client loop for io and server events */
void client_run(int server_fd);

#endif // !CHAT_CLIENT_H

