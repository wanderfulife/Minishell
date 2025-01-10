/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wander <wander@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2025/01/10 13:34:11 by wander           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "../libft/includes/libft.h"

typedef struct s_shell	t_shell;

/* env.c */
char					**env_init(char **original_env);
void					env_destroy(char **env);
int						env_set(t_shell *shell, char *var, char *value);

/* env_utils.c */
char					*env_get_name(char *var);
char					*env_create_entry(char *name, char *value);
int						env_find_index(char *name, char **env);
char					**env_add_entry(char **env, char *entry);
char					**env_remove_entry(char **env, int index);

/* env_utils2.c */
char					*get_value_from_var(char *var, char *value);
char					**free_env_array(char **env, int size);
int						env_unset(t_shell *shell, char *name);
char					*env_get_value(char *name, char **env);

/* env_expand.c */
char					*env_expand_vars(char *str, t_shell *shell);
int						env_var_length(char *str);
char					*env_get_var_name(char *str);

/* env_expand_utils.c */
char					*env_replace_var(char *str, int start, int len,
							char *value);

/* env_expand_utils2.c */
char					*expand_variable(char *expanded, int i, t_shell *shell);
char					*get_var_value(char *var_name, t_shell *shell);
int						handle_single_quotes(char c, int in_single_quote);
char					*handle_expansion_result(char *exp, char *new_str);
char					*init_expansion(char *str);

#endif