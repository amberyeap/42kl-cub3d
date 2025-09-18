#include "../inc/cub3d.h"

void init_keys(t_key *keys)
{
	keys->w = false;
	keys->a = false;
	keys->s = false;
	keys->d = false;
	keys->left = false;
	keys->right = false;
}

void update_direction(t_player *player)
{
	player->dir_x = cos(player->angle);
	player->dir_y = sin(player->angle);
	player->plane_x = cos(player->angle + M_PI_2) * 0.66;
	player->plane_y = sin(player->angle + M_PI_2) * 0.66;
}

void move(double new_x, double new_y, t_data *data)
{
	if (data->maps[(int)data->player.player_y][(int)new_x] == '0')
		data->player.player_x = new_x;
	if (data->maps[(int)new_y][(int)data->player.player_x] == '0')
		data->player.player_y = new_y;

}

void move_player(t_data *data)
{
	double move_speed = 0.05;
	double rot_speed = 0.02;

	// move forward
	if (data->keys.w == true)
	{
		double new_x = data->player.player_x + cos(data->player.angle) * move_speed;
		double new_y = data->player.player_y + sin(data->player.angle) * move_speed;

		move(new_x, new_y, data);
	}

	// move backward
	if (data->keys.s == true)
	{
		double new_x = data->player.player_x - cos(data->player.angle) * move_speed;
		double new_y = data->player.player_y - sin(data->player.angle) * move_speed;

		move(new_x, new_y, data);
	}

	// strafe right (A)
	if (data->keys.a == true)
	{
		double strafe_x = sin(data->player.angle);
		double strafe_y = -cos(data->player.angle);

		double new_x = data->player.player_x + strafe_x * move_speed;
		double new_y = data->player.player_y + strafe_y * move_speed;

		move(new_x, new_y, data);
	}

	// strafe left (D)
	if (data->keys.d == true)
	{
		double strafe_x = -sin(data->player.angle);
		double strafe_y = cos(data->player.angle);

		double new_x = data->player.player_x + strafe_x * move_speed;
		double new_y = data->player.player_y + strafe_y * move_speed;

		move(new_x, new_y, data);
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

int key_press(int keycode, t_data *data)
{
	// printf("Key pressed: %d\n", keycode);

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
