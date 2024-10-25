/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/10/25 13:44:02 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int	handle_file_not_found(char *file)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putendl_fd(": No such file or directory", 2);
	return (-1);
}

int	handle_open_error(char *file)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	if (errno == EACCES)
		ft_putendl_fd(": Permission denied", 2);
	else
		ft_putendl_fd(": No such file or directory", 2);
	return (-1);
}

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
		return (handle_file_not_found(file));
	flags = get_open_flags(type);
	if (flags == -1)
		return (-1);
	fd = open(file, flags, 0644);
	if (fd == -1)
		return (handle_open_error(file));
	return (fd);
}

int	executor_setup_redirects(t_redirect *redirs)
{
	t_redirect	*current;
	int			status;
	int			fd;

	status = 1;
	current = redirs;
	while (current && status)
	{
		fd = executor_open_file(current->file, current->type);
		if (fd == -1)
			return (0);
		if (current->type == TOKEN_REDIR_IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		current = current->next;
	}
	return (status);
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
