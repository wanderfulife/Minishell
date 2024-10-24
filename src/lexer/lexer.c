/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/10/24 11:17:14 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdlib.h>

t_token	*lexer_create_token(char *content, int type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->content = ft_strdup(content);
	if (!token->content)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

static int	lexer_process_token(t_token **tokens, t_token *new)
{
	if (!new)
		return (0);
	lexer_add_token(tokens, new);
	return (1);
}

t_token	*lexer_tokenize(char *input)
{
	t_token	*tokens;
	int		i;
	t_token	*new;

	if (!input)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (!lexer_skip_spaces(input, &i))
			continue ;
		if (input[i] == '\'' || input[i] == '"')
			new = lexer_handle_quotes(input, &i, input[i]);
		else if (lexer_is_operator(input[i]))
			new = lexer_handle_operator(input, &i);
		else
			new = lexer_handle_word(input, &i);
		if (!lexer_process_token(&tokens, new))
			return (lexer_clear_tokens(&tokens), NULL);
	}
	return (tokens);
}

void	lexer_destroy(t_token *list)
{
	t_token	*current;
	t_token	*next;

	current = list;
	while (current)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
}