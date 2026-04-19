#ifndef CHAT_UTILS_H
#define CHAT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#include <stddef.h>		// size_t

extern int G_SOCKFD;

/* trap ctl-c */
void handle_sigint(int sig);

/* send ansii clear screen */
void clear_screen(void);

/* prints a welcom messge at the start of the program */
void print_welcome_message(void);

/* Prints ascii art box with the app name */
void print_logo(void);

/* print error messge */
void print_error(const char * msg);

/* print error messge */
void print_info(const char * msg);

/* return 1 if msg is [/exit|/quit|/q] or 0 otherwise */
int chat_exit_messege(const char* msg);

/* print a messge to the screen */
void print_chat_message(const char* username, const char* msg);

/* print a prompt to user to type message */
void print_chat_prompt(const char* username);

/* terminate fd connection */
void chat_disconnect(int fd);

/* print the current time in a given color */
void print_current_time(const char* color);

/* get username return 0 on failure */
int get_user_name(char* out_name, size_t size);

/* exchange user names between peers */
void exchange_user_names(int fd, char* username, char* peername);

/* print the top border of the chat */
void print_chat_top_box(void);

/* print the top border of the chat */
void print_chat_left_box(void);

/* print the top border of the chat */
void print_chat_bottom_box(void);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !CHAT_UTILS_H

