/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:07:13 by JoWander          #+#    #+#             */
/*   Updated: 2024/11/04 16:12:19 by JoWander         ###   ########.fr       */
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
