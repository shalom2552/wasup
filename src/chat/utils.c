#include "chat/utils.h"
#include "chat/constants.h"

#include <stdio.h>		// printf(), fprintf
#include <string.h>		// strncmp()

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
		"  %s║%s          A chating tui (v.0.0.4)            %s║%s\n"
		"  %s╚═════════════════════════════════════════════╝%s\n\n",
		C_GREEN, C_GREEN,
		C_GREEN, C_CYAN, C_GREEN, C_GREEN,
		C_GREEN, C_CYAN, C_GREEN, C_GREEN,
		C_GREEN, C_CYAN, C_GREEN, C_GREEN,
		C_GREEN, C_CYAN, C_GREEN, C_GREEN,
		C_GREEN, C_CYAN, C_GREEN, C_GREEN,
		C_GREEN, C_CYAN, C_GREEN, C_GREEN,
		C_GREEN, C_GREEN,
		C_GREEN, C_YELLO, C_GREEN, C_GREEN,
		C_GREEN, C_NC
		);
}

void print_error(const char * msg)
{
	fprintf(stderr, "%s[ERROR]: %s%s\n", C_RED, C_NC, msg);
}

void print_info(const char * msg)
{
	printf("%s[INFO]: %s%s\n", C_CYAN, C_NC, msg);
}

int chat_exit_messege(const char* msg)
{
	if (strstr(msg, "/exit") || strstr(msg, "/quit") || strstr(msg, "/q"))
	{
		return 1;
	}
	return 0;
}

void print_client_message(const char* msg)
{
	printf("%sClient:%s %s\n", C_GREEN, C_NC, msg);
}

void print_server_message(const char* msg)
{
	printf("%sServer:%s %s\n", C_YELLO, C_NC, msg);
}

void print_chat_prompt(void)
{
	printf("%sType message:%s ", C_NC, C_NC);
}

