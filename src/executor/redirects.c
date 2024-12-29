/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/29 16:45:23 by jcohen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int	get_open_flags(int type)
{
	if (type == TOKEN_REDIR_IN)
		return (O_RDONLY);
	else if (type == TOKEN_REDIR_OUT)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	else if (type == TOKEN_REDIR_APPEND)
		return (O_WRONLY | O_CREAT | O_APPEND);
	return (-1);
}

int	executor_open_file(char *file, int type)
{
	int	flags;
	int	fd;

	if (type == TOKEN_REDIR_IN && access(file, F_OK) == -1)
		return (-1);
	flags = get_open_flags(type);
	if (flags == -1)
		return (-1);
	fd = open(file, flags, 0644);
	if (fd == -1)
		return (-1);
	return (fd);
}

int	executor_setup_redirects(t_redirect *redirs)
{
	t_redirect	*current;
	int			status;
	int			fd;
	int			saved_fd;

	status = 1;
	current = redirs;
	while (current && status)
	{
		if (current->type == TOKEN_HEREDOC)
			fd = open(current->file, O_RDONLY);
		else
			fd = executor_open_file(current->file, current->type);
		if (fd == -1)
			return (0);
		saved_fd = (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC) ? 
			STDIN_FILENO : STDOUT_FILENO;
		if (dup2(fd, saved_fd) == -1)
		{
			close(fd);
			return (0);
		}
		close(fd);
		current = current->next;
	}
	return (status);
}

int	executor_handle_heredoc(t_command *cmd)
{
	t_redirect	*redir;
	int			status;

	status = 1;
	redir = cmd->redirects;
	while (redir && status)
	{
		if (redir->type == TOKEN_HEREDOC)
		{
			if (!parser_setup_heredoc(redir))
				return (0);
		}
		redir = redir->next;
	}
	return (status);
}
