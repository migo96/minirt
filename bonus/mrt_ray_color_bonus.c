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

t_ray	reflect_color(t_ray r, double t, t_object *ob)
{
	t_ray		contact;
	t_vec		normal;
	t_sphere	*sp;
	t_cylinder	*cy;
	t_plane		*pl;
	t_circle	*cir;
	t_vec		ray;
	t_vec		center_vec;

	contact.orig = at(r, t);
	ray = v_sub(contact.orig, r.orig);
	if (ob->type == 0)
	{
		sp = ob->object;
		normal = unit_vector(v_sub(at(r, t), sp->center));
	}
	if (ob->type == 1)
	{
		cy = ob->object;
		center_vec = v_add(cy->center, \
		v_mul_n(cy->normal, dot(v_sub(contact.orig, cy->center), cy->normal)));
		normal = unit_vector(v_sub(contact.orig, center_vec));
	}
	if (ob->type == 2)
	{
		pl = ob->object;
		normal = pl->normal;
	}
	if (ob->type == 5 || ob->type == 6)
	{
		cir = ob->object;
		normal = cir->normal;
	}
	contact.dir = unit_vector(v_add(v_mul_n(normal, -2 * dot(ray, normal)), ray));
	return (contact);
}

t_ray	refract_color(t_ray r, double t, t_object *ob)
{
	t_ray		contact;
	t_vec		normal;
	t_sphere	*sp;
	t_cylinder	*cy;
	t_plane		*pl;
	t_circle	*cir;
	t_vec		ray;
	t_vec		center_vec;
	double		cos;
	double		sin;
	t_vec		A;
	t_vec		B;

	contact.orig = at(r, t);
	contact.dir = make_vec(0, 0, 0);
	ray = v_sub(r.orig, contact.orig);
	if (ob->type == 0)
	{
		sp = ob->object;
		normal = unit_vector(v_sub(at(r, t), sp->center));
	}
	if (ob->type == 1)
	{
		cy = ob->object;
		center_vec = v_add(cy->center, \
		v_mul_n(cy->normal, dot(v_sub(contact.orig, cy->center), cy->normal)));
		normal = unit_vector(v_sub(contact.orig, center_vec));
	}
	if (ob->type == 2)
	{
		pl = ob->object;
		normal = pl->normal;
	}
	if (ob->type == 5 || ob->type == 6)
	{
		cir = ob->object;
		normal = cir->normal;
	}
	cos = dot(normal, v_mul_n(ray, -1)) / (length(normal) * length(ray));
	sin = sqrt(1 - pow(cos, 2));
	sin = sin * ob->refr;
	A = v_mul_n(unit_vector(v_sub(contact.orig, v_sub(r.orig, v_mul_n(normal, dot(ray, normal))))), sin);
	if (sin > 1)
		B = normal;
	else
		B = v_mul_n(normal, -1 * sqrt(1 - pow(sin, 2)));
	contact.dir = unit_vector(v_add(A, B));
	contact.orig = v_add(contact.orig, contact.dir);
	return (contact);
}

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

t_vec	ray_color(t_ray r, t_set *set, double power, t_object *check)
{
	double		t;
	double		near_t;
	double		near_length;
	t_object	*ob;
	t_object	*near;
	double		refl;
	double		refr;
	double		tran;
	t_vec		color1;
	t_vec		color2;
	t_vec		color3;

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
			near_t = t;
			refl = ob->refl;
			refr = ob->refr;
			tran = ob->tran;
			near_length = ob->length;
		}
		ob = ob->next;
	}
	if (near_length == 184467440737095516)
		return (make_vec(0, 0, 0));
	near->ratio_f(r, near_t, near, set);
	if (power < 0.005)
		return (set_color(near->color, \
			near->ratio * power, set->am_light));
	else
	{
		color1 = (set_color(near->color, \
			near->ratio * power * (1 - refl) * (1 - tran), set->am_light));
		set->am_light.am_light = 0;
		color2 = ray_color(reflect_color(r, near_t, near), set, power * refl, near);
		color3 = ray_color(refract_color(r, near_t, near), set, power * (1 - refl) * tran, near);
		return (v_add(v_add(color1, color2), color3));
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
