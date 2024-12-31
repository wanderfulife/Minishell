/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jowander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:02:21 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/31 14:08:07 by jowander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int	handle_file_not_found(char *file)
{
	(void)file;
	return (-1);
}

int	handle_open_error(char *file)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	if (access(file, F_OK) == -1)
		ft_putendl_fd(": No such file or directory", 2);
	else if (access(file, W_OK) == -1 || errno == EACCES)
		ft_putendl_fd(": Permission denied", 2);
	else
		ft_putendl_fd(": Error opening file", 2);
	return (-1);
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
