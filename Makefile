GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
BLUE = \033[0;34m
RESET = \033[0m

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

.PHONY: all clean fclean re

# Source files
SRCS = src/main.c src/shell.c src/terminal.c \
	   src/builtins/cd.c \
	   src/builtins/echo.c \
	   src/builtins/env.c \
	   src/builtins/exit.c \
	   src/builtins/export.c \
	   src/builtins/export_utils.c \
	   src/builtins/pwd.c \
	   src/builtins/unset.c \
	   src/env/env.c \
	   src/env/env_expand.c \
	   src/env/env_expand_utils.c \
	   src/env/env_expand_utils2.c \
	   src/env/env_utils.c \
	   src/executor/executor.c \
	   src/executor/executor_utils.c \
	   src/executor/executor_path.c \
	   src/executor/executor_builtins.c \
	   src/executor/pipes.c \
	   src/executor/process.c \
	   src/executor/redirects.c \
	   src/executor/redirects_utils.c \
	   src/lexer/handle.c \
	   src/lexer/lexer.c \
	   src/lexer/lexer_utils.c \
	   src/lexer/token.c \
	   src/parser/heredoc.c \
	   src/parser/parser.c \
	   src/parser/parser_utils.c \
	   src/parser/parser_cleanup.c \
	   src/parser/redirections.c \
	   src/signals/signals.c

OBJS_DIR = objs
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libftprintf.a

# Include directories for libft and readline
CFLAGS += -I$(LIBFT_DIR)/includes -Iincludes -I/usr/include/readline

# Linker flags for libft and readline
LDFLAGS += -L$(LIBFT_DIR) -lftprintf -L/usr/lib -lreadline

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "$(BLUE)$(NAME) created!$(RESET)"

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@printf "$(YELLOW)Compiling $<... $(RESET)"
	@if $(CC) $(CFLAGS) -MMD -MP -c $< -o $@; then \
		printf "$(GREEN)Done!$(RESET)\n"; \
	else \
		printf "$(RED)Failed!$(RESET)\n"; \
		exit 1; \
	fi

$(LIBFT):
	@echo "$(YELLOW)Compiling libft...$(RESET)"
	make -C $(LIBFT_DIR)
	@echo "$(GREEN)libft compilation done!$(RESET)"

clean:
	@echo "$(YELLOW)Cleaning up...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR) clean
	@if [ -d "$(OBJS_DIR)" ]; then \
		rm -rf $(OBJS_DIR); \
		echo "$(GREEN)Object files cleaned!$(RESET)"; \
	fi

fclean: clean
	@echo "$(YELLOW)Full cleanup...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@if [ -f "$(NAME)" ]; then \
		rm -f $(NAME); \
		echo "$(GREEN)Executable removed!$(RESET)"; \
	fi

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)