#include "history.h"
#include <stdio.h>
#include <string.h>

#define MAX_HISTORY 100
#define MAX_CMD_LEN 1024

static char history[MAX_HISTORY][MAX_CMD_LEN];
static int history_count = 0;
static int current = -1; // used for navigation

void add_history(const char *cmd) {
    if (history_count < MAX_HISTORY) {
        strncpy(history[history_count], cmd, MAX_CMD_LEN - 1);
        history[history_count][MAX_CMD_LEN - 1] = '\0';
        history_count++;
    }
    current = history_count; // reset to latest
}

const char* get_prev_history() {
    if (current > 0) {
        current--;
        return history[current];
    }
    return NULL;
}

const char* get_next_history() {
    if (current < history_count - 1) {
        current++;
        return history[current];
    }
    return "";
}
