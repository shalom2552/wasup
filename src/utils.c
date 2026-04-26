#include "utils.h"
#include "constants.h"

#include <stdarg.h>		// va_list
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

void clear_above_line(void)
{
	printf(ANSI_MOVE_UP_ONCE ANSI_CLEAR_LINE);
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

void log_error(const char * msg, ...)
{
	va_list args;
	va_start(args, msg);
	printf("%s[ERROR]%s ", C_RED, C_NC);
	vprintf(msg, args);
	printf("\n");
	va_end(args);
}

void log_warn(const char * msg)
{
	printf("%s[WARN] %s%s\n", C_YELLOW, C_NC, msg);
}

void log_info(const char * msg, ...)
{
	va_list args;
	va_start(args, msg);
	printf("%s[INFO]%s ", C_CYAN, C_NC);
	vprintf(msg, args);
	printf("\n");
	va_end(args);
}

void split_message(char* raw_msg, char** username, char** msg)
{
    char *colon = strchr(raw_msg, ':');
    if (!colon) {
        *username = "";
        *msg = raw_msg;
        return;
    }
    *colon = '\0';
    *username = (char *)raw_msg;
    *msg = colon + 1;
}

void print_chat_message(char* raw_msg)
{
	char* username;
	char* msg;
	split_message(raw_msg, &username, &msg);
	print_current_time(C_GRAY);
	printf("%s%s%s%s: %s\n", C_BOLD, C_GREEN, username, C_NC, msg);
}

void print_chat_message_prompt(const char* username)
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

void print_info_prompt(const char* label)
{
    printf("%s%s[CHAT]%s %s %s❯%s ",
           C_YELLOW, C_BOLD, C_NC, label,
           C_YELLOW, C_NC);
    fflush(stdout);
}

void flush_stdin_line(const char* buffer, size_t size)
{
    if ( !memchr(buffer, '\n', size)) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

void strip_new_line_or_fallback(char* buffer, size_t size, const char* fallback)
{
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) == 0 && fallback) {
        strncpy(buffer, fallback, size - 1);
        buffer[size - 1] = '\0';
    }
}

int get_user_input(const char *label, char *out, size_t size, const char *fallback)
{
    print_info_prompt(label);

    // get input
    if (!fgets(out, size, stdin)) {
        log_error("Error(stdin): System error while reading from stdin.");
        return 1;
    }

    flush_stdin_line(out, size);
    strip_new_line_or_fallback(out, size, fallback);

    clear_above_line();
    return 0;
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

void save_cursor_position(void)
{
    printf(ANSI_SAVE_CURSUR);
    fflush(stdout);
}

void move_cursor_to_position(int y, int x)
{
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
}

void restore_cursor_position(void)
{
    printf(ANSI_RESTORE_CURSUR);
    fflush(stdout);
}

void print_room_count(int n)
{
    save_cursor_position();
    move_cursor_to_position(15, 20);
    printf("Users in room: %d", n);
    restore_cursor_position();
}

