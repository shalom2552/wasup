#ifndef CHAT_LOG_H
#define CHAT_LOG_H

// when adding a level, also add it's tag and color in log.c
typedef enum LogLevel {
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_COUNT
} LogLevel;

/* print formatted log message */
void log_msg(LogLevel level, const char* msg, ...);

#define log_info(...) log_msg(LOG_INFO, __VA_ARGS__)
#define log_debug(...) log_msg(LOG_DEBUG, __VA_ARGS__)
#define log_warn(...) log_msg(LOG_WARN, __VA_ARGS__)
#define log_error(...) log_msg(LOG_ERROR, __VA_ARGS__)

#endif // !CHAT_LOG_H

