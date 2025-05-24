#include "cub3d.h"

#define WIDTH 1280
#define HEIGHT 720
#define BLOCK 64
#define DEBUG 1

#define W 119
#define A 97
#define S 115
#define D 100
#define LEFT_ARROW 65361
#define RIGHT_ARROW 65363

#define PI 3.14159

typedef struct s_player
{
	float x;
	float y;
	float angle;

	bool key_w;
	bool key_a;
	bool key_s;
	bool key_d;

	bool left_rotate;
	bool right_rotate;
}	t_player;

typedef struct s_game
{
	void *mlx;
	void *win;
	void *img;

	char *data;
	int bpp;
	int size_line;
	int endian;
	t_player player;

	char **map;
}	t_game;

int key_press(int key, t_player *player)
{
	if (key == W)
		player->key_w = true;
	else if (key == A)
		player->key_a = true;
	else if (key == S)
		player->key_s = true;
	else if (key == D)
		player->key_d = true;
	else if (key == LEFT_ARROW)
		player->left_rotate = true;
	else if (key == RIGHT_ARROW)
		player->right_rotate = true;
	return (0);
}

int key_release(int key, t_player *player)
{
	if (key == W)
		player->key_w = false;
	else if (key == A)
		player->key_a = false;
	else if (key == S)
		player->key_s = false;
	else if (key == D)
		player->key_d = false;
	else if (key == LEFT_ARROW)
		player->left_rotate = false;
	else if (key == RIGHT_ARROW)
		player->right_rotate = false;
	return (0);
}

void init_player(t_player *player)
{
	// will vary based on the input map
	player->x = WIDTH / 2;
	player->y = HEIGHT / 2;
	player->angle = PI / 2;

	player->key_w = false;
	player->key_a = false;
	player->key_s = false;
	player->key_d = false;

	player->left_rotate = false;
	player->right_rotate = false;
}

void put_pixel(int x, int y, int colour, t_game *game)
{
	int index;

	if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
		return ;
	index = y * game->size_line + x * game->bpp / 8;
	game->data[index] = colour & 0xFF;
	game->data[index + 1] = (colour >> 8) & 0xFF;
	game->data[index + 2] = (colour >> 16) & 0xFF;
}

void draw_square(int x, int y, int size, int colour, t_game *game)
{
	int i;

	i = 0;
	while (i++ < size)
		put_pixel(x + i, y, colour, game);
	i = 0;
	while (i++ < size)
		put_pixel(x, y + i, colour, game);
	i = 0;
	while (i++ < size)
		put_pixel(x + size, y + i, colour, game);
	i = 0;
	while (i++ < size)
		put_pixel(x + i, y + size, colour, game);
}

char **get_map(void)
{
	char **map = malloc(sizeof(char *) * 6);
	map[0] = "1111111111111111";
	map[1] = "1000000000000001";
	map[2] = "1000000000000001";
	map[3] = "1000000000000001";
	map[4] = "1111111111111111";
	map[5] = NULL;
	return map;
}

void init_game(t_game *game)
{
	init_player(&game->player);
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3d");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
	game->map = get_map();
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

void move_player(t_player *player)
{
	int speed = 3;
	float angle_speed = 0.03;
	float cos_angle = cos(player->angle);
	float sin_angle = sin(player->angle);

	if (player->left_rotate)
		player->angle -= angle_speed;
	if (player->right_rotate)
		player->angle += angle_speed;
	if (player->angle > 2 * PI)
		player->angle = 0;
	if (player->angle < 0)
		player->angle = 2 * PI;

	if (player->key_w)
	{
		player->x += cos_angle * speed;
		player->y += sin_angle * speed;
	}
	if (player->key_s)
	{
		player->x -= cos_angle * speed;
		player->y -= sin_angle * speed;
	}
	if (player->key_a)
	{
		player->x += cos_angle * speed;
		player->y -= sin_angle * speed;
	}
	if (player->key_d)
	{
		player->x -= cos_angle * speed;
		player->y += sin_angle * speed;
	}
}

void clear_image(t_game *game)
{
	int y;
	int x;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			put_pixel(x, y, 0, game);
			x++;
		}
		y++;
	}
}

