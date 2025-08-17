#ifndef MYSH_H
#define MYSH_H

#include "mysh.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

// loop
void mysh_loop(void);

// I/O
char *mysh_read_line(void);
char **mysh_split_line(char *line);

// execution
int mysh_execute(char **args);

// builtins
int mysh_mkdir(char **args);
int mysh_pwd(char **args);

int mysh_cd(char **args);

void handle_redirection(char **args);


#endif
