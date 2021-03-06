#ifndef __JSH_H_
#define __JSH_H_

struct command {
    char *name;
    char **args;
    int fd;
};

char *prompt();
int is_builtin(struct command *com);
struct command *parse_command(char *line);
int execute_command(struct command);
int find_and_exec(char *f, char **args, char **env);
int fork_and_exec(char *f, char **args, int fd, char **env);

int change_dir(struct command *com);
#endif // __JSH_H_
