/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wander <wander@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2025/01/10 13:33:06 by wander           ###   ########.fr       */
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

char	**parser_handle_arg_error(char **args, int i)
{
	while (--i >= 0)
		free(args[i]);
	free(args);
	return (NULL);
}

void	parser_destroy_command(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->args)
		parser_free_args(cmd->args);
	if (cmd->redirects)
		parser_cleanup_redirects(cmd->redirects);
	if (cmd->pipe_next)
		parser_cleanup_pipe(cmd->pipe_next);
	if (cmd->pipe_fd[0] != -1)
		close(cmd->pipe_fd[0]);
	if (cmd->pipe_fd[1] != -1)
		close(cmd->pipe_fd[1]);
	free(cmd);
}
