/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:07:01 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/22 17:14:37 by JoWander         ###   ########.fr       */
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



t_token *initialize_empty_token(void)
{
    t_token *tokens = ft_calloc(1, sizeof(t_token));
    if (!tokens)
        return NULL;
    tokens[0].value = NULL;
    return tokens;
}
