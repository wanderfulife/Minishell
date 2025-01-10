/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wander <wander@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 10:00:00 by JoWander          #+#    #+#             */
/*   Updated: 2025/01/10 13:34:15 by wander           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "env.h"
# include "parser.h"
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_shell	t_shell;

/* executor_core.c */
int						executor_run_command(t_command *cmd, t_shell *shell);
void					executor_cleanup(t_command *cmd);
int						executor_handle_builtin(t_command *cmd, t_shell *shell);
int						executor_is_builtin(char *cmd);

/* executor_pipeline.c */
int						executor_pipeline(t_command *cmd, t_shell *shell);

/* executor_pipeline_utils.c */
void					executor_handle_child_pipes(int prev_pipe, int pipes[2],
							t_command *cmd);
void					executor_parent_process(int *prev_pipe, int pipes[2],
							t_command *cmd);
void					wait_for_last_process(pid_t last_pid, t_shell *shell);

/* executor_child.c */
void					executor_child_process(t_command *cmd, t_shell *shell);
void					executor_exit_status(int status, t_shell *shell);

/* executor_single.c */
int						executor_single_command(t_command *cmd, t_shell *shell);

/* executor_utils.c */
char					*executor_find_command(char *cmd, char **envp);
int						executor_count_commands(t_command *cmd);
void					executor_close_pipes(t_command *cmd);
void					executor_reset_fds(int saved_stdin, int saved_stdout);

/* executor_path.c */
char					*join_path(char *dir, char *cmd);
char					*check_paths(char **paths, char *cmd);
void					free_paths(char **paths);

/* pipes.c */
int						executor_setup_pipes(t_command *cmd);
void					executor_connect_pipes(t_command *cmd, int is_last);
void					executor_close_pipes(t_command *cmd);
int						executor_init_pipe(t_command *cmd);

/* redirects.c */
int						executor_setup_redirects(t_redirect *redirs);
void					executor_close_redirects(t_redirect *redirs);
int						executor_handle_heredoc(t_command *cmd);
int						executor_open_file(char *file, int type);

int						handle_file_not_found(char *file);
int						handle_open_error(char *file);

/* process.c */
pid_t					executor_fork_process(t_command *cmd, t_shell *shell);
int						executor_wait_all(int last_pid);

/* Error handling functions */
void					handle_command_not_found(char *cmd, t_shell *shell,
							t_command *command);
void					handle_execution_error(char *cmd, char *path,
							t_shell *shell, t_command *command);

#endif