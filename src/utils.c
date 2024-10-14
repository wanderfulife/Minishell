/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:09:51 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/14 17:46:42 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_tokens(char **tokens) {
    int i = 0;
    if (!tokens) return;
    while (tokens[i]) {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}

char **tokenize(char *line) {
    char **tokens;
    int token_count = 0;
    int token_size = INITIAL_TOKEN_SIZE;
    int i = 0, j = 0, token_length;
    
    // Allocate space for initial tokens
    tokens = malloc(sizeof(char *) * token_size);
    if (!tokens) {
        perror("malloc failed");
        exit(1);
    }

    while (line[i] != '\0') {
        // Skip leading delimiters (spaces, tabs, newlines)
        while (line[i] == ' ' || line[i] == '\t' || line[i] == '\n') {
            i++;
        }

        // Check if we have reached the end of the input string
        if (line[i] == '\0') break;

        // Find the length of the current token
        token_length = 0;
        while (line[i + token_length] != ' ' && line[i + token_length] != '\t' &&
               line[i + token_length] != '\n' && line[i + token_length] != '\0') {
            token_length++;
        }

        // Allocate space for the current token
        tokens[token_count] = malloc(token_length + 1);
        if (!tokens[token_count]) {
            perror("malloc failed");
            exit(1);
        }

        // Copy the token``
        for (j = 0; j < token_length; j++) {
            tokens[token_count][j] = line[i + j];
        }
        tokens[token_count][j] = '\0';  // Null-terminate the token

        // Move index to the next part of the input string
        i += token_length;

        // Increment the token count
        token_count++;

        // Check if we need to resize the token array
        if (token_count >= token_size) {
            token_size += TOKEN_INCREMENT;
            tokens = realloc(tokens, sizeof(char *) * token_size);
            if (!tokens) {
                perror("realloc failed");
                exit(1);
            }
        }
    }

    // Null-terminate the array of tokens
    tokens[token_count] = NULL;

    return tokens;
}

