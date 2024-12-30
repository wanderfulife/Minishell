/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcohen <jcohen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/30 18:05:09 by jcohen           ###   ########.fr       */
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
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (-1);
	}
	flags = get_open_flags(type);
	if (flags == -1)
		return (-1);
	fd = open(file, flags, 0644);
	if (fd == -1)
		return (handle_open_error(file));
	return (fd);
}

static int	get_saved_fd(int type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC)
		return (STDIN_FILENO);
	return (STDOUT_FILENO);
}

static int	handle_single_redirect(t_redirect *current)
{
	int	fd;
	int	saved_fd;

	if (current->type == TOKEN_HEREDOC)
		fd = open(current->file, O_RDONLY);
	else
		fd = executor_open_file(current->file, current->type);
	if (fd == -1)
		return (0);
	saved_fd = get_saved_fd(current->type);
	if (dup2(fd, saved_fd) == -1)
	{
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

int	executor_setup_redirects(t_redirect *redirs)
{
	t_redirect	*current;
	int			status;

	status = 1;
	current = redirs;
	while (current && status)
	{
		status = handle_single_redirect(current);
		current = current->next;
	}
	return (status);
}
