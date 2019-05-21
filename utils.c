#include "jsh.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define IF_ERROR(x) (x == -1) ? printf("%s\n", strerror(errno)) : printf("It's a pooper\n")

static char *builtins[] = {"cd", "\0"};
static int (*builtin_p[])(struct command *) = {change_dir};

int change_dir(struct command *com)
{
    if (com->args[1])
        return chdir(com->args[1]);
    else
        return chdir(getenv("HOME"));
}

char *prompt()
{
    char *input = malloc(sizeof(char) * 1024);
    fgets(input, 1023, stdin);
    return input;
}

struct command *parse_command(char *line)
{
    struct command *com = malloc(sizeof(struct command));
    com->fd = 1;
    int n_args = 1;
    char *p = strtok(line, " \n\t\r");
    com->name = strdup(p);
    com->args = calloc(sizeof(char *), 16);
    com->args[0] = strdup(p);

    for (p = strtok(NULL, " \n"); p; p = strtok(NULL, " \n")) {
        if (strcmp(p, ">") == 0) {
            p = strtok(NULL, " \n\t\r");
            com->fd = open(p, O_WRONLY | O_CREAT, 0644);
        } else
            com->args[n_args++] = strdup(p);
    }
    return com;
}

int is_builtin(struct command *com)
{
    for (int i = 0; builtins[i] != NULL; i++) {
        if (strcmp(builtins[i], com->name) == 0) {
            IF_ERROR(builtin_p[i](com));
            return 1;
        }
    }
    return 0;
}

int fork_and_exec(char *f, char **args, int fd, char **env)
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork failed: ");
        printf("%s\n", strerror(errno));
        return -1;
    }
    if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        return 0;
    }
    if (fd != 1) {
        dup2(fd, 1);
        close(fd);
    }
    IF_ERROR(execvp(f, args));
    _exit(0);
}
