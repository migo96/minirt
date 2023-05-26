/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_cam_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:44:09 by dmin              #+#    #+#             */
/*   Updated: 2023/05/24 13:57:35 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec	z_direction(t_cam *cam, t_vec z_axis)
{
	t_vec	horizontal;
	t_vec	vertical;
	double	t;

	horizontal = make_vec(1, 0, 0);
	t = sqrt(pow(cam->fov, 2) / dot(horizontal, horizontal));
	horizontal = v_mul_n(horizontal, t);
	cam->hor = horizontal;
	vertical = make_vec(0, 1, 0);
	t = sqrt(pow(cam->fov, 2) / dot(vertical, vertical));
	vertical = v_mul_n(vertical, t * HEIGHT / WIDTH);
	cam->ver = vertical;
	return (v_sub(cam->loc, v_add(v_add(v_div_n(horizontal, 2),
					v_div_n(vertical, 2)), z_axis)));
}

t_vec	set_lower_left_corner(t_cam *cam)
{
	t_vec	z;
	t_vec	z_axis;
	t_vec	horizontal;
	t_vec	vertical;
	double	t;

	z = make_vec(0, 0, 1);
	z_axis = make_vec(-cam->vec.x, \
			-cam->vec.y, -cam->vec.z);
	horizontal = cross(cam->vec, z);
	t = sqrt(pow(cam->fov, 2) / dot(horizontal, horizontal));
	horizontal = v_mul_n(horizontal, t * WIDTH / HEIGHT);
	cam->hor = horizontal;
	vertical = cross(horizontal, cam->vec);
	t = sqrt(pow(cam->fov, 2) / dot(vertical, vertical));
	vertical = v_mul_n(vertical, t);
	cam->ver = vertical;
	if (z_axis.x == 0 && z_axis.y == 0)
		return (z_direction(cam, z_axis));
	return (v_sub(cam->loc, v_add(v_add(v_div_n(horizontal, 2),
					v_div_n(vertical, 2)), z_axis)));
}
