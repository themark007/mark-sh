#ifndef HISTORY_H
#define HISTORY_H

void add_history(const char *cmd);
const char* get_prev_history();
const char* get_next_history();

#endif
