/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeap <ayeap@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 16:42:50 by ayeap             #+#    #+#             */
/*   Updated: 2025/07/25 16:42:50 by ayeap            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void init_keys(t_key *keys)
{
	keys->w = false;
	keys->a = false;
	keys->s = false;
	keys->d = false;
	keys->left = false;
	keys->right = false;
}

int	error_msg(char *msg, int status)
{
	ft_fprintf(2, "%s", msg);
	return (status);
}

int	is_cub_file(char *argv)
{
	int	len;

	if (!argv)
		return (0);
	len = ft_strlen(argv);
	if (len < 4)
		return (0);
	return (ft_strcmp(argv + len - 4, ".cub") == 0);
}

//checks if line is empty
int	empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

int	is_map_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '1' || line[i] == '0')
			return (1);
		i++;
	}
	return (0);
}

void	print_map(char **map)
{
	int i = 0;
	while (map[i])
	{
		printf("%s", map[i]);
		i++;
	}
}

void	flood_fill(t_data *data, char **map, int x, int y)
{
	if (x < 0 || y < 0 || !map[x] || y >= ft_strlen(map[x]))// unclosed map
		exit_error("Invalid: Map is enclosed");
	if (map[x][y] == '1' || map[x][y] == 'F')//check is wall visited
		return ;
	if (map[x][y] == ' ')
		exit_error("Invalid: Map is enclosed"); //if a space
	map[x][y] = 'F'; //mark the visited place
	flood_fill(data, map, x + 1, y); //down
	flood_fill(data, map, x - 1, y); //up
	flood_fill(data, map, x, y + 1); //right
	flood_fill(data, map, x, y - 1); //left
}

void	count_check(int count)
{
	if (count == 0)
		exit_error("Error: No player detected");
	if (count > 1)
		exit_error("Error: Multiple Player");
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
			newline[j++] = '0';
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
				data->player.player_x = i;
				data->player.player_y = j;
				data->player.p_direction = map[i][j];
				count++;
			}
			// if (map[i][j] != '1')
			// 	map[i][j] = '0';
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
			printf("what is it: %c\n",map[i][j]);
			c = map[i][j];
			if (c != '0' && c != '1' && c != 'N' && c != 'S'
				&& c != 'E' && c != 'W' && c != ' ')
				exit_error("Error Invalid Characters");
			j++;
		}
		i++;
	}
}

void	main_parse(char **map)
{
	int	i;
	int	j;
	int	only_space;

	i = 0;
	while (map[i])
	{
		j = 0;
		only_space = 1; // assume that the line is all space first
		while (map[i][j])
		{
			if (map[i][j] != ' ' && map[i][j] != '\t' && map[i][j] != '\n')
			{
				only_space = 0; //found a character means the whole line tak semua space
				break;
			}
			j++;
		}
		if (only_space)
		{
			printf("empty line at row %d: '%s'\n", i, map[i]);
			exit_error("invalid map here");
		}
		i++;
	}
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

//can be in the utils
void	free_array(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	main_parse2(t_data *data)
{
	char	**map_copy;
	char	**only_map;
	int		i = 0, j = 0;
	int		start;

	// 🔍 Find the first map line index
	while (data->map_info.file[i])
	{
		if (is_map_line(data->map_info.file[i])) // check if line starts with 1 or 0
			break;
		i++;
	}
	start = i;

	// 🧼 Copy only map lines from data->map_info.file
	while (data->map_info.file[i])
		i++;
	only_map = ft_calloc(i - start + 1, sizeof(char *));
	if (!only_map)
		exit_error("malloc failed");
	i = start;
	while (data->map_info.file[i])
	{
		if (!empty_line(data->map_info.file[i]))
		{
			only_map[j] = ft_strdup(data->map_info.file[i]);
			if (!only_map[j])
				exit_error("strdup failed");
			printf("MAP[%d] = '%s'\n", j, only_map[j]);  // ✅ Now safe
			j++;
		}
		i++;
	}
	only_map[j] = NULL;

	// ✅ Validate the clean map
	main_parse(only_map);
	valid_char(only_map);
	find_player(data, only_map);
	pad_map(only_map);
	map_copy = copy_map(only_map);
	flood_fill(data, map_copy, data->player.player_x, data->player.player_y);
	print_map(map_copy);
	free_array(map_copy);
	free_array(only_map);  // ❗ better use free_array to avoid leaks
}

//Check for if all configuration is stores
int	everything_good(t_data *data)
{
	if (!data->map_info.east || !data->map_info.south || !data->map_info.west
		|| !data->map_info.north || data->map_info.floor == -1
		|| data->map_info.ceiling == -1)
		return (1);
	return (0);
}

void	save_color(t_data * data,char **rgb, int mode)
{
	int	r;
	int	g;
	int	b;
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
		exit_error("duping color failed");
}

int	valid_color(char **rgb)
{
	int	i;
	int	j;
	int	value;

	if (!rgb[0] || !rgb[1] || !rgb[2] || rgb[3])
		return (0);
	i = 0;
	while(i < 3)
	{
		j = 0;
		while (rgb[i][j])
		{
			if (!ft_isdigit(rgb[i][j]))
				return (0);
			j++;
		}
		value = ft_atoi(rgb[i]);
		if (value < 0 || value > 255)
			return (0);
		i++;
	}
	return (1);
}

//Check for valid color
void	color_check(t_data *data, char *line, int mode)
{
	int		i;
	// int		j;
	char	*trim;
	char	**rgb;

	i = 0;
	// j = 0;
	while (ft_isspace(line[i]))
		i++;
	trim = ft_strdup(line + i);
	if (!trim)
		exit_error("failed to copy");
	rgb = ft_split(trim, ',');
	if (valid_color(rgb))
		exit_error("wrong color");
	save_color(data, rgb, mode);
	free(trim);
	free_array(rgb);
}

//check if the map files can be open
void	can_open_file(char *file, int fd)
{
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		printf("%d\n", fd);
		// printf("%s\n", file);
		exit_error("File cannot be open");
	}
	close(fd);
}

//Check the texture data and stores its in struct
void	texture_check(t_data *data, char *line, int dir)
{
	//need to add check if the texture file can be open
	// line += 3;
	// printf("opening: '%s'\n", line);
	if (dir == NORTH && !data->map_info.north)
		data->map_info.north = line;
		// data->map_info.north = ft_strdup(line);
	else if (dir == SOUTH && !data->map_info.south)
		data->map_info.south = line;
		// data->map_info.south = ft_strdup(line);
	else if (dir == WEST && !data->map_info.west)
		data->map_info.west = line;
		// data->map_info.west = ft_strdup(line);
	else if (dir == EAST && !data->map_info.east)
		data->map_info.east = line;
		// data->map_info.east = ft_strdup(line);
	else
		exit_error("Duplicating failed");
	can_open_file(line, 0);
}

// Checking the map files is in .xpm
char	*get_path(char *line, int mode)
{
	char	*path;
	char	*trim;
	int		i;

	i = 0;
	while (ft_isspace(line[i]))
		i++;
	path = ft_strdup(line + i);
	if (!path)
		exit_error("Malloc failed");
	trim = ft_strtrim(path, " \n\t\r");
	free(path);
	if (!trim)
		exit_error("trimmed failed");
	printf("trim: %s\n", trim);
	if (mode == TEX)
	{
		if (ft_strlen(trim) < 4 || !ft_strnstr(trim + ft_strlen(trim) - 4, ".xpm", 4))
			exit_error("Invalid texture file");
	}
	return (trim);
}

// Parsing Configuration
void	parsing_config(t_data *data, char *line, int i)
{
	// printf("testing %s\n", get_path(line + i, TEX));
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
	// else if (line[i] != '\0')
	// 	exit_error("Invalid params");
}

int	readmap(t_data *data)
{
	char	*line;
	char	**map_lines;
	int		i = 0;

	map_lines = ft_calloc(data->map_info.line_cub + 1, sizeof(char *));
	if (!map_lines)
		exit_error("malloc failed");
	
	line = get_next_line(data->map_info.fd);
	printf("line here is %s\n", line);
	// to process the texture files
	// could there be an endless loop here? FIXED - missing a get_next_line within the loop to move to next line
	while (line)
	{
		int j = 0;
		while (ft_isspace(line[j]))
			j++;
		parsing_config(data, line, j);
		printf("before map: %s\n", line);
		printf("start of map: %i\n",data->map_start);
		if (data->map_start)  // map begins
		{
			printf("why break my heart?\n");
			free(line);
			break;
		}
		printf("line here before free is %s\n", line);
		free(line);
		line = get_next_line(data->map_info.fd);
	}
	// line = get_next_line(data->map_info.fd);
	// printf("next line here is %s\n", line);

	// Then: read only the map lines
	while ((line = get_next_line(data->map_info.fd)))
	{
		printf("in a map:%s \n",line);
		map_lines[i] = ft_strdup(line);
		if (!map_lines[i])
			exit_error("failed to strdup map line");
		free(line);
		i++;
	}
	map_lines[i] = NULL;

	if (everything_good(data))
		exit_error("Wrong Configuration");

	data->map_info.file = map_lines;
	main_parse2(data);  // parse the collected map
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

//main map function
int	map_read(t_data *data, char *argv)
{
	//TODO: using gnl to take the .cub so can do parsing map
	//count line of map so can malloc later
	data->map_info.line_cub = count_line(argv);
	//need to save the path name/file name may not needed
	// need to malloc so can create a space for the map to be copied
	data->map_info.fd = open(argv, O_RDONLY);
	if (data->map_info.fd < 0)
		return (error_msg("failed to open",2));
	readmap(data);
	close(data->map_info.fd);
	return (0);
}

void	exit_error(const char *msg)
{
	printf("Error\n%s\n", msg);
	exit(EXIT_FAILURE);
}

void init_textures(t_data *data)
{
	// data->map_info.north = "assets/sprites/north_wall.xpm";
	// data->map_info.south = "assets/sprites/south_wall.xpm";
	// data->map_info.east = "assets/sprites/east_wall.xpm";
	// data->map_info.west = "assets/sprites/west_wall.xpm";

	data->textures[NORTH].img = mlx_xpm_file_to_image(data->mlx, data->map_info.north, &data->textures[NORTH].width, &data->textures[NORTH].height);
	data->textures[SOUTH].img = mlx_xpm_file_to_image(data->mlx, data->map_info.south, &data->textures[SOUTH].width, &data->textures[SOUTH].height);
	data->textures[EAST].img = mlx_xpm_file_to_image(data->mlx, data->map_info.east, &data->textures[EAST].width, &data->textures[EAST].height);
	data->textures[WEST].img = mlx_xpm_file_to_image(data->mlx, data->map_info.west, &data->textures[WEST].width, &data->textures[WEST].height);

	int i = 0;
	while (i < 4)
	{
		if (!data->textures[i].img)
		{
			printf("Failed to load image in: %d\n", i);
			exit(1);
		}
		i++;
	}

	i = 0;
	while (i < 4)
	{
		data->textures[i].addr = mlx_get_data_addr(data->textures[i].img, &data->textures[i].bpp, &data->textures[i].line_len, &data->textures[i].endian);
		i++;
	}
}

void init_frame(t_data *data)
{
	data->frame.img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->frame.addr = mlx_get_data_addr(data->frame.img, &data->frame.bpp, &data->frame.line_len, &data->frame.endian);
	data->frame.width = WIDTH;
	data->frame.height = HEIGHT;
}

void	init_data(t_data *data)
{
	data->map_start = 0;
	data->map_info.ceiling = -1;
	data->map_info.floor = -1;

	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "cub3d");
	// data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	// data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_len, &data->endian);
	init_frame(data);
	init_keys(&data->keys);
}

int get_pixel(t_img *tex, int x, int y)
{
	char *dest = tex->addr + (y * tex->line_len + x * (tex->bpp / 8));
	return *(unsigned int *)dest;
}

void put_pixel(t_img *img, int x, int y, int colour)
{
	char *dest = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)dest = colour;
}

void init_ray(t_ray *ray, int x, t_data *data)
{
	double camera_x = 2 * x / (double)WIDTH - 1;
	ray->ray_dir_x = data->player.dir_x + data->player.plane_x * camera_x;
	ray->ray_dir_y = data->player.dir_y + data->player.plane_y * camera_x;
	ray->delta_dist_x = (ray->ray_dir_x == 0) ? 1e30 : fabs(1 / ray->ray_dir_x);
	ray->delta_dist_y = (ray->ray_dir_y == 0) ? 1e30 : fabs(1 / ray->ray_dir_y);
	ray->map_x = (int)data->player.player_x;
	ray->map_y = (int)data->player.player_y;
	ray->hit = 0;
}

void count_step(t_ray *ray, t_data *data)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (data->player.player_x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - data->player.player_x) * ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (data->player.player_y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - data->player.player_y) * ray->delta_dist_y;
	}
}

void perform_dda(t_ray *ray, t_data *data)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (data->maps[ray->map_x][ray->map_y] > 0)
			ray->hit = 1;
	}
}

void count_perp_distance(t_ray *ray)
{
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->side_dist_x - ray->delta_dist_x);
	else
		ray->perp_wall_dist = (ray->side_dist_y - ray->delta_dist_y);
}

void draw_walls(t_ray *ray, int x, t_data *data)
{
	int line_height = (int)(HEIGHT / ray->perp_wall_dist);
	int start = -line_height / 2 + HEIGHT / 2;
	if (start < 0)
		start = 0;
	int end = line_height / 2 + HEIGHT / 2;
	if (end >= HEIGHT)
		end = HEIGHT - 1;

	// int colour;
	// if (ray->side == 1)
	// 	colour = RED;
	// else
	// 	colour = DARK_RED;

	// int y = start;
	// while (y++ < end)
	// 	my_pixel_put(data, x, y, colour);

	// t_img *tex;
	// if (ray->side == 0 && ray->ray_dir_x < 0)
	// 	tex = &data->textures[2]; // west
	// if (ray->side == 0 && ray->ray_dir_x > 0)
	// 	tex = &data->textures[3]; // east
	// if (ray->side == 1 && ray->ray_dir_y < 0)
	// 	tex = &data->textures[0]; // north
	// if (ray->side == 1 && ray->ray_dir_y > 0)
	// 	tex = &data->textures[1]; // south

	t_img *tex = &data->textures[NORTH]; 
    if (ray->side == 0)
    {
        if (ray->ray_dir_x < 0) 
            tex = &data->textures[WEST];
        else 
            tex = &data->textures[EAST];
    }
    else
    {
        if (ray->ray_dir_y < 0) 
            tex = &data->textures[NORTH];
        else 
            tex = &data->textures[SOUTH];
    }

	// int tex_num = data->maps[ray->map_x][ray->map_y];
	double wallX;
	if (ray->side == 0)
		wallX = data->player.player_y + ray->perp_wall_dist * ray->ray_dir_y;
	else
		wallX = data->player.player_x + ray->perp_wall_dist * ray->ray_dir_x;
	wallX -= floor(wallX);

	int texX = (int)(wallX * tex->width);
	if (ray->side == 0 && ray->ray_dir_x > 0)
		texX = tex->width - texX - 1;
	if (ray->side == 1 && ray->ray_dir_y < 0)
		texX = tex->width - texX - 1;

	double step = 1.0 * tex->height / line_height;
	double tex_pos = (start - HEIGHT / 2 + line_height / 2) * step;

	int y = start;
	while (y < end)
	{
		int tex_y = (int)tex_pos;
        if (tex_y < 0)
			tex_y = 0;
        if (tex_y >= tex->height)
			tex_y = tex->height - 1;
        tex_pos += step;

		int colour = get_pixel(tex, texX, tex_y);
		put_pixel(&data->frame, x, y, colour);
		y++;
	}
}

void draw_background(t_data *data)
{
    int x, y;

    for (y = 0; y < HEIGHT / 2; y++)
    {
        for (x = 0; x < WIDTH; x++)
            put_pixel(&data->frame, x, y, RED);
    }

    for (y = HEIGHT / 2; y < HEIGHT; y++)
    {
        for (x = 0; x < WIDTH; x++)
            put_pixel(&data->frame, x, y, DARK_RED);
    }
}

void raycast(t_data *data)
{
	t_ray ray;
	int x = 0;

	while (x < WIDTH)
	{
		init_ray(&ray, x, data);
		count_step(&ray, data);
		perform_dda(&ray, data);
		count_perp_distance(&ray);
		draw_walls(&ray, x, data);
		x++;
	}
}

void clear_image(t_data *data)
{
	int x = 0;
	int y;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			put_pixel(&data->frame, x, y, 0);
			y++;
		}
		x++;
	}
}

void update_direction(t_player *player)
{
	player->dir_x = cos(player->angle);
	player->dir_y = sin(player->angle);
	player->plane_x = cos(player->angle + M_PI_2) * 0.66;
	player->plane_y = sin(player->angle + M_PI_2) * 0.66;
}

void move_player(t_data *data)
{
	double move_speed = 0.03;
	double rot_speed = 0.02;

	// move forward
	if (data->keys.w == true)
	{
		double new_x = data->player.player_x + cos(data->player.angle) * move_speed;
		double new_y = data->player.player_y + sin(data->player.angle) * move_speed;

		if (data->maps[(int)new_x][(int)(data->player.player_y)] == 0)
			data->player.player_x = new_x;
		if (data->maps[(int)(data->player.player_x)][(int)new_y] == 0)
			data->player.player_y = new_y;
	}

	// move backward
	if (data->keys.s == true)
	{
		double new_x = data->player.player_x - cos(data->player.angle) * move_speed;
		double new_y = data->player.player_y - sin(data->player.angle) * move_speed;

		if (data->maps[(int)new_x][(int)(data->player.player_y)] == 0)
			data->player.player_x = new_x;
		if (data->maps[(int)(data->player.player_x)][(int)new_y] == 0)
			data->player.player_y = new_y;
	}

	// strafe right (D)
	if (data->keys.a == true)
	{
		double strafe_x = sin(data->player.angle);
		double strafe_y = -cos(data->player.angle);

		double new_x = data->player.player_x + strafe_x * move_speed;
		double new_y = data->player.player_y + strafe_y * move_speed;

		if (data->maps[(int)new_x][(int)(data->player.player_y)] == 0)
			data->player.player_x = new_x;
		if (data->maps[(int)(data->player.player_x)][(int)new_y] == 0)
			data->player.player_y = new_y;
	}

	// strafe left (A)
	if (data->keys.d == true)
	{
		double strafe_x = -sin(data->player.angle);
		double strafe_y = cos(data->player.angle);

		double new_x = data->player.player_x + strafe_x * move_speed;
		double new_y = data->player.player_y + strafe_y * move_speed;

		if (data->maps[(int)new_x][(int)(data->player.player_y)] == 0)
			data->player.player_x = new_x;
		if (data->maps[(int)(data->player.player_x)][(int)new_y] == 0)
			data->player.player_y = new_y;
	}

	// rotate to right
	if (data->keys.right == true)
	{
		data->player.angle += rot_speed;
		update_direction(&data->player);
	}

	// rotate to left
	if (data->keys.left == true)
	{
		data->player.angle -= rot_speed;
		update_direction(&data->player);
	}
}

int render(t_data *data)
{
	// clear_image(data);
	draw_background(data);
	raycast(data);
	move_player(data);
	mlx_put_image_to_window(data->mlx, data->win, data->frame.img, 0, 0);

	return (0);
}

int key_press(int keycode, t_data *data)
{
	if (keycode == ESC)
		exit(0);
	if (keycode == W)
		data->keys.w = true;
	if (keycode == A)
		data->keys.a = true;
	if (keycode == S)
		data->keys.s = true;
	if (keycode == D)
		data->keys.d = true;
	if (keycode == LEFT)
		data->keys.left = true;
	if (keycode == RIGHT)
		data->keys.right = true;
	return (1);
}

int key_release(int keycode, t_data *data)
{
	if (keycode == W)
		data->keys.w = false;
	if (keycode == A)
		data->keys.a = false;
	if (keycode == S)
		data->keys.s = false;
	if (keycode == D)
		data->keys.d = false;
	if (keycode == LEFT)
		data->keys.left = false;
	if (keycode == RIGHT)
		data->keys.right = false;
	return (1);
}

int exit_game(t_data *data)
{
	(void)data; // temp
	// add cleanup functions?
	exit(0);
}

int main(int argc, char **argv)
{
	t_data	data;

  /* LUL parts to do */
  //TODO: check for argument
	if (argc < 2)
		return (error_msg("Error\nWrong argument\n", 1));
  //TODO: .cub file format parsing
	if (!is_cub_file(argv[1]))
		return (error_msg("Error\nNeed to be .cub file\n", 1)); //can be error
	//TODO: Map parsing
	init_data(&data); //can use ft_bzero to initiliaze data
	if (map_read(&data, argv[1]) != 0)
		return (1);

	// for player movement
	mlx_hook(data.win, 2, 1L<<0, key_press, &data);
	mlx_hook(data.win, 3, 1L<<1, key_release, &data);

	// for the red 'x' button on the window
	mlx_hook(data.win, 17, 0, exit_game, &data);

	init_textures(&data);

	mlx_loop_hook(data.mlx, render, &data);
	mlx_loop(data.mlx);

	return (0);
}
