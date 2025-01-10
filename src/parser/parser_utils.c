/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wander <wander@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2025/01/10 13:33:26 by wander           ###   ########.fr       */
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
		if (parser_is_redirect(current->type) && current->next)
			current = current->next->next;
		else if (current->type == TOKEN_WORD)
		{
			args[i] = ft_strdup(current->content);
			if (!args[i])
				return (parser_handle_arg_error(args, i));
			i++;
			current = current->next;
		}
		else
			current = current->next;
	}
	return ((args[i] = NULL), args);
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
