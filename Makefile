NAME		=	minishell
LIBFT		=	libft/
LIBFT_A		=	$(addprefix $(LIBFT), libft.a)

CC			=	clang
INCLUDE 	=	./includes/
INCLUDE_LIBFT = $(addprefix $(LIBFT), includes/)

CFLAGS		=	-Wall -Werror -Wextra -g -I$(INCLUDE_LIBFT) -I$(INCLUDE)
RM			=	rm -f
OBJ_DIR		=	objs
SRCS		=	srcs/main.c \
				srcs/utils.c \
				srcs/split_quote.c \
				srcs/command/*.c

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
				./fdf test_maps/42.fdf 
	
test_leak:			$(NAME)
				valgrind --leak-check=full ./fdf test_maps/42.fdf 

re:				fclean all

.PHONY:			all clean fclean re localclean bonus 