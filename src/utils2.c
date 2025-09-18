/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 19:13:32 by muhabin-          #+#    #+#             */
/*   Updated: 2025/09/17 19:47:04 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

void	exit_error(const char *msg)
{
	printf("Error\n%s\n", msg);
	exit(EXIT_FAILURE);
}

int	map_height(char **m)
{
	int	h;

	h = 0;
	while (m && m[h])
		h++;
	return (h);
}

int	get_len(char **map)
{
	int	i;
	int	max;
	int	length;

	i = -1;
	max = 0;
	while (map[++i])
	{
		length = ft_strlen(map[i]);
		if (length > max)
			max = length;
	}
	return (max);
}

void	count_check(int count)
{
	if (count == 0)
		exit_error("Error: No player detected");
	if (count > 1)
		exit_error("Error: Multiple Player");
}
