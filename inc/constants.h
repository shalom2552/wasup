#ifndef CHAT_CONSTANTS_H
#define CHAT_CONSTANTS_H

// server notifications type
typedef enum {
    NOTIFY_NEW_MSG = 0,
    NOTIFY_ROOM_COUNT,
    NOTIFY_USER_JOIN,
    NOTIFY_USER_LEFT,
    NOTIFY_HISTORY_MSG
} NotifyCode;

#define CHAT_VERSION_NO		 "1.4.3"
#define CHAT_DEFAULT_PORT	 "1313"
#define CHAT_LOCAL_HOST		 "127.0.0.1"

#define HISTORY_SIZE       50

#define TIME_FMT "%H:%M:%S"
#define TIME_SIZE 9
#define TIMESTAMP_SIZE 20

#define CHAT_USER_NAME_SIZE         20
#define CHAT_MSG_BUFFER_SIZE        2048
#define CHAT_MAX_CLIENTS            64
#define CHAT_ROOM_SIZE              8
#define CHAT_MAX_ROOMS              16
#define CHAT_NOTIFY_PAYLOAD_SIZE    32

#define PAYLOAD_MAX_SIZE (TIMESTAMP_SIZE + 1 + CHAT_USER_NAME_SIZE + 1 + CHAT_MSG_BUFFER_SIZE)

// terminal
#define ANSI_CLEAR  "\033[H\033[2J\033[3J"	// clear screen
#define ANSI_CLEAR_LINE  "\r\033[K"			// clear line
#define ANSI_SAVE_CURSUR "\0337"            // save cursor position
#define ANSI_RESTORE_CURSUR "\0338"         // restore cursor position
#define ANSI_MOVE_UP_ONCE   "\033[A"        // move up one line

// colors
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

#endif // !CHAT_CONSTANTS_H

