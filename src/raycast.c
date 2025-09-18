#include "../inc/cub3d.h"

void init_textures(t_data *data)
{
	if (!data->map_info.north || !data->map_info.south
        || !data->map_info.east || !data->map_info.west)
        exit_error("Texture path missing");

	data->textures[NORTH].img = mlx_xpm_file_to_image(data->mlx, data->map_info.north, &data->textures[NORTH].width, &data->textures[NORTH].height);
	data->textures[SOUTH].img = mlx_xpm_file_to_image(data->mlx, data->map_info.south, &data->textures[SOUTH].width, &data->textures[SOUTH].height);
	data->textures[EAST].img = mlx_xpm_file_to_image(data->mlx, data->map_info.east, &data->textures[EAST].width, &data->textures[EAST].height);
	data->textures[WEST].img = mlx_xpm_file_to_image(data->mlx, data->map_info.west, &data->textures[WEST].width, &data->textures[WEST].height);

	if (!data->textures[NORTH].img)
		exit_error("Failed to load north_wall.xpm");
	if (!data->textures[SOUTH].img)
		exit_error("Failed to load south_wall.xpm");
	if (!data->textures[EAST].img)
		exit_error("Failed to load east_wall.xpm");
	if (!data->textures[WEST].img)
		exit_error("Failed to load west_wall.xpm");

	data->textures[NORTH].addr = mlx_get_data_addr(data->textures[NORTH].img, &data->textures[NORTH].bpp, &data->textures[NORTH].line_len, &data->textures[NORTH].endian);
	data->textures[SOUTH].addr = mlx_get_data_addr(data->textures[SOUTH].img, &data->textures[SOUTH].bpp, &data->textures[SOUTH].line_len, &data->textures[SOUTH].endian);
	data->textures[EAST].addr = mlx_get_data_addr(data->textures[EAST].img, &data->textures[EAST].bpp, &data->textures[EAST].line_len, &data->textures[EAST].endian);
	data->textures[WEST].addr = mlx_get_data_addr(data->textures[WEST].img, &data->textures[WEST].bpp, &data->textures[WEST].line_len, &data->textures[WEST].endian);
}

void init_frame(t_data *data)
{
	data->frame.img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->frame.addr = mlx_get_data_addr(data->frame.img, &data->frame.bpp, &data->frame.line_len, &data->frame.endian);
	data->frame.width = WIDTH;
	data->frame.height = HEIGHT;
}

void	init_data(t_data *data)
{
	data->map_start = 0;
	data->map_info.ceiling = -1;
	data->map_info.floor = -1;

	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "cub3d");
	// data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	// data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_len, &data->endian);
	init_frame(data);
}

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

void init_player(t_data *data)
{
	init_keys(&data->keys);
	t_player *player = &data->player;
	if (player->p_direction == 'N')
		player->angle = 3 * M_PI / 2;
	else if (player->p_direction == 'S')
		player->angle = M_PI / 2;
	else if (player->p_direction == 'E')
		player->angle = 0;
	else if (player->p_direction == 'W')
		player->angle = M_PI;
	else
		player->angle = M_PI;

	// player->angle = M_PI;
	player->dir_x = cos(player->angle);
	player->dir_y = sin(player->angle);

	player->plane_x = cos(player->angle + M_PI_2) * 0.66;
	player->plane_y = sin(player->angle + M_PI_2) * 0.66;
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
	ray->step_x = 0;
	ray->step_y = 0;
	ray->perp_wall_dist = 0;
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
		// printf("mapy: %d, mapx: %d\n", ray->map_y, ray->map_x);

		if (data->maps[ray->map_y][ray->map_x] == '1')
			ray->hit = 1;
		// printf("test");
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
	int	y;
	int	x;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			if (y < HEIGHT / 2)
				put_pixel(&data->frame, x, y, data->map_info.ceiling);
			else
				put_pixel(&data->frame, x, y, data->map_info.floor);
			x++;
		}
		y++;
	}
}

void raycast(t_data *data)
{
	t_ray ray;
	int x = 0;

	while (x < WIDTH)
	{
		// init_player(data);
		init_ray(&ray, x, data);
		count_step(&ray, data);
		perform_dda(&ray, data);
		count_perp_distance(&ray);
		draw_walls(&ray, x, data);
		
		x++;
	}
	mlx_put_image_to_window(data->mlx, data->win, data->frame.img, 0, 0);
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

int render(t_data *data)
{
	// clear_image(data);
	move_player(data);
	draw_background(data);
	raycast(data);

	return (0);
}

int exit_game(t_data *data)
{
	(void)data; // temp
	// add cleanup functions?
	exit(0);
}

int main(int argc, char **argv)
{
	t_data	data;

  /* LUL parts to do */
  //TODO: check for argument
	if (argc < 2)
		return (error_msg("Error\nWrong argument\n", 1));
  //TODO: .cub file format parsing
	if (!is_cub_file(argv[1]))
		return (error_msg("Error\nNeed to be .cub file\n", 1)); //can be error
	//TODO: Map parsing
	ft_bzero(&data, sizeof(data));
	init_data(&data); //can use ft_bzero to initiliaze data
	if (map_read(&data, argv[1]) != 0)
		return (1);
	// print_map(data.maps);

	init_player(&data);
	// for player movement
	mlx_hook(data.win, 2, 1L<<0, key_press, &data);
	mlx_hook(data.win, 3, 1L<<1, key_release, &data);

	// for the red 'x' button on the window
	mlx_hook(data.win, 17, 0, exit_game, &data);

	init_textures(&data);

	mlx_loop_hook(data.mlx, render, &data);
	mlx_loop(data.mlx);

	return (0);
}
