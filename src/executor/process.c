/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jowander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/31 14:08:17 by jowander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

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
