#include "log.h"
#include "constants.h"

#include <stdarg.h>		// va_list
#include <stdio.h>		// printf()

void log_error(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	printf("%s[ERROR]%s ", C_RED, C_NC);
	vprintf(msg, args);
	printf("\n");
	va_end(args);
}

void log_warn(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	printf("%s[WARN]%s ", C_YELLOW, C_NC);
	vprintf(msg, args);
	printf("\n");
	va_end(args);
}

void log_info(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	printf("%s[INFO]%s ", C_CYAN, C_NC);
	vprintf(msg, args);
	printf("\n");
	va_end(args);
}
