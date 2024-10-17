/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:18:02 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/17 18:55:59 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// token_helpers.c
t_token *add_token_to_list(t_token **head, t_token *new_token) {
    if (!*head) {
        *head = new_token;  // Modify the head of the list
    } else {
        t_token *temp = *head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = new_token;
    }
    return *head;
}



// In token_helpers.c
int handle_token_delimiters(char *line, int *i, char *buffer, int *buf_idx, t_token **head) {
    if (line[*i] == ' ' && *buf_idx > 0) {
        buffer[*buf_idx] = '\0';
        t_token *new_token = create_token(buffer, ARG);
        add_token_to_list(head, new_token);
        *buf_idx = 0;
    }
    return *i;
}
