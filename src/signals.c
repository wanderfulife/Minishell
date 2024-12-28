void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	close(0);
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
} 