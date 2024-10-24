/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/10/24 18:42:14 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <unistd.h>
#include <stdlib.h>

char	*executor_find_command(char *cmd, char **envp)
{
	char	*path;
	char	*dir;
	char	*cmd_path;
	char	**paths;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (access(cmd, X_OK) == 0 ? ft_strdup(cmd) : NULL);
	path = env_get_value("PATH", envp);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		dir = ft_strjoin(paths[i], "/");
		cmd_path = ft_strjoin(dir, cmd);
		free(dir);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

int	executor_count_commands(t_command *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->pipe_next;
	}
	return (count);
}

void	executor_reset_fds(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
