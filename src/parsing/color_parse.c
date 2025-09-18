/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parse2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:18:45 by muhabin-          #+#    #+#             */
/*   Updated: 2025/09/17 16:16:18 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	save_color(t_data *data, char **rgb, int mode)
{
	int				r;
	int				g;
	int				b;
	unsigned int	color;

	r = ft_atoi(rgb[0]);
	g = ft_atoi(rgb[1]);
	b = ft_atoi(rgb[2]);
	color = (r << 16 | g << 8 | b);
	if (mode == FLOOR && data->map_info.floor == -1)
		data->map_info.floor = color;
	else if (mode == CEILING && data->map_info.ceiling == -1)
		data->map_info.ceiling = color;
	else
		exit_error("Duping color failed");
}

static int	color_component(char *rgb)
{
	char	*trimmed;
	int		value;
	int		j;

	trimmed = ft_strtrim(rgb, " \t\n\r");
	if (!trimmed)
		return (0);
	j = 0;
	while (trimmed[j])
	{
		if (!ft_isdigit(trimmed[j]))
		{
			free(trimmed);
			return (0);
		}
		j++;
	}
	value = ft_atoi(trimmed);
	free(trimmed);
	if (value < 0 || value > 255)
		return (0);
	return (1);
}

int	valid_color(char **rgb)
{
	int	i;

	if (!rgb[0] || !rgb[1] || !rgb[2] || rgb[3])
		return (0);
	i = 0;
	while (i < 3)
	{
		if (!color_component(rgb[i]))
			return (0);
		i++;
	}
	return (1);
}
