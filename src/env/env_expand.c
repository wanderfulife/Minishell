/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/31 15:58:47 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"
#include <stdlib.h>

static char	*env_replace_var(char *str, int start, int len, char *value)
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

char	*get_var_value(char *var_name, t_shell *shell)
{
	if (ft_strncmp(var_name, "?", 2) == 0)
		return (ft_itoa(shell->last_exit_status));
	return (env_get_value(var_name, shell->env));
}

char	*expand_variable(char *expanded, int i, t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	char	*new_str;

	var_name = env_get_var_name(expanded + i + 1);
	if (!var_name)
		return (NULL);
	var_value = get_var_value(var_name, shell);
	new_str = env_replace_var(expanded, i, ft_strlen(var_name) + 1, var_value);
	if (var_value && ft_strncmp(var_name, "?", 2) == 0)
		free(var_value);
	free(var_name);
	return (new_str);
}

char	*env_expand_vars(char *str, t_shell *shell)
{
	char	*exp;
	int		i;
	int		in_single_quote;

	exp = env_expand_exit_status(str, shell);
	if (!exp)
		return (NULL);
	i = 0;
	in_single_quote = 0;
	while (exp[i])
	{
		in_single_quote = handle_single_quotes(exp[i], in_single_quote);
		if (!in_single_quote && exp[i] == '$' && exp[i + 1] && exp[i + 1] != ' '
			&& exp[i + 1] != '\'' && exp[i + 1] != '"')
		{
			str = expand_variable(exp, i, shell);
			free(exp);
			if (!str)
				return (NULL);
			exp = str;
			continue ;
		}
		i++;
	}
	return (exp);
}
