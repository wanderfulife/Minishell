/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/31 16:13:47 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include <sys/wait.h>
#include <unistd.h>

static int	execute_command(t_command *cmd, t_shell *shell)
{
	int	status;

	if (executor_is_builtin(cmd->args[0]))
		status = executor_handle_builtin(cmd, shell);
	else
		status = executor_execute(cmd, shell);
	return (status);
}

int	executor_single_command(t_command *cmd, t_shell *shell)
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
	status = execute_command(cmd, shell);
	executor_reset_fds(saved_stdin, saved_stdout);
	return (status);
}

void	executor_handle_child_pipes(int prev_pipe, int pipes[2], t_command *cmd)
{
	if (prev_pipe != STDIN_FILENO)
	{
		dup2(prev_pipe, STDIN_FILENO);
		close(prev_pipe);
	}
	if (cmd->pipe_next)
	{
		close(pipes[0]);
		dup2(pipes[1], STDOUT_FILENO);
		close(pipes[1]);
	}
}

void	executor_child_process(t_command *cmd, t_shell *shell)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!executor_setup_redirects(cmd->redirects))
		exit(1);
	if (!cmd->args[0])
		exit(0);
	if (executor_is_builtin(cmd->args[0]))
	{
		executor_handle_builtin(cmd, shell);
		exit(0);
	}
	path = executor_find_command(cmd->args[0], shell->env);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->args[0], 2);
		exit(127);
	}
	execve(path, cmd->args, shell->env);
	free(path);
	ft_putstr_fd("minishell: execution failed: ", 2);
	ft_putendl_fd(cmd->args[0], 2);
	exit(126);
}

int	executor_pipeline(t_command *cmd, t_shell *shell)
{
	int		pipes[2];
	pid_t	pid;
	int		status;
	int		prev_pipe;

	prev_pipe = STDIN_FILENO;
	while (cmd)
	{
		if (cmd->pipe_next && pipe(pipes) < 0)
			return (1);
		pid = fork();
		if (pid < 0)
			return (1);
		if (pid == 0)
		{
			executor_handle_child_pipes(prev_pipe, pipes, cmd);
			executor_child_process(cmd, shell);
		}
		executor_parent_process(&prev_pipe, pipes, cmd);
		cmd = cmd->pipe_next;
	}
	while (wait(&status) > 0)
		;
	return (WEXITSTATUS(status));
}
