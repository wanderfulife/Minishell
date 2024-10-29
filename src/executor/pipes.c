/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/29 13:32:11 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <unistd.h>
#include <stdio.h>

int	executor_init_pipe(t_command *cmd)
{
	if (pipe(cmd->pipe_fd) == -1)
		return (0);
	return (1);
}

void	executor_connect_pipes(t_command *cmd, int is_last)
{
	if (!is_last && cmd->pipe_fd[1] != -1)
	{
		dup2(cmd->pipe_fd[1], STDOUT_FILENO);
		close(cmd->pipe_fd[1]);
		cmd->pipe_fd[1] = -1;
	}
	if (cmd->pipe_next && cmd->pipe_next->pipe_fd[0] != -1)
	{
		dup2(cmd->pipe_next->pipe_fd[0], STDIN_FILENO);
		close(cmd->pipe_next->pipe_fd[0]);
		cmd->pipe_next->pipe_fd[0] = -1;
	}
}

void	executor_close_pipes(t_command *cmd)
{
	t_command	*current;

	current = cmd;
	while (current)
	{
		if (current->pipe_fd[0] != -1)
		{
			close(current->pipe_fd[0]);
			current->pipe_fd[0] = -1;
		}
		if (current->pipe_fd[1] != -1)
		{
			close(current->pipe_fd[1]);
			current->pipe_fd[1] = -1;
		}
		current = current->pipe_next;
	}
}

int	executor_setup_pipes(t_command *cmd)
{
	t_command	*current;

	current = cmd;
	while (current && current->pipe_next)
	{
		if (pipe(current->pipe_fd) == -1)
			return (0);
		current = current->pipe_next;
	}
	return (1);
}
