#ifndef CHAT_TCP_H
#define CHAT_TCP_H

/* create and bind tcp socket to local port */
int tcp_chat_bind(const char* port);

/* accept incoming connection and set to non-blocking */
int tcp_chat_accept(int listen_sockfd);

/* connect to remote tcp server */
int tcp_chat_connect(const char* host, const char* port);

#endif // !CHAT_TCP_H
