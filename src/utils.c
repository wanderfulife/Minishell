/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:09:51 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/14 17:14:17 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Free tokenized arguments
void free_tokens(char **tokens) {
    if (tokens) {
        for (int i = 0; tokens[i] != NULL; i++) {
            free(tokens[i]);  // Free each token
        }
        free(tokens);  // Free the token array itself
    }
}

// Simple tokenizer function to split input into arguments (space-separated)
char **tokenize(char *line) {
    char **tokens;
    char *token;
    int i = 0;

    tokens = malloc(sizeof(char *) * 64);  // Allocate space for 64 tokens (can adjust)
    if (!tokens) {
        perror("malloc failed");
        exit(1);
    }

    token = strtok(line, " \t\r\n");  // Tokenize based on spaces and newlines
    while (token != NULL) {
        tokens[i] = strdup(token);  // Duplicate the token
        i++;
        token = strtok(NULL, " \t\r\n");  // Get the next token
    }
    tokens[i] = NULL;  // NULL-terminate the array

    return tokens;
}

