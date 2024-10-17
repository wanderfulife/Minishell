/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:09:30 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/17 19:33:03 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_double_redirection(t_parse_context *ctx, t_token **head)
{
	t_token	*new_token;

	ctx->buffer[0] = ctx->line[*ctx->i];
	ctx->buffer[1] = ctx->line[*ctx->i + 1];
	ctx->buffer[2] = '\0';
	(*ctx->i)++; // Skip the next character
	new_token = create_token(ctx->buffer, REDIRECT_OUT);
	add_token_to_list(head, new_token);
}

void	handle_single_redirection(t_parse_context *ctx, t_token **head)
{
	t_token_type	type;
	t_token			*new_token;

	ctx->buffer[0] = ctx->line[*ctx->i];
	ctx->buffer[1] = '\0';
	if (ctx->line[*ctx->i] == '|')
		type = PIPE;
	else if (ctx->line[*ctx->i] == '<')
		type = REDIRECT_IN;
	else
		type = REDIRECT_OUT;
	new_token = create_token(ctx->buffer, type);
	add_token_to_list(head, new_token);
}

int	is_redirection_or_pipe(char c)
{
	return (c == '>' || c == '<' || c == '|');
}

void	flush_buffer(t_parse_context *ctx, t_token **head)
{
	t_token	*new_token;

	if (*ctx->buf_idx > 0)
	{
		ctx->buffer[*ctx->buf_idx] = '\0';
		new_token = create_token(ctx->buffer, ARG);
		add_token_to_list(head, new_token);
		*ctx->buf_idx = 0;
	}
}

int	handle_redirection_and_pipes(t_parse_context *ctx, t_token **head)
{
	int	redirection_found;

	redirection_found = 0;
	while (is_redirection_or_pipe(ctx->line[*ctx->i]))
	{
		redirection_found = 1;
		flush_buffer(ctx, head);
		if ((ctx->line[*ctx->i] == '>' && ctx->line[*ctx->i + 1] == '>')
			|| (ctx->line[*ctx->i] == '<' && ctx->line[*ctx->i + 1] == '<'))
		{
			handle_double_redirection(ctx, head);
		}
		else
		{
			handle_single_redirection(ctx, head);
		}
		(*ctx->i)++; // Move past the current redirection token
		// Move past any spaces to get to the next part of the input
		while (ctx->line[*ctx->i] == ' ')
			(*ctx->i)++;
	}
	return (redirection_found);
}
