// Function to print the command prompt in the format [username@hostname current_directory]$
void print_prompt() {
    char machine_name[1024];  // Buffer to hold the hostname
    char current_dir[1024];  // Buffer to hold the current directory path
    char *username;  // Pointer to hold the username

    struct passwd *pwd;  // Pointer to struct holding user database entry
    pwd = getpwuid(getuid());  // Get user database entry for the current user
    username = pwd->pw_name;  // Extract username from the entry

    gethostname(machine_name, sizeof(machine_name));  // Get the hostname of the machine
    getcwd(current_dir, sizeof(current_dir));  // Get the current working directory
    printf("[%s@%s %s]$ ", username, machine_name, current_dir);  // Print the formatted prompt
}

// Function to read a command from standard input
void read_command(char *buffer) {
    fgets(buffer, MAX_ARG_LEN, stdin);  // Read a line from stdin into buffer
}

// Function to parse the command line into arguments
int parse_command(char *cl, struct command_t *cmd) {
    char **cl_ptr;  // Pointer to pointer for command line to assist parsing
    int argc;  // Counter for arguments

    argc = 0;  // Initialize argument count
    cl_ptr = &cl;  // Point cl_ptr to the command line string
    cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);  // Allocate memory for the first argument

    // Parse the command line into tokens based on whitespace
    while ((cmd->argv[argc] = strsep(cl_ptr, WHITESPACE)) != NULL) {
        cmd->argv[++argc] = (char*)malloc(MAX_ARG_LEN);  // Allocate memory for the next argument
    }

    cmd->argc = argc - 1;  // Set the number of arguments
    cmd->name = (char *) malloc(sizeof(cmd->argv[0]));  // Allocate memory for the command name
    strcpy(cmd->name, cmd->argv[0]);  // Copy the first argument as the command name
    return 1;  // Return success
}

// Function to parse the system PATH environment variable
int parse_path(char* dirs[]) {  
    char *path;  // Copy of the PATH variable for manipulation
    char *path_env;  // Pointer to hold the PATH environment variable

    for (int i = 0; i < MAX_ARGS; i++)
        dirs[i] = NULL;  // Initialize all entries in dirs to NULL

    path_env = (char *) getenv("PATH");  // Get the PATH environment variable
    path = (char *) malloc(strlen(path_env) + 1);  // Allocate memory for path

    strcpy(path, path_env);  // Copy the PATH variable

    char *parse = strtok(path, ":");  // Tokenize the path on ':'
    int parsed_string = 0;  // Counter for parsed directories

    // Iterate over all tokens and store them in dirs
    while (parse != NULL) {
        dirs[parsed_string] = malloc(strlen(parse) + 1);  // Allocate memory for a directory
        strcpy(dirs[parsed_string], parse);  // Copy the directory
        parsed_string++;  // Increment the counter
        parse = strtok(NULL, ":");  // Get the next token
    }

    return parsed_string;  // Return the number of parsed directories
}

// Function to find the full path of a command
char *lookup_path(char **argv, char **dir) {
    char *resultant;  // Pointer to hold the resultant
    char path_name[MAX_PATH_LEN];  // Buffer to construct the potential path

    if (*argv[0] == '/') {  // Check if the command is an absolute path
        strcpy(path_name, argv[0]);  // Copy the command
        if (access(path_name, F_OK) == 0) {  // Check if the command exists
            resultant = path_name;  // Set result to the command path
            return resultant;  // Return the result
        }
    }

    // Iterate over all possible directories to find the command
    for (int i = 0; i < MAX_PATHS; i++) {
        // Stop if a NULL directory is found
        if (dir[i] == NULL) {
            break;
        }

        strcpy(path_name, dir[i]);  // Copy the directory path
        strcat(strcat(path_name, "/"), argv[0]);  // Append the command to the directory path

        if (access(path_name, F_OK) == 0) {  // Check if the file exists
            resultant = path_name;  // Set result to the full path
            return resultant;  // Return the result
        }
    }

    fprintf(stderr, "%s: command not found (double check spelling and existence)\n", argv[0]);  // Print an error message
    return NULL;  // Return NULL if the command is not found
}

// Function to split a string by delimiters
char *strsep(char **s_ptr, const char *delimiter) {
    char* s = *s_ptr;  // Point s to the current position of s_ptr

    if (s) {
        *s_ptr += strcspn(*s_ptr, delimiter);  // Advance s_ptr by the number of chars not including delimiter

        if (**s_ptr)  // If not at the end of the string
            *(*s_ptr)++ = '\0';  // Replace the delimiter with '\0' and advance
        else
            *s_ptr = 0;  // Set s_ptr to NULL if at the end
    }

    return s;  // Return the initial position of s, which is the token
}
