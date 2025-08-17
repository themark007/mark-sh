#include "../include/mysh.h"
#include <stdio.h>
#include <signal.h>

// Signal handler for Ctrl+C
void sigint_handler(int sig) {
    printf("\nmarksh> ");  // Print prompt again
    fflush(stdout);         // Flush output immediately
}

int main() {
    printf("Welcome to Mark Shell (marksh)!\n");

    // Register Ctrl+C handler
    signal(SIGINT, sigint_handler);

    // Start shell loop
    mysh_loop();

    return 0;
}
