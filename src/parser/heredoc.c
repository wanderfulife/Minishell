/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wander <wander@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/30 15:26:24 by wander           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
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

static int	read_heredoc_input(int fd, char *delimiter)
{
	char	*line;
	int		status;
	size_t	delim_len;

	status = 1;
	delim_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, delim_len + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	return (status);
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
	if (!read_heredoc_input(fd, redir->file))
	{
		close(fd);
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
	char	*num;
	char	*filename;
	int		i;

	i = 0;
	while (i < 100)
	{
		num = ft_itoa(i);
		if (!num)
			return ;
		filename = ft_strjoin("/tmp/.heredoc_", num);
		free(num);
		if (!filename)
			return ;
		unlink(filename);
		free(filename);
		i++;
	}
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
