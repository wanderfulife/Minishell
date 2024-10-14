/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:16:57 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/14 17:16:27 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>  // Add this header for waitpid


// Builtin functions
void    ft_cd(char *path);
void    ft_pwd(void);
void    ft_exit(char *status, char **tokens, char *line);

// Utilities
void    free_tokens(char **tokens);
char    **tokenize(char *line);
void    execute_command(char **tokens);

#endif