/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:18:02 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/17 18:17:14 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Runs a single iteration of the shell loop
void run_shell_iteration(void) {
    char *line;
    t_token *tokens;
    t_cmd *commands;

    line = readline("minishell> ");
    if (!line) return;

    if (*line) {
        add_history(line);
        tokens = tokenize_input(line);
        commands = build_cmd_list(tokens);

        if (commands) {
            execute_command(commands);
            free_cmd_list(commands);
        }

        free_tokens(tokens);
    }

    free(line);
}

int main(void) {
    while (1) {
        run_shell_iteration();
    }
    return 0;
}
