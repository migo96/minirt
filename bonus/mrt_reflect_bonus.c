/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_reflect_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmin <dmin@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 14:49:51 by dmin              #+#    #+#             */
/*   Updated: 2023/06/02 14:49:56 by dmin             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

t_vec	get_refl_norm(t_ray r, double t, t_object *ob, t_ray contact)
{
	t_vec	normal;

	if (ob->type == SPHERE)
		normal = get_refl_sp_norm(r, t, ob, contact);
	else if (ob->type == CYLINDER)
		normal = get_refl_cy_norm(r, t, ob, contact);
	else if (ob->type == PLANE)
		normal = get_refl_pl_norm(r, t, ob, contact);
	else if (ob->type == CONE)
		normal = get_refl_cn_norm(r, t, ob, contact);
	else if (ob->type == 5 || ob->type == 6)
		normal = get_refl_cir_norm(r, t, ob, contact);
	return (normal);
}

t_ray	reflect_color(t_ray r, double t, t_object *ob)
{
	t_ray		contact;
	t_vec		normal;
	t_vec		ray;

	contact.orig = at(r, t);
	ray = v_sub(contact.orig, r.orig);
	normal = get_refl_norm(r, t, ob, contact);
	contact.dir = unit_vector(v_add(v_mul_n(normal, -2 * dot(ray, normal)), \
				ray));
	return (contact);
}
