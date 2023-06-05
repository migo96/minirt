/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_check_if_inside_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmin <dmin@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:14:06 by dmin              #+#    #+#             */
/*   Updated: 2023/06/02 16:14:08 by dmin             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

int	check_in_cn(t_cone *cn, t_set *set, t_vec normal)
{
	double		check;
	t_vec		center;

	check = dot(v_sub(set->cam.loc, cn->center), normal);
	if (check < cn->height)
	{
		center = v_add(cn->center, v_mul_n(normal, check));
		if (length(v_sub(center, set->cam.loc)) < 1 - cn->height)
			return (1);
	}
	return (0);
}

int	check_in_sp(t_sphere *sphere, t_set *set, t_ray contact)
{
	if (length(v_sub(set->cam.loc, contact.orig)) < sphere->radius)
		return (1);
	return (0);
}

int	check_in_cy(t_cylinder *cy, t_set *set, t_vec normal)
{
	double		check;
	t_vec		center;

	check = dot(v_sub(set->cam.loc, cy->center), normal);
	if (check < cy->height / 2)
	{
		center = v_add(cy->center, v_mul_n(normal, check));
		if (length(v_sub(center, set->cam.loc)) < cy->radius)
			return (1);
	}
	return (0);
}
