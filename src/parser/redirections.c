/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/10/24 18:43:11 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

t_redirect	*parser_create_redirect(int type, char *file)
{
	t_redirect	*redir;

	redir = (t_redirect *)malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}

static void	parser_add_redirect_to_list(t_command *cmd, t_redirect *new_redir)
{
	t_redirect	*current;

	if (!cmd->redirects)
	{
		cmd->redirects = new_redir;
		return ;
	}
	current = cmd->redirects;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

int	parser_add_redirect(t_command *cmd, t_token **tokens)
{
	t_redirect	*redir;
	int			type;
	t_token		*file_token;

	if (!tokens || !*tokens || !(*tokens)->next)
		return (0);
	type = (*tokens)->type;
	file_token = (*tokens)->next;
	if (file_token->type != TOKEN_WORD)
		return (0);
	redir = parser_create_redirect(type, file_token->content);
	if (!redir)
		return (0);
	parser_add_redirect_to_list(cmd, redir);
	if (type == TOKEN_HEREDOC)
		if (!parser_handle_heredoc(cmd, file_token->content))
			return (0);
	parser_skip_redirect(tokens);
	return (1);
}

void	parser_destroy_redirects(t_redirect *redirect)
{
	t_redirect	*current;
	t_redirect	*next;

	current = redirect;
	while (current)
	{
		next = current->next;
		free(current->file);
		free(current);
		current = next;
	}
}

int	parser_handle_heredoc(t_command *cmd, char *delim)
{
	t_redirect	*redir;

	(void)delim;
	redir = cmd->redirects;
	while (redir->next)
		redir = redir->next;
	return (parser_setup_heredoc(redir));
}
