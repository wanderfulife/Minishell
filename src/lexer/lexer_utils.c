/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/10/25 15:39:36 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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

char	*join_word_parts(char **parts)
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

char	*get_next_word_part(char *input, int *i)
{
	char	buffer[1024];
	int		j;

	j = 0;
	while (input[*i] && !lexer_is_operator(input[*i]) && input[*i] != ' '
		&& input[*i] != '\t' && input[*i] != '\'' && input[*i] != '"')
	{
		buffer[j++] = input[*i];
		(*i)++;
	}
	buffer[j] = '\0';
	return (ft_strdup(buffer));
}

char	*handle_quoted_part(char *input, int *i, char quote)
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
