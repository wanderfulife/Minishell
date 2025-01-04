/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2025/01/04 12:50:42 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include <sys/stat.h>

static int	is_directory(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

void	handle_command_not_found(char *cmd, t_shell *shell, t_command *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
	env_destroy(shell->env);
	parser_destroy_command(command);
	exit(127);
}

static void	handle_no_file_error(char *cmd, char *path,
	t_shell *shell, t_command *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	free(path);
	env_destroy(shell->env);
	parser_destroy_command(command);
	exit(127);
}

static void	handle_permission_error(char *cmd, char *path,
	t_shell *shell, t_command *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": Permission denied\n", 2);
	free(path);
	env_destroy(shell->env);
	parser_destroy_command(command);
	exit(126);
}

void	handle_execution_error(char *cmd, char *path,
	t_shell *shell, t_command *command)
{
	if (access(cmd, F_OK) != 0)
		handle_no_file_error(cmd, path, shell, command);
	else if (access(cmd, X_OK) != 0)
		handle_permission_error(cmd, path, shell, command);
	else if (is_directory(cmd))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		free(path);
		env_destroy(shell->env);
		parser_destroy_command(command);
		exit(126);
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": Error executing command\n", 2);
	free(path);
	env_destroy(shell->env);
	parser_destroy_command(command);
	exit(126);
}
