#define _POSIX_C_SOURCE 200112L
#include "client.h"
#include "chat_utils.h"
#include "constants.h"
#include "log.h"
#include "tcp.h"
#include "ui.h"

#include <errno.h>   // errno
#include <netdb.h>   // getaddrinfo(), struct addrinfo
#include <poll.h>    // poll(), POLLIN, POLLHUP, POLLERR
#include <stdbool.h> // bool
#include <stdio.h>
#include <stdlib.h>     // atoi()
#include <string.h>     // memset()
#include <sys/socket.h> // socket(), bind(), listen(), accept(), setsockopt()
#include <unistd.h>     // close()

static struct {
    int fd;
    char name[CHAT_USER_NAME_SIZE];
    char room[CHAT_ROOM_SIZE];
} Client;

int chat_client_setup(const char *ip, const char *port)
{
    // get user info
    if (chat_get_input_username(Client.name, sizeof(Client.name)) ||
        chat_get_input_room(Client.room, sizeof(Client.room))) {
        log_error("Could not get client info. Exiting.");
        return -1;
    }

    // connect
    Client.fd = chat_tcp_connect(ip, port);
    if (Client.fd < 0) {
        log_error("Error(setup): Could not set up conection.");
        return -1;
    }

    // handshake
    char handshake[CHAT_USER_NAME_SIZE + CHAT_ROOM_SIZE + 1];
    snprintf(handshake, sizeof(handshake), "%s:%s", Client.name, Client.room);
    if (chat_send_all(Client.fd, handshake, strlen(handshake))) {
        close(Client.fd);
        return -1;
    }
    return Client.fd;
}

void handle_chat_message(char* payload)
{
    printf(ANSI_CLEAR_LINE);
    print_chat_message(payload);
    print_chat_message_prompt(Client.name);
}

void handle_notify_message(char* buffer)
{
    char* colon = strchr(buffer, ':');
    if (!colon) return;
    *colon = '\0';
    int code = atoi(buffer);
    char* data = colon + 1;

    switch ((NotifyCode)code) {
        case NOTIFY_NEW_MSG:
            handle_chat_message(data);
            break;
        case NOTIFY_ROOM_COUNT:
            print_room_count(atoi(data));
            break;
        case NOTIFY_USER_JOIN:
            print_user_join(data);
            break;
        case NOTIFY_USER_LEFT:
            print_user_left(data);
            break;
    }
}

void chat_run_client(int server_fd)
{
    char buffer[CHAT_MSG_BUFFER_SIZE];

    struct pollfd pfds[2];
    pfds[0].fd = STDIN_FILENO; // watch keyboard input
    pfds[0].events = POLLIN;
    pfds[1].fd = server_fd; // watch server messages
    pfds[1].events = POLLIN;

    print_chat_top_box();
    print_room_header(Client.room);
    print_chat_message_prompt(Client.name);

    while (1) {
        int ready = poll(pfds, 2, -1);
        if (ready < 0) {
            if (errno == EINTR) {
                continue;
            }
            log_error("Error(poll): failed.");
            break;
        }

        // server input event
        if (pfds[1].revents & (POLLIN | POLLHUP | POLLERR)) {
            int n = chat_recv_all(server_fd, buffer, sizeof(buffer));
            if (n <= 0) {
                printf("\n");
                log_error("Server disconnected.");
                break;
            }
            handle_notify_message(buffer);
        }

        // keyboard input event
        if (pfds[0].revents & POLLIN) {
            if (chat_get_input_message(buffer)) {
                break;
            }
            if (strlen(buffer) == 0) {
                print_chat_message_prompt(Client.name);
                continue; // empty message - dont send
            }
            if (chat_trap_exit_message(buffer)) {
                break;
            }
            if (chat_send_all(server_fd, buffer, strlen(buffer)) < 0) {
                break;
            }
            print_chat_message_prompt(Client.name);
        }
    }

    print_chat_bottom_box();
    chat_disconnect(server_fd);
    log_info("Chat ended.");
}
