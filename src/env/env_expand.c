/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jowander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/31 14:06:37 by jowander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"
#include <stdlib.h>

static char	*handle_exit_status_expansion(char *exp, int i, t_shell *shell)
{
	char	*status_str;
	char	*str;

	status_str = ft_itoa(shell->last_exit_status);
	if (!status_str)
		return (NULL);
	str = env_replace_var(exp, i, 2, status_str);
	free(status_str);
	return (str);
}

static char	*process_expansion(char *exp, int *i, t_shell *shell)
{
	char	*str;

	if (exp[*i] == '$' && exp[*i + 1] && exp[*i + 1] != ' '
		&& exp[*i + 1] != '\'' && exp[*i + 1] != '"')
	{
		if (exp[*i + 1] == '?')
			return (handle_exit_status_expansion(exp, *i, shell));
		str = expand_variable(exp, *i, shell);
		if (!str)
			return (NULL);
		return (str);
	}
	(*i)++;
	return (exp);
}

static char	*process_char(char *exp, int *i, int *in_single_quote,
	t_shell *shell)
{
	char	*new_str;

	*in_single_quote = handle_single_quotes(exp[*i], *in_single_quote);
	if (!*in_single_quote)
	{
		new_str = process_expansion(exp, i, shell);
		exp = handle_expansion_result(exp, new_str);
		if (!exp)
			return (NULL);
		if (new_str != exp)
			return (exp);
	}
	else
		(*i)++;
	return (exp);
}

char	*env_expand_vars(char *str, t_shell *shell)
{
	char	*exp;
	int		i;
	int		in_single_quote;

	exp = init_expansion(str);
	if (!exp)
		return (NULL);
	i = 0;
	in_single_quote = 0;
	while (exp[i])
	{
		exp = process_char(exp, &i, &in_single_quote, shell);
		if (!exp)
			return (NULL);
	}
	return (exp);
}
