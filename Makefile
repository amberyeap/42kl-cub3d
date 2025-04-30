# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/29 12:06:13 by ayeap             #+#    #+#              #
#    Updated: 2025/04/29 20:16:42 by ayeap            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

MLX = -Iminilibx-linux
MLX_FLAGS = -lmlx_linux -lXext -lX11 -lm -lz

INC = -Iinc

LIBFT = library/libft.a

SRC_DIR = src
SRC_FILES = $(wildcard *.c)
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

# BONUS_FILES = 
# BONUS_SRC = $(addprefix $(SRC_DIR)/, $(BONUS_FILES))

OBJ_DIR = obj
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# BONUS_OBJ = $(BONUS_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(NAME): $(OBJ_DIR) $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -c library

all: $(NAME)

bonus: $(OBJ_DIR) $(OBJ) $(BONUS_OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(BONUS_OBJ) $(LIBFT) -o $(NAME)

clean:
	$(RM) $(OBJ) $(BONUS_OBJ)
	$(RM) $(OBJ_DIR)
	make clean -C library

fclean: clean
	$(RM) $(NAME)
	make fclean -c library

re: fclean all

.PHONY: all clean fclean re