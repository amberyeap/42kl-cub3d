/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:16:18 by muhabin-          #+#    #+#             */
/*   Updated: 2025/05/28 15:04:58 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


// WILL BE REFACTOR LATER SO CAN SEPARATE THE .cub TEXTURE AND MAP 
int	readmap(t_data *data)
{
	int		i;
	char	*line;
	char	*cleanline;

	i = 0;
	line = get_next_line(data->map_info.fd);
	while (line)
	{
		cleanline = ft_strtrim(line, "\n");
		if (!cleanline)
		{
			free(line);
			return (error_msg("failed malloc", 2));
		}
		data->map_info.file[i++] = cleanline;
		free(line);
		line = get_next_line(data->map_info.fd);
	}
	data->map_info.file[i] = NULL;
	return (0);
}
int	count_line(char *map)
{
	int		count;
	int		fd;
	char	*line;

	count = 0;
	fd = open(map, O_RDONLY);
	if (fd < 0)
		return (error_msg("failed to open", 2));
	else
	{
		line = get_next_line(fd);
		while (line)
		{
			count++;
			free(line);
			line = NULL;
			line = get_next_line(fd);
		}
		close(fd);
	}
	return (count);
}
int	map_read(t_data *data, char *argv)
{
	t_map	maps;

	//TODO: using gnl to take the .cub so can do parsing map
	//count line of map so can malloc later
	maps.line_map = count_line(argv);
	//need to save the path name/file name may not needed
	// need to malloc so can create a space for the map to be copied
	maps.file = malloc(maps.line_map + 1 * sizeof(char *));
	if (!maps.file)
		return (error_msg("failed to malloc", 2));
	maps.fd = open(argv, O_RDONLY);
	if (maps.fd < 0)
		return (error_msg("failed to open",2));
	readmap(data);
	close(maps.fd);
	return (0);
}

/*
* utk map, kalau ada island tapi enclosed still consider okay,
	invalid kalau player pos bila flood fill tak enclosed map,
	boleh ada multiple island tapi player x boleh cross

*/
