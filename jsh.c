#include "jsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char **environ;

int main(int argc, char *argv[])
{
    struct command *com;
    while (1) {
        printf("jsh> ");
        char *input = prompt();
        com = parse_command(input);
        if (strcmp(com->name, "exit") == 0)
            exit(0);
        if (!is_builtin(com))
            fork_and_exec(com->args[0], com->args, com->fd, environ);
    }
    return 0;
}
