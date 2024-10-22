/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:07:01 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/22 13:21:21 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_token(t_token **tokens, int *token_count, char *value,
		t_token_type type)
{
	t_token	*new_tokens;
	int		i;

	new_tokens = ft_calloc(*token_count + 2, sizeof(t_token));
	if (!new_tokens)
		return ;
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

t_token_type	get_token_type(char c, char next_c)
{
	if (c == '>' && next_c == '>')
		return (TOKEN_REDIR_APPEND);
	if (c == '<' && next_c == '<')
		return (TOKEN_HEREDOC);
	if (c == '|')
		return (TOKEN_PIPE);
	if (c == '<')
		return (TOKEN_REDIR_IN);
	if (c == '>')
		return (TOKEN_REDIR_OUT);
	if (c == '"')
		return (TOKEN_DQUOTE);
	if (c == '\'')
		return (TOKEN_SQUOTE);
	if (c == ' ' || c == '\t')
		return (TOKEN_SPACE);
	return (TOKEN_WORD);
}

void	handle_quotes(char *input, int *i, t_token_type quote_type)
{
	(*i)++;
	while (input[*i] && input[*i] != (quote_type == TOKEN_DQUOTE ? '"' : '\''))
		(*i)++;
	if (input[*i])
		(*i)++;
}

void	free_tokens(t_token *tokens)
{
	int	i;

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

    if (!input || !*input)
        return initialize_empty_token();

    while (input[i])
    {
        skip_whitespace(input, &i);
        if (!input[i])
            break;
        parse_token(&tokens, &token_count, input, &i);
    }
    
    if (!tokens)
        return initialize_empty_token();
    
    return tokens;
}

void skip_whitespace(char *input, int *i)
{
    while (input[*i] == ' ' || input[*i] == '\t')
        (*i)++;
}

t_token *initialize_empty_token(void)
{
    t_token *tokens = ft_calloc(1, sizeof(t_token));
    if (!tokens)
        return NULL;
    tokens[0].value = NULL;
    return tokens;
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
