/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_refract2_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmin <dmin@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 14:50:19 by dmin              #+#    #+#             */
/*   Updated: 2023/06/02 14:50:28 by dmin             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

t_vec	get_refr_sp_norm(t_ray r, double t, t_object *ob, t_ray contact)
{
	t_sphere	*sp;

	sp = ob->object;
	return (unit_vector(v_sub(at(r, t), sp->center)));
}

t_vec	get_refr_cy_norm(t_ray r, double t, t_object *ob, t_ray contact)
{
	t_cylinder	*cy;
	t_vec		center_vec;

	cy = ob->object;
	center_vec = v_add(cy->center, \
	v_mul_n(cy->normal, dot(v_sub(contact.orig, cy->center), cy->normal)));
	return (unit_vector(v_sub(contact.orig, center_vec)));
}

t_vec	get_refr_pl_norm(t_ray r, double t, t_object *ob, t_ray contact)
{
	t_plane		*pl;	

	pl = ob->object;
	return (pl->normal);
}

t_vec	get_refr_cn_norm(t_ray r, double t, t_object *ob, t_ray contact)
{
	t_cone	*cn;
	t_vec	center_vec;

	cn = ob->object;
	center_vec = v_add(cn->center, \
	v_mul_n(cn->normal, dot(v_sub(contact.orig, cn->center), cn->normal)));
	return (unit_vector(v_sub(contact.orig, center_vec)));
}

t_vec	get_refr_cir_norm(t_ray r, double t, t_object *ob, t_ray contact)
{
	t_circle	*cir;

	cir = ob->object;
	return (cir->normal);
}
