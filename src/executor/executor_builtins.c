/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 16:10:01 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/31 16:15:50 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include <sys/wait.h>
#include <unistd.h>

int	str_equals(const char *s1, const char *s2)
{
	return (!ft_strncmp(s1, s2, ft_strlen(s2) + 1));
}

int	executor_is_builtin(char *cmd)
{
	return (str_equals(cmd, "echo") || str_equals(cmd, "cd") || str_equals(cmd,
			"pwd") || str_equals(cmd, "export") || str_equals(cmd, "unset")
		|| str_equals(cmd, "env") || str_equals(cmd, "exit"));
}

int	executor_handle_builtin(t_command *cmd, t_shell *shell)
{
	if (str_equals(cmd->args[0], "echo"))
		return (builtin_echo(cmd->args));
	if (str_equals(cmd->args[0], "cd"))
		return (builtin_cd(cmd->args, shell));
	if (str_equals(cmd->args[0], "pwd"))
		return (builtin_pwd());
	if (str_equals(cmd->args[0], "export"))
		return (builtin_export(cmd->args, shell));
	if (str_equals(cmd->args[0], "unset"))
		return (builtin_unset(cmd->args, shell));
	if (str_equals(cmd->args[0], "env"))
		return (builtin_env(shell));
	if (str_equals(cmd->args[0], "exit"))
		return (builtin_exit(cmd->args, shell));
	return (1);
}

int	executor_run_command(t_command *cmd, t_shell *shell)
{
	int	status;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (!cmd->pipe_next)
		status = executor_single_command(cmd, shell);
	else
		status = executor_pipeline(cmd, shell);
	return (status);
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
