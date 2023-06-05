/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_utils2_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmin <dmin@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:21:59 by dmin              #+#    #+#             */
/*   Updated: 2023/06/02 16:22:01 by dmin             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

void	ft_lstclear(t_object **lst)
{
	t_object	*tmp;

	if (*lst == 0)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free ((*lst)->object);
		free(*lst);
		*lst = tmp;
	}
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

double	degrees_to_radians(double degrees)
{
	return (degrees * 3.141529 / 180.0);
}

int	gain_color(t_data *data, int x, int y)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	return (*(unsigned int *) dst);
}

void	set_fr_fl_color(char **map, t_object *ob)
{
	ob->color = make_vec(ft_atof(map), ft_atof(map), ft_atof(map));
	ob->refl = ft_atof(map);
	ob->refr = ft_atof(map);
}
