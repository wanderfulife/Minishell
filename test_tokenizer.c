#include "./includes/minishell.h"
#include <fcntl.h>

void	print_tokens(t_token *tokens, int fd)
{
	int	i;
		char type[2];

	i = 0;
	if (!tokens)
	{
		ft_putstr_fd("No tokens\n", fd);
		return ;
	}
	if (!tokens[0].value)
	{
		ft_putstr_fd("Empty input (no tokens)\n", fd);
		ft_putstr_fd("-------------------\n", fd);
		return ;
	}
	while (tokens[i].value)
	{
		ft_putstr_fd("Token: '", fd);
		ft_putstr_fd(tokens[i].value, fd);
		ft_putstr_fd("', Type: ", fd);
		type[0] = tokens[i].type + '0';
		type[1] = '\0';
		ft_putstr_fd(type, fd);
		ft_putstr_fd("\n", fd);
		i++;
	}
	ft_putstr_fd("-------------------\n", fd);
}

void	test_tokenizer(char *type, char *input, int fd)
{
	t_token	*tokens;

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

int	main(void)
{
	int fd = open("tokenizer_test_output.txt", O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (fd == -1)
	{
		printf("Error opening file\n");
		return (1);
	}

	/* Basic cases */
	test_tokenizer("Empty Input", "", fd);
	test_tokenizer("Multiple Spaces", "   ", fd);
	test_tokenizer("Tab Characters", "\t\t\t", fd);
	test_tokenizer("Mixed Whitespace", " \t \t ", fd);

	/* Quotes with operators */
	test_tokenizer("Quote With Operator 1", "echo 'hello>'world", fd);
	test_tokenizer("Quote With Operator 2", "echo 'hello|'world", fd);
	test_tokenizer("Quote With Operator 3", "echo 'hello<'world", fd);
	test_tokenizer("Quote Inside Operator", "echo >>'>>'file", fd);
	test_tokenizer("Quote Around Operator", "echo '>>'file", fd);

	/* Multiple operators */
	test_tokenizer("Multiple Heredoc", "cat << EOF << END << STOP", fd);
	test_tokenizer("Mixed Redirection", "cat < file1 >> file2 < file3", fd);
	test_tokenizer("Consecutive Different", "echo ><|>><<", fd);

	/* Quote combinations */
	test_tokenizer("Triple Single", "'''", fd);
	test_tokenizer("Triple Double", "\"\"\"", fd);
	test_tokenizer("Alternating Empty", "'\"'\"'\"", fd);
	test_tokenizer("Mixed Content", "a'b\"c'd\"e'f", fd);
	test_tokenizer("Quoted Space Start", "echo ' 'hello", fd);
	test_tokenizer("Quoted Space End", "echo hello' '", fd);
	test_tokenizer("Multiple Quoted Spaces", "echo' 'hello' 'world", fd);

	/* Edge cases with redirections */
	test_tokenizer("Empty Redirection", ">", fd);
	test_tokenizer("Multiple Empty", ">>>", fd);
	test_tokenizer("Space Between", "> > >", fd);
	test_tokenizer("Mixed With Words", "echo>text>>file>output", fd);

	/* Special characters in quotes */
	test_tokenizer("Quotes with $", "echo '$USER' \"$USER\"", fd);
	test_tokenizer("Quotes with spaces", "echo '   ' \"   \"", fd);
	test_tokenizer("Quote with tabs", "echo '\t' \"\t\"", fd);
	test_tokenizer("Quotes with operators", "echo '<|>' \"<|>\"", fd);

	/* Command-like inputs */
	test_tokenizer("Complex Pipeline", "cat file.txt | grep 'pattern' | sort-r > output.txt", fd);
	test_tokenizer("Multiple Heredoc","cat << EOF << END\nsome text\nEOF\nmore text\nEND", fd);
	test_tokenizer("Mixed Redirections","cmd < in1 < in2 >> out1 > out2 << EOF", fd);
	test_tokenizer("Quoted Command", "'echo' 'hello' '|' 'grep' 'pattern'", fd);

	/* Spaces and operators */
	test_tokenizer("Spaces Around Op", "echo    >>    file", fd);
	test_tokenizer("No Spaces Op", "echo>>file>>output", fd);
	test_tokenizer("Mixed Spaces Op", "echo >file>> output   >>>   final", fd);

	/* Error cases */
	test_tokenizer("Multiple Pipes", "echo |||", fd);
	test_tokenizer("Multiple Redirections", "echo >>>", fd);
	test_tokenizer("Mixed Invalid", "echo |>|<|", fd);
	test_tokenizer("Invalid Sequence", ">>>>><<<<<", fd);

	/* Newline cases */
	test_tokenizer("Simple Newline", "echo hello\nworld", fd);
	test_tokenizer("Multiple Newlines", "echo\n\n\nhi", fd);
	test_tokenizer("Newline in Quotes", "echo \"hello\nworld\"", fd);
	test_tokenizer("Newline in Single Quotes", "echo 'hello\nworld'", fd);

	/* Quote edge cases */
	test_tokenizer("Quoted Special", "echo '~!@#$%^&*()'", fd);
	test_tokenizer("Mixed Quote Types", "echo \"'\" '\"'", fd);
	test_tokenizer("All Operators Quoted", "'|' '<' '>' '>>' '<<'", fd);

	/* Space edge cases */
	test_tokenizer("Space Only Quoted", "echo ' '", fd);
	test_tokenizer("Multiple Space Quoted", "echo '   '", fd);
	test_tokenizer("Mixed Spaces", "echo    '  '    \"  \"     ", fd);

	/* Mixed edge cases */
	test_tokenizer("Everything Mixed", "'echo'\"hello\"'>'\"file\"'|'\"cat\"",fd);
	test_tokenizer("Quote Between Words", "hello'  'world", fd);
	test_tokenizer("Quote Between Operators", ">'>>'<", fd);
	test_tokenizer("Quote End Start", "'>><'", fd);

	/* Heredoc variations */
	test_tokenizer("Heredoc Quoted", "cat << 'EOF'", fd);
	test_tokenizer("Heredoc Double Quoted", "cat << \"EOF\"", fd);
	test_tokenizer("Multiple Heredoc Mixed", "cat << EOF << 'END' << \"STOP\"",fd);
	close(fd);
	return (0);
}