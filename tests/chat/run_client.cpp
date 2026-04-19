#include "chat/client.h"
#include "chat/utils.h"
#include "chat/constants.h"

#include <signal.h>		// SIGINT

int main (int argc, char *argv[]) {
	signal(SIGINT, handle_sigint);
	print_welcome_message();

	// setup listiner
	const char* ip   = (argc > 1) ? argv[1] : LOCAL_HOST;
	const char* port = (argc > 2) ? argv[2] : DEFAULT_PORT;

	int server_fd = chat_client_setup(ip, port);
	if (server_fd < 0) {
		print_error("Connection failed.");
		return 1;
	}

	G_SOCKFD = server_fd; // to close on trap sigint

	print_info("Connected to server!");
	chat_run_client(server_fd);

	chat_disconnect(server_fd);
	return 0;
}


