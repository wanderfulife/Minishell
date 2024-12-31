/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_single.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jowander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/31 14:07:49 by jowander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include <sys/wait.h>
#include <unistd.h>

static int	execute_command(t_command *cmd, t_shell *shell)
{
	int		status;
	pid_t	pid;

	if (executor_is_builtin(cmd->args[0]) && !cmd->pipe_next)
		return (executor_handle_builtin(cmd, shell));
	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
		executor_child_process(cmd, shell);
	while (wait(&status) != pid)
		continue ;
	executor_exit_status(status, shell);
	return (shell->last_exit_status);
}

int	executor_single_command(t_command *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		if (saved_stdin != -1)
			close(saved_stdin);
		if (saved_stdout != -1)
			close(saved_stdout);
		return (1);
	}
	if (!executor_setup_redirects(cmd->redirects))
	{
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}
	status = execute_command(cmd, shell);
	executor_reset_fds(saved_stdin, saved_stdout);
	return (status);
}
