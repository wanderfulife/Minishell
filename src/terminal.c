/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/29 13:32:11 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	terminal_init(t_shell *shell)
{
	tcgetattr(STDIN_FILENO, &shell->original_term);
}

void	terminal_restore(t_shell *shell)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &shell->original_term);
}

void	terminal_disable_echoctl(t_shell *shell)
{
	struct termios	term;

	(void)shell;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
