/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jowander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/31 14:06:51 by jowander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"
#include <stdlib.h>

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

char	*handle_expansion_result(char *exp, char *new_str)
{
	char	*tmp;

	if (!new_str)
		return (NULL);
	if (new_str != exp)
	{
		tmp = exp;
		exp = new_str;
		free(tmp);
	}
	return (exp);
}

char	*init_expansion(char *str)
{
	char	*exp;

	if (!str)
		return (NULL);
	exp = ft_strdup(str);
	if (!exp)
		return (NULL);
	return (exp);
}

int	handle_single_quotes(char c, int in_single_quote)
{
	if (c == '\'')
		return (!in_single_quote);
	return (in_single_quote);
}
