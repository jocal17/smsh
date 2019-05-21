#include "jsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char **environ;

void cleanup(struct command *c, char *l);

int main(int argc, char *argv[])
{
    struct command *com;
    while (1) {
        printf("jsh> ");
        char *input = prompt();
        com = parse_command(input);
        if (strcmp(com->name, "exit") == 0) {
            cleanup(com, input);
            exit(0);
        }
        if (!is_builtin(com))
            fork_and_exec(com->args[0], com->args, com->fd, environ);
        cleanup(com, input);
    }
    return 0;
}

void cleanup(struct command *c, char *l)
{
    for (int i = 0; c->args[i] != NULL; i++)
        free(c->args[i]);
    free(c->args);
    free(c->name);
    free(l);
    free(c);
}
