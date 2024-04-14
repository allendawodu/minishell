#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pwd.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_LINE_LEN  80
#define MAX_ARGS      64
#define MAX_ARG_LEN   16
#define MAX_PATHS     64
#define MAX_PATH_LEN  96
#define WHITESPACE    " .,\t\n"

struct command_t {
    char* name;
    int argc;
    char* argv[MAX_ARGS];
};

void print_prompt();
void read_command(char *buffer);

int parse_path(char* dirs[]);
int parse_command(char* cLine, struct command_t* cmd);

char* strsep(char** stringp, const char* delim);
char* lookup_path(char** argv, char** dir);
