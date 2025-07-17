#include "../inc/cub3d.h"

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

#define RED 0xC70039
#define DARK_RED 0x900C3F

typedef struct s_game t_game;

typedef struct s_player
{
	double x;
	double y;
	double dir_x;
	double dir_y;
	double plane_x;
	double plane_y;

	// float angle;

    // bool key_up;
    // bool key_down;
    // bool key_left;
    // bool key_right;

    // bool left_rotate;
    // bool right_rotate;

	// t_game *game;
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

struct s_game
{
	void *mlx; // mlx void pointer
	void *win; // window pointer for mlx
	void *img;
	char *addr;
	int bpp; // bits per pixel
	int line_len;
	int endian;

	int map[WIDTH][HEIGHT];

	t_player player;
};

void init_player(t_game *game)
{
	t_player *player = &game->player;

	player->x = 12.0;
	player->y = 6.0;
	player->dir_x = -1.0;
	player->dir_y = 0.0;
	player->plane_x = 0.0;
	player->plane_y = 0.66;
	// player.game = game;
}

void init_game(t_game *game)
{
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3d");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->addr = mlx_get_data_addr(game->img, &game->bpp, &game->line_len, &game->endian);
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

	memcpy(game->map, temp_map, sizeof(temp_map));

	init_player(game);
}

void my_pixel_put(t_game *game, int x, int y, int colour)
{
	// need to add edge case checks?

	char *dest;

	dest = game->addr + (y * game->line_len + x * (game->bpp / 8));
	*(unsigned int *)dest = colour;
}

void init_ray(t_ray *ray, int x, t_game *game)
{
	double camera_x = 2 * x / (double)WIDTH - 1;
	ray->ray_dir_x = game->player.dir_x + game->player.plane_x * camera_x;
	ray->ray_dir_y = game->player.dir_y + game->player.plane_y * camera_x;
	ray->delta_dist_x = (ray->ray_dir_x == 0) ? 1e30 : fabs(1 / ray->ray_dir_x);
	ray->delta_dist_y = (ray->ray_dir_y == 0) ? 1e30 : fabs(1 / ray->ray_dir_y);
	ray->map_x = (int)game->player.x;
	ray->map_y = (int)game->player.y;
	ray->hit = 0;
}

void count_step(t_ray *ray, t_game *game)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (game->player.x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - game->player.x) * ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (game->player.y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - game->player.y) * ray->delta_dist_y;
	}
}

void perform_dda(t_ray *ray, t_game *game)
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
		if (game->map[ray->map_x][ray->map_y] > 0)
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

void draw_walls(t_ray *ray, int x, t_game *game)
{
	int line_height = (int)(HEIGHT / ray->perp_wall_dist);
	int start = -line_height / 2 + HEIGHT / 2;
	if (start < 0)
		start = 0;
	int end = line_height / 2 + HEIGHT / 2;
	if (end >= HEIGHT)
		end = HEIGHT - 1;

	int colour;
	if (ray->side == 1)
		colour = RED;
	else
		colour = DARK_RED;

	int y = start;
	while (y++ < end)
		my_pixel_put(game, x, y, colour);
}

void raycast(t_game *game)
{
	t_ray ray;
	int x = 0;

	while (x < WIDTH)
	{
		init_ray(&ray, x, game);
		count_step(&ray, game);
		perform_dda(&ray, game);
		count_perp_distance(&ray);
		draw_walls(&ray, x, game);
		x++;
	}
	
}

void clear_image(t_game *game)
{
	int x = 0;
	int y;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			my_pixel_put(game, x, y, 0);
			y++;
		}
		x++;
	}
}

int render(t_game *game)
{
	//clear image 
	clear_image(game);

	raycast(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);

	return (0);
}

int key_hook(int keycode, t_game *game)
{
	double move_speed = 0.1;
	double rot_speed = 0.05;

	if (keycode == ESC)
		exit(0);

	// move forward
	if (keycode == W)
	{
		if (game->map[(int)(game->player.x + game->player.dir_x * move_speed)][(int)(game->player.y)] == 0)
			game->player.x += game->player.dir_x * move_speed;
		if (game->map[(int)(game->player.x)][(int)(game->player.y + game->player.dir_y * move_speed)] == 0)
			game->player.y += game->player.dir_y * move_speed;
	}

	// move backward
	if (keycode == S)
	{
		if (game->map[(int)(game->player.x - game->player.dir_x * move_speed)][(int)(game->player.y)] == 0)
			game->player.x -= game->player.dir_x * move_speed;
		if (game->map[(int)(game->player.x)][(int)(game->player.y - game->player.dir_y * move_speed)] == 0)
			game->player.y -= game->player.dir_y * move_speed;
	}

	// rotate to right
	if (keycode == D)
	{
		double old_dir_x = game->player.dir_x;
		double old_plane_x = game->player.plane_x;

		game->player.dir_x = game->player.dir_x * cos(-rot_speed) - game->player.dir_y * sin(-rot_speed);
		game->player.dir_y = old_dir_x * sin(-rot_speed) + game->player.dir_y * cos(-rot_speed);
		game->player.plane_x = game->player.plane_x * cos(-rot_speed) - game->player.plane_y * sin(-rot_speed);
		game->player.plane_y = old_plane_x * sin(-rot_speed) + game->player.plane_y * cos(-rot_speed);
	}

	// rotate to left
	if (keycode == A)
	{
		double old_dir_x = game->player.dir_x;
		double old_plane_x = game->player.plane_x;

		game->player.dir_x = game->player.dir_x * cos(rot_speed) - game->player.dir_y * sin(rot_speed);
		game->player.dir_y = old_dir_x * sin(rot_speed) + game->player.dir_y * cos(rot_speed);
		game->player.plane_x = game->player.plane_x * cos(rot_speed) - game->player.plane_y * sin(rot_speed);
		game->player.plane_y = old_plane_x * sin(rot_speed) + game->player.plane_y * cos(rot_speed);
	}
	return (0);
}

