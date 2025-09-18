# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/29 12:06:13 by ayeap             #+#    #+#              #
#    Updated: 2025/09/18 15:31:11 by ayeap            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
RM = rm -rf

MLX_DIR = minilibx-linux
MLX_INC = -I$(MLX_DIR)
MLX_FLAGS = -lXext -lX11 -lm -lz

INC = -Iinc

LIBFT = library/libft.a
MINILIB = -lmlx

SRC_DIR = src
SRC_FILES = \
	src/draw.c \
	src/init_texture.c \
	src/init.c \
	src/key_hook.c \
	src/main.c \
	src/player_movement.c \
	src/raycast.c \
	src/utils.c \
	src/utils2.c \
	src/utils3.c \
	src/parsing/color_parse.c \
	src/parsing/config_parse.c \
	src/parsing/parser.c \
	src/parsing/parser2.c \
	src/validation/map_validation.c \

OBJ_DIR = obj
OBJ = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(NAME): $(OBJ_DIR) $(OBJ) $(LIBFT) $(MINILIB)
	$(CC) $(CFLAGS) $(OBJ) -L$(MLX_DIR) $(MINILIB) $(LIBFT) $(MLX_INC) $(MLX_FLAGS) -o $(NAME)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) $(MLX_INC) -c $< -o $@

$(LIBFT):
	make -C library

$(MINILIB):
	make -C $(MLX_DIR)

all: $(NAME)

clean:
	$(RM) $(OBJ) $(BONUS_OBJ)
	$(RM) $(OBJ_DIR)
	make clean -C library
	make clean -C $(MLX_DIR)

fclean: clean
	$(RM) $(NAME)
	make fclean -C library
	make clean -C $(MLX_DIR)

re: fclean all

.PHONY: all clean fclean re
