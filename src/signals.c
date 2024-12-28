/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JWander <jowander@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:02:21 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/28 22:39:18 by JWander          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	close(0);
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}
