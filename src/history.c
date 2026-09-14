#include "history.h"

#include "constants.h"
#include "chat_utils.h"

#include <stdio.h>      // snprintf
#include <time.h>

void history_init(ChatHistory* history, int room_id)
{
    history->room_id = room_id;
    history->_capacity = HISTORY_SIZE;
    history->_last = -1;
    history->_size = 0;
}

void history_update_msg(ChatHistory* history, char* name, char* msg)
{
    HistoryItem item;

    sprintf(item.msg, "%s", msg);
    sprintf(item.user, "%s", name);
    item.timestamp = time(NULL);

    history->_last = (history->_last + 1) % history->_capacity;
    history->items[history->_last] = item;
    history->_size = history->_size == history->_capacity ? history->_capacity : history->_size + 1;
}

void history_notify_client(ChatHistory* history, int fd)
{
    char data[PAYLOAD_MAX_SIZE];

    int idx = (history->_last - history->_size + 1 + history->_capacity) % history->_capacity;
    for (int i = 0; i < history->_size; ++i) {

        HistoryItem item = history->items[idx];
        snprintf(data, sizeof(data), "%lld:%s:%s", (long long)item.timestamp, item.user, item.msg);

        chat_notify_client(fd, NOTIFY_HISTORY_MSG, data);

        idx = (idx + 1) % history->_capacity;
    }
}

