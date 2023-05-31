/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_ratio_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:03:25 by migo              #+#    #+#             */
/*   Updated: 2023/05/17 13:36:34 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

void	ratio_hy(t_ray r, double t, t_object *ob, t_set *set)
{
	t_ray		contact;
	t_vec		normal;
	t_vec		center_vec;
	t_hyper		*hy;

	hy = ob->object;
	contact.orig = at(r, t);
	contact.dir = unit_vector(v_sub(set->light->loc, contact.orig));
	contact.orig = v_sub(contact.orig, contact.dir);
	center_vec = v_add(hy->center, \
	v_mul_n(hy->normal, dot(v_sub(contact.orig, hy->center), hy->normal)));
	normal = unit_vector(v_sub(contact.orig, center_vec));
	set_obj(ob, set, normal, contact);
	hit_something(set, contact, ob);
}

void	ratio_cn(t_ray r, double t, t_object *ob, t_set *set)
{
	t_ray		contact;
	t_vec		normal;
	t_cone		*cn;

	cn = ob->object;
	contact.orig = at(r, t);
	contact.dir = unit_vector(v_sub(set->light->loc, contact.orig));
	contact.orig = v_sub(contact.orig, contact.dir);
	if (ob->hit_part == 0)
	{
		normal = v_sub(v_sub(contact.orig, cn->center), \
		v_mul_n(cn->normal, length_squared(v_sub(contact.orig, cn->center)) \
		/ dot(v_sub(contact.orig, cn->center), cn->normal)));
		normal = unit_vector(normal);
		set_obj(ob, set, normal, contact);
	}
	else if (ob->hit_part == 1)
	{
		normal = v_mul_n(cn->normal, -1);
		set_obj(ob, set, normal, contact);
	}
	hit_something(set, contact, ob);
}

void	ratio_cy(t_ray r, double t, t_object *ob, t_set *set)
{
	t_ray		contact;
	t_vec		normal;
	t_vec		center_vec;
	t_cylinder	*cy;

	cy = ob->object;
	contact.orig = at(r, t);
	contact.dir = unit_vector(v_sub(set->light->loc, contact.orig));
	contact.orig = v_sub(contact.orig, contact.dir);
	center_vec = v_add(cy->center, \
	v_mul_n(cy->normal, dot(v_sub(contact.orig, cy->center), cy->normal)));
	normal = unit_vector(v_sub(contact.orig, center_vec));
	set_obj(ob, set, normal, contact);
	hit_something(set, contact, ob);
}

void	ratio_sp(t_ray r, double t, t_object *ob, t_set *set)
{
	t_ray		contact;
	t_vec		normal;
	t_sphere	*sphere;

	sphere = ob->object;
	contact.orig = at(r, t);
	normal = unit_vector(v_sub(at(r, t), sphere->center));
	contact.dir = unit_vector(v_sub(set->light->loc, contact.orig));
	contact.orig = v_sub(contact.orig, contact.dir);
	set_obj(ob, set, normal, contact);
	hit_something(set, contact, ob);
}

void	ratio_cir(t_ray r, double t, t_object *ob, t_set *set)
{
	t_ray		contact;
	t_circle	*cir;

	cir = ob->object;
	contact.orig = at(r, t);
	contact.dir = unit_vector(v_sub(set->light->loc, contact.orig));
	contact.orig = v_sub(contact.orig, contact.dir);
	set_obj(ob, set, cir->normal, contact);
	hit_something(set, contact, ob);
}

void	ratio_pl(t_ray r, double t, t_object *ob, t_set *set)
{
	t_ray	contact;
	t_plane	*pl;
	t_vec	normal;

	pl = ob->object;
	contact.orig = at(r, t);
	contact.dir = unit_vector(v_sub(set->light->loc, contact.orig));
	contact.orig = v_sub(contact.orig, contact.dir);
	if (dot(pl->normal, v_sub(set->cam.loc, pl->center)) < 0)
		normal = v_mul_n(pl->normal, -1);
	else
		normal = pl->normal;
	set_obj(ob, set, normal, contact);
	hit_something(set, contact, ob);
}
