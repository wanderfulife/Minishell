/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/10/24 18:41:02 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

static void	executor_child_process(t_command *cmd, t_shell *shell)
{
	char	*cmd_path;

	if (!cmd->args[0])
		exit(0);
	cmd_path = executor_find_command(cmd->args[0], shell->env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->args[0], 2);
		exit(127);
	}
	if (execve(cmd_path, cmd->args, shell->env) == -1)
	{
		ft_putstr_fd("minishell: execution failed: ", 2);
		ft_putendl_fd(cmd->args[0], 2);
		free(cmd_path);
		exit(126);
	}
}

pid_t	executor_fork_process(t_command *cmd, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (executor_is_builtin(cmd->args[0]))
		{
			executor_handle_builtin(cmd, shell);
			exit(shell->last_exit_status);
		}
		cmd->pipe_fd[1] = -1;
		cmd->pipe_fd[0] = -1;
		executor_child_process(cmd, shell);
	}
	return (pid);
}

void	executor_exit_status(int status, t_shell *shell)
{
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->last_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", 2);
	}
}

int	executor_wait_all(int last_pid)
{
	int	status;
	int	pid;

	status = 0;
	while (1)
	{
		pid = wait(&status);
		if (pid == -1)
			break ;
		if (pid == last_pid)
			break ;
	}
	return (status);
}

int	executor_execute(t_command *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = executor_fork_process(cmd, shell);
	if (pid == -1)
		return (1);
	waitpid(pid, &status, 0);
	executor_exit_status(status, shell);
	return (shell->last_exit_status);
}
