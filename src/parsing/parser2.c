/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:18:23 by muhabin-          #+#    #+#             */
/*   Updated: 2025/09/18 11:50:08 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	print_map(char **map)
{
	int i = 0;
	while (map[i])
	{
		printf("%s\n", map[i]);
		i++;
	}
	// printf("\n");
}

static int	find_map_start(t_data *data)
{
	int	i;

	i = 0;
	while (data->map_info.file[i])
	{
		if (is_map_line(data->map_info.file[i]))
			break ;
		i++;
	}
	return (i);
}

static void	fill_map(char **only_map, t_data *data, int start)
{
	int	i;
	int	j;

	i = start;
	j = 0;
	while (data->map_info.file[i])
	{
		if (!empty_line(data->map_info.file[i]))
		{
			only_map[j] = ft_strdup(data->map_info.file[i]);
			if (!only_map[j])
				exit_error("strdup failed");
			j++;
		}
		i++;
	}
	only_map[j] = NULL;
	if (j == 0)
		exit_error("Empty Map");
}

static char	**extract_map(t_data *data, int start)
{
	char	**only_map;
	int		i;

	i = start;
	while (data->map_info.file[i])
		i++;
	only_map = ft_calloc(i - start + 1, sizeof(char *));
	if (!only_map)
		exit_error("Malloc failed");
	fill_map(only_map, data, start);
	return (only_map);
}

void	main_parse(char **map)
{
	int	i;
	int	j;
	int	only_space;

	i = -1;
	while (map[++i])
	{
		j = 0;
		only_space = 1;
		while (map[i][j])
		{
			if (map[i][j] != ' ' && map[i][j] != '\t' && map[i][j] != '\n')
			{
				only_space = 0;
				break ;
			}
			j++;
		}
		if (only_space)
		{
			printf("empty line at row %d: '%s'\n", i, map[i]);
			exit_error("invalid map here");
		}
	}
}

void	parse_map(t_data *data)
{
	char	**map_copy;
	char	**only_map;
	int		h;
	int		w;
	int		start;

	start = find_map_start(data);
	only_map = extract_map(data, start);
	h = map_height(only_map);
	w = get_len(only_map);
	if (h < 3 || w < 3)
		exit_error("Map too small");
	main_parse(only_map);
	valid_char(only_map);
	find_player(data, only_map);
	pad_map(only_map);
	map_copy = copy_map(only_map);
	flood_fill(data, map_copy,
		data->player.player_x, data->player.player_y);
	data->maps = only_map;
	free_array(map_copy);
}
