#include "../include/mysh.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   // for chdir(), getcwd()
#include <limits.h>   // for PATH_MAX
#include <errno.h>

#define HOME_DIR "/Users/markdsouza"  // <-- Change this to your username path

char prev_dir[PATH_MAX];  // store previous directory

int mysh_cd(char **args) {
    char *target_dir = args[1];
    char cwd[PATH_MAX];

    // save current dir before changing
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return 1;
    }

    if (target_dir == NULL || strcmp(target_dir, "~") == 0) {
        // case: cd or cd ~
        if (chdir(HOME_DIR) != 0) {
            perror("cd");
            return 1;
        }
    } else if (strcmp(target_dir, "-") == 0) {
        // case: cd -
        if (strlen(prev_dir) == 0) {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return 1;
        }
        if (chdir(prev_dir) != 0) {
            perror("cd");
            return 1;
        }
        printf("%s\n", prev_dir); // print the dir you moved into
    } else {
        // case: cd <path>
        if (chdir(target_dir) != 0) {
            perror("cd");
            return 1;
        }
    }

    // update prev_dir after successful cd
    strcpy(prev_dir, cwd);

    return 1; // continue shell
}






int mysh_mkdir(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "marksh: expected argument to \"mkdir\"\n");
        return 1;
    }

    // mode_t sets permission bits: rwxr-xr-x
    mode_t mode = 0755;

    // Try to create directory
    if (mkdir(args[1], mode) != 0) {
        perror("marksh: mkdir failed");
    }

    return 1;
}

int mysh_pwd(char **args) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("marksh: pwd");
    }
    return 1;
}
