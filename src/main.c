#include "cub3d.h"

#define WIDTH 1280
#define HEIGHT 720

#define W 119
#define A 97
#define S 115
#define D 100

typedef struct s_player
{
	float x;
	float y;
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
	t_player *player;
}	t_game;

void init_player(t_player *player)
{
	player->x = WIDTH / 2;
	player->y = HEIGHT / 2;
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

void init_game(t_game *game)
{
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3d");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

int main(void)
{
	t_game game;

	init_game(&game);
	draw_square(WIDTH / 2, HEIGHT / 2, 10, 0xFF0000, &game);
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
