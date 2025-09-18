// #include "../inc/cub3d.h"
# include "../minilibx-linux/mlx.h"
# include "../library/inc/libft.h"

# include <stdio.h> // open, close, read, printf, perror
# include <stdlib.h> // malloc, free, exit
# include <unistd.h> // write
# include <fcntl.h> // open
# include <string.h> // strerror
# include <math.h>
# include <stdbool.h>

# define WIDTH 1280
# define HEIGHT 720
// # define BLOCK 64
// # define DEBUG 1

# define PI 3.14159265359

# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363
# define ESC 65307

# define NORTH 0
# define SOUTH 1
# define WEST 2
# define EAST 3

#define RED 0xC70039
#define DARK_RED 0x900C3F
// #define SKY_BLUE 0x87CEEB
// #define GREEN 0x228B22

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
	double player_x;
	double player_y;
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
	void *mlx; // mlx void pointer
	void *win; // window pointer for mlx
	// void *img;
	// char *addr;
	// int bpp; // bits per pixel
	// int line_len;
	// int endian;

	int maps[WIDTH][HEIGHT];

	// t_tex textures;
	t_map map_info;
	t_img frame;
	t_img textures[4];
	t_key keys;
	t_player player;
}	t_data;

void init_player(t_data *data)
{
	t_player *player = &data->player;

	player->player_x = 12.0;
	player->player_y = 6.0;
	player->angle = M_PI;
	player->dir_x = cos(player->angle);
	player->dir_y = sin(player->angle);
	player->plane_x = cos(player->angle + M_PI_2) * 0.66;
	player->plane_y = sin(player->angle + M_PI_2) * 0.66;
	// player.game = game;
}

void init_keys(t_key *keys)
{
	keys->w = false;
	keys->a = false;
	keys->s = false;
	keys->d = false;
	keys->left = false;
	keys->right = false;
}

void init_textures(t_data *data)
{
	data->map_info.north = "assets/sprites/north_wall.xpm";
	data->map_info.south = "assets/sprites/south_wall.xpm";
	data->map_info.east = "assets/sprites/east_wall.xpm";
	data->map_info.west = "assets/sprites/west_wall.xpm";

	data->textures[NORTH].img = mlx_xpm_file_to_image(data->mlx, data->map_info.north, &data->textures[NORTH].width, &data->textures[NORTH].height);
	data->textures[SOUTH].img = mlx_xpm_file_to_image(data->mlx, data->map_info.south, &data->textures[SOUTH].width, &data->textures[SOUTH].height);
	data->textures[EAST].img = mlx_xpm_file_to_image(data->mlx, data->map_info.east, &data->textures[EAST].width, &data->textures[EAST].height);
	data->textures[WEST].img = mlx_xpm_file_to_image(data->mlx, data->map_info.west, &data->textures[WEST].width, &data->textures[WEST].height);

	int i = 0;
	while (i < 4)
	{
		if (!data->textures[i].img)
		{
			printf("Failed to load image in: %d\n", i);
			exit(1);
		}
		i++;
	}

	i = 0;
	while (i < 4)
	{
		data->textures[i].addr = mlx_get_data_addr(data->textures[i].img, &data->textures[i].bpp, &data->textures[i].line_len, &data->textures[i].endian);
		i++;
	}
}

void init_frame(t_data *data)
{
	data->frame.img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->frame.addr = mlx_get_data_addr(data->frame.img, &data->frame.bpp, &data->frame.line_len, &data->frame.endian);
	data->frame.width = WIDTH;
	data->frame.height = HEIGHT;
}

void init_game(t_data *data)
{
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "cub3d");
	// data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	// data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_len, &data->endian);
	int temp_map[WIDTH][HEIGHT] = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,0,1,0,1,0,0,0,1},
		{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1},
		{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,0,1,0,1,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

	ft_memcpy(data->maps, temp_map, sizeof(temp_map));

	init_frame(data);
	init_textures(data);
	init_keys(&data->keys);
	init_player(data);
}

// void my_pixel_put(t_data *data, int x, int y, int colour)
// {
// 	// need to add edge case checks?

// 	char *dest;

// 	dest = data->addr + (y * data->line_len + x * (data->bpp / 8));
// 	*(unsigned int *)dest = colour;
// }

int get_pixel(t_img *tex, int x, int y)
{
	char *dest = tex->addr + (y * tex->line_len + x * (tex->bpp / 8));
	return *(unsigned int *)dest;
}

void put_pixel(t_img *img, int x, int y, int colour)
{
	char *dest = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)dest = colour;
}

