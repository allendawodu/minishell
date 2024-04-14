#include <stdio.h>

#include "definitions.h"
#include "functions.h"

// Entry point for program
int main(int argc, char *argv[])
{
    int pid;  // Variable to store the process ID
    char command_line[MAX_LINE_LEN];  // Array to store the command line input
    char *pathv[MAX_PATHS];  // Array of pointers to hold directories where commands can be found

    struct command_t command;  // Structure to hold parsed command information

    parse_path(pathv);  // Function to parse the system path and fill pathv with directories to search for executables

    while (1) {  // Infinite loop to continuously prompt for commands
        print_prompt();  // Display the command prompt

        read_command(command_line);  // Read a command from the user into command_line
        parse_command(command_line, &command);  // Parse the command_line into the command structure

        if (strcmp(command_line, "exit") == 0) {  // Check if the command is 'exit'
            exit(0);  // Exit the program if command is 'exit'
        }

        command.argv[command.argc] = NULL;  // Ensure the argument vector is properly terminated with NULL
        command.name = lookup_path(command.argv, pathv);  // Lookup the full path of the command to execute

        if ((pid = fork()) == 0) {  // Fork a child process. Check if we are in the child process.
            execv(command.name, command.argv);  // Replace the child process with the command to be executed
        }

        wait(NULL);  // Parent process waits here for the child process to complete
    }

    return 0;  // End of program (not reachable due to infinite loop)
}
