/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_ray_color_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:29:38 by migo              #+#    #+#             */
/*   Updated: 2023/05/31 16:23:00 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

int	re_color(t_ray r, t_set *set)
{
	t_vec	color;
	t_light	*light;

	light = set->light;
	color = make_vec(0, 0, 0);
	while (set->light)
	{
		color = v_add(color, ray_color(r, set, set->light->power, NULL));
		set->am_light.am_light = 0.1;
		set->light = set->light->next;
	}
	set->light = light;
	return (fl_color(color));
}

t_object	*get_near(t_set *set, t_ray r, double *near_t, t_object *check)
{
	double		t;
	double		near_length;
	t_object	*ob;
	t_object	*near;

	near_length = 184467440737095516;
	ob = set->objects;
	while (ob)
	{
		t = ob->hit_f(ob, r);
		ob->length = length(v_sub(set->cam.loc, at(r, t)));
		if (check != NULL && check->rank == ob->rank)
			ob->length = 184467440737095516;
		if (t > 0 && ob->length < near_length)
		{
			near = ob;
			*near_t = t;
			near_length = ob->length;
		}
		ob = ob->next;
	}
	return (near);
}

t_vec	ray_color(t_ray r, t_set *set, double power, t_object *check)
{
	double		near_t;
	double		near_length;
	t_object	*near;
	t_vec		color[3];

	near_t = -1;
	near = get_near(set, r, &near_t, check);
	if (near_t < 0)
		return (make_vec(0, 0, 0));
	near->ratio_f(r, near_t, near, set);
	if (power < 0.005)
		return (set_color(near->color, \
			near->ratio * power, set->am_light));
	else
	{
		color[0] = (set_color(near->color, near->ratio * \
				power * (1 - near->refl) * (1 - near->tran), set->am_light));
		set->am_light.am_light = 0;
		color[1] = ray_color(reflect_color(r, near_t, near), set, \
					power * near->refl, near);
		color[2] = ray_color(refract_color(r, near_t, near), set, \
					power * (1 - near->refl) * near->tran, near);
		return (v_add(v_add(color[0], color[1]), color[2]));
	}
}

t_vec	set_color(t_vec ob_color, double ratio, t_am_light am)
{
	t_vec	color;
	int		re_color;
	t_vec	ratio1;

	if (ratio < 0)
		ratio = 0;
	ratio1 = v_add_n(v_mul_n(am.color, am.am_light), ratio);
	if (ratio1.x < 1)
		color.x = (int)(ob_color.x * ratio1.x);
	else
		color.x = (int)(ob_color.x * (2 - ratio1.x) + 255 * (ratio1.x - 1));
	if (ratio1.y < 1)
		color.y = (int)(ob_color.y * ratio1.y);
	else
		color.y = (int)(ob_color.y * (2 - ratio1.y) + 255 * (ratio1.y - 1));
	if (ratio1.z < 1)
		color.z = (int)(ob_color.z * ratio1.z);
	else
		color.z = (int)(ob_color.z * (2 - ratio1.z) + 255 * (ratio1.z - 1));
	re_color = (color.x * 65536 + color.y * 256 + color.z);
	return (color);
}
