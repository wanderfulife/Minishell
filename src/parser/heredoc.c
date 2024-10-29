/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/29 13:32:11 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <fcntl.h>
#include <unistd.h>

static char	*get_heredoc_filename(void)
{
	static int	count = 0;
	char		*num;
	char		*name;

	num = ft_itoa(count++);
	if (!num)
		return (NULL);
	name = ft_strjoin("/tmp/.heredoc_", num);
	free(num);
	return (name);
}

int	parser_setup_heredoc(t_redirect *redir)
{
	char	*filename;
	int		fd;

	filename = get_heredoc_filename();
	if (!filename)
		return (0);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(filename);
		return (0);
	}
	free(redir->file);
	redir->file = filename;
	close(fd);
	return (1);
}

void	parser_close_heredoc(void)
{
	unlink("/tmp/.heredoc_0");
}

int	parser_heredoc_count(t_command *cmd)
{
	t_redirect	*redir;
	int			count;

	count = 0;
	while (cmd)
	{
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
				count++;
			redir = redir->next;
		}
		cmd = cmd->pipe_next;
	}
	return (count);
}
