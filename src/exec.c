#include "../includes/minishell.h"

// Execute non-built-in commands using execve
void execute_command(char **tokens) {
    pid_t pid;
    int status;

    pid = fork();  // Fork a child process
    if (pid == 0) {
        // Child process: execute the command
        if (execvp(tokens[0], tokens) == -1) {
            perror("execvp");
        }
        exit(EXIT_FAILURE);  // If execvp fails, exit the child process
    } else if (pid < 0) {
        // Fork failed
        perror("fork");
    } else {
        // Parent process: wait for the child to complete
        waitpid(pid, &status, 0);
    }
}

