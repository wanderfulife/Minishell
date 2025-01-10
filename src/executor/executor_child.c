/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wander <wander@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2025/01/10 13:32:57 by wander           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

static void	cleanup_and_exit(t_shell *shell, t_command *cmd, int status)
{
	env_destroy(shell->env);
	parser_destroy_command(cmd);
	exit(status);
}

static void	handle_builtin_child(t_command *cmd, t_shell *shell)
{
	int	status;

	status = executor_handle_builtin(cmd, shell);
	cleanup_and_exit(shell, cmd, status);
}

void	executor_child_process(t_command *cmd, t_shell *shell)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!executor_setup_redirects(cmd->redirects))
		cleanup_and_exit(shell, cmd, 1);
	if (!cmd->args[0])
		cleanup_and_exit(shell, cmd, 0);
	if (executor_is_builtin(cmd->args[0]))
		handle_builtin_child(cmd, shell);
	path = executor_find_command(cmd->args[0], shell->env);
	if (!path)
		handle_command_not_found(cmd->args[0], shell, cmd);
	execve(path, cmd->args, shell->env);
	handle_execution_error(cmd->args[0], path, shell, cmd);
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
