/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:25:37 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/22 13:25:49 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void parse_token(t_token **tokens, int *token_count, char *input, int *i)
{
    int start = *i;
    t_token_type type = get_token_type(input[*i], input[*i + 1]);

    if (type == TOKEN_DQUOTE || type == TOKEN_SQUOTE)
        parse_quoted_token(tokens, token_count, input, i);
    else if (type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC)
        parse_redir_append_or_heredoc(tokens, token_count, input, i, start);
    else if (type == TOKEN_PIPE || type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT)
        parse_pipe_or_redir(tokens, token_count, input, i, start);
    else
        parse_word_token(tokens, token_count, input, i, start);
}

void parse_quoted_token(t_token **tokens, int *token_count, char *input, int *i)
{
    char quote = input[*i];
    (*i)++;
    int start = *i;

    while (input[*i] && input[*i] != quote)
        (*i)++;

    if (input[*i] == quote)
    {
        if (*i > start || (input[*i + 1] && (input[*i + 1] == '"' || input[*i + 1] == '\'')))
            add_token(tokens, token_count, ft_substr(input, start, *i - start), TOKEN_WORD);
        else if (input[*i + 1] == quote)
            (*i)++;
        else
            add_token(tokens, token_count, ft_strdup(""), TOKEN_WORD);
        (*i)++;
    }
    else
    {
        add_token(tokens, token_count, ft_substr(input, start - 1, ft_strlen(input) - start + 1), TOKEN_WORD);
    }
}


void parse_redir_append_or_heredoc(t_token **tokens, int *token_count, char *input, int *i, int start)
{
    if (*i > start)
        add_token(tokens, token_count, ft_substr(input, start, *i - start), TOKEN_WORD);
    add_token(tokens, token_count, ft_substr(input, *i, 2), get_token_type(input[*i], input[*i + 1]));
    (*i) += 2;
}

void parse_pipe_or_redir(t_token **tokens, int *token_count, char *input, int *i, int start)
{
    if (*i > start)
        add_token(tokens, token_count, ft_substr(input, start, *i - start), TOKEN_WORD);
    add_token(tokens, token_count, ft_substr(input, *i, 1), get_token_type(input[*i], input[*i + 1]));
    (*i)++;
}

void parse_word_token(t_token **tokens, int *token_count, char *input, int *i, int start)
{
    while (input[*i] && !ft_strchr(" \t|<>\"'", input[*i]))
        (*i)++;
    if (*i > start)
        add_token(tokens, token_count, ft_substr(input, start, *i - start), TOKEN_WORD);
}
