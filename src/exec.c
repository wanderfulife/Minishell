/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:18:02 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/17 18:26:05 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Handles input and output redirection
void handle_redirection(t_cmd *cmd) {
    if (cmd->infile != 0) {
        dup2(cmd->infile, 0);
        close(cmd->infile);
    }
    if (cmd->outfile != 1) {
        dup2(cmd->outfile, 1);
        close(cmd->outfile);
    }
}

// Executes a single command
void execute_single_command(t_cmd *cmd) {
    if (execvp(cmd->argv[0], cmd->argv) == -1) {
        perror("execvp");
        exit(EXIT_FAILURE);
    }
}

// Executes the commands in the list
void execute_command(t_cmd *cmd) {
    while (cmd) {
        pid_t pid = fork();
        if (pid == 0) {
            handle_redirection(cmd);  // Redirect input/output if needed
            execute_single_command(cmd);  // Execute the command
        } else if (pid < 0) {
            perror("fork failed");
        } else {
            waitpid(pid, NULL, 0);  // Wait for the child process
        }
        cmd = cmd->next;
    }
}
