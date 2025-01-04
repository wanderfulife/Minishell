/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2025/01/04 12:21:36 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"
#include <stdlib.h>

char	**env_init(char **original_env)
{
	char	**new_env;
	int		i;
	int		size;

	if (!original_env)
		return (NULL);
	size = 0;
	while (original_env[size])
		size++;
	new_env = (char **)malloc(sizeof(char *) * (size + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_env[i] = ft_strdup(original_env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	env_destroy(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

char	*env_get_value(char *name, char **env)
{
	int		i;
	size_t	name_len;

	if (!name || !env)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, name_len) && env[i][name_len] == '=')
			return (env[i] + name_len + 1);
		i++;
	}
	return (NULL);
}

int	env_set(t_shell *shell, char *var, char *value)
{
	char	*name;
	char	*equals_pos;
	int		index;

	name = env_get_name(var);
	if (!name)
		return (0);
	if (!value)
	{
		equals_pos = ft_strchr(var, '=');
		if (equals_pos)
			value = equals_pos + 1;
	}
	index = env_find_index(name, shell->env);
	if (index >= 0)
	{
		free(shell->env[index]);
		shell->env[index] = env_create_entry(name, value);
		free(name);
		return (shell->env[index] != NULL);
	}
	shell->env = env_add_entry(shell->env, env_create_entry(name, value));
	free(name);
	return (shell->env != NULL);
}

int	env_unset(t_shell *shell, char *name)
{
	int	index;

	if (!name || !shell->env)
		return (0);
	index = env_find_index(name, shell->env);
	if (index < 0)
		return (1);
	shell->env = env_remove_entry(shell->env, index);
	return (shell->env != NULL);
}
