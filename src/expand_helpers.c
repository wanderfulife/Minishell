/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_hepers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:09:30 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/17 18:49:32 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

// Helper function to extract and expand a variable name
char *expand_variable(const char *str) {
    char var_name[256];
    int i = 0;

    // Get the variable name after '$'
    while (str[i] && (isalnum(str[i]) || str[i] == '_')) {
        var_name[i] = str[i];
        i++;
    }
    var_name[i] = '\0';

    // Get the value from the environment
    return getenv(var_name);
}

// Handles $VAR expansion and copies to buffer
int handle_var_expansion(char *buffer, const char *line, int *i, int *buf_idx) {
    char *var_value = expand_variable(line + *i + 1);
    if (var_value) {
        strcpy(buffer + *buf_idx, var_value);
        *buf_idx += strlen(var_value);
    }
    while (isalnum(line[*i + 1]) || line[*i + 1] == '_') {
        (*i)++;  // Skip over the variable name
    }
    return *i;
}
