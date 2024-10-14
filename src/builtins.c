/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:09:30 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/14 17:13:54 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void ft_cd(char *path) {
    char *home;

    if (path == NULL || strcmp(path, "") == 0) {
        home = getenv("HOME");
        if (home == NULL) {
            perror("cd: HOME not set");
            return;
        }
        path = home;
    }

    if (chdir(path) == -1) {
        perror("cd");
    }
}

void ft_pwd(void) {
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
}

void ft_exit(char *status, char **tokens, char *line) {
    int exit_status = 0;

    if (status != NULL) {
        exit_status = atoi(status);
    }

    free_tokens(tokens);  // Free tokens before exiting
    free(line);           // Free the user input line before exiting

    // Add any other cleanup if necessary

    exit(exit_status);  // Exit with the given status code
}

