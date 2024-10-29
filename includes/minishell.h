/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/29 13:32:11 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define PROMPT "minishell$ "

# include "../libft/includes/libft.h"
# include "builtins.h"
# include "env.h"
# include "executor.h"
# include "lexer.h"
# include "parser.h"
# include "signals.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <termios.h>

/* Define the shell structure first */
typedef struct s_shell
{
	char				**env;
	int					last_exit_status;
	int					in_heredoc;
	struct termios		original_term;
	struct s_command	*current_cmd;
}						t_shell;

/* main.c */
void					shell_init(t_shell *shell, char **env);
void					shell_cleanup(t_shell *shell);

/* shell.c */
int						shell_loop(t_shell *shell);
void					shell_reset_signals(void);
int						shell_process_input(t_shell *shell, char *input);

/* signals.c */
void					setup_signals(void);
void					handle_sigint(int sig);
void					handle_sigquit(int sig);
void					restore_prompt(int sig);

/* terminal.c */
void					terminal_init(t_shell *shell);
void					terminal_restore(t_shell *shell);
void					terminal_disable_echoctl(t_shell *shell);

#endif