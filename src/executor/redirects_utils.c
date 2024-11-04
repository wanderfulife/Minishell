/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:02:21 by JoWander          #+#    #+#             */
/*   Updated: 2024/11/04 16:12:30 by JoWander         ###   ########.fr       */
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
