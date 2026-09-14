#include "log.h"
#include "constants.h"

#include <stdarg.h>		// va_list
#include <stdio.h>		// printf

static const char* log_tags[] = {
    [LOG_ERROR] = "ERROR",
    [LOG_WARN] = "WARN",
    [LOG_INFO] = "INFO",
    [LOG_DEBUG] = "DEBUG"
};

static const char* log_colors[] = {
    [LOG_ERROR] = C_RED,
    [LOG_WARN] = C_YELLOW,
    [LOG_INFO] = C_CYAN,
    [LOG_DEBUG] = C_GRAY
};

void log_msg(LogLevel level, const char* msg, ...)
{
    if (level >= LOG_COUNT) {
        return;
    }

    printf("%s[%s]%s ", log_colors[level], log_tags[level], C_NC);

    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);

    printf("\n");
}

