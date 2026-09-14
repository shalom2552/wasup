#include "constants.h"
#include "server.h"
#include "client.h"
#include "ui.h"
#include "log.h"

#include <string.h>     // strcmp()

int main(int argc, char* argv[])
{
	ui_print_welcome_message();

	if (argc > 1 && strcmp(argv[1], "-s") == 0) {
		const char* port = (argc > 2) ? argv[2] : CHAT_DEFAULT_PORT;
        int listen_fd = server_setup(port);
        if (listen_fd < 0) {
            log_error("Setup failed.");
            return 1;
        }

        server_run(listen_fd);
	} else {
		const char* ip   = (argc > 1) ? argv[1] : CHAT_LOCAL_HOST;
		const char* port = (argc > 2) ? argv[2] : CHAT_DEFAULT_PORT;
        int server_fd = client_setup(ip, port);
        if (server_fd < 0) {
            log_error("Connection failed.");
            return 1;
        }

        client_run(server_fd);
	}

	return 0;
}

