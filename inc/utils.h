#ifndef CHAT_UTILS_H
#define CHAT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#include <stddef.h>		// size_t

/* trap ctl-c */
void handle_sigint(int sig);

/* send ansii clear screen */
void clear_screen(void);

/* prints a welcom messge at the start of the program */
void print_welcome_message(void);

/* Prints ascii art box with the app name */
void print_logo(void);

/* print error messge */
void log_error(const char * msg, ...);

/* print warning messge */
void log_warn(const char * msg);

/* print error messge */
void log_info(const char * msg, ...);

/* prompt the user for info input like username or  room number */
int get_user_input(const char *label, char *out, size_t size, const char *fallback);

/* split username and message framed as username:msg */
void split_message(char* raw_msg, char** username, char** msg);

/* print a messge to the screen */
void print_chat_message(char* raw_msg);

/* print a prompt to user to type message */
void print_chat_prompt(const char* username);

/* print the current time in a given color */
void print_current_time(const char* color);

/* print the top border of the chat */
void print_chat_top_box(void);

/* print the top border of the chat */
void print_chat_left_box(void);

/* print the top border of the chat */
void print_chat_bottom_box(void);

/* save  the cursor position so we can restore later */
void save_cursor_position(void);

/* move the cursor to a given position on the screen */
void move_cursor_to_position(int y, int x);

/* move the cursor to the last saved position */
void restore_cursor_position(void);

/* print the current clients in the room */
void print_room_count(int n);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !CHAT_UTILS_H

