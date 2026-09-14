#ifndef CHAT_HISTORY_H
#define CHAT_HISTORY_H

#include "constants.h"
#include <time.h>

typedef struct {
    time_t timestamp;
    char user[CHAT_USER_NAME_SIZE];
    char msg[CHAT_MSG_BUFFER_SIZE];
} HistoryItem;

typedef struct {
    HistoryItem items[HISTORY_SIZE];

    int _last;
    int _size;
    int _capacity;
} ChatHistory;


/* Initializes the chat history for a given room */
void history_init(ChatHistory* history);

/* Inserts a new history item into the chat history */
void history_save_msg(ChatHistory* history, char* name, char* msg);

/* Notify a client with the latest chat history */
void history_notify_client(ChatHistory* history, int fd);

#endif // !CHAT_HISTORY_H

