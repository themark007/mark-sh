#include "../include/mysh.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct CommandNode {
    char *cmd;
    char type; // '&' for &&, '|' for ||, '0' for last command
    struct CommandNode *next;
} CommandNode;


CommandNode* split_chained_commands(char *line) {
    CommandNode *head = NULL, *tail = NULL;

    char *start = line;
    while (*start) {
        char *pos_and = strstr(start, "&&");
        char *pos_or  = strstr(start, "||");

        char *pos = NULL;
        char type = '0';

        if (pos_and && (!pos_or || pos_and < pos_or)) {
            pos = pos_and;
            type = '&';
        } else if (pos_or) {
            pos = pos_or;
            type = '|';
        }

        CommandNode *node = malloc(sizeof(CommandNode));
        if (pos) {
            *pos = '\0';
            node->cmd = strdup(start);
            node->type = type;
            start = pos + 2;
        } else {
            node->cmd = strdup(start);
            node->type = '0';
            start += strlen(start);
        }

        node->next = NULL;
        if (!head) head = tail = node;
        else { tail->next = node; tail = node; }
    }

    return head;
}

void mysh_loop(void) {
    char *line;
    char **args;
    int status;

    while (1) {
        printf("marksh> ");
        line = mysh_read_line();

        CommandNode *head = split_chained_commands(line);
        CommandNode *curr = head;
        int last_status = 0;

        while (curr) {
            args = mysh_split_line(curr->cmd);

            // Skip based on chaining type
            if (curr->type == '&' && last_status != 0) {
                free(curr->cmd);
                free(args);
                curr = curr->next;
                continue;
            }
            if (curr->type == '|' && last_status == 0) {
                free(curr->cmd);
                free(args);
                curr = curr->next;
                continue;
            }

            last_status = mysh_execute(args);

            free(curr->cmd);
            free(args);
            curr = curr->next;
        }

        // Free CommandNode list
        while (head) {
            CommandNode *tmp = head;
            head = head->next;
            free(tmp);
        }

        free(line);
    }
}