int main(void)
{
	t_game game;

	init_game(&game);
	mlx_hook(game.win, 2, 1L<<0, key_hook, &game);
	// mlx_hook(game.win, 2, 1L<<0, key_press, &game);
	// mlx_hook(game.win, 3, 1L<<1, key_release, &game);
	mlx_loop_hook(game.mlx, render, &game);
	mlx_loop(game.mlx);

	return (0);
}

// char **get_map(void)
// {	
//     char **map;
//     const char *template[] = {
//         "1111111111111",
//         "1000000001001",
//         "1110000000001",
//         "1000000001001",
//         "10000000010N1",
//         "1111111111111"
//     };
    
//     map = malloc(sizeof(char *) * 7);
//     if (!map)
//         return NULL;
        
//     for (int i = 0; i < 6; i++) {
//         map[i] = strdup(template[i]);
//         if (!map[i]) {
//             // Cleanup already allocated rows
//             while (--i >= 0)
//                 free(map[i]);
//             free(map);
//             return NULL;
//         }
//     }
//     map[6] = NULL;
//     return map;
// }

// int is_player(char c)
// {
// 	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
// 		return 1;
// 	return 0;
// }

// void print_values(t_game *g)
// {
// 	printf("%f\n", g->player->pos_x);
// 	printf("%f\n", g->player->pos_y);
// }

// void init_player(t_game *game)
// {
// 	game->player = malloc(sizeof(t_player));
// 	int y = 0;
// 	int x;
// 	while (game->map[y])
// 	{
// 		x = 0;
// 		while (game->map[y][x])
// 		{
// 			if (is_player(game->map[y][x]))
// 			{
// 				game->player->pos_x = x;
// 				game->player->pos_y = y;
// 				break;
// 			}
// 			x++;
// 		}
// 		y++;
// 	}
// 	print_values(game);
// 	game->player->dir_x = 0.0;
// 	game->player->dir_y = -1.0;
// 	game->player->plane_x = 0.66;
// 	game->player->plane_y = 0.0;

// 	game->player->angle = PI / 2;

//     game->player->key_up = false;
//     game->player->key_down = false;
//     game->player->key_right = false;
//     game->player->key_left = false;

//     game->player->left_rotate = false;
//     game->player->right_rotate = false;

// 	game->player->game = game;
// }

// void init_game(t_game *game)
// {
// 	// game = malloc(sizeof(t_game));
// 	game->mlx = mlx_init();
// 	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3d");
// 	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
// 	game->addr = mlx_get_data_addr(game->img, &game->bpp, &game->line_len, &game->endian);

// 	game->map = get_map();
// 	init_player(game);
// }

// void my_pixel_put(t_game *game, int x, int y, int colour)
// {
//     if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
//         return;
        
//     char *dest = game->addr + (y * game->line_len + x * (game->bpp / 8));
//     *(unsigned int *)dest = colour;
// }

// void draw_column(t_game *game, int x, int start, int end, int colour)
// {
// 	for (int y = start; y <= end; y++)
// 		my_pixel_put(game, x, y, colour);
// }

// void start_game(t_game *game)
// {
// 	t_player *player = game->player;
// 	for (int x = 0; x < WIDTH; x++)
// 	{
// 		double cameraX = 2 * x / (double)WIDTH - 1;
// 		double raydirX = player->dir_x + player->plane_x * cameraX;
// 		double raydirY = player->dir_y + player->plane_y * cameraX;

// 		int mapX = (int)player->pos_x;
// 		int mapY = (int)player->pos_y;

// 		double deltaX = fabs(1 / raydirX);
// 		double deltaY = fabs(1 / raydirY);

// 		// calculates the ray direction
// 		double sideX, sideY;
// 		int stepX, stepY;

