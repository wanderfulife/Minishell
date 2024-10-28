#!/bin/bash

# Path to your minishell executable
MINISHELL_EXEC="./minishell"

# Extended set of test commands
commands=(
    # Basic commands with different options and flags
    "echo 'Testing basic echo command'"
    "ls -la"
    "ls /usr/bin | head -n 10"
    "pwd"
    "whoami"
    "date"
    "uname -a"
    "echo 'Multiline\nTest'"
    
    # Directory navigation and error handling
    "cd / && pwd"
    "cd /tmp && pwd"
    "cd /nonexistentdir"                # Should handle errors
    "cd -"
    "cd $HOME"
    
    # Variable manipulation and usage
    "export VAR1=Hello"
    "echo \$VAR1"
    "export VAR2='Multi word variable'"
    "echo \$VAR2"
    "unset VAR1"
    "echo \$VAR1"                       # Should be empty
    "export VAR3='Test with special chars $* @ !'"
    "echo \$VAR3"
    "unset VAR2 VAR3"
    
    # Redirections: output redirection, appending, and input redirection
    "echo 'Hello, File!' > testfile.txt"
    "cat < testfile.txt"
    "echo 'Appending to file' >> testfile.txt"
    "cat testfile.txt"
    "rm testfile.txt"
    
    # Pipe multiple commands
    "echo 'hello world' | tr 'a-z' 'A-Z'"
    "echo '123 456 789' | awk '{print \$2}'"
    "ls | grep minishell | sort"
    "ls | wc -l"
    "cat /etc/passwd | grep root | wc -l"
    
    # Chaining commands with &&
    "echo 'starting chain' && echo 'second command'"
    "mkdir testdir && cd testdir && touch testfile && ls && cd .. && rm -r testdir"
    "export CHAIN_VAR=42 && echo \$CHAIN_VAR && unset CHAIN_VAR && echo \$CHAIN_VAR"
    
    # Complex chained commands and nested commands
    "(echo 'nested subshell') && echo 'after subshell'"
    "(ls && pwd) | cat"
    "(echo 'inner' && echo 'loop') | grep inner"
    "(echo test | grep test) && (echo passed && echo success)"
    
    # Error handling: invalid commands, missing files, and bad arguments
    "nonexistentcommand"
    "ls nonexistentfile"                 # Non-existent file error
    "cat nonexistentfile"
    "exit not_a_number"                  # Invalid exit status
    
    # Built-ins and environment
    "env | grep PATH"
    "export NEWVAR=123"
    "env | grep NEWVAR"
    "unset NEWVAR"
    "echo \$NEWVAR"                      # Should be empty

    # Tests with multiple arguments and quotes
    "echo 'String with single quotes'"
    "echo \"String with double quotes\""
    "echo \"Nested 'quotes' within double quotes\""
    "export TEST_QUOTE=\"Value with spaces and symbols * \$ & !\""
    "echo \$TEST_QUOTE"
    "unset TEST_QUOTE"

	# Here-doc with variable expansion
    "export VAR=hello && cat << EOF
	Value of VAR is \$VAR
	EOF"

    # Here-doc inside pipes and redirects
    "cat << EOF | grep 'inner'
	This is the first line.
	This is an inner line.
	Another inner line.
	EOF"

    # Exit command variations
    "exit"
    "exit 0"
    "exit 1"
    "exit 255"                            # Edge case for maximum exit code
)

# Run each command in minishell with valgrind and check for leaks
for cmd in "${commands[@]}"; do
    echo "Testing command: $cmd"
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 \
        "$MINISHELL_EXEC" -c "$cmd" > /dev/null 2> valgrind_output.txt

    # Check if valgrind detected any leaks
    if grep -q "definitely lost:" valgrind_output.txt; then
        echo -e "Memory leak detected for command: '$cmd'\n"
        grep "definitely lost:" valgrind_output.txt
    else
        echo "No memory leaks for command: '$cmd'"
    fi
    echo "--------------------------------------"
done

# Clean up
rm -f valgrind_output.txt

