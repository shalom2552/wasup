#ifndef CHAT_LOG_H
#define CHAT_LOG_H

/* print formatted error message in red */
void log_error(const char* msg, ...);

/* print formatted warning message in yellow */
void log_warn(const char* msg, ...);

/* print formatted info message in cyan */
void log_info(const char* msg, ...);

#endif // !CHAT_LOG_H
