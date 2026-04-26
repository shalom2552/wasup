#include "ui.h"
#include "log.h"
#include "chat_utils.h"
#include "constants.h"

#include <stddef.h>		// size_t
#include <stdio.h>		// printf(), fprintf
#include <string.h>		// strncmp()
#include <stdlib.h>		// exit()
#include <time.h>		// time_t, struct tm, time(), localtime(), strftime()

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

void handle_sigint(int sig) {
	printf("\n%sCaught signal %d. Server shut down.%s\n", C_RED, sig, C_NC);
    exit(0);
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

void split_message(char* raw_msg, char** username, char** msg)
{
    char* colon = strchr(raw_msg, ':');
    if (!colon) {
        *username = "";
        *msg = raw_msg;
        return;
    }
    *colon = '\0';
    *username = (char*)raw_msg;
    *msg = colon + 1;
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

void print_room_count(int n)
{
    int y = 16;
    int x = 3;
    save_cursor_position();
    move_cursor_to_position(y, x);
    printf("%sonline: ", C_YELLOW);
    printf("%s%d%s",C_GREEN, n, C_NC);
    restore_cursor_position();
}

void print_room_header(char* room)
{
    int y = 17;
    int x = 20;
    save_cursor_position();
    move_cursor_to_position(y, x);
    printf("%s< room %s >%s", C_YELLOW, room, C_NC);
    restore_cursor_position();
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

int get_user_input(const char* label, char* out, size_t size, const char* fallback)
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

int chat_get_input_message(char* buffer)
{
	if ( !fgets(buffer, CHAT_MSG_BUFFER_SIZE, stdin) ) {
		log_error("Error(stdin): System error while reading from stdin.");
		return 1;
	}
	buffer[strcspn(buffer, "\n")] = 0;
	return 0;
}

int chat_get_input_username(char* out, size_t size)
{
    return get_user_input("Enter user name", out, size, "Anonymous");
}

int chat_get_input_room(char* out, size_t size)
{
    char label[36];
    snprintf(label, sizeof(label), "Enter room number [1-%d]", CHAT_MAX_ROOMS);
    if (get_user_input(label, out, size, "0")) {
        return 1;
    }
    int room = validate_room_input(out);
    snprintf(out, size, "%d", room);
    return 0;
}
