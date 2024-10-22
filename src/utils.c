/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:23:18 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/22 13:27:59 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void skip_whitespace(char *input, int *i)
{
    while (input[*i] == ' ' || input[*i] == '\t')
        (*i)++;
}

void	handle_quotes(char *input, int *i, t_token_type quote_type)
{
	(*i)++;
	while (input[*i] && input[*i] != (quote_type == TOKEN_DQUOTE ? '"' : '\''))
		(*i)++;
	if (input[*i])
		(*i)++;
}