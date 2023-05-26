/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_ray_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:29:38 by migo              #+#    #+#             */
/*   Updated: 2023/05/26 17:17:30 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec	set_color(t_vec ob_color, double ratio, t_am_light am);

t_ray	refract_color(t_ray r, double t, t_object *ob, t_set *set)
{
	t_ray		contact;
	t_vec		normal;
	t_sphere	*sp;
	t_cylinder	*cy;
	t_plane		*pl;
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
		if (check_in_sp(sp, set, r))
			normal = v_mul_n(normal, -1);
		normal = unit_vector(v_sub(at(r, t), sp->center));
	}
	if (ob->type == 1)
	{
		cy = ob->object;
		if (ob->hit_part == 0)
		{
			center_vec = v_add(cy->center, \
			v_mul_n(cy->normal, dot(v_sub(contact.orig, cy->center), cy->normal)));
			normal = unit_vector(v_sub(contact.orig, center_vec));
		}
		else if (ob->hit_part == 1)
			normal = cy->normal;
		else
			normal = v_mul_n(cy->normal, -1);
	}
	if (ob->type == 2)
	{
		pl = ob->object;
		normal = pl->normal;
	}
	cos = dot(normal, v_mul_n(ray, -1)) / (length(normal) * length(ray));
	sin = sqrt(1 - pow(cos, 2));
	sin = sin * ob->refr;
	if (sin > 1)
		return (contact);
	A = v_mul_n(unit_vector(v_sub(contact.orig, v_sub(r.orig, v_mul_n(normal, dot(ray, normal))))), sin);
	B = v_mul_n(normal, -1 * sqrt(1 - pow(sin, 2)));
	contact.dir = unit_vector(v_add(A, B));
	contact.orig = v_add(contact.orig, contact.dir);
	return (contact);
}

t_ray	reflect_color(t_ray r, double t, t_object *ob)
{
	t_ray		contact;
	t_vec		normal;
	t_sphere	*sp;
	t_cylinder	*cy;
	t_plane		*pl;
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
		if (ob->hit_part == 0)
		{
			center_vec = v_add(cy->center, \
			v_mul_n(cy->normal, dot(v_sub(contact.orig, cy->center), cy->normal)));
			normal = unit_vector(v_sub(contact.orig, center_vec));
		}
		else if (ob->hit_part == 1)
			normal = cy->normal;
		else
			normal = v_mul_n(cy->normal, -1);
	}
	if (ob->type == 2)
	{
		pl = ob->object;
		normal = pl->normal;
	}
	contact.dir = unit_vector(v_add(v_mul_n(normal, -2 * dot(ray, normal)), ray));
	return (contact);
}

t_vec	ray_color2(t_ray r, t_set *set, t_object *check, double i)
{
	double		t;
	double		near_t;
	double		near_length;
	t_object	*ob;
	t_object	*near;
	t_vec		color1;
	t_vec		color2;
	double		tran;
	t_ray		check1;

	near_length = 184467440737095516;
	color2 = make_vec(0, 0, 0);
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
			tran = ob->tran;
			near_length = ob->length;
		}
		ob = ob->next;
	}
	if (near_length == 184467440737095516 || i == 0)
		return (color2);
	near->ratio_f(r, near_t, near, set);
	color1 = set_color(near->color, near->ratio * set->light.power * i, \
	set->am_light);
	if (i > 0.001)
	{
		check1 = refract_color(r, near_t, near, set);
		if (check1.dir.x != 0 || check1.dir.y != 0 || check1.dir.z != 0)
		{
			color1 = v_mul_n(color1, 1 -tran);
			color2 = ray_color2(check1, set, near, tran * i);
		}
	}
	return (v_add(color1, color2));
}

t_vec	ray_color(t_ray r, t_set *set, int i, t_object *check)
{
	double		t;
	double		near_t;
	double		near_length;
	t_object	*ob;
	t_object	*near;
	t_vec		color1;
	t_vec		color2;
	t_vec		color3;
	double		refl;
	double		tran;
	t_ray		check1;

	near_length = 184467440737095516;
	color2 = make_vec(0, 0, 0);
	color3 = make_vec(0, 0, 0);
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
			tran = ob->tran;
			near_length = ob->length;
		}
		ob = ob->next;
	}
	if (near_length == 184467440737095516)
		return (color2);
	near->ratio_f(r, near_t, near, set);
	color1 = set_color(near->color, near->ratio * set->light.power, \
	set->am_light);
	if (i < 3)
	{
		i++;
		check1 = refract_color(r, near_t, near, set);
		if (check1.dir.x == 0 && check1.dir.y == 0 && check1.dir.z == 0)
			color1 = v_mul_n(color1, (1 - refl));
		else
		{
			color1 = v_mul_n(color1, (1 - refl) * 1 -tran);
			color3 = ray_color2(check1, set, near, tran);
		}
		set->light.power *= refl;
		set->am_light.am_light = 0;
		color2 = ray_color(reflect_color(r, near_t, near), set, i, near);
	}
	return (v_add(v_add(color1, color2), color3));
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
