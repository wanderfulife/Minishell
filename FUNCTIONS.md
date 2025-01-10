# Minishell Functions Documentation

This document provides a comprehensive overview of all functions in the Minishell project.

## Table of Contents
1. [Main Functions](#main-functions)
2. [Shell Core](#shell-core)
3. [Terminal Handling](#terminal-handling)
4. [Signal Handling](#signal-handling)
5. [Environment Variables](#environment-variables)
6. [Parser](#parser)
7. [Lexer](#lexer)
8. [Executor](#executor)
9. [Builtins](#builtins)

## Main Functions

Let's examine the core functions that initialize and run the shell.

### main.c

#### `void shell_init(t_shell *shell, char **env)`
Initializes the shell structure with:
- Environment variables
- Exit status
- Heredoc status
- Terminal settings
- Signal handlers

#### `void shell_cleanup(t_shell *shell)`
Performs cleanup operations when the shell exits:
- Frees current command
- Destroys environment variables
- Restores terminal settings
- Closes heredoc
- Cleans up readline history and pending input

#### `int main(int argc, char **argv, char **env)`
The entry point of the shell:
- Initializes the shell structure
- Runs the main shell loop
- Performs cleanup before exit

## Shell Core

### shell.c

#### `static void shell_free_command(t_shell *shell)`
Internal function to free the current command structure.

#### `int shell_process_input(t_shell *shell, char *input)`
Processes user input through several stages:
1. Expands environment variables
2. Tokenizes the input
3. Parses tokens into commands
4. Executes the command
5. Cleans up

#### `static void shell_cleanup_and_break(t_shell *shell, char *input)`
Internal function to handle cleanup when breaking from the shell loop.

#### `int shell_loop(t_shell *shell)`
The main loop of the shell that:
- Displays prompt and reads input
- Processes the input
- Maintains command history
- Handles shell exit

#### `void shell_reset_signals(void)`
Resets signal handlers to their default shell behavior:
- SIGINT (Ctrl+C)
- SIGQUIT (Ctrl+\\)
- SIGTERM

## Terminal Handling

### terminal.c

#### `void terminal_init(t_shell *shell)`
Initializes the terminal settings by saving the original terminal attributes.

#### `void terminal_restore(t_shell *shell)`
Restores the terminal to its original settings using saved attributes.

#### `void terminal_disable_echoctl(t_shell *shell)`
Disables the echoing of control characters (like ^C) in the terminal.

## Signal Handling

### signals.c

#### `void handle_sigquit(int sig)`
Handles the SIGQUIT signal (Ctrl+\\):
- Maintains the current line
- Redisplays the prompt

#### `void restore_prompt(int sig)`
Restores the prompt after a signal interruption:
- Prints a newline
- Prepares for new input

#### `void setup_signals(void)`
Sets up the signal handlers for the shell:
- Configures SIGINT (Ctrl+C) handler
- Configures SIGQUIT (Ctrl+\\) handler

#### `void handle_sigint(int sig)`
Handles the SIGINT signal (Ctrl+C):
- Interrupts current operation
- Displays a new prompt

## Environment Variables

### env.c

#### `char **env_init(char **original_env)`
Initializes the environment by:
- Creating a copy of the original environment
- Allocating new memory for the environment array
- Duplicating each environment variable string

#### `void env_destroy(char **env)`
Cleans up the environment:
- Frees each environment variable string
- Frees the environment array

#### `static int update_env_entry(char **env, int index, char *name, char *value)`
Updates an existing environment variable:
- Creates a new entry with the updated value
- Replaces the old entry at the specified index

#### `static int add_new_env_entry(t_shell *shell, char *name, char *value)`
Adds a new environment variable:
- Creates a new entry
- Extends the environment array
- Adds the new entry

#### `int env_set(t_shell *shell, char *var, char *value)`
Sets or updates an environment variable:
- Extracts variable name and value
- Updates existing variable if found
- Adds new variable if not found

### env_expand.c

#### `static char *handle_exit_status_expansion(char *exp, int i, t_shell *shell)`
Handles expansion of $? variable:
- Converts exit status to string
- Replaces $? with the status value

#### `static char *process_expansion(char *exp, int *i, t_shell *shell)`
Processes environment variable expansion:
- Handles special cases ($?, etc.)
- Expands variables to their values
- Handles quotes and spaces

#### `static char *process_char(char *exp, int *i, int *in_single_quote, t_shell *shell)`
Processes each character during expansion:
- Handles single quotes
- Processes variable expansions
- Manages string state

#### `char *env_expand_vars(char *str, t_shell *shell)`
Main function for variable expansion:
- Initializes expansion process
- Processes each character
- Handles quotes and variables
- Returns expanded string 

## Parser

### parser.c

#### `t_command *parser_create_command(void)`
Creates a new command structure:
- Allocates memory for command
- Initializes command fields (args, redirects, pipes)
- Sets default pipe file descriptors

#### `static int parser_fill_command(t_command *cmd, t_token **tokens)`
Fills a command structure with data:
- Processes redirections
- Counts and creates argument array
- Advances token pointer
- Returns success status

#### `static t_command *parser_handle_pipe(t_token **tokens)`
Handles command pipeline creation:
- Creates new command
- Fills command with data
- Recursively handles piped commands
- Links commands in pipeline

#### `t_command *parser_parse_tokens(t_token *tokens)`
Main parsing function:
- Takes token list as input
- Creates command structure
- Returns parsed command tree

### parser_cleanup.c

#### `void parser_destroy_command(t_command *cmd)`
Cleans up command structure:
- Frees arguments
- Frees redirections
- Recursively frees piped commands

### parser_count.c

#### `int parser_count_args(t_token *tokens)`
Counts command arguments:
- Skips redirections
- Counts valid argument tokens
- Returns argument count

### parser_redirect_utils.c

#### `bool parser_is_redirect(t_token_type type)`
Checks if token is redirection:
- Validates token type
- Returns true for redirection tokens

#### `bool parser_add_redirect(t_command *cmd, t_token **tokens)`
Adds redirection to command:
- Creates redirection structure
- Links to command
- Advances token pointer

### redirections.c

#### `t_redirect *parser_create_redirect(t_token_type type, char *file)`
Creates redirection structure:
- Allocates memory
- Sets type and filename
- Returns redirection object

### heredoc.c

#### `void parser_handle_heredoc(t_command *cmd)`
Handles heredoc input:
- Creates temporary file
- Reads delimiter-terminated input
- Sets up input redirection 

## Lexer

### lexer.c

#### `t_token *lexer_create_token(char *content, int type)`
Creates a new token:
- Allocates memory for token structure
- Duplicates content string
- Sets token type
- Initializes next pointer

#### `int lexer_process_token(t_token **tokens, t_token *new)`
Processes and adds a new token:
- Validates token
- Adds to token list
- Returns success status

#### `t_token *cleanup_and_create_token(char *parts[1024], int part_count)`
Creates a token from word parts:
- Joins word parts
- Cleans up temporary arrays
- Creates word token
- Returns new token

#### `void lexer_destroy(t_token *list)`
Cleans up token list:
- Frees token content
- Frees token structures
- Handles linked list cleanup

#### `void lexer_add_token(t_token **list, t_token *new)`
Adds token to list:
- Handles empty list case
- Finds list end
- Links new token

### lexer_utils.c

#### `int lexer_is_operator(char c)`
Checks if character is operator:
- Validates pipes (|)
- Validates redirections (<, >)
- Returns boolean result

#### `t_token *lexer_handle_operator(char *input, int *i)`
Processes operator tokens:
- Identifies operator type
- Handles double operators (>>, <<)
- Creates appropriate token
- Updates input position

#### `char *join_word_parts(char **parts)`
Joins array of word parts:
- Handles empty parts
- Concatenates strings
- Manages memory
- Returns joined string

#### `char *get_next_word_part(char *input, int *i)`
Extracts next word part:
- Skips operators and whitespace
- Handles quotes
- Returns word part
- Updates input position

#### `char *handle_quoted_part(char *input, int *i, char quote)`
Processes quoted strings:
- Extracts quoted content
- Handles quote matching
- Updates input position
- Returns quoted content 

## Executor

### executor_core.c

#### `int executor_run_command(t_command *cmd, t_shell *shell)`
Main command execution function:
- Validates command
- Handles single commands
- Handles command pipelines
- Returns execution status

#### `void executor_cleanup(t_command *cmd)`
Cleans up after command execution:
- Closes pipe file descriptors
- Closes redirection file descriptors
- Handles memory cleanup

#### `int executor_handle_builtin(t_command *cmd, t_shell *shell)`
Executes builtin commands:
- Identifies builtin type
- Calls appropriate builtin handler
- Manages builtin arguments
- Returns builtin status

#### `int executor_is_builtin(char *cmd)`
Checks if command is builtin:
- Validates against builtin list
- Handles command comparison
- Returns boolean result

### executor_pipeline.c

#### `int executor_pipeline(t_command *cmd, t_shell *shell)`
Handles command pipeline execution:
- Creates pipes between commands
- Manages process creation
- Coordinates command execution
- Handles pipeline cleanup

### executor_single.c

#### `int executor_single_command(t_command *cmd, t_shell *shell)`
Executes single command:
- Sets up command environment
- Handles redirections
- Executes command
- Returns command status

### executor_path.c

#### `char *executor_find_command(char *cmd, char **env)`
Locates command in PATH:
- Searches PATH directories
- Validates executable
- Returns command path
- Handles errors

### pipes.c

#### `int executor_setup_pipes(t_command *cmd)`
Sets up command pipeline:
- Creates pipe file descriptors
- Links commands
- Handles pipe errors

### redirects.c

#### `int executor_setup_redirects(t_redirect *redirects)`
Handles input/output redirections:
- Opens files
- Sets up file descriptors
- Handles heredoc
- Manages errors

### executor_child.c

#### `void executor_run_child(t_command *cmd, t_shell *shell)`
Handles child process execution:
- Sets up environment
- Handles redirections
- Executes command
- Manages errors

### executor_errors.c

#### `void executor_handle_error(char *cmd, char *error)`
Handles execution errors:
- Formats error messages
- Displays to user
- Sets error status 

## Builtins

### cd.c

#### `int builtin_cd(char **args, t_shell *shell)`
Changes current directory:
- Handles path argument
- Manages HOME directory
- Updates PWD and OLDPWD
- Handles directory errors

### echo.c

#### `int builtin_echo(char **args)`
Displays text to stdout:
- Handles -n flag
- Processes arguments
- Manages whitespace
- Returns success status

### env.c

#### `int builtin_env(t_shell *shell)`
Displays environment variables:
- Lists all variables
- Formats output
- Handles errors

### exit.c

#### `int builtin_exit(char **args, t_shell *shell)`
Exits the shell:
- Handles exit status
- Validates arguments
- Performs cleanup
- Terminates shell

### export.c

#### `int builtin_export(char **args, t_shell *shell)`
Exports environment variables:
- Validates variable names
- Sets environment variables
- Handles display format
- Manages errors

### pwd.c

#### `int builtin_pwd(void)`
Prints working directory:
- Gets current path
- Handles errors
- Displays path

### unset.c

#### `int builtin_unset(char **args, t_shell *shell)`
Removes environment variables:
- Validates arguments
- Removes variables
- Handles errors 