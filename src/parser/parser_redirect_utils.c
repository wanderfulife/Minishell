/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wander <wander@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2025/01/10 13:33:19 by wander           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	parser_is_redirect(int type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

void	parser_skip_redirect(t_token **tokens)
{
	if (!tokens || !*tokens)
		return ;
	*tokens = (*tokens)->next;
	if (*tokens)
		*tokens = (*tokens)->next;
}
