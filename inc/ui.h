#ifndef CHAT_UI_H
#define CHAT_UI_H

#include <stddef.h>		// size_t

/* clear terminal screen */
void clear_screen(void);

/* clear current terminal line */
void clear_current_line(void);

/* move up and clear previous terminal line */
void clear_above_line(void);

/* display startup screen with logo and welcome text */
void print_welcome_message(void);

/* print application ascii art logo */
void print_logo(void);

/* handle interrupt signal to exit cleanly */
void handle_sigint(int sig);

/* print top decorative border for chat box */
void print_chat_top_box(void);

/* print left decorative border for chat line */
void print_chat_left_box(void);

/* print bottom decorative border for chat box */
void print_chat_bottom_box(void);

/* parse raw string into username and message parts */
void split_message(char* raw_msg, char** username, char** msg);

/* print current timestamp with color */
void print_current_time(const char* color);

/* format and display incoming chat message */
void print_chat_message(char* raw_msg);

/* display interactive input prompt for user */
void print_chat_message_prompt(const char* username);

/* display notification when user join or leaves room */
void print_user_event(const char* username, const char* event);

/* update and display number of online users in room */
void print_room_count(int n);

/* display room name/number in chat header */
void print_room_header(char* room);

/* save current terminal cursor coordinates */
void save_cursor_position(void);

/* move terminal cursor to specific x and y */
void move_cursor_to_position(int y, int x);

/* restore terminal cursor to last saved position */
void restore_cursor_position(void);

/* print generic system info input prompt */
void print_info_prompt(const char* label);

/* clear remaining characters from stdin buffer */
void flush_stdin_line(const char* buffer, size_t size);

/* remove newline from string or use default */
void strip_new_line_or_fallback(char* buffer, size_t size, const char* fallback);

/* generic helper to prompt and read user input */
int get_user_input(const char* label, char* out, size_t size, const char* fallback);

/* read chat message from user stdin */
int chat_get_input_message(char* buffer);

/* prompt and read user name */
int chat_get_input_username(char* out, size_t size);

/* prompt and read room selection */
int chat_get_input_room(char* out, size_t size);

#endif // !CHAT_UI_H
