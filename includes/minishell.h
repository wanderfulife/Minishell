#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_DQUOTE,
	TOKEN_SQUOTE,
	TOKEN_SPACE
}					t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
}					t_token;

// Function prototypes
void				setup_project(void);
t_token				*tokenize_input(char *input);
t_token_type		get_token_type(char c, char next_c);
void				add_token(t_token **tokens, int *token_count, char *value,
						t_token_type type);
void				handle_quotes(char *input, int *i, t_token_type quote_type);
void				process_input(char *input);
void				free_tokens(t_token *tokens);
void				skip_whitespace(char *input, int *i);
t_token				*initialize_empty_token(void);
void				parse_token(t_token **tokens, int *token_count, char *input,
						int *i);
void				parse_quoted_token(t_token **tokens, int *token_count,
						char *input, int *i);
void				parse_redir_append_or_heredoc(t_token **tokens,
						int *token_count, char *input, int *i, int start);
void				parse_pipe_or_redir(t_token **tokens, int *token_count,
						char *input, int *i, int start);
void				parse_word_token(t_token **tokens, int *token_count,
						char *input, int *i, int start);

#endif