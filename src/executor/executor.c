/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/10/25 13:14:07 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "builtins.h"
#include <unistd.h>
#include <sys/wait.h>  

static int	str_equals(const char *s1, const char *s2)
{
	return (!ft_strncmp(s1, s2, ft_strlen(s2) + 1));
}

int	executor_is_builtin(char *cmd)
{
	return (str_equals(cmd, "echo") || str_equals(cmd, "cd") ||
		str_equals(cmd, "pwd") || str_equals(cmd, "export") ||
		str_equals(cmd, "unset") || str_equals(cmd, "env") ||
		str_equals(cmd, "exit"));
}

int	executor_handle_builtin(t_command *cmd, t_shell *shell)
{
	if (str_equals(cmd->args[0], "echo"))
		return (builtin_echo(cmd->args));
	if (str_equals(cmd->args[0], "cd"))
		return (builtin_cd(cmd->args, shell));
	if (str_equals(cmd->args[0], "pwd"))
		return (builtin_pwd());
	if (str_equals(cmd->args[0], "export"))
		return (builtin_export(cmd->args, shell));
	if (str_equals(cmd->args[0], "unset"))
		return (builtin_unset(cmd->args, shell));
	if (str_equals(cmd->args[0], "env"))
		return (builtin_env(shell));
	if (str_equals(cmd->args[0], "exit"))
		return (builtin_exit(cmd->args, shell));
	return (1);
}


static int	executor_single_command(t_command *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
		return (0);
	if (!executor_setup_redirects(cmd->redirects))
	{
		close(saved_stdin);
		close(saved_stdout);
		return (0);
	}
	status = executor_is_builtin(cmd->args[0]) ? 
		executor_handle_builtin(cmd, shell) : executor_execute(cmd, shell);
	executor_reset_fds(saved_stdin, saved_stdout);
	return (status);
}

static int executor_pipeline(t_command *cmd, t_shell *shell)
{
    int     prev_pipe;
    pid_t   pid;
    int     status;
    int     last_pid;
    int     pipe_fd[2];

    prev_pipe = STDIN_FILENO;
    while (cmd)
    {
        if (cmd->pipe_next && pipe(pipe_fd) < 0)
            return (1);

        pid = fork();
        if (pid == 0)
        {
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);

            if (prev_pipe != STDIN_FILENO)
            {
                dup2(prev_pipe, STDIN_FILENO);
                close(prev_pipe);
            }
            
            if (cmd->pipe_next)
            {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }

            if (!executor_setup_redirects(cmd->redirects))
                exit(1);

            if (executor_is_builtin(cmd->args[0]))
            {
                status = executor_handle_builtin(cmd, shell);
                exit(status);
            }

            executor_execute(cmd, shell);
            exit(127);
        }

        if (prev_pipe != STDIN_FILENO)
            close(prev_pipe);

        if (cmd->pipe_next)
        {
            close(pipe_fd[1]);
            prev_pipe = pipe_fd[0];
        }

        last_pid = pid;
        cmd = cmd->pipe_next;
    }

    status = executor_wait_all(last_pid);
    return (WEXITSTATUS(status));
}

int	executor_run_command(t_command *cmd, t_shell *shell)
{
	int	status;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (!cmd->pipe_next)
		status = executor_single_command(cmd, shell);
	else
		status = executor_pipeline(cmd, shell);
	return (status);
}