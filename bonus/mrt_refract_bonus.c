/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_refract_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmin <dmin@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 14:50:35 by dmin              #+#    #+#             */
/*   Updated: 2023/06/02 14:50:37 by dmin             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

t_vec	get_refr_norm(t_ray r, double t, t_object *ob, t_ray contact)
{
	t_vec	normal;

	if (ob->type == SPHERE)
		normal = get_refr_sp_norm(r, t, ob, contact);
	else if (ob->type == CYLINDER)
		normal = get_refr_cy_norm(r, t, ob, contact);
	else if (ob->type == PLANE)
		normal = get_refr_pl_norm(r, t, ob, contact);
	else if (ob->type == CONE)
		normal = get_refr_cn_norm(r, t, ob, contact);
	else if (ob->type == 5 || ob->type == 6)
		normal = get_refr_cir_norm(r, t, ob, contact);
	return (normal);
}

t_ray	refract_color(t_ray r, double t, t_object *ob)
{
	t_ray		contact;
	t_vec		normal;
	t_vec		ray;
	double		sin;
	t_vec		nv[2];

	contact.orig = at(r, t);
	contact.dir = make_vec(0, 0, 0);
	ray = v_sub(r.orig, contact.orig);
	normal = get_refr_norm(r, t, ob, contact);
	sin = dot(normal, v_mul_n(ray, -1)) / (length(normal) * length(ray));
	sin = sqrt(1 - pow(sin, 2));
	sin = sin * ob->refr;
	nv[0] = v_mul_n(unit_vector(v_sub(contact.orig, v_sub(r.orig, \
				v_mul_n(normal, dot(ray, normal))))), sin);
	if (sin > 1)
		nv[1] = normal;
	else
		nv[1] = v_mul_n(normal, -1 * sqrt(1 - pow(sin, 2)));
	contact.dir = unit_vector(v_add(nv[0], nv[1]));
	contact.orig = v_add(contact.orig, contact.dir);
	return (contact);
}
