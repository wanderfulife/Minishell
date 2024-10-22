/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:25:37 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/22 17:37:27 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void handle_consecutive_quotes(t_token **tokens, int *token_count, char *input, int *i, char quote)
{
    int j = *i + 1;
    while (input[j] == quote)
        j++;
    if (j > *i + 1)
    {
        add_token(tokens, token_count, ft_strdup(""), TOKEN_WORD);
        *i = j;
        return;
    }
    add_token(tokens, token_count, ft_strdup(""), TOKEN_WORD);
}

void parse_quoted_token(t_token **tokens, int *token_count, char *input, int *i)
{
    char quote = input[*i];
    int start = *i + 1;

    (*i)++;
    while (input[*i] && input[*i] != quote)
        (*i)++;
    
    if (input[*i] == quote)
    {
        if (start == *i)
        {
            handle_consecutive_quotes(tokens, token_count, input, i, quote);
            return;
        }
        else
            add_token(tokens, token_count, ft_substr(input, start, *i - start), TOKEN_WORD);
        (*i)++;
    }
    else
    {
        add_token(tokens, token_count, ft_substr(input, start - 1, ft_strlen(input) - start + 1), TOKEN_WORD);
    }
}


void merge_quote_tokens(t_token **tokens, int *token_count)
{
    int i = 0;
    while (i < *token_count - 1)
    {
        if (tokens[i]->type == TOKEN_WORD && tokens[i + 1]->type == TOKEN_WORD &&
            (ft_strchr("\"'", tokens[i]->value[ft_strlen(tokens[i]->value) - 1]) ||
             ft_strchr("\"'", tokens[i + 1]->value[0])))
        {
            char *merged = ft_strjoin(tokens[i]->value, tokens[i + 1]->value);
            free(tokens[i]->value);
            free(tokens[i + 1]->value);
            tokens[i]->value = merged;
            remove_token(tokens, token_count, i + 1);
            continue;
        }
        i++;
    }
}
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
