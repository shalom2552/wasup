#include "chat/constants.h"
#include "chat/server.h"
#include "chat/utils.h"


int main (int argc, char *argv[]) {
	print_welcome_message();

	// setup listiner
	const char* port = (argc > 1) ? argv[1] : DEFAULT_PORT;
	int listen_fd = chat_server_setup(port);
	if (listen_fd < 0) {
		print_error("Setup failed.");
		return 1;
	}

	print_info("Server is listening...");

	// wait for client connection
	int client_fd = chat_accept_client(listen_fd);
	if (client_fd < 0) {
		chat_disconnect(listen_fd);
		return 1;
	}

	print_info("Connected!");
	chat_run_server(client_fd);

	// cleanup
	chat_disconnect(client_fd);
	chat_disconnect(listen_fd);

	return 0;
}


