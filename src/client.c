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

void handle_recived_message(char *buffer)
{

    // TODO: implement correctly
    char *NOTIFY_PREFIX = "SERVER_NOTIFY:";
    int code = -1;
    int count = -1;

    if (strncmp(buffer, NOTIFY_PREFIX, strlen(NOTIFY_PREFIX)) == 0) {
        char *notification = buffer + strlen(NOTIFY_PREFIX);
        if (sscanf(notification, "%d:%d", &code, &count) == 2) {
            print_room_count(count);
        }
        return;
    }

    printf(ANSI_CLEAR_LINE);
    print_chat_message(buffer);
    print_chat_message_prompt(Client.name);
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
            handle_recived_message(buffer);
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
