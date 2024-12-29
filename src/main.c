/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <JoWander@student.42.fr>              +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/28 16:52:13 by JoWander           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_init(t_shell *shell, char **env)
{
	shell->env = env_init(env);
	shell->last_exit_status = 0;
	shell->in_heredoc = 0;
	shell->current_cmd = NULL;
	terminal_init(shell);
	terminal_disable_echoctl(shell);
	setup_signals();
}

void	shell_cleanup(t_shell *shell)
{
	if (shell->current_cmd)
	{
		parser_destroy_command(shell->current_cmd);
		shell->current_cmd = NULL;
	}
	if (shell->env)
	{
		env_destroy(shell->env);
		shell->env = NULL;
	}
	terminal_restore(shell);
	parser_close_heredoc();
	rl_clear_history();
	// Clean up readline's internal buffers
	rl_clear_pending_input();
	rl_cleanup_after_signal();
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	int		status;

	(void)argc;
	(void)argv;
	shell_init(&shell, env);
	if (!shell.env)
	{
		ft_putendl_fd("minishell: failed to initialize environment", 2);
		return (1);
	}
	status = shell_loop(&shell);
	shell_cleanup(&shell);
	return (status);
}
