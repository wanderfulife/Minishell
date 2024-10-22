/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:23:18 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/22 17:26:12 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void skip_whitespace(char *input, int *i)
{
    while (input[*i] == ' ' || input[*i] == '\t')
        (*i)++;
}

void handle_quotes(char *input, int *i, t_token_type quote_type)
{
    (*i)++;
    while (input[*i] && input[*i] != (quote_type == TOKEN_DQUOTE ? '"' : '\''))
        (*i)++;
    if (input[*i])
        (*i)++;
}

int is_token_delimiter(char c, char next_c)
{
    if (c == ' ' || c == '\t')
        return (1);
    if (c == '>' && next_c == '>')
        return (1);
    if (c == '<' && next_c == '<')
        return (1);
    if (c == '|' || c == '<' || c == '>' || c == '"' || c == '\'')
        return (1);
    return (0);
}
void remove_token(t_token **tokens, int *token_count, int index)
{
    int i = index;
    while (i < *token_count - 1)
    {
        tokens[i]->value = tokens[i + 1]->value;
        tokens[i]->type = tokens[i + 1]->type;
        i++;
    }
    tokens[i]->value = NULL;
    (*token_count)--;
}