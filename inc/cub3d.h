/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:14:14 by ayeap             #+#    #+#             */
/*   Updated: 2025/05/28 09:07:09 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../minilibx-linux/mlx.h"
# include "libft.h"

# include <stdio.h> // open, close, read, printf, perror
# include <stdlib.h> // malloc, free, exit
# include <unistd.h> // write
# include <fcntl.h> // open
# include <string.h> // strerror

typedef struct s_map
{
	int		fd; //fd for opening the map
	int		line_map; // how many lines inside the map files
	char	**file;

} t_map;

typedef struct s_data
{
	char	**maps; // an 2d array of map
	t_map	map_info;

}	t_data;

// utils.c
int		error_msg(char *msg, int status);
int		is_cub_file(char *argv);

#endif
