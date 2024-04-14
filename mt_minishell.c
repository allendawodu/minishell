#include "definitions.h"   // Include header for definitions and global declarations
#include "functions.h"     // Include header for custom function declarations

#include <unistd.h>        // Include for POSIX operating system API (e.g., fork, exec)
#include <pthread.h>       // Include for POSIX threads

struct command_t command;  // Global declaration of a command structure to hold command data
void *exec_wrapper(void *arg);  // Prototype for a thread function

int main() {
    char command_line[MAX_LINE_LEN];  // Buffer to hold the command line input
    char *pathv[MAX_PATHS];  // Array to store paths where executables can be found
    
    pthread_t cmd_exec_thread;  // Thread identifier for the command execution thread

    parse_path(pathv);  // Initialize pathv with directories from the system path

    while(1 == 1) {  // Infinite loop for continuous command processing
        print_prompt();  // Print the shell prompt

        read_command(command_line);  // Read a command from stdin into command_line
        parse_command(command_line, &command);  // Parse the input into the command struct

        pthread_create(&cmd_exec_thread, NULL, exec_wrapper, &command);  // Create a new thread to execute the command

        command.argv[command.argc] = NULL;  // Properly terminate the argument vector
        command.name = lookup_path(command.argv, pathv);  // Resolve the full pathname of the command

        pthread_join(cmd_exec_thread, NULL);  // Wait for the command execution thread to finish
    }

    return 0;
}

void *exec_wrapper(void *arg) {
    struct command_t *cmd = (struct command_t *)arg;  // Cast arg to command_t pointer and execute the command
    execv(cmd->name, cmd->argv);  // Replace the thread's process image with the command
    return NULL;  // Thread terminates after execv if execv fails
}
