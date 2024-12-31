/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jowander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/31 14:07:10 by jowander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include <sys/wait.h>
#include <unistd.h>

int	executor_run_command(t_command *cmd, t_shell *shell)
{
	if (!cmd)
		return (0);
	if (!cmd->pipe_next)
		return (executor_single_command(cmd, shell));
	return (executor_pipeline(cmd, shell));
}

void	executor_cleanup(t_command *cmd)
{
	if (!cmd)
		return ;
	executor_close_pipes(cmd);
	executor_close_redirects(cmd->redirects);
}

int	executor_handle_builtin(t_command *cmd, t_shell *shell)
{
	if (!ft_strncmp(cmd->args[0], "echo", 5))
		return (builtin_echo(cmd->args));
	if (!ft_strncmp(cmd->args[0], "cd", 3))
		return (builtin_cd(cmd->args, shell));
	if (!ft_strncmp(cmd->args[0], "pwd", 4))
		return (builtin_pwd());
	if (!ft_strncmp(cmd->args[0], "export", 7))
		return (builtin_export(cmd->args, shell));
	if (!ft_strncmp(cmd->args[0], "unset", 6))
		return (builtin_unset(cmd->args, shell));
	if (!ft_strncmp(cmd->args[0], "env", 4))
		return (builtin_env(shell));
	if (!ft_strncmp(cmd->args[0], "exit", 5))
		return (builtin_exit(cmd->args, shell));
	return (1);
}

int	executor_is_builtin(char *cmd)
{
	return (!ft_strncmp(cmd, "echo", 5) || !ft_strncmp(cmd, "cd", 3)
		|| !ft_strncmp(cmd, "pwd", 4) || !ft_strncmp(cmd, "export", 7)
		|| !ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "env", 4)
		|| !ft_strncmp(cmd, "exit", 5));
}
