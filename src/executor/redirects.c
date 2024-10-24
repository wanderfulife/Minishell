/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/10/24 18:41:13 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <fcntl.h>
#include <unistd.h>

int	executor_open_file(char *file, int type)
{
	int	flags;
	int	fd;

	if (type == TOKEN_REDIR_IN)
		flags = O_RDONLY;
	else if (type == TOKEN_REDIR_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (type == TOKEN_REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		return (-1);
	fd = open(file, flags, 0644);
	return (fd);
}

static int	executor_apply_redirect(t_redirect *redir)
{
	int	fd;

	fd = executor_open_file(redir->file, redir->type);
	if (fd == -1)
		return (0);
	if (redir->type == TOKEN_REDIR_IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	executor_setup_redirects(t_redirect *redirs)
{
	t_redirect	*current;

	current = redirs;
	while (current)
	{
		if (!executor_apply_redirect(current))
			return (0);
		current = current->next;
	}
	return (1);
}

void	executor_close_redirects(t_redirect *redirs)
{
	t_redirect	*current;
	int			fd;

	current = redirs;
	while (current)
	{
		fd = executor_open_file(current->file, current->type);
		if (fd != -1)
			close(fd);
		current = current->next;
	}
}

int	executor_handle_heredoc(t_command *cmd)
{
	t_redirect	*redir;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
		{
			if (!parser_setup_heredoc(redir))
				return (0);
		}
		redir = redir->next;
	}
	return (1);
}
