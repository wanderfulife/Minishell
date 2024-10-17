/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:16:57 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/17 19:27:51 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Standard headers
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>
# include <errno.h>
# include <fcntl.h>           // For open() and O_* constants
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
#include <ctype.h>

# define INITIAL_TOKEN_SIZE 10  // Start with 10 tokens
# define TOKEN_INCREMENT 10     // Increase by 10 when more space is needed

// Token types enum
typedef enum e_token_type {
    CMD,          // Command (e.g., "ls", "echo")
    ARG,          // Argument (e.g., "-l", "file.txt")
    PIPE,         // Pipe ("|")
    REDIRECT_IN,  // Input redirection ("<")
    REDIRECT_OUT, // Output redirection (">")
    APPEND_OUT,   // Output redirection with append (">>")
    HERE_DOC      // Here document ("<<")
} t_token_type;

// Token structure
typedef struct s_token {
    char            *str;      // The actual token string (e.g., "ls", "-l", "|")
    t_token_type    type;      // The type of token (command, argument, pipe, etc.)
    struct s_token  *next;     // Pointer to the next token in the list
} t_token;

// Command structure
typedef struct s_cmd {
    char            **argv;    // Array of arguments (e.g., ["ls", "-l", NULL])
    int             infile;    // Input file descriptor (default: 0 for stdin)
    int             outfile;   // Output file descriptor (default: 1 for stdout)
    int             pipe;      // Pipe file descriptor
    struct s_cmd    *next;     // Pointer to the next command (for piping)
} t_cmd;

typedef struct s_parse_context {
    char *line;
    int *i;
    char *buffer;
    int *buf_idx;
} t_parse_context;


// Function prototypes for token management
t_token *create_token(char *str, t_token_type type);
t_token *add_token_to_list(t_token **head, t_token *new_token);
t_token *tokenize_input(char *line);
void free_tokens(t_token *tokens);

// Function prototypes for command handling
t_cmd   *create_cmd(void);
void    handle_redirection_tokens(t_cmd *cmd, t_token **tokens);
t_cmd   *build_cmd_list(t_token *tokens);
void    add_argument_to_cmd(t_cmd *cmd, char *arg, int *argc);
void    free_cmd_list(t_cmd *cmds);

// Function prototypes for execution and redirection handling
void    handle_redirection(t_cmd *cmd);
void    execute_single_command(t_cmd *cmd);
void    execute_command(t_cmd *cmd);

// Function prototypes for built-in commands
void    ft_cd(char *path);
void    ft_pwd(void);
void    ft_exit(void);

// Declaration for functions used in tokenizer
int handle_single_quote(char c, int in_single_quote, int in_double_quote);
int handle_double_quote(char c, int in_single_quote, int in_double_quote);
int handle_token_delimiters(char *line, int *i, char *buffer, int *buf_idx, t_token **head);
int handle_var_expansion(char *buffer, const char *line, int *i, int *buf_idx);
void copy_to_buffer(char *buffer, char c, int *buf_idx);

// Declaration for functions used in parser
int handle_redirection_and_pipes(t_parse_context *ctx, t_token **head);
int handle_spaces(char *line, int *i, char *buffer, int *buf_idx, t_token **head);
#endif
