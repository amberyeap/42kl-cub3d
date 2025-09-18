/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:47:34 by ayeap             #+#    #+#             */
/*   Updated: 2025/09/18 15:31:54 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	exit_game(t_data *data)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (data->textures[i].img)
			mlx_destroy_image(data->mlx, data->textures[i].img);
		i++;
	}
	if (data->frame.img)
		mlx_destroy_image(data->mlx, data->frame.img);
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
		mlx_destroy_display(data->mlx);
	exit(0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 2)
		return (error_msg("Error\nWrong argument\n", 1));
	if (!is_cub_file(argv[1]))
		return (error_msg("Error\nNeed to be .cub file\n", 1));
	ft_bzero(&data, sizeof(data));
	init_data(&data);
	if (map_read(&data, argv[1]) != 0)
		return (1);
	init_player(&data);
	mlx_hook(data.win, 2, 1L << 0, key_press, &data);
	mlx_hook(data.win, 3, 1L << 1, key_release, &data);
	mlx_hook(data.win, 17, 0, exit_game, &data);
	init_textures(&data);
	mlx_loop_hook(data.mlx, render, &data);
	mlx_loop(data.mlx);
	exit_game(&data);
	return (0);
}
