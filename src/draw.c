/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:50:27 by ayeap             #+#    #+#             */
/*   Updated: 2025/09/18 15:30:56 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static t_img	*get_texture(t_ray *ray, t_data *data)
{
	if (ray->side == 0)
	{
		if (ray->ray_dir_x < 0)
			return (&data->textures[WEST]);
		else
			return (&data->textures[EAST]);
	}
	else
	{
		if (ray->ray_dir_y < 0)
			return (&data->textures[NORTH]);
		else
			return (&data->textures[SOUTH]);
	}
}

static void	draw_vertical_line(int x, t_draw *line, t_img *tex, t_data *data)
{
	double	step;
	double	tex_pos;
	int		y;
	int		tex_y;
	int		colour;

	step = 1.0 * tex->height / line->line_height;
	tex_pos = (line->start - HEIGHT / 2 + line->line_height / 2) * step;
	y = line->start;
	while (y < line->end)
	{
		tex_y = (int)tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= tex->height)
			tex_y = tex->height - 1;
		tex_pos += step;
		colour = get_pixel(tex, line->tex_x, tex_y);
		put_pixel(&data->frame, x, y, colour);
		y++;
	}
}

void	draw_walls(t_ray *ray, int x, t_data *data)
{
	t_img	*tex;
	t_draw	line;
	double	wall_x;

	line.line_height = (int)(HEIGHT / ray->perp_wall_dist);
	line.start = -line.line_height / 2 + HEIGHT / 2;
	if (line.start < 0)
		line.start = 0;
	line.end = line.line_height / 2 + HEIGHT / 2;
	if (line.end >= HEIGHT)
		line.end = HEIGHT - 1;
	tex = get_texture(ray, data);
	if (ray->side == 0)
		wall_x = data->player.player_y + ray->perp_wall_dist * ray->ray_dir_y;
	else
		wall_x = data->player.player_x + ray->perp_wall_dist * ray->ray_dir_x;
	wall_x -= floor(wall_x);
	line.tex_x = (int)(wall_x * tex->width);
	if (ray->side == 0 && ray->ray_dir_x > 0)
		line.tex_x = tex->width - line.tex_x - 1;
	if (ray->side == 1 && ray->ray_dir_y < 0)
		line.tex_x = tex->width - line.tex_x - 1;
	draw_vertical_line(x, &line, tex, data);
}

void	draw_background(t_data *data)
{
	int	y;
	int	x;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			if (y < HEIGHT / 2)
				put_pixel(&data->frame, x, y, data->map_info.ceiling);
			else
				put_pixel(&data->frame, x, y, data->map_info.floor);
			x++;
		}
		y++;
	}
}
