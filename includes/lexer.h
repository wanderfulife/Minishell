/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/10/25 15:44:27 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "libft.h"
# include <stdlib.h>

# define TOKEN_WORD 1
# define TOKEN_PIPE 2
# define TOKEN_REDIR_IN 3
# define TOKEN_REDIR_OUT 4
# define TOKEN_REDIR_APPEND 5
# define TOKEN_HEREDOC 6

typedef struct s_token
{
	char			*content;
	int				type;
	struct s_token	*next;
}					t_token;

/* lexer.c */
t_token				*lexer_create_token(char *content, int type);
t_token				*lexer_tokenize(char *input);
void				lexer_destroy(t_token *list);

/* lexer_utils.c */
void				lexer_add_token(t_token **list, t_token *new);
t_token				*lexer_handle_operator(char *input, int *i);
int					lexer_is_operator(char c);
t_token				*lexer_handle_word(char *input, int *i);

/* token.c */
t_token				*lexer_handle_quotes(char *input, int *i, char quote);
int					lexer_skip_spaces(char *input, int *i);
void				lexer_clear_tokens(t_token **list);
int					lexer_count_tokens(t_token *list);
char				*handle_quoted_part(char *input, int *i, char quote);
char				*get_next_word_part(char *input, int *i);
char				*join_word_parts(char **parts);
int					lexer_process_token(t_token **tokens, t_token *new);
t_token				*cleanup_and_create_token(char *parts[1024],
						int part_count);

#endif