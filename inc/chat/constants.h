#ifndef CHAT_CONSTANTS_H
#define CHAT_CONSTANTS_H

static const char* const VERSION_NO = "1.3.2";

// connection
static const char* const DEFAULT_PORT = "1313";
static const char* const LOCAL_HOST   = "127.0.0.1";

// user name string max size
static const int UNAME_SIZE = 20;

// clear screen
static const char* const ANSI_CLEAR = "\033[H\033[2J\033[3J";

// colors
static const char* const C_CYAN	 = "\033[36m";
static const char* const C_GREEN = "\033[32m";
static const char* const C_YELLO = "\033[33m";
static const char* const C_RED	 = "\033[31m";
static const char* const C_GRAY   = "\033[37m";
static const char* const C_BOLD   = "\033[1m";
static const char* const C_NC	 = "\033[0m";

// chat box
static const int CBOX_WIDTH = 60;
static const char* const CBOX_LEFT = "┃";    // chat box left
static const char* const CBOX_HRZL = "━";    // chat box middle
static const char* const CBOX_TPLT = "┏";    // chat box top left
static const char* const CBOX_TPRR = "┓";    // chat box top right
static const char* const CBOX_BTLT = "┗";    // chat box bottom left
static const char* const CBOX_BTRG = "┛";    // chat box bottom right

#endif // !CHAT_CONSTANTS_H

