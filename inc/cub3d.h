/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:14:14 by ayeap             #+#    #+#             */
/*   Updated: 2025/08/12 13:55:09 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../minilibx-linux/mlx.h"
# include "../library/inc/libft.h"

# include <stdio.h> // open, close, read, printf, perror
# include <stdlib.h> // malloc, free, exit
# include <unistd.h> // write
# include <fcntl.h> // open
# include <string.h> // strerror
# include <math.h>
# include <stdbool.h>

# define WIDTH 1920
# define HEIGHT 1080

//Configure
# define NORTH		1
# define SOUTH		2
# define WEST		3
# define EAST		4
# define FLOOR		5
# define CEILING	6

//Type configure
# define TEX		7
# define COL		8

//Files type
# define CUB		9
# define XPM		10

// keycodes
# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363
# define ESC 65307

// colour codes for testing
#define RED 0xC70039
#define DARK_RED 0x900C3F

typedef struct s_map
{
	int				fd; //fd for opening the map
	int				line_cub; // how many lines inside the map files
	int				floor;
	int				ceiling;
	char			**file;
	char			*north;
	char			*south;
	char			*west;
	char			*east;
	int				empty_line;

}	t_map;

typedef struct s_player
{
	double				player_x;
	double				player_y;
	char			p_direction; // player direction

	double angle;
	double dir_x;
	double dir_y;
	double plane_x;
	double plane_y;

}	t_player;

typedef struct s_ray
{
	double ray_dir_x;
	double ray_dir_y;
	double delta_dist_x;
	double delta_dist_y;
	double side_dist_x;
	double side_dist_y;
	int map_x;
	int map_y;
	int step_x;
	int step_y;
	int hit;
	int side;
	double perp_wall_dist;
}	t_ray;

typedef struct s_key
{
    bool w;
    bool s;
    bool a;
    bool d;

    bool left;
    bool right;
}	t_key;

typedef struct s_img
{
	void *img;
	char *addr;
	int bpp;
	int line_len;
	int endian;
	int width;
	int height;
}	t_img;

typedef struct s_data
{
	char		**maps; // an 2d array of map
	int			map_start;
	t_map		map_info;
	t_player	player;

	void *mlx; // mlx void pointer
	void *win; // window pointer for mlx
	void *img;
	char *addr;
	int bpp; // bits per pixel
	int line_len;
	int endian;
	t_img frame;
	t_img textures[4];
	t_key keys;

}	t_data;

// color_parse.c
void	save_color(t_data *data, char **rgb, int mode);
int	valid_color(char **rgb);

// config_parse.c
void	color_check(t_data *data, char *line, int mode);
void	texture_check(t_data *data, char *line, int dir);
char	*get_path(char *line, int mode);
void	parsing_config(t_data *data, char *line, int i);

// key_hook.c
void init_keys(t_key *keys);
void move_player(t_data *data);
int key_press(int keycode, t_data *data);
int key_release(int keycode, t_data *data);

// map_validation.c
void	flood_fill(t_data *data, char **map, int x, int y);
char	**copy_map(char **map);
void	pad_map(char **map);
void	find_player(t_data *data, char **map);
void	valid_char(char **map);

// parser.c
int	readmap(t_data *data);
int	map_read(t_data *data, char *argv);

// parser2.c
void	print_map(char **map); // to delete
void	main_parse(char **map);
void	parse_map(t_data *data);

// utils.c
int		error_msg(char *msg, int status);
int		is_cub_file(char *argv);
void	can_open_file(char *file, int fd);
void	free_array(char **str);
int		is_map_line(char *line);

// utils2.c
int	count_line(char *map);
void	exit_error(const char *msg);
int	map_height(char **m);
int	get_len(char **map);
void	count_check(int count);

int	empty_line(char *line);

#endif