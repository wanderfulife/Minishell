/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jowander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/31 14:10:00 by jowander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

t_command	*parser_create_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->pipe_next = NULL;
	cmd->pipe_fd[0] = -1;
	cmd->pipe_fd[1] = -1;
	return (cmd);
}

static int	parser_fill_command(t_command *cmd, t_token **tokens)
{
	int		arg_count;
	t_token	*current;

	current = *tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (parser_is_redirect(current->type))
		{
			if (!parser_add_redirect(cmd, &current))
				return (0);
			continue ;
		}
		current = current->next;
	}
	arg_count = parser_count_args(*tokens);
	cmd->args = parser_create_args(*tokens, arg_count);
	if (!cmd->args)
		return (0);
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
		*tokens = (*tokens)->next;
	return (1);
}

static t_command	*parser_handle_pipe(t_token **tokens)
{
	t_command	*cmd;
	t_command	*next_cmd;

	cmd = parser_create_command();
	if (!cmd)
		return (NULL);
	if (!parser_fill_command(cmd, tokens))
	{
		parser_destroy_command(cmd);
		return (NULL);
	}
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		next_cmd = parser_handle_pipe(tokens);
		if (!next_cmd)
		{
			parser_destroy_command(cmd);
			return (NULL);
		}
		cmd->pipe_next = next_cmd;
	}
	return (cmd);
}

t_command	*parser_parse_tokens(t_token *tokens)
{
	t_command	*cmd;
	t_token		*current;

	if (!tokens)
		return (NULL);
	current = tokens;
	cmd = parser_handle_pipe(&current);
	return (cmd);
}