void init_ray(t_ray *ray, int x, t_data *data)
{
	double camera_x = 2 * x / (double)WIDTH - 1;
	ray->ray_dir_x = data->player.dir_x + data->player.plane_x * camera_x;
	ray->ray_dir_y = data->player.dir_y + data->player.plane_y * camera_x;
	ray->delta_dist_x = (ray->ray_dir_x == 0) ? 1e30 : fabs(1 / ray->ray_dir_x);
	ray->delta_dist_y = (ray->ray_dir_y == 0) ? 1e30 : fabs(1 / ray->ray_dir_y);
	ray->map_x = (int)data->player.player_x;
	ray->map_y = (int)data->player.player_y;
	ray->hit = 0;
}

void count_step(t_ray *ray, t_data *data)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (data->player.player_x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - data->player.player_x) * ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (data->player.player_y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - data->player.player_y) * ray->delta_dist_y;
	}
}

void perform_dda(t_ray *ray, t_data *data)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (data->maps[ray->map_x][ray->map_y] > 0)
			ray->hit = 1;
	}
}

void count_perp_distance(t_ray *ray)
{
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->side_dist_x - ray->delta_dist_x);
	else
		ray->perp_wall_dist = (ray->side_dist_y - ray->delta_dist_y);
}

void draw_walls(t_ray *ray, int x, t_data *data)
{
	int line_height = (int)(HEIGHT / ray->perp_wall_dist);
	int start = -line_height / 2 + HEIGHT / 2;
	if (start < 0)
		start = 0;
	int end = line_height / 2 + HEIGHT / 2;
	if (end >= HEIGHT)
		end = HEIGHT - 1;

	// int colour;
	// if (ray->side == 1)
	// 	colour = RED;
	// else
	// 	colour = DARK_RED;

	// int y = start;
	// while (y++ < end)
	// 	my_pixel_put(data, x, y, colour);

	// t_img *tex;
	// if (ray->side == 0 && ray->ray_dir_x < 0)
	// 	tex = &data->textures[2]; // west
	// if (ray->side == 0 && ray->ray_dir_x > 0)
	// 	tex = &data->textures[3]; // east
	// if (ray->side == 1 && ray->ray_dir_y < 0)
	// 	tex = &data->textures[0]; // north
	// if (ray->side == 1 && ray->ray_dir_y > 0)
	// 	tex = &data->textures[1]; // south

	t_img *tex = &data->textures[NORTH]; 
    if (ray->side == 0)
    {
        if (ray->ray_dir_x < 0) 
            tex = &data->textures[WEST];
        else 
            tex = &data->textures[EAST];
    }
    else
    {
        if (ray->ray_dir_y < 0) 
            tex = &data->textures[NORTH];
        else 
            tex = &data->textures[SOUTH];
    }

	// int tex_num = data->maps[ray->map_x][ray->map_y];
	double wallX;
	if (ray->side == 0)
		wallX = data->player.player_y + ray->perp_wall_dist * ray->ray_dir_y;
	else
		wallX = data->player.player_x + ray->perp_wall_dist * ray->ray_dir_x;
	wallX -= floor(wallX);

	int texX = (int)(wallX * tex->width);
	if (ray->side == 0 && ray->ray_dir_x > 0)
		texX = tex->width - texX - 1;
	if (ray->side == 1 && ray->ray_dir_y < 0)
		texX = tex->width - texX - 1;

	double step = 1.0 * tex->height / line_height;
	double tex_pos = (start - HEIGHT / 2 + line_height / 2) * step;

	int y = start;
	while (y < end)
	{
		int tex_y = (int)tex_pos;
        if (tex_y < 0)
			tex_y = 0;
        if (tex_y >= tex->height)
			tex_y = tex->height - 1;
        tex_pos += step;

		int colour = get_pixel(tex, texX, tex_y);
		put_pixel(&data->frame, x, y, colour);
		y++;
	}
}

void draw_background(t_data *data)
{
    int x, y;

    // Ceiling (top half)
    for (y = 0; y < HEIGHT / 2; y++)
    {
        for (x = 0; x < WIDTH; x++)
            put_pixel(&data->frame, x, y, RED); // sky blue
    }

    // Floor (bottom half)
    for (y = HEIGHT / 2; y < HEIGHT; y++)
    {
        for (x = 0; x < WIDTH; x++)
            put_pixel(&data->frame, x, y, DARK_RED); // forest green
    }
}

void raycast(t_data *data)
{
	t_ray ray;
	int x = 0;

	while (x < WIDTH)
	{
		init_ray(&ray, x, data);
		count_step(&ray, data);
		perform_dda(&ray, data);
		count_perp_distance(&ray);
		draw_walls(&ray, x, data);
		x++;
	}
	
}

