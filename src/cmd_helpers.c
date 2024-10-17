/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:18:02 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/17 18:27:39 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Adds an argument to the command's argv
void add_argument_to_cmd(t_cmd *cmd, char *arg, int *argc) {
    cmd->argv = realloc(cmd->argv, sizeof(char *) * (*argc + 2));
    cmd->argv[*argc] = strdup(arg);
    cmd->argv[*argc + 1] = NULL;
    (*argc)++;
}

