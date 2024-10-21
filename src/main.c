/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:53:23 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/21 17:43:02 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	setup_project(void)
{
	// TODO: Initialize any necessary data structures or settings
}

void	process_input(char *input)
{
	t_token	*tokens;
	int		i;

	tokens = tokenize_input(input);
	if (tokens)
	{
		i = 0;
		while (tokens[i].value)
		{
			ft_printf("Token: %s, Type: %d\n", tokens[i].value, tokens[i].type);
			i++;
		}
		free_tokens(tokens);
	}
}

int	main(void)
{
	char *input;

	setup_project();
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			process_input(input);
		}
		free(input);
	}
	ft_printf("Exiting minishell\n");
	return (0);
}