/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/29 13:32:11 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*join_path(char *dir, char *cmd)
{
	char	*cmd_path;

	cmd_path = ft_strjoin(dir, cmd);
	free(dir);
	return (cmd_path);
}

void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

char	*check_paths(char **paths, char *cmd)
{
	char	*dir;
	char	*cmd_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		dir = ft_strjoin(paths[i], "/");
		if (!dir)
			return (NULL);
		cmd_path = join_path(dir, cmd);
		if (!cmd_path || access(cmd_path, X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char	*executor_find_command(char *cmd, char **envp)
{
	char	*path;
	char	**paths;
	char	*cmd_path;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	path = env_get_value("PATH", envp);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	cmd_path = check_paths(paths, cmd);
	free_paths(paths);
	return (cmd_path);
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
