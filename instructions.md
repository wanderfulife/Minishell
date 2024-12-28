Okay, let's craft a comprehensive instruction.md file based on the outline from the image you provided, and by also incorporating the missing elements we identified as necessary for a complete instruction document. This will be tailored for the Minishell project.

# Minishell Project Instructions

This document provides a complete guide for developing the Minishell project. It outlines the project's objectives, core functionalities, required resources, file structure, and important guidelines.

## Project Overview

The Minishell project involves creating a simplified command-line interpreter (shell) in C. The goal is to understand and implement core shell concepts like command parsing, execution, redirection, and piping, along with basic signal handling and built-in command management. This project aims to provide a hands-on experience of system programming. The target audience are students interested in understanding core operating system concepts.

## Core Functionalities

The Minishell must implement the following core functionalities:

*   **Prompt Display:**
    *   Display a prompt to receive user commands.
    *   *Purpose:* To clearly indicate when the shell is waiting for input.
*   **Command History:**
    *   Store and retrieve previously entered commands using `readline` functionalities.
    *   *Purpose:* To allow users to easily access and re-execute previous commands.
*   **Command Execution:**
    *   Search for executable files in the `PATH` variable, relative, or absolute paths.
    *   Execute the identified program with appropriate arguments in a child process.
    *  The parent process must wait for the child process to terminate using the `wait`, `waitpid` or similar functions.
    *   *Purpose:* To allow users to run system commands through the shell.
*  **Signal Handling:**
    *  Implement handling of `Ctrl+C`, `Ctrl+D`, and `Ctrl+\` to mimic Bash behavior.
    *  *Purpose:* To allow for proper control and behavior in the terminal.
    *   Only one global variable can be used for indicating a received signal, and it cannot be used to access data or be a norm type struct in the global scope.
    *   *Purpose:* To adhere to coding standards and control the access to global data.
*   **Input Handling:**
    *   Handle single quotes (`'`) to prevent interpretation of metacharacters.
    *   Handle double quotes (`"`) to prevent interpretation of metacharacters except the dollar sign (`$`).
    *   Not interpret special characters such as backslash `\` and semicolon `;`.
    *    *Purpose:* To correctly manage strings and user input.
*   **Redirections:**
    *   Implement input redirection (`<`).
    *   Implement output redirection (`>`).
    *   Implement here-document (`<<`).
    *   Implement append redirection (`>>`).
    *    *Purpose:* To allow redirection of input and output of commands.
*   **Pipes:**
    *   Connect the standard output of one command to the standard input of the next command using `|`.
    *   *Purpose:* To allow chaining of commands.
*   **Variable Expansion:**
    *  Implement environment variable expansion (`$VARIABLE`).
    *   Implement exit status expansion (`$?`).
    *  *Purpose:* To allow users to use environment and get the exit status of the commands.
*   **Built-in Commands:**
    *   Implement `echo -n`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit` commands.
    *   *Purpose:* To provide essential shell features.

## Documentation of Required Packages/Functions

This project primarily relies on the C standard library and the `readline` library. Here's a brief overview of some functions you should use:

*   **`readline` & `rl_*` Functions:**
    *   Used for reading user input and managing the command history.
    *   Documentation can be found in the `readline` library documentation.
*   **Process Management:**
    *   `fork()`: Creates a new child process.
    *   `execve()`: Executes a program in a child process.
    *   `wait()`/ `waitpid()`, `wait3()`, `wait4()`: Used by parent process to wait for child processes to terminate.
    *    Documentation can be found in the standard libraries documentation.
*   **File Handling:**
    *   `open()`, `close()`, `read()`, `write()`: Used to manage file descriptors.
    *   Documentation can be found in the standard libraries documentation.
*  **Signal handling:**
    * `signal()` or `sigaction()` to handle signals sent to the program, such as `ctrl+C`, `ctrl+D` or `ctrl+\`.
    * Documentation can be found in the standard libraries documentation.
*   **Error Handling:**
    *   `strerror()`, `perror()`: Used to handle errors.
    *   Documentation can be found in the standard libraries documentation.
*   **Environment Variable functions**
    *   `getenv()`: Used for getting environment variables.
    * Documentation can be found in the standard libraries documentation.

## Current File Structure

The project file structure should look like this:
content_copy
download
Use code with caution.
Markdown

minishell/
├── src/
│ ├── main.c
│ ├── <other_c_files> # (e.g., parsing.c, execution.c, builtins.c, etc.)
├── include/
│ ├── <header_files> # (e.g., minishell.h, parsing.h, etc)
├── libft/ # If you are using libft
│ ├── <libft_files>
│ └── Makefile
├── Makefile

You can modify or create other folders if needed, provided that the project follows the main rules (such as all `*.c` and `*.h` files being present and well organized).

## Project Setup Instructions

1.  **Create the Project Structure:** Create the `minishell`, `src`, `include` and `libft` (if you are using it) directories as indicated above.
2.  **Write the Source Code:** Place your C source code in `src` folder.
3.  **Write the Header files:**  Place your C header files in the `include` folder.
4.  **Write the Makefile:** Create the `Makefile` with the rules `$(NAME), all, clean, fclean, re`, and with the needed flags `-Wall`, `-Wextra` and `-Werror` and make sure that the makefile does not relink.
5.  **Compile:** Use the `make` command to compile your project.
6.  **Run:** Execute the `minishell` binary.

## Constraints

*   **Language:** The project must be implemented in the C programming language.
*   **Coding Style:** Adhere to the `Norm` coding style guidelines.
Each files can contains a maximum of 5 functions. Each function can be maximun 25 lines.
*   **Memory Management:** No memory leaks are permitted. Ensure you free all allocated memory correctly.
*    **Error Handling:** The project must not crash due to memory leaks, invalid input or other common error conditions.
*    **Forbidden functions:** You should not use any functions that is not in the authorized functions list.
*   **Subject Requirements:** Only implement features that are explicitly described in the subject description.
*   **`readline()` Leaks:** The `readline()` function may cause memory leaks that you do not need to fix.
* **Global Variables:** Only one global variable can be used for indicating a received signal, and it cannot be used to access data or be a norm type struct in the global scope.
*    **Evaluation Criteria:** The project will be evaluated based on adherence to coding standards (`Norm`), correct implementation of core functionalities, proper error handling, absence of memory leaks, and correct implementation of the required functions. Any norm error or unexpected behavior will result in a grade of `0`.
*   **Bash Reference:** When in doubt about a requirement's behavior, consult the behavior of the `bash` shell as a reference.

## Testing Instructions
*   **Unit Testing:** Implement unit tests for each specific function.
*   **End-to-End Testing:** Implement tests that cover the full flow of the application, from user input to command execution. Test different redirection cases, pipes, and built-in commands.

## Evaluation
* The project will be graded according to the correct implementation of each feature present in this document.
* If a norm error is present in the code it will result in a 0.
* If the code presents unexpected behavior it will result in a 0.
* If the project does not adhere to the constraints it will result in a 0.
* The project will be graded after a peer evaluation has been made.
* The evaluation will stop if a critical error is found.
