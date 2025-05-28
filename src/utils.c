/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhabin- <muhabin-@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:29:30 by muhabin-          #+#    #+#             */
/*   Updated: 2025/05/26 14:29:46 by muhabin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

