/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jowander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/31 14:08:29 by jowander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>

static int	is_directory(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

static void	handle_command_not_found(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
	exit(127);
}

static void	handle_execution_error(char *cmd, char *path)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	if (access(cmd, F_OK) != 0)
	{
		ft_putendl_fd("No such file or directory", 2);
		free(path);
		exit(127);
	}
	else if (access(cmd, X_OK) != 0)
	{
		ft_putendl_fd("Permission denied", 2);
		free(path);
		exit(126);
	}
	else if (is_directory(cmd))
	{
		ft_putendl_fd("Is a directory", 2);
		free(path);
		exit(126);
	}
	ft_putendl_fd("Error executing command", 2);
	free(path);
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
	handle_execution_error(cmd->args[0], path);
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
