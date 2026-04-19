#ifndef CHAT_CONSTANTS_H
#define CHAT_CONSTANTS_H

// connection
static const char* const DEFAULT_PORT = "1313";
static const char* const LOCAL_HOST   = "127.0.0.1";

// clear screen
static const char* const ANSI_CLEAR = "\033[H\033[2J\033[3J";

// colors
static const char* const C_CYAN	 = "\033[36m";
static const char* const C_GREEN = "\033[32m";
static const char* const C_YELLO = "\033[33m";
static const char* const C_RED	 = "\033[31m";
static const char* const C_NC	 = "\033[0m";

#endif // !CHAT_CONSTANTS_H

