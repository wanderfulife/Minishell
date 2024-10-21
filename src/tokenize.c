/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:07:01 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/21 18:00:34 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void add_token(t_token **tokens, int *token_count, char *value, t_token_type type)
{
    t_token *new_tokens;
    int i;

    new_tokens = ft_calloc(*token_count + 2, sizeof(t_token));
    if (!new_tokens)
        return;
    i = 0;
    while (i < *token_count)
    {
        new_tokens[i] = (*tokens)[i];
        i++;
    }
    new_tokens[*token_count].value = ft_strdup(value);
    new_tokens[*token_count].type = type;
    (*token_count)++;
    free(*tokens);
    *tokens = new_tokens;
}

t_token_type get_token_type(char c, char next_c)
{
    if (c == '>' && next_c == '>')
        return TOKEN_REDIR_APPEND;
    if (c == '<' && next_c == '<')
        return TOKEN_HEREDOC;
    if (c == '|')
        return TOKEN_PIPE;
    if (c == '<')
        return TOKEN_REDIR_IN;
    if (c == '>')
        return TOKEN_REDIR_OUT;
    if (c == '"')
        return TOKEN_DQUOTE;
    if (c == '\'')
        return TOKEN_SQUOTE;
    if (c == ' ' || c == '\t')
        return TOKEN_SPACE;
    return TOKEN_WORD;
}

void handle_quotes(char *input, int *i, t_token_type quote_type)
{
    (*i)++;
    while (input[*i] && input[*i] != (quote_type == TOKEN_DQUOTE ? '"' : '\''))
        (*i)++;
    if (input[*i])
        (*i)++;
}

void free_tokens(t_token *tokens)
{
    int i;

    i = 0;
    while (tokens[i].value)
    {
        free(tokens[i].value);
        i++;
    }
    free(tokens);
}


t_token *tokenize_input(char *input)
{
    t_token *tokens = NULL;
    int token_count = 0;
    int i = 0;
    int start = 0;
    t_token_type type;

    while (input[i])
    {
        while (input[i] == ' ' || input[i] == '\t')
            i++;
        start = i;
        type = get_token_type(input[i], input[i + 1]);
        if (type == TOKEN_SQUOTE || type == TOKEN_DQUOTE)
        {
            char quote = input[i];
            i++;
            while (input[i] && input[i] != quote)
                i++;
            if (input[i] == quote)
                i++;
            // Whether the quote is closed or not, we treat it as part of the word
            add_token(&tokens, &token_count, ft_substr(input, start, i - start), TOKEN_WORD);
        }
        else if (type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC)
        {
            add_token(&tokens, &token_count, ft_substr(input, i, 2), type);
            i += 2;
        }
        else if (type == TOKEN_PIPE || type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT)
        {
            add_token(&tokens, &token_count, ft_substr(input, i, 1), type);
            i++;
        }
        else // This includes TOKEN_WORD and any unrecognized characters
        {
            while (input[i] && !ft_strchr(" \t|<>", input[i]))
                i++;
            add_token(&tokens, &token_count, ft_substr(input, start, i - start), TOKEN_WORD);
        }
    }
    return tokens;
}