/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:33:22 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/22 17:33:50 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void parse_redir_append_or_heredoc(t_token **tokens, int *token_count, 
    char *input, int *i, int start)
{
    if (*i > start)
        add_token(tokens, token_count, ft_substr(input, start, *i - start), TOKEN_WORD);
    add_token(tokens, token_count, ft_substr(input, *i, 2), 
        get_token_type(input[*i], input[*i + 1]));
    (*i) += 2;
}

void parse_pipe_or_redir(t_token **tokens, int *token_count, char *input, 
    int *i, int start)
{
    if (*i > start)
        add_token(tokens, token_count, ft_substr(input, start, *i - start), TOKEN_WORD);
    add_token(tokens, token_count, ft_substr(input, *i, 1), 
        get_token_type(input[*i], input[*i + 1]));
    (*i)++;
}

void parse_word_token(t_token **tokens, int *token_count, char *input, 
    int *i, int start)
{
    while (input[*i] && !ft_strchr(" \t|<>\"'", input[*i]))
        (*i)++;
    if (*i > start)
        add_token(tokens, token_count, ft_substr(input, start, *i - start), 
            TOKEN_WORD);
}