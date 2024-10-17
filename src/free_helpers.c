/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:18:02 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/17 18:27:48 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Frees the linked list of tokens
void free_tokens(t_token *tokens) {
    t_token *temp;
    while (tokens) {
        temp = tokens;
        tokens = tokens->next;
        free(temp->str);
        free(temp);
    }
}

// Frees the linked list of commands
void free_cmd_list(t_cmd *cmds) {
    t_cmd *temp;
    while (cmds) {
        temp = cmds;
        cmds = cmds->next;
        for (int i = 0; temp->argv[i]; i++) {
            free(temp->argv[i]);
        }
        free(temp->argv);
        free(temp);
    }
}
