/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:05:54 by ayeap             #+#    #+#             */
/*   Updated: 2025/09/18 14:30:34 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	update_direction(t_player *player)
{
	player->dir_x = cos(player->angle);
	player->dir_y = sin(player->angle);
	player->plane_x = cos(player->angle + M_PI_2) * 0.66;
	player->plane_y = sin(player->angle + M_PI_2) * 0.66;
}

void	move(double new_x, double new_y, t_data *data)
{
	if (data->maps[(int)data->player.player_y][(int)new_x] == '0')
		data->player.player_x = new_x;
	if (data->maps[(int)new_y][(int)data->player.player_x] == '0')
		data->player.player_y = new_y;
}

int	key_press(int keycode, t_data *data)
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

int	key_release(int keycode, t_data *data)
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
