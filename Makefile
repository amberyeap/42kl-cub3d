# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/29 12:06:13 by ayeap             #+#    #+#              #
#    Updated: 2025/08/12 15:47:04 by ayeap            ###   ########.fr        #
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
# SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
SRC_FILES = src/main.c
# SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

# BONUS_FILES = 
# BONUS_SRC = $(addprefix $(SRC_DIR)/, $(BONUS_FILES))

OBJ_DIR = obj
OBJ = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# BONUS_OBJ = $(BONUS_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

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

# bonus: $(OBJ_DIR) $(OBJ) $(BONUS_OBJ) $(LIBFT)
# 	$(CC) $(CFLAGS) $(OBJ) $(BONUS_OBJ) $(LIBFT) -o $(NAME)

clean:
	$(RM) $(OBJ) $(BONUS_OBJ)
	$(RM) $(OBJ_DIR)
	make clean -C library

fclean: clean
	$(RM) $(NAME)
	make fclean -C library

re: fclean all

.PHONY: all clean fclean re