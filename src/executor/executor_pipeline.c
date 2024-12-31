/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jowander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/31 14:07:43 by jowander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

static int	setup_pipeline_process(t_command *cmd, int *pipes,
	int *prev_pipe, pid_t *pid)
{
	if (cmd->pipe_next && pipe(pipes) < 0)
		return (0);
	*pid = fork();
	if (*pid < 0)
		return (0);
	if (*pid == 0)
	{
		executor_handle_child_pipes(*prev_pipe, pipes, cmd);
		return (2);
	}
	return (1);
}

static int	execute_pipeline_command(t_command *cmd, int *pipes,
	int *prev_pipe, t_shell *shell)
{
	pid_t	pid;
	int		setup_status;

	setup_status = setup_pipeline_process(cmd, pipes, prev_pipe, &pid);
	if (setup_status == 0)
		return (-1);
	if (setup_status == 2)
		executor_child_process(cmd, shell);
	executor_parent_process(prev_pipe, pipes, cmd);
	return (pid);
}

int	executor_pipeline(t_command *cmd, t_shell *shell)
{
	int		pipes[2];
	int		prev_pipe;
	int		last_pid;
	int		current_pid;

	prev_pipe = STDIN_FILENO;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while (cmd)
	{
		current_pid = execute_pipeline_command(cmd, pipes, &prev_pipe, shell);
		if (current_pid == -1)
			return (1);
		if (!cmd->pipe_next)
			last_pid = current_pid;
		cmd = cmd->pipe_next;
	}
	wait_for_last_process(last_pid, shell);
	return (shell->last_exit_status);
}
