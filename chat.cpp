#include "chat_utils.h"
#include "constants.h"
#include "server.h"
#include "client.h"
#include "utils.h"

#include <string.h>     // strcmp()
#include <signal.h>		// SIGINT

/* setup server and run chat */
static void start_server(const char* port);

/* connect to server and run chat */
static void connect_client(const char* ip, const char* port);

int main (int argc, char *argv[]) {
	signal(SIGINT, handle_sigint);
	print_welcome_message();

	if (argc > 1 && strcmp(argv[1], "-s") == 0) {
		const char *port = (argc > 2) ? argv[2] : CHAT_DEFAULT_PORT;
		start_server(port);
	} else {
		const char *ip   = (argc > 1) ? argv[1] : CHAT_LOCAL_HOST;
		const char *port = (argc > 2) ? argv[2] : CHAT_DEFAULT_PORT;
		connect_client(ip, port);
	}

	return 0;
}

static void start_server(const char* port)
{
	int listen_fd = chat_server_setup(port);
	if (listen_fd < 0) {
		log_error("Setup failed.");
		return;
	}

	log_info("Connected!");
	chat_run_server(listen_fd);
	chat_disconnect(listen_fd);
}

static void connect_client(const char* ip, const char* port)
{
	int server_fd = chat_client_setup(ip, port);
	if (server_fd < 0) {
		log_error("Connection failed.");
		return;
	}

	log_info("Connected to server!");
	chat_run_client(server_fd);
	chat_disconnect(server_fd);
}
