/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:51:28 by ayeap             #+#    #+#             */
/*   Updated: 2025/09/18 14:27:46 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	init_player(t_data *data)
{
	t_player	*player;

	init_keys(&data->keys);
	player = &data->player;
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
	player->dir_x = cos(player->angle);
	player->dir_y = sin(player->angle);
	player->plane_x = cos(player->angle + M_PI_2) * 0.66;
	player->plane_y = sin(player->angle + M_PI_2) * 0.66;
}

void	init_ray(t_ray *ray, int x, t_data *data)
{
	double	camera_x;

	camera_x = 2 * x / (double)WIDTH - 1;
	ray->ray_dir_x = data->player.dir_x + data->player.plane_x * camera_x;
	ray->ray_dir_y = data->player.dir_y + data->player.plane_y * camera_x;
	if (ray->ray_dir_x == 0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = fabs(1 / ray->ray_dir_x);
	if (ray->ray_dir_y == 0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = fabs(1 / ray->ray_dir_y);
	ray->map_x = (int)data->player.player_x;
	ray->map_y = (int)data->player.player_y;
	ray->hit = 0;
	ray->step_x = 0;
	ray->step_y = 0;
	ray->perp_wall_dist = 0;
}

void	init_frame(t_data *data)
{
	data->frame.img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->frame.addr = mlx_get_data_addr(data->frame.img, &data->frame.bpp,
			&data->frame.line_len, &data->frame.endian);
	data->frame.width = WIDTH;
	data->frame.height = HEIGHT;
}

void	init_data(t_data *data)
{
	data->map_start = 0;
	data->map_info.ceiling = -1;
	data->map_info.floor = -1;
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "cub3D");
	init_frame(data);
}

void	init_keys(t_key *keys)
{
	keys->w = false;
	keys->a = false;
	keys->s = false;
	keys->d = false;
	keys->left = false;
	keys->right = false;
}
