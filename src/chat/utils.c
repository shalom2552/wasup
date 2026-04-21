#include "chat/utils.h"
#include "chat/constants.h"

#include <stdio.h>		// printf(), fprintf
#include <string.h>		// strncmp()
#include <stdlib.h>		// exit()
#include <sys/socket.h>
#include <unistd.h>		// close()
#include <time.h>		// time_t, struct tm, time(), localtime(), strftime()

void handle_sigint(int sig) {
	printf("\n%sCaught signal %d. Server shut down.%s\n", C_RED, sig, C_NC);
    exit(0);
}

void clear_screen(void)
{
    printf(ANSI_CLEAR);
}

void print_welcome_message(void)
{
	clear_screen();
	print_logo();
	printf("\t\t%sWelcome to wasup!\n", C_CYAN);
	printf("\tA chat in your terminal, (TUI chat).%s\n\n\n", C_NC);
}

void print_logo(void)
{
	printf("\n"
		"  %s╔═════════════════════════════════════════════╗%s\n"
		"  %s║%s ██╗    ██╗ █████╗ ███████╗██╗   ██╗██████╗  %s║%s\n"
		"  %s║%s ██║    ██║██╔══██╗██╔════╝██║   ██║██╔══██╗ %s║%s\n"
		"  %s║%s ██║ █╗ ██║███████║███████╗██║   ██║██████╔╝ %s║%s\n"
		"  %s║%s ██║███╗██║██╔══██║╚════██║██║   ██║██╔═══╝  %s║%s\n"
		"  %s║%s ╚███╔███╔╝██║  ██║███████║╚██████╔╝██║      %s║%s\n"
		"  %s║%s  ╚══╝╚══╝ ╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═╝      %s║%s\n"
		"  %s╠═════════════════════════════════════════════╣%s\n"
		"  %s║%s          A chating tui (v.%s)            %s║%s\n"
		"  %s╚═════════════════════════════════════════════╝%s\n\n",
		C_GREEN, C_GREEN,
		C_GREEN, C_CYAN, C_GREEN, C_GREEN,
		C_GREEN, C_CYAN, C_GREEN, C_GREEN,
		C_GREEN, C_CYAN, C_GREEN, C_GREEN,
		C_GREEN, C_CYAN, C_GREEN, C_GREEN,
		C_GREEN, C_CYAN, C_GREEN, C_GREEN,
		C_GREEN, C_CYAN, C_GREEN, C_GREEN,
		C_GREEN, C_GREEN,
		C_GREEN, C_YELLOW, CHAT_VERSION_NO, C_GREEN, C_GREEN,
		C_GREEN, C_NC
		);
}

void log_error(const char * msg)
{
	fprintf(stderr, "%s[ERROR] %s%s\n", C_RED, C_NC, msg);
}

void log_warn(const char * msg)
{
	printf("%s[WARN] %s%s\n", C_YELLOW, C_NC, msg);
}

void log_info(const char * msg)
{
	printf("%s[INFO] %s%s\n", C_CYAN, C_NC, msg);
}

void print_chat_message(const char* username, const char* msg)
{
	print_current_time(C_GRAY);
	printf("%s%s%s%s: %s\n", C_BOLD, C_GREEN, username, C_NC, msg);
}

void print_chat_prompt(const char* username)
{
    print_current_time(C_GRAY);

    // Format: [HH:MM:SS] username ❯
	printf("%s%s%s%s %s❯%s ",
        C_BOLD, C_CYAN, username, C_NC,
        C_YELLOW, C_NC);

    fflush(stdout);
}

void print_current_time(const char* color)
{
	char buffer[10];
	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
	print_chat_left_box();
	printf("%s[%s]%s ", color, buffer, C_NC);
}

int get_user_name(char* out_name, size_t size)
{
	printf("%s%s[CHAT]%s Enter user name %s❯%s ",
           C_YELLOW, C_BOLD, C_NC,
           C_YELLOW, C_NC);
    fflush(stdout);
	fflush(stdout);

	if ( !fgets(out_name, size, stdin) ) {
		log_error("Error(stdin): System error while reading from stdin.");
		return 1;
	}

	// clean new line
	out_name[strcspn(out_name, "\n")] = 0;

	// set default name
	if (strlen(out_name) == 0) {
		strncpy(out_name, "Anonymous", size);
	}

	return 0; // Success
}

void exchange_user_names(int fd, char* username, char* peername)
{
	send(fd, username, CHAT_USER_NAME_SIZE, 0);
	recv(fd, peername, CHAT_USER_NAME_SIZE, 0);
}

void print_chat_top_box(void)
{
	printf(C_YELLOW);
	printf(" %s", CBOX_TPLT);
	for (int i = 0; i < CBOX_WIDTH; ++i) {
		printf("%s", CBOX_HRZL);
	}
	printf("%s", CBOX_TPRR);
	printf(C_NC);
	printf("%s\n", C_NC);
}

void print_chat_left_box(void)
{
	printf(C_YELLOW);
	printf(" %s ", CBOX_LEFT);
	printf(C_NC);
}

void print_chat_bottom_box(void)
{
	printf(C_YELLOW);
	printf(" %s", CBOX_BTLT);
	for (int i = 0; i < CBOX_WIDTH; ++i) {
		printf("%s", CBOX_HRZL);
	}
	printf("%s", CBOX_BTRT);
	printf("%s\n", C_NC);
}

