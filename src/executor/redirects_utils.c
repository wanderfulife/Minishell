/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JWander <jowander@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:02:21 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/28 22:39:14 by JWander          ###   ########.fr       */
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
	(void)file;
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
