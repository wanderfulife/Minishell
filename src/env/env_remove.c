/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_remove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 13:52:00 by JoWander          #+#    #+#             */
/*   Updated: 2025/01/04 14:27:05 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "builtins.h"
#include <stdlib.h>

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
	i = -1;
	j = 0;
	while (++i < size)
	{
		if (i != index)
		{
			new_env[j] = ft_strdup(env[i]);
			if (!new_env[j++])
				return (free_array(new_env), free_array(env), NULL);
		}
		free(env[i]);
	}
	return (new_env[j] = NULL, free(env), new_env);
}
