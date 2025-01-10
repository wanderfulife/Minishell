/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wander <wander@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2025/01/10 13:33:56 by wander           ###   ########.fr       */
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
	new_env[size] = NULL;
	i = 0;
	while (i < size)
	{
		new_env[i] = ft_strdup(original_env[i]);
		if (!new_env[i])
			return (free_env_array(new_env, i));
		i++;
	}
	return (new_env);
}

void	env_destroy(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
}

static int	update_env_entry(char **env, int index, char *name, char *value)
{
	char	*entry;

	entry = env_create_entry(name, value);
	if (!entry)
		return (free(name), 0);
	free(env[index]);
	env[index] = entry;
	free(name);
	return (1);
}

static int	add_new_env_entry(t_shell *shell, char *name, char *value)
{
	char	*entry;

	entry = env_create_entry(name, value);
	if (!entry)
		return (free(name), 0);
	shell->env = env_add_entry(shell->env, entry);
	free(name);
	return (shell->env != NULL);
}

int	env_set(t_shell *shell, char *var, char *value)
{
	char	*name;
	int		index;

	name = env_get_name(var);
	if (!name)
		return (0);
	value = get_value_from_var(var, value);
	index = env_find_index(name, shell->env);
	if (index >= 0)
		return (update_env_entry(shell->env, index, name, value));
	return (add_new_env_entry(shell, name, value));
}
