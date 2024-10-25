/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/10/25 13:44:42 by JoWander         ###   ########.fr       */
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

static char	*env_replace_var(char *str, int start, int len, char *value)
{
	char	*new_str;
	int		new_len;
	int		value_len;

	value_len = value ? ft_strlen(value) : 0;
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

char	*env_expand_exit_status(char *str, t_shell *shell)
{
	char	*status_str;
	char	*new_str;
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			status_str = ft_itoa(shell->last_exit_status);
			if (!status_str)
				return (NULL);
			new_str = env_replace_var(str, i, 2, status_str);
			free(status_str);
			free(str);
			return (new_str);
		}
		i++;
	}
	return (str);
}

char	*env_expand_vars(char *str, t_shell *shell)
{
	char	*expanded;
	char	*var_name;
	char	*var_value;
	int		i;
	int		in_single_quote;

	expanded = env_expand_exit_status(str, shell);
	if (!expanded)
		return (NULL);
	i = 0;
	in_single_quote = 0;
	while (expanded[i])
	{
		if (expanded[i] == '\'')
			in_single_quote = !in_single_quote;
		if (!in_single_quote && expanded[i] == '$' && expanded[i + 1]
			&& expanded[i + 1] != ' ' && expanded[i + 1] != '\'' && expanded[i
			+ 1] != '"')
		{
			var_name = env_get_var_name(expanded + i + 1);
			if (!var_name)
				return (NULL);
			if (ft_strncmp(var_name, "?", 2) == 0)
				// Changed to ft_strncmp with length 2
			{
				var_value = ft_itoa(shell->last_exit_status);
			}
			else
			{
				var_value = env_get_value(var_name, shell->env);
			}
			str = env_replace_var(expanded, i, ft_strlen(var_name) + 1,
					var_value);
			free(expanded);
			free(var_name);
			if (var_name && ft_strncmp(var_name, "?", 2) == 0)
				free(var_value);
			if (!str)
				return (NULL);
			expanded = str;
			continue ;
		}
		i++;
	}
	return (expanded);
}
