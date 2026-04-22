#ifndef CHAT_CONSTANTS_H
#define CHAT_CONSTANTS_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define CHAT_VERSION_NO		 "1.3.2"
#define CHAT_DEFAULT_PORT	 "1313"
#define CHAT_LOCAL_HOST		 "127.0.0.1"
#define CHAT_LISTEN_BACKLOG  1

enum ChatLimits {
	CHAT_USER_NAME_SIZE  = 20,
	CHAT_MSG_BUFFER_SIZE = 2048,
	CHAT_MAX_CLIENTS	 = 16,
	CHAT_ROOM_SIZE		 = 8,
	CHAT_MAX_ROOMS		 = 100
};

// colors
#define ANSI_CLEAR  "\033[H\033[2J\033[3J"	// clear screen
#define ANSI_CLEAR_LINE  "\r\033[K"			// clear line
#define C_CYAN	 "\033[36m"					// color cyan
#define C_GREEN  "\033[32m"					// color green
#define C_YELLOW "\033[33m"					// color yellow
#define C_RED	 "\033[31m"					// color red
#define C_GRAY	 "\033[37m"					// color gray
#define C_BOLD	 "\033[1m"					// bold
#define C_NC	 "\033[0m"					// no color

// chat box
#define CBOX_WIDTH 45
#define CBOX_LEFT  "┃"						// chat box left
#define CBOX_HRZL  "━"    					// chat box middle
#define CBOX_TPLT  "┏"    					// chat box top left
#define CBOX_TPRR  "┓"    					// chat box top right
#define CBOX_BTLT  "┗"    					// chat box bottom left
#define CBOX_BTRT  "┛"    					// chat box bottom right

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !CHAT_CONSTANTS_H

