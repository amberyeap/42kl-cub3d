/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:34:49 by muhabin-          #+#    #+#             */
/*   Updated: 2025/09/18 11:48:28 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static int	everything_good(t_data *data)
{
	if (!data->map_info.east || !data->map_info.south || !data->map_info.west
		|| !data->map_info.north || data->map_info.floor == -1
		|| data->map_info.ceiling == -1)
		return (1);
	return (0);
}

static char	**reading_map(int fd, int line_cub, char *line)
{
	char	**map_lines;
	int		i;

	i = 0;
	map_lines = ft_calloc(line_cub + 1, sizeof(char *));
	if (!map_lines)
		exit_error("Malloc Failed");
	while (line)
	{
		map_lines[i] = ft_strdup(line);
		if (!map_lines[i])
			exit_error("Failed to Dup Map line");
		free(line);
		line = get_next_line(fd);
		i++;
	}
	map_lines[i] = NULL;
	return (map_lines);
}

static void	skip_config(t_data *data, char **line)
{
	int	j;

	while (*line)
	{
		j = 0;
		while (ft_isspace((*line)[j]))
			j++;
		parsing_config(data, *line, j);
		if (data->map_start)
			break ;
		free(*line);
		*line = get_next_line(data->map_info.fd);
	}
}

int	readmap(t_data *data)
{
	char	*line;

	line = get_next_line(data->map_info.fd);
	skip_config(data, &line);
	data->map_info.file = reading_map(data->map_info.fd,
			data->map_info.line_cub, line);
	if (everything_good(data))
		exit_error("Wrong Configuration");
	parse_map(data);
	return (0);
}

int	map_read(t_data *data, char *argv)
{
	data->map_info.line_cub = count_line(argv);
	data->map_info.fd = open(argv, O_RDONLY);
	if (data->map_info.fd < 0)
		exit_error("Failed to Open Fd");
	readmap(data);
	close(data->map_info.fd);
	return (0);
}
