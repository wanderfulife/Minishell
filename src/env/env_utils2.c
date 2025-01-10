/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wander <wander@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2025/01/10 13:33:52 by wander           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"
#include <stdlib.h>

char	*get_value_from_var(char *var, char *value)
{
	char	*equals_pos;

	if (value)
		return (value);
	equals_pos = ft_strchr(var, '=');
	if (equals_pos)
		return (equals_pos + 1);
	return (NULL);
}

char	**free_env_array(char **env, int size)
{
	while (--size >= 0)
		free(env[size]);
	free(env);
	return (NULL);
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
