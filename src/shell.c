/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/10/24 12:07:08 by JoWander         ###   ########.fr       */
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
// In src/shell.c

int	shell_process_input(t_shell *shell, char *input)
{
	t_token		*tokens;
	char		*expanded;

	if (!input || !*input)
		return (1);
	expanded = env_expand_vars(ft_strdup(input), shell);
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
	shell->current_cmd = NULL;  // Ensure command is cleared
	return (1);
}

int	shell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		shell_reset_signals();
		input = readline(PROMPT);
		
		// Handle EOF (Ctrl+D) or errors
		if (!input)
		{
			ft_putendl_fd("exit", 2);
			break ;
		}

		// Add to history if not empty
		if (*input)
			add_history(input);

		// Process input and check for exit
		if (!shell_process_input(shell, input))
		{
			free(input);
			break ;
		}

		free(input);
	}
	// No need for rl_clear_history() as it's handled in cleanup
	return (shell->last_exit_status);
}



void	shell_reset_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
}