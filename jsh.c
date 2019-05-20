#include "jsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char **environ;

int main(int argc, char *argv[])
{
    while (1) {
        struct command *com;
        printf("jsh> ");
        char *input = prompt();
        com = parse_command(input);
        if (strcmp(com->name, "exit"))
            exit(0);
        if (!is_builtin(com))
            fork_and_exec(com->args[0], com->args, com->fd, environ);
    }
    return 0;
}
