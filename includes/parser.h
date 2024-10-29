/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/29 13:32:11 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"

typedef struct s_redirect
{
	int					type;
	char				*file;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_command
{
	char				**args;
	t_redirect			*redirects;
	struct s_command	*pipe_next;
	int					pipe_fd[2];
}	t_command;

/* parser.c */
t_command	*parser_create_command(void);
t_command	*parser_parse_tokens(t_token *tokens);
void		parser_destroy_command(t_command *cmd);

/* parser_utils.c */
int			parser_count_args(t_token *tokens);
char		**parser_create_args(t_token *tokens, int count);
int			parser_is_redirect(int type);
void		parser_skip_redirect(t_token **tokens);

/* redirections.c */
t_redirect	*parser_create_redirect(int type, char *file);
int			parser_add_redirect(t_command *cmd, t_token **tokens);
void		parser_destroy_redirects(t_redirect *redirect);
int			parser_handle_heredoc(t_command *cmd, char *delim);

/* heredoc.c */
int			parser_setup_heredoc(t_redirect *redir);
void		parser_close_heredoc(void);
int			parser_heredoc_count(t_command *cmd);

#endif