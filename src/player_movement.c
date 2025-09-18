/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:20:19 by ayeap             #+#    #+#             */
/*   Updated: 2025/09/18 14:29:56 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	rotate(t_data *data, double rot_speed)
{
	if (data->keys.right == true)
	{
		data->player.angle += rot_speed;
		update_direction(&data->player);
	}
	if (data->keys.left == true)
	{
		data->player.angle -= rot_speed;
		update_direction(&data->player);
	}
}

static void	left_right(t_data *data, double move_speed)
{
	double	strafe_x;
	double	strafe_y;
	double	new_x;
	double	new_y;

	if (data->keys.a == true)
	{
		strafe_x = sin(data->player.angle);
		strafe_y = -cos(data->player.angle);
		new_x = data->player.player_x + strafe_x * move_speed;
		new_y = data->player.player_y + strafe_y * move_speed;
		move(new_x, new_y, data);
	}
	if (data->keys.d == true)
	{
		strafe_x = -sin(data->player.angle);
		strafe_y = cos(data->player.angle);
		new_x = data->player.player_x + strafe_x * move_speed;
		new_y = data->player.player_y + strafe_y * move_speed;
		move(new_x, new_y, data);
	}
}

static void	forward_backward(t_data *data, double move_speed)
{
	double	new_x;
	double	new_y;

	if (data->keys.w == true)
	{
		new_x = data->player.player_x + cos(data->player.angle) * move_speed;
		new_y = data->player.player_y + sin(data->player.angle) * move_speed;
		move(new_x, new_y, data);
	}
	if (data->keys.s == true)
	{
		new_x = data->player.player_x - cos(data->player.angle) * move_speed;
		new_y = data->player.player_y - sin(data->player.angle) * move_speed;
		move(new_x, new_y, data);
	}
}

void	move_player(t_data *data)
{
	double	move_speed;
	double	rot_speed;

	move_speed = 0.05;
	rot_speed = 0.02;
	forward_backward(data, move_speed);
	left_right(data, move_speed);
	rotate(data, rot_speed);
}
