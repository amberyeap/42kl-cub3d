/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 19:32:01 by muhabin-          #+#    #+#             */
/*   Updated: 2025/09/18 11:54:40 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	flood_fill(t_data *data, char **map, int x, int y)
{
	if (x < 0 || y < 0 || !map[x] || y >= ft_strlen(map[x]))
		exit_error("Invalid 1: Map is enclosed");
	if (map[x][y] == '1' || map[x][y] == 'F')
		return ;
	if (map[x][y] == ' ')
		exit_error("Invalid 2: Map is enclosed");
	map[x][y] = 'F';
	flood_fill(data, map, x + 1, y);
	flood_fill(data, map, x - 1, y);
	flood_fill(data, map, x, y + 1);
	flood_fill(data, map, x, y - 1);
}

char	**copy_map(char **map)
{
	char	**copy;
	int		lines;
	int		i;

	i = 0;
	lines = 0;
	while (map[lines])
		lines++;
	copy = ft_calloc(lines + 1, sizeof(char *));
	if (!copy)
		exit_error("failed to malloc");
	while (map[i])
	{
		copy[i] = ft_strdup(map[i]);
		if (!copy[i])
			exit_error("bro failed to make copy map");
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

void	pad_map(char **map)
{
	int		i;
	int		j;
	int		max_len;
	char	*newline;

	i = -1;
	max_len = get_len(map);
	while (map[++i])
	{
		newline = ft_calloc(max_len + 1, sizeof(char));
		j = 0;
		while (map[i][j])
		{
			newline[j] = map[i][j];
			j++;
		}
		while (j < max_len)
			newline[j++] = ' ';
		free(map[i]);
		map[i] = newline;
	}
}

void	find_player(t_data *data, char **map)
{
	int	i;
	int	j;
	int	count;

	i = -1;
	count = 0;
	while (map[++i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == 'N' || map[i][j] == 'S'
				|| map[i][j] == 'E' || map[i][j] == 'W')
			{
				data->player.player_x = j;
				data->player.player_y = i;
				data->player.p_direction = map[i][j];
				map[i][j] = '0';
				count++;
			}
			j++;
		}
	}
	count_check(count);
}

void	valid_char(char **map)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			c = map[i][j];
			if (c != '0' && c != '1' && c != 'N' && c != 'S'
				&& c != 'E' && c != 'W' && c != ' ')
			{
				if (ft_isspace(c))
					break ;
				exit_error("Error Invalid Characters");
			}
			j++;
		}
		i++;
	}
}
