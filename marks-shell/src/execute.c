#include "../include/mysh.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int mysh_execute(char **args) {
    if (args[0] == NULL) {
        return 1; // empty command
    }

    // Builtins
    if (strcmp(args[0], "cd") == 0) {
        return mysh_cd(args);
    }
    if (strcmp(args[0], "mkdir") == 0) {
        return mysh_mkdir(args);
    }
    if (strcmp(args[0], "pwd") == 0) {
        return mysh_pwd(args);
    }

    if (strcmp(args[0], "exit") == 0) {
        exit(EXIT_SUCCESS); // exit shell
    }

    // Otherwise fork + exec
    pid_t pid = fork();
    if (pid == 0) {
        // child process

        // Handle I/O redirection
        handle_redirection(args);

        // Execute the command
        if (execvp(args[0], args) == -1) {
            perror("mysh");
        }
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // parent process waits
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("mysh");
    }

    return 1;
}
