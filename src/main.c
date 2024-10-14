/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:18:02 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/14 17:20:23 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(void) {
    char *line;
    char **tokens;

    while (1) {
        line = readline("minishell> ");
        if (!line) {
            break;  // Handle EOF (ctrl-D)
        }

        if (*line) {
            add_history(line);
            tokens = tokenize(line);  // Tokenize the input command
            
            if (tokens[0]) {
                if (strcmp(tokens[0], "cd") == 0)
                    ft_cd(tokens[1]);
                else if (strcmp(tokens[0], "pwd") == 0)
                    ft_pwd();
                else if (strcmp(tokens[0], "exit") == 0)
                    ft_exit(tokens[1], tokens, line);
                else
                    execute_command(tokens);  // Non-builtin commands
            }

            free_tokens(tokens);  // Free the tokens after execution
        }

        free(line);  // Free the line buffer after each loop iteration
    }

    return 0;
}


