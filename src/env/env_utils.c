/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jowander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/29 16:24:59 by jowander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <stdlib.h>

char	*env_get_name(char *var)
{
	int		i;
	char	*name;

	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	name = ft_substr(var, 0, i);
	return (name);
}

char	*env_create_entry(char *name, char *value)
{
	char	*entry;
	size_t	total_len;

	if (!value)
		return (ft_strdup(name));
	total_len = ft_strlen(name) + ft_strlen(value) + 2;
	entry = (char *)malloc(total_len);
	if (!entry)
		return (NULL);
	ft_strlcpy(entry, name, total_len);
	ft_strlcat(entry, "=", total_len);
	ft_strlcat(entry, value, total_len);
	return (entry);
}

int	env_find_index(char *name, char **env)
{
	int		i;
	size_t	len;

	if (!name || !env)
		return (-1);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, len) && (env[i][len] == '='
			|| env[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

char	**env_add_entry(char **env, char *entry)
{
	char	**new_env;
	int		size;
	int		i;

	size = 0;
	while (env[size])
		size++;
	new_env = (char **)malloc(sizeof(char *) * (size + 2));
	if (!new_env)
	{
		free(entry);
		return (NULL);
	}
	i = 0;
	while (i < size)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[i] = entry;
	new_env[i + 1] = NULL;
	free(env);
	return (new_env);
}

char	**env_remove_entry(char **env, int index)
{
	char	**new_env;
	int		size;
	int		i;
	int		j;

	size = 0;
	while (env[size])
		size++;
	new_env = (char **)malloc(sizeof(char *) * size);
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (i < size)
	{
		if (i != index)
			new_env[j++] = env[i];
		else
			free(env[i]);
		i++;
	}
	new_env[j] = NULL;
	free(env);
	return (new_env);
}
