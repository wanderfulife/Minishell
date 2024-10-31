/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2024/10/31 15:39:19 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

/* echo.c */
int		builtin_echo(char **args);

/* cd.c */
int		builtin_cd(char **args, t_shell *shell);
char	*get_home_dir(t_shell *shell);

/* pwd.c */
int		builtin_pwd(void);

/* export.c */
int		builtin_export(char **args, t_shell *shell);
int		is_valid_identifier(char *str);
char	**dup_env_array(char **env);
void	free_array(char **arr);

/* unset.c */
int		builtin_unset(char **args, t_shell *shell);

/* env.c */
int		builtin_env(t_shell *shell);

/* exit.c */
int		builtin_exit(char **args, t_shell *shell);
int		is_numeric_arg(char *str);

void	print_env_vars(char **env);
void	print_sorted_env(char **env);
void	sort_env_array(char **env);
void	swap_env_vars(char **a, char **b);

#endif