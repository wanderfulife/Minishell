/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/29 13:32:35 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*lexer_handle_quotes(char *input, int *i, char quote)
{
	char	word[4096];
	int		j;

	j = 0;
	(*i)++;
	while (input[*i] && input[*i] != quote)
	{
		word[j++] = input[*i];
		(*i)++;
	}
	if (input[*i] == quote)
		(*i)++;
	word[j] = '\0';
	return (lexer_create_token(word, TOKEN_WORD));
}

int	lexer_skip_spaces(char *input, int *i)
{
	while (input[*i] && (input[*i] == ' ' || input[*i] == '\t'))
		(*i)++;
	return (input[*i] != '\0');
}

void	lexer_clear_tokens(t_token **list)
{
	if (!list || !*list)
		return ;
	lexer_destroy(*list);
	*list = NULL;
}

int	lexer_count_tokens(t_token *list)
{
	int		count;
	t_token	*current;

	count = 0;
	current = list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

t_token	*lexer_tokenize(char *input)
{
	t_token	*tokens;
	t_token	*new;
	int		i;

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