void clear_image(t_data *data)
{
	int x = 0;
	int y;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			put_pixel(&data->frame, x, y, 0);
			y++;
		}
		x++;
	}
}

void update_direction(t_player *player)
{
	player->dir_x = cos(player->angle);
	player->dir_y = sin(player->angle);
	player->plane_x = cos(player->angle + M_PI_2) * 0.66;
	player->plane_y = sin(player->angle + M_PI_2) * 0.66;
}

int key_press(int keycode, t_data *data)
{
	if (keycode == ESC)
		exit(0);
	if (keycode == W)
		data->keys.w = true;
	if (keycode == A)
		data->keys.a = true;
	if (keycode == S)
		data->keys.s = true;
	if (keycode == D)
		data->keys.d = true;
	if (keycode == LEFT)
		data->keys.left = true;
	if (keycode == RIGHT)
		data->keys.right = true;
	return (1);
}

int key_release(int keycode, t_data *data)
{
	if (keycode == W)
		data->keys.w = false;
	if (keycode == A)
		data->keys.a = false;
	if (keycode == S)
		data->keys.s = false;
	if (keycode == D)
		data->keys.d = false;
	if (keycode == LEFT)
		data->keys.left = false;
	if (keycode == RIGHT)
		data->keys.right = false;
	return (1);
}

void move_player(t_data *data)
{
	double move_speed = 0.03;
	double rot_speed = 0.02;

	// move forward
	if (data->keys.w == true)
	{
		double new_x = data->player.player_x + cos(data->player.angle) * move_speed;
		double new_y = data->player.player_y + sin(data->player.angle) * move_speed;

		if (data->maps[(int)new_x][(int)(data->player.player_y)] == 0)
			data->player.player_x = new_x;
		if (data->maps[(int)(data->player.player_x)][(int)new_y] == 0)
			data->player.player_y = new_y;
	}

	// move backward
	if (data->keys.s == true)
	{
		double new_x = data->player.player_x - cos(data->player.angle) * move_speed;
		double new_y = data->player.player_y - sin(data->player.angle) * move_speed;

		if (data->maps[(int)new_x][(int)(data->player.player_y)] == 0)
			data->player.player_x = new_x;
		if (data->maps[(int)(data->player.player_x)][(int)new_y] == 0)
			data->player.player_y = new_y;
	}

	// strafe right (D)
	if (data->keys.a == true)
	{
		double strafe_x = sin(data->player.angle);
		double strafe_y = -cos(data->player.angle);

		double new_x = data->player.player_x + strafe_x * move_speed;
		double new_y = data->player.player_y + strafe_y * move_speed;

		if (data->maps[(int)new_x][(int)(data->player.player_y)] == 0)
			data->player.player_x = new_x;
		if (data->maps[(int)(data->player.player_x)][(int)new_y] == 0)
			data->player.player_y = new_y;
	}

	// strafe left (A)
	if (data->keys.d == true)
	{
		double strafe_x = -sin(data->player.angle);
		double strafe_y = cos(data->player.angle);

		double new_x = data->player.player_x + strafe_x * move_speed;
		double new_y = data->player.player_y + strafe_y * move_speed;

		if (data->maps[(int)new_x][(int)(data->player.player_y)] == 0)
			data->player.player_x = new_x;
		if (data->maps[(int)(data->player.player_x)][(int)new_y] == 0)
			data->player.player_y = new_y;
	}

	// rotate to right
	if (data->keys.right == true)
	{
		data->player.angle += rot_speed;
		update_direction(&data->player);
	}

	// rotate to left
	if (data->keys.left == true)
	{
		data->player.angle -= rot_speed;
		update_direction(&data->player);
	}
}

int render(t_data *data)
{
	clear_image(data);
	draw_background(data);
	raycast(data);
	move_player(data);
	mlx_put_image_to_window(data->mlx, data->win, data->frame.img, 0, 0);

	return (0);
}

int exit_game(t_data *data)
{
	(void)data; // temp
	// add cleanup functions?
	exit(0);
}

// 2 KeyPress Event; 1L<<0 KeyPress Mask
// 3 KeyRelease Event; 1L<<1 KeyRelease Mask
int main(void)
{
	t_data data;

	init_game(&data);

	// for player movement
	// mlx_hook(game.win, 2, 1L<<0, key_hook, &game);
	mlx_hook(data.win, 2, 1L<<0, key_press, &data);
	mlx_hook(data.win, 3, 1L<<1, key_release, &data);

	// for the red 'x' button on the window
	mlx_hook(data.win, 17, 0, exit_game, &data);

	mlx_loop_hook(data.mlx, render, &data);
	mlx_loop(data.mlx);

	return (0);
}
