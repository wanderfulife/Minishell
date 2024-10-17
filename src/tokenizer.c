/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:09:51 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/17 19:27:39 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Allocates and initializes a token
t_token	*create_token(char *str, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		perror("malloc failed");
		exit(1);
	}
	token->str = strdup(str);
	token->type = type;
	token->next = NULL;
	return (token);
}
t_token *tokenize_input(char *line) {
    t_token *head = NULL;
    char buffer[1024];
    int i = 0, buf_idx = 0;
    int in_single_quote = 0, in_double_quote = 0;
    t_parse_context ctx = {line, &i, buffer, &buf_idx};  // Initialize context

    while (line[i] != '\0') {
        in_single_quote = handle_single_quote(line[i], in_single_quote, in_double_quote);
        in_double_quote = handle_double_quote(line[i], in_single_quote, in_double_quote);

        if (handle_redirection_and_pipes(&ctx, &head))
            continue;

        if (handle_spaces(line, &i, buffer, &buf_idx, &head))
            i++;
        else {
            buffer[buf_idx++] = line[i++];
        }
    }

    if (buf_idx > 0) {
        buffer[buf_idx] = '\0';
        t_token *new_token = create_token(buffer, ARG);
        add_token_to_list(&head, new_token);
    }

    return head;
}

