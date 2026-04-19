#ifndef CHAT_UTILS_H
#define CHAT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

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

/* print the client messge to the screen */
void print_client_message(const char* msg);

/* print the server messge to the screen */
void print_server_message(const char* msg);

/* print a prompt to user to type message */
void print_chat_prompt(void);

/* terminate fd connection */
void chat_disconnect(int fd);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !CHAT_UTILS_H

