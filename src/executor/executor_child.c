/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/30 17:03:08 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

static void	handle_command_not_found(char *cmd)
{
	ft_putstr_fd("minishell: command not found: ", 2);
	ft_putendl_fd(cmd, 2);
	exit(127);
}

static void	handle_execution_failed(char *cmd, char *path)
{
	free(path);
	ft_putstr_fd("minishell: execution failed: ", 2);
	ft_putendl_fd(cmd, 2);
	exit(126);
}

void	executor_child_process(t_command *cmd, t_shell *shell)
{
	char	*path;
	int		status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!executor_setup_redirects(cmd->redirects))
		exit(1);
	if (!cmd->args[0])
		exit(0);
	if (executor_is_builtin(cmd->args[0]))
	{
		status = executor_handle_builtin(cmd, shell);
		exit(status);
	}
	path = executor_find_command(cmd->args[0], shell->env);
	if (!path)
		handle_command_not_found(cmd->args[0]);
	execve(path, cmd->args, shell->env);
	handle_execution_failed(cmd->args[0], path);
}

void	executor_exit_status(int status, t_shell *shell)
{
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->last_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit: 3", 2);
	}
}
