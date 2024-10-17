/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   space_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:09:30 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/17 19:13:17 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int handle_spaces(char *line, int *i, char *buffer, int *buf_idx, t_token **head) {
    if (isspace(line[*i])) {
        if (*buf_idx > 0) {
            buffer[*buf_idx] = '\0';
            t_token *new_token = create_token(buffer, ARG);
            add_token_to_list(head, new_token);
            *buf_idx = 0;
        }
        return 1;
    }
    return 0;
}
