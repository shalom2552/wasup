#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

/* bind socket to port and start listening */
int server_setup(const char* port);

/* start main server loop to manage connections and messages */
void server_run(const int listen_fd);

#endif // !CHAT_SERVER_H
