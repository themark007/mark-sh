#include "../include/mysh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include "tab_complete.h"


#include "history.h"  // <-- add this

#define MAX_CMD_LEN 1024

// enable raw mode
static void enable_raw_mode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// disable raw mode
static void disable_raw_mode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

char *mysh_read_line(void) {
    static char buffer[MAX_CMD_LEN];
    int pos = 0;
    char c;

    
    fflush(stdout);

    enable_raw_mode();

    while (1) {
        c = getchar();

        if (c == '\n') {  // Enter
            buffer[pos] = '\0';
            printf("\n");
            break;
        }
        else if (c == 27) { // ESC sequence
            getchar(); // skip '['
            switch(getchar()) {
                case 'A': { // UP arrow
                    const char* prev = get_prev_history();
                    if (prev) {
                        printf("\33[2K\rmysh> %s", prev); // clear line + reprint
                        strcpy(buffer, prev);
                        pos = strlen(buffer);
                    }
                    break;
                }
                case 'B': { // DOWN arrow
                    const char* next = get_next_history();
                    if (next) {
                        printf("\33[2K\rmysh> %s", next);
                        strcpy(buffer, next);
                        pos = strlen(buffer);
                    }
                    break;
                }
            }
        }else if (c == '\t') {  // Tab key
    buffer[pos] = '\0';
    tab_complete(buffer, &pos);   // fill buffer with match
    printf("\33[2K\rmarksh> %s", buffer); // clear line + print
    fflush(stdout);
}else if (c == 127 || c == 8) { // Backspace
    if (pos > 0) {
        pos--;
        buffer[pos] = '\0';
        printf("\b \b");  // erase character visually
        fflush(stdout);
    }
} else { // normal character
            if (pos < MAX_CMD_LEN - 1) {
                buffer[pos++] = c;
                putchar(c);
            }
        }
    }

    disable_raw_mode();

    if (strlen(buffer) > 0) {
        add_history(buffer); // save to history
    }

    // allocate new memory so caller can free it like getline()
    return strdup(buffer);
}
