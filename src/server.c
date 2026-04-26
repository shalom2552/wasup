#include "server.h"
#include "utils.h"
#include "chat_utils.h"
#include "constants.h"
#include "tcp.h"

#include <string.h>			// strchr()
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
static int room_count[CHAT_MAX_ROOMS] = {0};
static int client_count = 0;

int chat_server_setup(const char* port)
{
	int sockfd = chat_tcp_bind(port);
	if (sockfd == -1) {
		return -1;
	}

	// listen
	if (listen(sockfd, SOMAXCONN) == -1) {
		log_error("Error(listen): Could not listen on socket.");
		close(sockfd);
		return -1;
	}

	log_info("Server is up!");
	return sockfd;
}

void remove_client(const int idx)
{
    log_info("[%s] left room [%d]", clients[idx].name, clients[idx].room);
	broadcast(idx, "left the room.");
    chat_disconnect(clients[idx].fd);
    // swap-remove
    clients[idx] = clients[client_count - 1];
    client_count--;
}

void broadcast(const int from_idx, const char *msg)
{
	// format: "name: message"
    char framed[CHAT_MSG_BUFFER_SIZE + CHAT_USER_NAME_SIZE + 1];
    snprintf(framed, sizeof(framed), "%s:%s", clients[from_idx].name, msg);

    int room = clients[from_idx].room;
    for (int i = 0; i < client_count; ++i) {
        if (i == from_idx) {
			continue;
		}
        if (clients[i].room != room) {
			continue;
		}
        chat_send_all(clients[i].fd, framed, strlen(framed));
    }
}

void notify_room(int to_room, int count)
{
    char* NOTIFY_PREFIX = "SERVER_NOTIFY:";
    char notification[256];

    snprintf(notification, sizeof(notification), "%s%d:%d", NOTIFY_PREFIX, NOTIFY_ROOM_COUNT_UPDATE, count);
    for (int i = 0; i < client_count; ++i) {
        if (clients[i].room == to_room) {
            chat_send_all(clients[i].fd, notification, strlen(notification));
        }
    }
}

int handle_handshake(const int fd, const int idx)
{
	char buffer[CHAT_USER_NAME_SIZE + CHAT_ROOM_SIZE];
	int n = chat_recv_all(fd, buffer, sizeof(buffer));
	if (n <= 0) {
		return -1;
	}

	char* colon = strchr(buffer,  ':');
	if (!colon) {
		return -1;
	}

	*colon = '\0';
	strncpy(clients[idx].name, buffer, CHAT_USER_NAME_SIZE - 1);
	clients[idx].name[CHAT_USER_NAME_SIZE -  1] = '\0';
	clients[idx].room = validate_room_input(colon + 1);
	clients[idx].fd = fd;
	return 0;
}

void handle_new_connection(const int listen_fd)
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

	int idx = client_count;
	if (handle_handshake(fd, idx) == -1) {
		log_error("Error(handshake): Could not establish connection.");
		close(fd);
		return;
	}

    ++client_count;
    notify_room(clients[idx].room, ++room_count[clients[idx].room]);
	broadcast(idx, "joined the room.");
    log_info("[%s] joined room [%d].", clients[idx].name, clients[idx].room);
}

void handle_client_message(const int idx)
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

    broadcast(idx, buffer);
}

void chat_run_server(const int listen_fd)
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

    chat_disconnect(listen_fd);
	log_info("Chat ended.");
}

