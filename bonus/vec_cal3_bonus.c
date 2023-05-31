/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_cal3_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:10:42 by dmin              #+#    #+#             */
/*   Updated: 2023/05/31 16:47:09 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

double	length(t_vec e)
{
	return (sqrt(length_squared(e)));
}

t_vec	unit_vector(t_vec v)
{
	return (v_div_n(v, length(v)));
}

t_vec	v_add_n(t_vec v1, double n)
{
	t_vec	result;

	result.x = v1.x + n;
	result.y = v1.y + n;
	result.z = v1.z + n;
	return (result);
}

int	fl_color(t_vec color)
{
	int	color_red;
	int	color_green;
	int	color_blue;

	color_red = (int)color.x;
	color_green = (int)color.y;
	color_blue = (int)color.z;
	if (color_red > 255)
		color_red = 255;
	if (color_green > 255)
		color_green = 255;
	if (color_blue > 255)
		color_blue = 255;
	return (color_red * 65536 + color_green * 256 + color_blue);
}
