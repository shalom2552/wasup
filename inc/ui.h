#ifndef CHAT_UI_H
#define CHAT_UI_H

#include <stddef.h>		// size_t

/* display startup screen with logo and welcome text */
void ui_print_welcome_message(void);

/* print application ascii art logo */
void ui_print_logo(void);

/* print top decorative border for chat box */
void ui_print_chat_top_box(void);

/* print bottom decorative border for chat box */
void ui_print_chat_bottom_box(void);

/* print given time in specified color */
void ui_print_time(const char* time, const char* color);

/* print current timestamp with color */
void ui_print_current_time(const char* color);

/* format and display incoming chat message */
void ui_print_chat_message(char* raw_msg);

/* format and display history message */
void ui_print_chat_history_message(char* raw_msg);

/* display interactive input prompt for user */
void ui_print_chat_message_prompt(const char* username);

/* display notification when user join or leaves room */
void ui_print_user_event(const char* username, const char* event);

/* update and display number of online users in room */
void ui_print_room_count(int n);

/* display room name/number in chat header */
void ui_print_room_header(char* room);

/* read chat message from user stdin */
int ui_chat_get_input_message(char* buffer);

/* prompt and read user name */
int ui_chat_get_input_username(char* out, size_t size);

/* prompt and read room selection */
int ui_chat_get_input_room(char* out, size_t size);

#endif // !CHAT_UI_H
