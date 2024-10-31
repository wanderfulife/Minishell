/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:58:06 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/31 15:58:59 by JoWander         ###   ########.fr       */
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

int	handle_single_quotes(char c, int in_single_quote)
{
	if (c == '\'')
		return (!in_single_quote);
	return (in_single_quote);
}
