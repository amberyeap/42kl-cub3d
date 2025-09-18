/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:46:47 by ayeap             #+#    #+#             */
/*   Updated: 2025/09/18 14:16:44 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	check_textures(t_data *data)
{
	if (!data->textures[NORTH].img)
		exit_error("Failed to load north_wall.xpm");
	if (!data->textures[SOUTH].img)
		exit_error("Failed to load south_wall.xpm");
	if (!data->textures[EAST].img)
		exit_error("Failed to load east_wall.xpm");
	if (!data->textures[WEST].img)
		exit_error("Failed to load west_wall.xpm");
}

static void	load_textures(t_data *data)
{
	data->textures[NORTH].img = mlx_xpm_file_to_image(data->mlx,
			data->map_info.north, &data->textures[NORTH].width,
			&data->textures[NORTH].height);
	data->textures[SOUTH].img = mlx_xpm_file_to_image(data->mlx,
			data->map_info.south, &data->textures[SOUTH].width,
			&data->textures[SOUTH].height);
	data->textures[EAST].img = mlx_xpm_file_to_image(data->mlx,
			data->map_info.east, &data->textures[EAST].width,
			&data->textures[EAST].height);
	data->textures[WEST].img = mlx_xpm_file_to_image(data->mlx,
			data->map_info.west, &data->textures[WEST].width,
			&data->textures[WEST].height);
	check_textures(data);
}

void	init_textures(t_data *data)
{
	if (!data->map_info.north || !data->map_info.south
		|| !data->map_info.east || !data->map_info.west)
		exit_error("Texture path missing");
	load_textures(data);
	data->textures[NORTH].addr = mlx_get_data_addr(data->textures[NORTH].img,
			&data->textures[NORTH].bpp, &data->textures[NORTH].line_len,
			&data->textures[NORTH].endian);
	data->textures[SOUTH].addr = mlx_get_data_addr(data->textures[SOUTH].img,
			&data->textures[SOUTH].bpp, &data->textures[SOUTH].line_len,
			&data->textures[SOUTH].endian);
	data->textures[EAST].addr = mlx_get_data_addr(data->textures[EAST].img,
			&data->textures[EAST].bpp, &data->textures[EAST].line_len,
			&data->textures[EAST].endian);
	data->textures[WEST].addr = mlx_get_data_addr(data->textures[WEST].img,
			&data->textures[WEST].bpp, &data->textures[WEST].line_len,
			&data->textures[WEST].endian);
}
