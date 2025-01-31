

NAME		=	minishell
LIBFT		=	libft/
LIBFT_A		=	$(addprefix $(LIBFT), libft.a)

CC			=	clang
INCLUDE 	=	./includes/
INCLUDE_LIBFT = $(addprefix $(LIBFT), includes/)

#CFLAGS		=	-Wall -Werror -Wextra -g -I$(INCLUDE_LIBFT) -I$(INCLUDE)
CFLAGS		=	-g -I$(INCLUDE_LIBFT) -I$(INCLUDE)
RM			=	rm -f
OBJ_DIR		=	objs
SRCS		=	srcs/main.c \
				srcs/utils.c \
				srcs/signals.c \
				srcs/exec/exec.c \
				srcs/exec/clear_env.c \
				srcs/exec/complex/cmd_flags.c \
				srcs/exec/complex/pipes.c \
				srcs/exec/complex/exec_pipe.c \
				srcs/exec/complex/fork.c \
				srcs/exec/complex/open_files.c \
				srcs/exec/complex/exec_utils.c \
				srcs/parsing/expansion.c \
				srcs/parsing/split_quote.c \
				srcs/parsing/split_quote_2.c \
				srcs/parsing/meta.c \
				srcs/parsing/environement.c \
				srcs/command/ft_cd.c \
				srcs/command/ft_echo.c \
				srcs/command/ft_pwd.c \
				srcs/command/ft_env.c \
				srcs/command/ft_unset.c \
				srcs/command/ft_export.c \
				srcs/command/ft_exit.c \
				srcs/cmd_path.c \

OBJS		=	$(SRCS:/%.c=%.o)

$(NAME):		$(OBJS) $(LIBFT_A)
				@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT) -lft  -L/opt/homebrew/opt/readline/lib -lreadline -I/opt/homebrew/opt/readline/include -o $(NAME);
				@echo "Linked into executable \033[0;32m$(NAME)\033[0m."

$(LIBFT_A):
				@$(MAKE) -s -C $(LIBFT)
				@echo "Compiled $(LIBFT_A)."

all:			$(NAME)


bonus:			all

.c.o:
				@$(CC) $(CFLAGS) -c $< -o $(OBJ_DIR)/$(<:.c=.o)
				@echo "Compiling $<."

localclean:
				@$(RM) -rf $(OBJ_DIR)
				@mkdir $(OBJ_DIR)
				@echo "Removed object files."

clean:			localclean
				@$(MAKE) clean -s -C $(LIBFT)
				@echo "Clean libft."

fclean:			localclean
				@$(MAKE) fclean -s -C $(LIBFT)
				@echo "Full clean libft."
				@$(RM) $(NAME)
				@echo "Removed executable."

test:			$(NAME)
				./$(NAME)
	
test_leak:			$(NAME)
				valgrind --leak-check=full ./$(NAME)

re:				fclean all

.PHONY:			all clean fclean re localclean bonus 