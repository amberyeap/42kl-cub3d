# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/29 12:06:13 by ayeap             #+#    #+#              #
#    Updated: 2025/09/18 11:19:05 by muhabin-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
RM = rm -rf

MLX_DIR = minilibx-linux
MLX_INC = -I$(MLX_DIR)
MLX_FLAGS = -lXext -lX11 -lm #-lz

INC = -Iinc

LIBFT = library/libft.a

SRC_DIR = src
SRC_FILES = \
	src/key_hook.c \
	src/raycast.c \
	src/utils.c \
	src/utils2.c \
	src/parsing/color_parse.c \
	src/parsing/config_parse.c \
	src/parsing/parser.c \
	src/parsing/parser2.c \
	src/validation/map_validation.c \

OBJ_DIR = obj
OBJ = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(NAME): $(OBJ_DIR) $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -L$(MLX_DIR) -lmlx_Linux $(LIBFT) $(MLX_INC) $(MLX_FLAGS) -o $(NAME)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) $(MLX_INC) -c $< -o $@

$(LIBFT):
	make -C library

all: $(NAME)

clean:
	$(RM) $(OBJ) $(BONUS_OBJ)
	$(RM) $(OBJ_DIR)
	make clean -C library

fclean: clean
	$(RM) $(NAME)
	make fclean -C library

re: fclean all

.PHONY: all clean fclean re
