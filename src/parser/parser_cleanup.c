/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/30 16:11:06 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

static void	parser_free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
}

static void	parser_cleanup_redirects(t_redirect *redirects)
{
	if (!redirects)
		return ;
	parser_destroy_redirects(redirects);
}

static void	parser_cleanup_pipe(t_command *pipe_next)
{
	if (!pipe_next)
		return ;
	parser_destroy_command(pipe_next);
}

void	parser_destroy_command(t_command *cmd)
{
	if (!cmd)
		return ;
	parser_free_args(cmd->args);
	parser_cleanup_redirects(cmd->redirects);
	parser_cleanup_pipe(cmd->pipe_next);
	free(cmd);
}
