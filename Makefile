GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
BLUE = \033[0;34m
RESET = \033[0m

NAME = Minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = src/main.c
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
	@$(MAKE) -C $(LIBFT_DIR)
	@echo "$(GREEN)libft compilation done!$(RESET)"

clean:
	@echo "$(YELLOW)Cleaning up...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -rf $(OBJS_DIR)
	@echo "$(GREEN)Clean done!$(RESET)"

fclean: clean
	@echo "$(YELLOW)Full cleanup...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)
	@echo "$(GREEN)Full cleanup done!$(RESET)"

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)