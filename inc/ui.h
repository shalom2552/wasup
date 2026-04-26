#ifndef CHAT_UI_H
#define CHAT_UI_H

#include <stddef.h>		// size_t

/* send ansii clear screen */
void clear_screen(void);

/* send ansi codes to go up line and clear the line */
void clear_above_line(void);

/* prints a welcom messge at the start of the program */
void print_welcome_message(void);

/* Prints ascii art box with the app name */
void print_logo(void);

/* trap ctl-c */
void handle_sigint(int sig);

/* print the top border of the chat */
void print_chat_top_box(void);

/* print the top border of the chat */
void print_chat_left_box(void);

/* print the top border of the chat */
void print_chat_bottom_box(void);

/* split username and message framed as username:msg */
void split_message(char* raw_msg, char** username, char** msg);

/* print the current time in a given color */
void print_current_time(const char* color);

/* print a messge to the screen */
void print_chat_message(char* raw_msg);

/* print a prompt to user to type message */
void print_chat_message_prompt(const char* username);

/* print the current clients in the room */
void print_room_count(int n);

/* save  the cursor position so we can restore later */
void save_cursor_position(void);

/* move the cursor to a given position on the screen */
void move_cursor_to_position(int y, int x);

/* move the cursor to the last saved position */
void restore_cursor_position(void);

/* print prompt system info input from user */
void print_info_prompt(const char* label);

/* flush stdin for leftovers */
void flush_stdin_line(const char* buffer, size_t size);

/* replace new line with terminator or if the buffer is empty set the fallback */
void strip_new_line_or_fallback(char* buffer, size_t size, const char* fallback);

/* prompt the user for info input like username or  room number */
int get_user_input(const char* label, char* out, size_t size, const char* fallback);

/* prompt a user for a message and save to buffer */
int chat_get_input_message(char* buffer);

/* get username return 0 on failure */
int chat_get_input_username(char* out, size_t size);

/* get chat room return 0 on failure */
int chat_get_input_room(char* out, size_t size);

#endif // !CHAT_UI_H
