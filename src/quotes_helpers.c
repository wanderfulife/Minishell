/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:09:30 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/17 19:09:59 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int handle_single_quote(char c, int in_single_quote, int in_double_quote) {
    if (c == '\'' && !in_double_quote)
        return !in_single_quote;
    return in_single_quote;
}

int handle_double_quote(char c, int in_single_quote, int in_double_quote) {
    if (c == '"' && !in_single_quote)
        return !in_double_quote;
    return in_double_quote;
}


// Copies characters into the buffer within single or double quotes
void copy_to_buffer(char *buffer, char c, int *buf_idx) {
    buffer[(*buf_idx)++] = c;
}
