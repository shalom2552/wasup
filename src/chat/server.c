#include "chat/server.h"
#include "chat/utils.h"
#include "chat/chat_utils.h"
#include "chat/constants.h"
#include "chat/tcp.h"

#include <sys/socket.h>		// listen()
#include <unistd.h>			// close()
#include <poll.h>			// poll(), POLLIN, POLLHUP, POLLERR
#include <errno.h>			// errno
#include <stdio.h>			// snprintf()

typedef struct {
    int  fd;
    int  room;
    char name[CHAT_USER_NAME_SIZE];
} client_t;

static client_t clients[CHAT_MAX_CLIENTS];
static int      client_count = 0;

/* remove client from the list and close his fd */
static void remove_client(int idx);

/* broadcast a new message to all other clients in the room */
static void broadcast(int from_idx, const char *msg, size_t len);

/* add new client to chat room */
static void handle_new_connection(int listen_fd);

int chat_server_setup(const char* port)
{
	int sockfd = chat_tcp_bind(port);
	if (sockfd == -1) {
		return -1;
	}

	// listen
	if (listen(sockfd, 1) == -1) {
		log_error("Error(listen): Could not listen on socket.");
		close(sockfd);
		return -1;
	}

	return sockfd;
}

static void remove_client(int idx)
{
    log_info("Client disconnected.");
    chat_disconnect(clients[idx].fd);
    // swap-remove
    clients[idx] = clients[client_count - 1];
    client_count--;
}

static void broadcast(int from_idx, const char *msg, size_t len)
{
    int room = clients[from_idx].room;
    for (int i = 0; i < client_count; i++) {
        if (i == from_idx) {
			continue;
		}
        if (clients[i].room != room) {
			continue;
		}
        chat_send_all(clients[i].fd, msg, len);
    }
}

static void handle_new_connection(int listen_fd)
{
    int fd = chat_tcp_accept(listen_fd);
    if (fd < 0) {
		return;
	}

    if (client_count >= CHAT_MAX_CLIENTS) {
        log_error("Error(connect): Max clients reached, rejecting.");
        close(fd);
        return;
    }

    // TODO: handshake (read "name:room\n").
    clients[client_count].fd = fd;
    clients[client_count].room = 0;
    snprintf(clients[client_count].name, CHAT_USER_NAME_SIZE, "anon%d", client_count);

    ++client_count;
    log_info("Client connected.");
}

static void handle_client_message(int idx)
{
    char buffer[CHAT_MSG_BUFFER_SIZE];
    int n = chat_recv_all(clients[idx].fd, buffer, sizeof(buffer));
    if (n <= 0) { // 0 = closed, -1 = error
        remove_client(idx);
        return;
    }

	if (chat_trap_exit_message(buffer)) {
        remove_client(idx);
        return;
    }

    // format: "name: message"
    char framed[CHAT_MSG_BUFFER_SIZE];
    int len = snprintf(framed, sizeof(framed), "%s: %s",
                       clients[idx].name, buffer);
    if (len > 0) {
		broadcast(idx, framed, (size_t)len);
	}
}

void chat_run_server(int listen_fd)
{
	struct pollfd pfds[CHAT_MAX_CLIENTS + 1];

	while (1) {

		// rebuild pfds every iteration
        pfds[0].fd = listen_fd;
        pfds[0].events = POLLIN;
        for (int i = 0; i < client_count; i++) {
            pfds[i + 1].fd = clients[i].fd;
            pfds[i + 1].events = POLLIN;
        }

        int ready = poll(pfds, client_count + 1, -1);
        if (ready < 0) {
            if (errno == EINTR) continue;
            log_error("Error(poll): failed.");
            break;
        }

        // listen socket
        if (pfds[0].revents & POLLIN) {
            handle_new_connection(listen_fd);
        }

        // clients — iterate backwards because remove_client swaps
        for (int i = client_count - 1; i >= 0; i--) {
            if (pfds[i + 1].revents & (POLLIN | POLLHUP | POLLERR)) {
                handle_client_message(i);
            }
        }
    }

    close(listen_fd);
	log_info("Chat ended.");
}

