/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2025/01/04 12:21:36 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

int	parser_count_args(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (parser_is_redirect(current->type))
		{
			current = current->next;
			if (current)
				current = current->next;
			continue ;
		}
		if (current->type == TOKEN_WORD)
			count++;
		current = current->next;
	}
	return (count);
}

char	**parser_create_args(t_token *tokens, int count)
{
	char	**args;
	int		i;
	t_token	*current;

	args = (char **)malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (parser_is_redirect(current->type))
		{
			current = current->next;
			if (current)
				current = current->next;
			continue ;
		}
		if (current->type == TOKEN_WORD)
		{
			args[i] = ft_strdup(current->content);
			if (!args[i])
			{
				while (--i >= 0)
					free(args[i]);
				free(args);
				return (NULL);
			}
			i++;
		}
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

int	parser_is_redirect(int type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

void	parser_skip_redirect(t_token **tokens)
{
	if (!tokens || !*tokens)
		return ;
	*tokens = (*tokens)->next;
	if (*tokens)
		*tokens = (*tokens)->next;
}