float distance(float x, float y)
{
	return sqrt(x * x + y * y);
}

float fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
{
	float delta_x = x2 - x1;
	float delta_y = y2 - y1;
	float angle = atan2(delta_y, delta_x) - game->player.angle;
	float fix_dist = distance(delta_x, delta_y) * cos(angle);
	return fix_dist;
}

bool touch(float px, float py, t_game *game)
{
	int x = px / BLOCK;
	int y = py / BLOCK;
	if (game->map[y][x] == '1')
		return true;
	return false;
}

void draw_line(t_player *player, t_game *game, float start_x, int i)
{
	float cos_angle = cos(start_x);
	float sin_angle = sin(start_x);
	float ray_x = player->x;
	float ray_y = player->y;

	while (!touch(ray_x, ray_y, game))
	{
		if (DEBUG)
			put_pixel(ray_x, ray_y, 0x00FF00, game);
		ray_x += cos_angle;
		ray_y += sin_angle;
	}
	if (!DEBUG)
	{
		float dist = fixed_dist(player->x, player->y, ray_x, ray_y, game);
		float height = (BLOCK / dist) * (WIDTH / 2);
		int start_y = (HEIGHT - height) / 2;
		int end = start_y + height;
		while (start_y < end)
		{
			put_pixel(i, start_y, 255, game);
			start_y++;
		}
	}
}

void draw_map(t_game *game)
{
	char **map = game->map;
	int colour = 0x0000FF;
	for (int y = 0; map[y]; y++)
	{
		for (int x = 0; map[y][x]; x++)
		{
			if (map[y][x] == '1')
				draw_square(x * BLOCK, y * BLOCK, BLOCK, colour, game);
			// else if (map[y][x] == '0')
			// 	draw_square(x * BLOCK, y * BLOCK, BLOCK, 0xFFFFFF, game);
			// else if (map[y][x] == ' ')
			// 	draw_square(x * BLOCK, y * BLOCK, BLOCK, 0x000000, game);
		}
	}
}

int draw_loop(t_game *game)
{
	t_player *player = &game->player;
	move_player(player);
	clear_image(game);
	
	if (DEBUG)
	{
		draw_square(player->x, player->y, 10, 0xFF0000, game);
		draw_map(game);
	}

	float fraction = PI / 3 / WIDTH;
	float start_x = player->angle - PI / 6;
	int i = 0;
	while (i < WIDTH)
	{
		draw_line(player, game, start_x, i);
		start_x += fraction;
		i++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}

int main(void)
{
	t_game game;

	init_game(&game);
	draw_square(WIDTH / 2, HEIGHT / 2, 10, 0xFF0000, &game);
	mlx_hook(game.win, 2, 1L<<0, key_press, &game.player);
	mlx_hook(game.win, 3, 1L<<1, key_release, &game.player);
	mlx_loop_hook(game.mlx, draw_loop, &game);
	mlx_loop(game.mlx);
	return (0);
}

// typedef struct	s_data {
// 	void	*img;
// 	char	*addr;
// 	int		bits_per_pixel;
// 	int		line_length;
// 	int		endian;
// }				t_data;

// void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
// {
//     char	*dst;

//     dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
//     *(unsigned int*)dst = color;
// }

// int	main(void)
// {
// 	void	*mlx;
// 	void	*mlx_win;
// 	t_data	img;
// 	int x, y;

// 	mlx = mlx_init();
// 	mlx_win = mlx_new_window(mlx, 1920, 1080, "Hello world!");
// 	img.img = mlx_new_image(mlx, 1920, 1080);
// 	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
// 								&img.endian);
//    // Draw a 10x10 red square
//     y = 5;
//     while (y < 15)
//     {
//         x = 5;
//         while (x < 15)
//         {
//             my_mlx_pixel_put(&img, x, y, 0xFF0000);
//             x++;
//         }
//         y++;
//     }
    
// 	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
// 	mlx_loop(mlx);
// }
