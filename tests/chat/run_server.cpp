#include "chat/chat_utils.h"
#include "chat/constants.h"
#include "chat/server.h"
#include "chat/utils.h"
#include "chat/tcp.h"

#include <signal.h>		// SIGINT

// TODO: unified
int main (int argc, char *argv[]) {
	signal(SIGINT, handle_sigint);
	print_welcome_message();

	// setup listiner
	const char* port = (argc > 1) ? argv[1] : CHAT_DEFAULT_PORT;
	int listen_fd = chat_server_setup(port);
	if (listen_fd < 0) {
		log_error("Setup failed.");
		return 1;
	}

	G_SOCKFD = listen_fd; // to close on trap sigint

	log_info("Server is listening...");

	// wait for client connection
	int client_fd = chat_tcp_accept(listen_fd);
	if (client_fd < 0) {
		chat_disconnect(listen_fd);
		return 1;
	}

	log_info("Connected!");
	chat_run_server(client_fd);

	// cleanup
	chat_disconnect(client_fd);
	chat_disconnect(listen_fd);

	return 0;
}


