/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>           +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/28 22:28:25 by JoWander          ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	shell_free_command(t_shell *shell)
{
	if (shell->current_cmd)
	{
		parser_destroy_command(shell->current_cmd);
		shell->current_cmd = NULL;
	}
}

int	shell_process_input(t_shell *shell, char *input)
{
	t_token	*tokens;
	char	*expanded;

	if (!input || !*input)
		return (1);
	expanded = env_expand_vars(input, shell);
	if (!expanded)
		return (1);
	tokens = lexer_tokenize(expanded);
	free(expanded);
	if (!tokens)
		return (1);
	shell->current_cmd = parser_parse_tokens(tokens);
	lexer_destroy(tokens);
	if (!shell->current_cmd)
		return (1);
	shell->last_exit_status = executor_run_command(shell->current_cmd, shell);
	shell_free_command(shell);
	return (1);
}

int	shell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		shell_reset_signals();
		input = readline(PROMPT);
		if (!input)
		{
			if (shell->current_cmd)
			{
				parser_destroy_command(shell->current_cmd);
				shell->current_cmd = NULL;
			}
			break;
		}
		if (*input)
			add_history(input);
		if (!shell_process_input(shell, input))
		{
			free(input);
			if (shell->current_cmd)
			{
				parser_destroy_command(shell->current_cmd);
				shell->current_cmd = NULL;
			}
			break;
		}
		free(input);
	}
	return (shell->last_exit_status);
}

void	shell_reset_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
}
