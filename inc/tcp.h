#ifndef CHAT_TCP_H
#define CHAT_TCP_H

/* set socket to non-blocking mode */
int set_nonblocking(int fd);

/* create and bind tcp socket to local port */
int chat_tcp_bind(const char* port);

/* accept incoming connection and set to non-blocking */
int chat_tcp_accept(int listen_sockfd);

/* connect to remote tcp server */
int chat_tcp_connect(const char* host, const char* port);

#endif // !CHAT_TCP_H
