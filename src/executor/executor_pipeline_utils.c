/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/30 17:05:24 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

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

void	executor_parent_process(int *prev_pipe, int pipes[2], t_command *cmd)
{
	if (*prev_pipe != STDIN_FILENO)
		close(*prev_pipe);
	if (cmd->pipe_next)
	{
		close(pipes[1]);
		*prev_pipe = pipes[0];
	}
}

void	wait_for_last_process(pid_t last_pid, t_shell *shell)
{
	int	status;

	while (wait(&status) != last_pid)
		continue ;
	shell_reset_signals();
	executor_exit_status(status, shell);
}
