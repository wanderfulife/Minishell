/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/30 16:33:42 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"
#include <stdlib.h>

int	env_var_length(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '?')
		return (1);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

char	*env_get_var_name(char *str)
{
	int		len;
	char	*name;

	len = env_var_length(str);
	name = ft_substr(str, 0, len);
	return (name);
}

char	*env_replace_var(char *str, int start, int len, char *value)
{
	char	*new_str;
	int		new_len;
	int		value_len;

	if (value)
		value_len = ft_strlen(value);
	else
		value_len = 0;
	new_len = ft_strlen(str) - len + value_len;
	new_str = (char *)malloc(sizeof(char) * (new_len + 1));
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, str, start + 1);
	if (value)
		ft_strlcpy(new_str + start, value, value_len + 1);
	ft_strlcpy(new_str + start + value_len, str + start + len, ft_strlen(str
			+ start + len) + 1);
	return (new_str);
}
