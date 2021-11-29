SRCS =	pipex.c \
		ft_split.c \
		libft.c \
		child_stuff.c \
		
 
NAME = pipex

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g # -g3-fsanitize=address

OBJ_DIR = objs
SRCS_DIR = srcs
INC_DIR = includes

OBJ = $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))

.c.o:
	${CC} ${CFLAGS} -c$< -o ${<:.c=.o}

all:
	@$(MAKE) -j $(NAME)

$(NAME): $(OBJ)
		${CC} $(CFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.c | .gitignore
		@mkdir -p $(OBJ_DIR)
		${CC} $(CFLAGS) -I $(INC_DIR) -c $< -o $@

.gitignore:
		@echo $(NAME) > .gitignore

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(NAME)

fclean:	clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re