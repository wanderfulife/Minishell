/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/10/24 18:44:51 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	lexer_add_token(t_token **list, t_token *new)
{
	t_token	*current;

	if (!list || !new)
		return ;
	if (!*list)
	{
		*list = new;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new;
}

int	lexer_is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token	*lexer_handle_operator(char *input, int *i)
{
	char	op[3];
	int		type;

	op[0] = input[*i];
	op[1] = '\0';
	op[2] = '\0';
	if (input[*i + 1] == input[*i])
	{
		op[1] = input[*i];
		(*i)++;
	}
	type = TOKEN_PIPE;
	if (op[0] == '<' && !op[1])
		type = TOKEN_REDIR_IN;
	else if (op[0] == '>' && !op[1])
		type = TOKEN_REDIR_OUT;
	else if (op[0] == '>' && op[1])
		type = TOKEN_REDIR_APPEND;
	else if (op[0] == '<' && op[1])
		type = TOKEN_HEREDOC;
	(*i)++;
	return (lexer_create_token(op, type));
}

static char	*join_word_parts(char **parts)
{
	char	*result;
	char	*temp;
	int		i;

	if (!parts[0])
		return (ft_strdup(""));
	result = ft_strdup(parts[0]);
	i = 1;
	while (parts[i])
	{
		temp = result;
		result = ft_strjoin(result, parts[i]);
		free(temp);
		i++;
	}
	return (result);
}

static char	*get_next_word_part(char *input, int *i)
{
	char	buffer[1024];
	int		j;

	j = 0;
	while (input[*i] && !lexer_is_operator(input[*i]) && input[*i] != ' ' && input[*i] != '\t' && input[*i] != '\'' && input[*i] != '"')
	{
		buffer[j++] = input[*i];
		(*i)++;
	}
	buffer[j] = '\0';
	return (ft_strdup(buffer));
}

static char	*handle_quoted_part(char *input, int *i, char quote)
{
	char	buffer[1024];
	int		j;

	j = 0;
	(*i)++;
	while (input[*i] && input[*i] != quote)
	{
		buffer[j++] = input[*i];
		(*i)++;
	}
	if (input[*i] == quote)
		(*i)++;
	buffer[j] = '\0';
	return (ft_strdup(buffer));
}

t_token	*lexer_handle_word(char *input, int *i)
{
	char	*parts[1024];
	int		part_count;
	char	*word;
	t_token	*token;
	
	part_count = 0;
	while (input[*i] && !lexer_is_operator(input[*i]) && 
		input[*i] != ' ' && input[*i] != '\t')
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			parts[part_count] = handle_quoted_part(input, i, input[*i]);
			if (!parts[part_count])
				return (NULL);
			part_count++;
		}
		else
		{
			parts[part_count] = get_next_word_part(input, i);
			if (!parts[part_count])
				return (NULL);
			part_count++;
		}
	}
	parts[part_count] = NULL;
	word = join_word_parts(parts);
	while (--part_count >= 0)
		free(parts[part_count]);
	token = lexer_create_token(word, TOKEN_WORD);
	free(word);
	return (token);
}
