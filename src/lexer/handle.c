/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:39:09 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/25 15:41:51 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	handle_part(char *parts[1024], char *input, int *i, int *part_count)
{
	if (input[*i] == '\'' || input[*i] == '"')
		parts[*part_count] = handle_quoted_part(input, i, input[*i]);
	else
		parts[*part_count] = get_next_word_part(input, i);
	if (!parts[*part_count])
		return (0);
	(*part_count)++;
	return (1);
}

t_token	*lexer_handle_word(char *input, int *i)
{
	char	*parts[1024];
	int		part_count;

	part_count = 0;
	while (input[*i] && !lexer_is_operator(input[*i]) && input[*i] != ' '
		&& input[*i] != '\t')
	{
		if (!handle_part(parts, input, i, &part_count))
			return (NULL);
	}
	return (cleanup_and_create_token(parts, part_count));
}
