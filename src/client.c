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

// === TYPES ==================================================================
typedef struct {
    int fd;
    char name[CHAT_USER_NAME_SIZE];
    char room[CHAT_ROOM_SIZE];
} Client;

// === GLOBALS ================================================================
static Client client;

// === HELPERS ================================================================
static void handle_notify_message(char* buffer)
{
    char* colon = strchr(buffer, ':');
    if (!colon) return;
    *colon = '\0';
    int code = atoi(buffer);
    char* data = colon + 1;

    switch ((NotifyCode)code) {
        case NOTIFY_NEW_MSG:
            ui_print_chat_message(data);
            ui_print_chat_message_prompt(client.name);
            break;
        case NOTIFY_ROOM_COUNT:
            ui_print_room_count(atoi(data));
            break;
        case NOTIFY_USER_JOIN:
            ui_print_user_event(data, "joined");
            ui_print_chat_message_prompt(client.name);
            break;
        case NOTIFY_USER_LEFT:
            ui_print_user_event(data, "left");
            ui_print_chat_message_prompt(client.name);
            break;
        case NOTIFY_HISTORY_MSG:
            ui_print_chat_history_message(data);
            ui_print_chat_message_prompt(client.name);
            break;
        default:
            log_error("Unknown notify code: %d", code);
            break;
    }
}

// === PUBLIC API =============================================================
int client_setup(const char *ip, const char *port)
{
    // get user info
    if (ui_chat_get_input_username(client.name, sizeof(client.name)) ||
        ui_chat_get_input_room(client.room, sizeof(client.room))) {
        log_error("Could not get client info. Exiting.");
        return -1;
    }

    // connect
    client.fd = tcp_chat_connect(ip, port);
    if (client.fd < 0) {
        log_error("(setup): Could not set up conection.");
        return -1;
    }

    // handshake
    char handshake[CHAT_USER_NAME_SIZE + CHAT_ROOM_SIZE + 1];
    snprintf(handshake, sizeof(handshake), "%s:%s", client.name, client.room);
    if (chat_send_all(client.fd, handshake, strlen(handshake))) {
        close(client.fd);
        return -1;
    }
    return client.fd;
}

void client_run(int server_fd)
{
    char buffer[PAYLOAD_MAX_SIZE];

    struct pollfd pfds[2];
    pfds[0].fd = STDIN_FILENO; // watch keyboard input
    pfds[0].events = POLLIN;
    pfds[1].fd = server_fd; // watch server messages
    pfds[1].events = POLLIN;

    ui_print_chat_top_box();
    ui_print_room_header(client.room);
    ui_print_chat_message_prompt(client.name);

    while (1) {
        int ready = poll(pfds, 2, -1);
        if (ready < 0) {
            if (errno == EINTR) {
                continue;
            }
            log_error("(poll): failed.");
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
            if (ui_chat_get_input_message(buffer)) {
                break;
            }
            if (strlen(buffer) == 0) {
                ui_print_chat_message_prompt(client.name);
                continue; // empty message - dont send
            }
            if (chat_trap_exit_message(buffer)) {
                break;
            }
            if (chat_send_all(server_fd, buffer, strlen(buffer)) < 0) {
                break;
            }
            ui_print_chat_message_prompt(client.name);
        }
    }

    ui_print_chat_bottom_box();
    chat_disconnect(server_fd);
    log_info("Chat ended.");
}
