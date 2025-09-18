/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:04:47 by muhabin-          #+#    #+#             */
/*   Updated: 2025/09/17 14:46:18 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	color_check(t_data *data, char *line, int mode)
{
	int		i;
	char	*trim;
	char	**rgb;

	i = 0;
	while (ft_isspace(line[i]))
		i++;
	trim = ft_strdup(line + i);
	if (!trim)
		exit_error("failed to copy");
	rgb = ft_split(trim, ',');
	if (!valid_color(rgb))
		exit_error("wrong color");
	save_color(data, rgb, mode);
	free(trim);
	free_array(rgb);
}

void	texture_check(t_data *data, char *line, int dir)
{
	if (dir == NORTH && !data->map_info.north)
		data->map_info.north = line;
	else if (dir == SOUTH && !data->map_info.south)
		data->map_info.south = line;
	else if (dir == WEST && !data->map_info.west)
		data->map_info.west = line;
	else if (dir == EAST && !data->map_info.east)
		data->map_info.east = line;
	else
		exit_error("Duplicating failed");
	can_open_file(line, 0);
}

char	*get_path(char *line, int mode)
{
	char	*trim;
	int		i;

	i = 0;
	while (ft_isspace(line[i]))
		i++;
	trim = ft_strtrim(line + i, " \n\t\r");
	if (!trim)
		exit_error("trimmed failed");
	if (mode == TEX)
	{
		if (ft_strlen(trim) < 4 || !ft_strnstr(trim
				+ ft_strlen(trim) - 4, ".xpm", 4))
			exit_error("Invalid texture file");
	}
	return (trim);
}

void	parsing_config(t_data *data, char *line, int i)
{
	if (ft_strncmp(line + i, "NO", 2) == 0)
		texture_check(data, get_path(line + i + 2, TEX), NORTH);
	else if (ft_strncmp(line + i, "SO", 2) == 0)
		texture_check(data, get_path(line + i + 2, TEX), SOUTH);
	else if (ft_strncmp(line + i, "WE", 2) == 0)
		texture_check(data, get_path(line + i + 2, TEX), WEST);
	else if (ft_strncmp(line + i, "EA", 2) == 0)
		texture_check(data, get_path(line + i + 2, TEX), EAST);
	else if (ft_strncmp(line + i, "F", 1) == 0)
		color_check(data, line + i + 1, FLOOR);
	else if (ft_strncmp(line + i, "C", 1) == 0)
		color_check(data, line + i + 1, CEILING);
	else if (line[i] == '1' || line[i] == '0')
		data->map_start = 1;
}
