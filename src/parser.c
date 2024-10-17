/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:18:02 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/17 18:27:56 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

// Allocates memory for a command
t_cmd *create_cmd() {
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd) {        perror("malloc failed");
        exit(1);
    }
    cmd->argv = NULL;
    cmd->infile = 0;  // stdin
    cmd->outfile = 1; // stdout
    cmd->next = NULL;
    return cmd;
}

// Handles redirection tokens
void handle_redirection_tokens(t_cmd *cmd, t_token **tokens) {
    if ((*tokens)->type == REDIRECT_IN) {
        (*tokens) = (*tokens)->next;
        cmd->infile = open((*tokens)->str, O_RDONLY);
    } else if ((*tokens)->type == REDIRECT_OUT) {
        (*tokens) = (*tokens)->next;
        cmd->outfile = open((*tokens)->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    } else if ((*tokens)->type == APPEND_OUT) {
        (*tokens) = (*tokens)->next;
        cmd->outfile = open((*tokens)->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
    }
}

// Builds the command list from the token list
t_cmd *build_cmd_list(t_token *tokens) {
    t_cmd *cmd_list = NULL;
    t_cmd *current_cmd = NULL;
    int argc = 0;

    while (tokens) {
        if (tokens->type == PIPE) {
            current_cmd = NULL;
        } else if (tokens->type == REDIRECT_IN || tokens->type == REDIRECT_OUT || tokens->type == APPEND_OUT) {
            handle_redirection_tokens(current_cmd, &tokens);
        } else {
            if (!current_cmd) {
                current_cmd = create_cmd();
                if (!cmd_list) cmd_list = current_cmd;
            }
            add_argument_to_cmd(current_cmd, tokens->str, &argc);
        }
        tokens = tokens->next;
    }

    return cmd_list;
}
