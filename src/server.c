#include "server.h"
#include "chat_utils.h"
#include "constants.h"
#include "history.h"
#include "log.h"
#include "tcp.h"

#include <errno.h>      // errno
#include <poll.h>       // poll(), POLLIN, POLLHUP, POLLERR
#include <stdio.h>      // snprintf()
#include <string.h>     // strchr()
#include <sys/socket.h> // listen()
#include <unistd.h>     // close()

// === TYPES ==================================================================
typedef struct {
    int fd;
    int room;
    char name[CHAT_USER_NAME_SIZE];
} Client;

typedef struct {
    int room_count;
    ChatHistory history;
} Room;

// === GLOBALS ================================================================
static int client_count = 0;
static Room rooms[CHAT_MAX_ROOMS];
static Client clients[CHAT_MAX_CLIENTS];

// === HELPERS ================================================================
static void notify_room(int room, int exclude_idx, NotifyCode code, const char* data)
{
    for (int i = 0; i < client_count; ++i) {
        if (i == exclude_idx) {
            continue;
        }
        if (clients[i].room == room) {
            chat_notify_client(clients[i].fd, code, data);
        }
    }
}

static void notify_room_new_msg(const int from_idx, const char* msg)
{
    // format: "name:message"
    char framed[CHAT_MSG_BUFFER_SIZE + CHAT_USER_NAME_SIZE + 1];
    snprintf(framed, sizeof(framed), "%s:%s", clients[from_idx].name, msg);
    notify_room(clients[from_idx].room, from_idx, NOTIFY_NEW_MSG, framed);
    log_info("<%s> messaged at room #%d", clients[from_idx].name, clients[from_idx].room);
}

static void notify_room_users_count(const int room, const int count)
{
    char buffer[CHAT_NOTIFY_PAYLOAD_SIZE];
    snprintf(buffer, sizeof(buffer), "%d", count);
    notify_room(room, -1, NOTIFY_ROOM_COUNT, buffer);
}

static int handle_handshake(const int fd, const int idx)
{
    char buffer[CHAT_USER_NAME_SIZE + CHAT_ROOM_SIZE];
    int n = chat_recv_all(fd, buffer, sizeof(buffer));
    if (n <= 0) {
        return -1;
    }

    char *colon = strchr(buffer, ':');
    if (!colon) {
        return -1;
    }

    *colon = '\0';
    strncpy(clients[idx].name, buffer, CHAT_USER_NAME_SIZE - 1);
    clients[idx].name[CHAT_USER_NAME_SIZE - 1] = '\0';
    clients[idx].room = chat_validate_room_input(colon + 1);
    clients[idx].fd = fd;
    return 0;
}

static void remove_client(const int idx)
{
    Client client = clients[idx];

    --rooms[client.room].room_count;
    clients[idx] = clients[--client_count]; // swap-remove

    chat_disconnect(client.fd);
    notify_room(client.room, -1, NOTIFY_USER_LEFT, client.name);
    notify_room_users_count(client.room, rooms[client.room].room_count);
    log_info("<%s> left room #%d", client.name, client.room);
}

static void handle_new_connection(const int listen_fd)
{
    int fd = tcp_chat_accept(listen_fd);
    if (fd < 0) {
        return;
    }

    if (client_count >= CHAT_MAX_CLIENTS) {
        log_error("(connect): Max clients reached, rejecting.");
        close(fd);
        return;
    }

    int idx = client_count;
    if (handle_handshake(fd, idx) == -1) {
        log_error("(handshake): Could not establish connection.");
        close(fd);
        return;
    }

    ++client_count;
    ++rooms[clients[idx].room].room_count;
    history_notify_client(&rooms[clients[idx].room].history, clients[idx].fd);
    notify_room(clients[idx].room, idx, NOTIFY_USER_JOIN, clients[idx].name);
    notify_room_users_count(clients[idx].room, rooms[clients[idx].room].room_count);
    log_info("<%s> joined room #%d.", clients[idx].name, clients[idx].room);
}

static void handle_client_message(const int idx)
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

    history_save_msg(&rooms[clients[idx].room].history, clients[idx].name, buffer);
    notify_room_new_msg(idx, buffer);
}

static void handle_server_events(struct pollfd* pfds, int listen_fd)
{
    // clients — iterate backwards because remove_client swaps
    for (int i = client_count - 1; i >= 0; i--) {
        if (pfds[i + 1].revents & (POLLIN | POLLHUP | POLLERR)) {
            handle_client_message(i);
        }
    }

    // listen socket
    if (pfds[0].revents & POLLIN) {
        handle_new_connection(listen_fd);
    }
}

// === PUBLIC API =============================================================
int server_setup(const char *port)
{
    int sockfd = tcp_chat_bind(port);
    if (sockfd == -1) {
        return -1;
    }

    // listen
    if (listen(sockfd, SOMAXCONN) == -1) {
        log_error("(listen): Could not listen on socket.");
        close(sockfd);
        return -1;
    }

    // intilize rooms history
    for (int room = 0; room < CHAT_MAX_ROOMS; ++room) {
        history_init(&rooms[room].history);
    }

    log_info("Server is up!");
    return sockfd;
}

void server_run(const int listen_fd)
{
    struct pollfd pfds[CHAT_MAX_CLIENTS + 1];

    while (1) {

        // rebuild pfds every iteration
        pfds[0].fd = listen_fd; // watch for new connections
        pfds[0].events = POLLIN;
        for (int i = 0; i < client_count; i++) {
            pfds[i + 1].fd = clients[i].fd;     // watch each client
            pfds[i + 1].events = POLLIN;
        }

        int ready = poll(pfds, client_count + 1, -1);
        if (ready < 0) {
            if (errno == EINTR) {
                continue;   // interupt keep going
            }
            log_error("(poll): failed.");
            break;
        }

        handle_server_events(pfds, listen_fd);
    }

    chat_disconnect(listen_fd);
    log_info("Chat ended.");
}

