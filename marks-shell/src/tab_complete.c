#include "../include/tab_complete.h"
#include "../include/mysh.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

// List of built-in commands
const char *builtins[] = {
    "pwd", "ls", "mkdir", "cd", "exit", NULL
};

// Helper: match built-in commands
static int match_builtin(const char *prefix, char *match) {
    for (int i = 0; builtins[i] != NULL; i++) {
        if (strncmp(prefix, builtins[i], strlen(prefix)) == 0) {
            strcpy(match, builtins[i]);
            return 1;
        }
    }
    return 0;
}

// Helper: match file/folder names
static int match_file(const char *prefix, char *match) {
    DIR *dir = opendir(".");
    if (!dir) return 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strncmp(prefix, entry->d_name, strlen(prefix)) == 0) {
            strcpy(match, entry->d_name);
            closedir(dir);
            return 1;
        }
    }
    closedir(dir);
    return 0;
}

// Main tab completion function
void tab_complete(char *buffer, int *pos) {
    char match[1024];

    // First try builtins
    if (match_builtin(buffer, match)) {
        strcpy(buffer, match);
        *pos = strlen(buffer);
        return;
    }

    // Then try files/folders
    if (match_file(buffer, match)) {
        strcpy(buffer, match);
        *pos = strlen(buffer);
        return;
    }

    // If no match, do nothing
}