// 		if (raydirX < 0)
// 		{
// 			stepX = -1;
// 			sideX = (player->pos_x - mapX) * deltaX;
// 		}
// 		else
// 		{
// 			stepX = 1;
// 			sideX = (mapX + 1.0 - player->pos_x) * deltaX;
// 		}
// 		if (raydirY < 0)
// 		{
// 			stepY = -1;
// 			sideY = (player->pos_y - mapY) * deltaY;
// 		}
// 		else
// 		{
// 			stepY = 1;
// 			sideY = (mapY + 1.0 - player->pos_y) * deltaY;
// 		}

// 		int hit = 0, side;
// 		while (!hit)
// 		{
// 			if (sideX < sideY)
// 			{
// 				sideX += deltaX;
// 				mapX += stepX;
// 				side = 0;
// 			}
// 			else
// 			{
// 				sideY += deltaY;
// 				mapY += stepY;
// 				side = 1;
// 			}
// 			if (game->map[mapY][mapX] == '1')
// 				hit = 1;
// 		}
		
// 		// fixes the "fisheye" effect 
// 		double perpWall;
// 		if (side == 0)
// 			perpWall = (mapX - player->pos_x + (1 - stepX) / 2) / raydirX;
// 		else
// 			perpWall = (mapY - player->pos_y + (1 - stepY) / 2) / raydirY;

// 		int line_h = (int)(HEIGHT / perpWall);
// 		int drawStart = -line_h / 2 + HEIGHT / 2;
// 		if (drawStart < 0)
// 			drawStart = 0;
// 		int drawEnd = line_h / 2 + HEIGHT / 2;
// 		if (drawEnd >= HEIGHT)
// 			drawEnd = HEIGHT - 1;

// 		int colour;
// 		if (side == 0)
// 			colour = 0x00FF0000;
// 		else
// 			colour = 0x00880000;
		
// 		draw_column(game, x, drawStart, drawEnd, colour);
// 	}
// 	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
// 	// mlx_destroy_image(game->mlx, game->img);
// }

// int key_press(int keycode, t_player *player)
// {
//     if(keycode == W)
//         player->key_up = true;
//     if(keycode == S)
//         player->key_down = true;
//     if(keycode == A)
//         player->key_left = true;
//     if(keycode == D)
//         player->key_right = true;
//     if(keycode == LEFT)
//         player->left_rotate = true;
//     if(keycode == RIGHT)
//         player->right_rotate = true;
//     return 0;
// }

// int key_release(int keycode, t_player *player)
// {
//     if(keycode == W)
//         player->key_up = false;
//     if(keycode == S)
//         player->key_down = false;
//     if(keycode == A)
//         player->key_left = false;
//     if(keycode == D)
//         player->key_right = false;
//     if(keycode == LEFT)
//         player->left_rotate = false;
//     if(keycode == RIGHT)
//         player->right_rotate = false;
//     return 0;
// }

// void move_player(t_player *player)
// {
//     int speed = 3;
//     float angle_speed = 0.03;
//     float cos_angle = cos(player->angle);
//     float sin_angle = sin(player->angle);

//     if (player->left_rotate)
//         player->angle -= angle_speed;
//     if (player->right_rotate)
//         player->angle += angle_speed;
//     if (player->angle > 2 * PI)
//         player->angle = 0;
//     if (player->angle < 0)
//         player->angle = 2 * PI;

//     if (player->key_up)
//     {
//         player->pos_x += cos_angle * speed;
//         player->pos_y += sin_angle * speed;
//     }
//     if (player->key_down)
//     {
//         player->pos_x -= cos_angle * speed;
//         player->pos_y -= sin_angle * speed;
//     }
//     if (player->key_left)
//     {
//         player->pos_x += sin_angle * speed;
//         player->pos_y -= cos_angle * speed;
//     }
//     if (player->key_right)
//     {
//         player->pos_x -= sin_angle * speed;
//         player->pos_y += cos_angle * speed;
//     }
// }

// // void print_values(t_game *game)
// // {
// // 	printf("x: %d\ny: %d\n", game->player->x, game->player->y);
// // 	printf("player->game: %p\n", game);
// // 	printf("player->game: %p\n", game->player->game);
	
// // 	int i = 0;
// // 	while (game->map[i])
// // 	{
// // 		printf("%s\n", game->map[i]);
// // 		i++;
// // 	}
// // }

// void clear_image(t_game *game)
// {
//     for(int y = 0; y < HEIGHT; y++)
//         for(int x = 0; x < WIDTH; x++)
//             my_pixel_put(game, x, y, 0);
// }

// int draw_loop(t_game *game)
// {
// 	move_player(game->player);
// 	clear_image(game);
// 	start_game(game);
// 	return 0;
// }

// int main(void)
// {
// 	t_game game;

// 	init_game(&game);
// 	start_game(&game);
// 	move_player(game.player);
//     mlx_hook(game.win, 2, 1L<<0, key_press, &game.player);
//     mlx_hook(game.win, 3, 1L<<1, key_release, &game.player);
// 	// mlx_loop_hook(game.mlx, draw_loop, &game);
// 	// mlx_loop_hook(game.mlx, &start_game, &game);
// 	// print_values(&game);
// 	mlx_loop(game.mlx);
// }
