#include "./includes/minishell.h"
#include <fcntl.h>

void print_tokens(t_token *tokens, int fd)
{
    int i = 0;

    if (!tokens)
    {
        ft_putstr_fd("No tokens\n", fd);
        return;
    }
    if (!tokens[0].value)
    {
        ft_putstr_fd("Empty input (no tokens)\n", fd);
        ft_putstr_fd("-------------------\n", fd);
        return;
    }
    while (tokens[i].value)
    {
        ft_putstr_fd("Token: '", fd);
        ft_putstr_fd(tokens[i].value, fd);
        ft_putstr_fd("', Type: ", fd);
        char type[2];
        type[0] = tokens[i].type + '0';
        type[1] = '\0';
        ft_putstr_fd(type, fd);
        ft_putstr_fd("\n", fd);
        i++;
    }
    ft_putstr_fd("-------------------\n", fd);
}

void test_tokenizer(char *type, char *input, int fd)
{
    t_token *tokens;

    ft_putstr_fd("\n--- ", fd);
    ft_putstr_fd(type, fd);
    ft_putstr_fd(" ---\n", fd);
    ft_putstr_fd("Input: '", fd);
    ft_putstr_fd(input, fd);
    ft_putstr_fd("'\n", fd);
    tokens = tokenize_input(input);
    print_tokens(tokens, fd);
    free_tokens(tokens);
}

int main()
{
    int fd = open("tokenizer_test_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        printf("Error opening file\n");
        return (1);
    }

    /* Basic tests */
    test_tokenizer("Basic - Empty", "", fd);
    test_tokenizer("Basic - Spaces", "   ", fd);
    test_tokenizer("Basic - Simple Command", "echo hello world", fd);
    test_tokenizer("Basic - Multiple Words", "ls -la /home/user", fd);
    
    /* Quote tests */
    test_tokenizer("Quotes - Single Empty", "''", fd);
    test_tokenizer("Quotes - Double Empty", "\"\"", fd);
    test_tokenizer("Quotes - Single Word", "'hello'", fd);
    test_tokenizer("Quotes - Double Word", "\"hello\"", fd);
    test_tokenizer("Quotes - Mixed Empty", "'\"'", fd);
    test_tokenizer("Quotes - Multiple", "echo '''''''", fd);
    test_tokenizer("Quotes - Multiple 2", "echo \"\"\"\"\"\"\"", fd);
    test_tokenizer("Quotes - Alternating", "echo ''\"\"''\"\"", fd);
    test_tokenizer("Quotes - Complex 1", "echo \"'\"'\"'\"'\"", fd);
    test_tokenizer("Quotes - Complex 2", "echo '\"'\"'\"'\"'", fd);

    /* Operator tests */
    test_tokenizer("Operators - Basic Pipe", "ls | grep a", fd);
    test_tokenizer("Operators - Basic Input", "cat < file", fd);
    test_tokenizer("Operators - Basic Output", "echo hello > file", fd);
    test_tokenizer("Operators - Append", "echo hello >> file", fd);
    test_tokenizer("Operators - Heredoc", "cat << EOF", fd);
    test_tokenizer("Operators - Multiple >", "echo >>>>>>>", fd);
    test_tokenizer("Operators - Multiple <", "echo <<<<<<<", fd);
    test_tokenizer("Operators - Multiple |", "echo |||||||", fd);
    test_tokenizer("Operators - Mixed >|", "echo >|>|>|>", fd);
    test_tokenizer("Operators - Mixed <|", "echo <|<|<|<", fd);

    /* Edge cases */
    test_tokenizer("Edge - No Space Quotes", "echo\"hello\"", fd);
    test_tokenizer("Edge - No Space Single", "echo'hello'", fd);
    test_tokenizer("Edge - No Space Op", "echo>file", fd);
    test_tokenizer("Edge - Op Quote", "echo>\"file\"", fd);
    test_tokenizer("Edge - Multi Space", "echo     >     file", fd);
    test_tokenizer("Edge - Empty Command", ">file", fd);
    test_tokenizer("Edge - Only Spaces", "     ", fd);

    /* Complex combinations */
    test_tokenizer("Complex - Pipeline", "ls -l | grep .txt | sort -r | uniq", fd);
    test_tokenizer("Complex - Redirections", "cat < in > out >> append 2> err", fd);
    test_tokenizer("Complex - Mixed Quotes", "echo \"'hello'\" 'world \"!' \"how\" 'are' \"you\"'?'", fd);
    test_tokenizer("Complex - Nested Commands", "echo $(echo \"hello\")", fd);
    test_tokenizer("Complex - Env Vars", "echo \"$USER\"'$HOME'\"$PATH\"", fd);

    /* Previous test cases */
    test_tokenizer("Previous - Mixed 1", "echo 'hello'\"world\"|grep", fd);
    test_tokenizer("Previous - Mixed 2", "echo \"hello\"'world'>file", fd);
    test_tokenizer("Previous - Mixed 3", "echo 'hello'\"world\">>file", fd);
    test_tokenizer("Previous - Heredoc Var", "cat<<'EOF'", fd);
    test_tokenizer("Previous - Complex 1", "echo test>'file'|cat -e>'output'", fd);
    test_tokenizer("Previous - Complex 2", "<<EOF1 cat | grep 'pattern\"here' | >output", fd);

    /* Error cases */
    test_tokenizer("Error - Unclosed Single", "echo 'hello", fd);
    test_tokenizer("Error - Unclosed Double", "echo \"hello", fd);
    test_tokenizer("Error - Mixed Unclosed", "echo \"'hello", fd);
    test_tokenizer("Error - Pipe Only", "|", fd);
    test_tokenizer("Error - Redir Only", ">", fd);

    close(fd);
    return (0);
}