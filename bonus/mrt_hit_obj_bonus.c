/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_hit_obj_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmin <dmin@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:26:56 by dmin              #+#    #+#             */
/*   Updated: 2023/06/02 16:26:58 by dmin             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

double	hit_cone(t_object *ob, t_ray r)
{
	t_vec		oc;
	t_vec		top;
	double		coe[4];
	double		t;
	t_cone		*cn;

	cn = (t_cone *) ob->object;
	r.dir = unit_vector(r.dir);
	top = v_add(cn->center, v_mul_n(cn->normal, cn->height / 2));
	oc = v_sub(r.orig, top);
	coe[0] = -(pow(dot(r.dir, cn->normal), 2) - pow(cn->height, 2) \
			/ (pow(cn->height, 2) + pow(cn->radius, 2)));
	coe[1] = -(dot(oc, cn->normal) * dot(r.dir, cn->normal) - dot(oc, r.dir) \
			* pow(cn->height, 2) / (pow(cn->height, 2) + pow(cn->radius, 2)));
	coe[2] = -(pow(dot(oc, cn->normal), 2) - dot(oc, oc) * pow(cn->height, 2) \
			/ (pow(cn->height, 2) + pow(cn->radius, 2)));
	coe[3] = coe[1] * coe[1] - coe[0] * coe[2];
	t = (-coe[1] - sqrt(coe[3])) / coe[0];
	if (t < 0)
		t = (-coe[1] - sqrt(coe[3])) / coe[0];
	if (coe[3] > 0 && \
		fabs(dot(v_sub(cn->center, at(r, t)), cn->normal)) <= cn->height / 2)
		return (t);
	else
		return (-1.0);
}

double	hit_plane(t_object *ob, t_ray r)
{
	double	coefficient;
	double	constant;
	t_plane	*pl;

	pl = (t_plane *) ob->object;
	ob->tran = pl->tran;
	coefficient = r.dir.x * pl->normal.x \
				+ r.dir.y * pl->normal.y \
				+ r.dir.z * pl->normal.z;
	constant = pl->normal.x * (pl->center.x - r.orig.x) \
				+ pl->normal.y * (pl->center.y - r.orig.y) \
				+ pl->normal.z * (pl->center.z - r.orig.z);
	if (coefficient == 0)
		return (-1);
	return (constant / coefficient);
}

double	hit_sphere(t_object *ob, t_ray r)
{
	t_vec		oc;
	double		coe[3];
	double		discriminant;
	t_sphere	*s;
	double		t;

	s = (t_sphere *) ob->object;
	ob->tran = s->tran;
	oc = v_sub(r.orig, s->center);
	coe[0] = length_squared(r.dir);
	coe[1] = dot(oc, r.dir);
	coe[2] = length_squared(oc) - s->radius * s->radius;
	discriminant = coe[1] * coe[1] - coe[0] * coe[2];
	if (discriminant < 0)
		return (-1.0);
	else
	{
		t = (-coe[1] - sqrt(discriminant)) / coe[0];
		if (t < 0)
			return ((-coe[1] + sqrt(discriminant)) / coe[0]);
		return (t);
	}
}

double	hit_cylinder(t_object *ob, t_ray r)
{
	t_vec		oc;
	double		coe[3];
	double		discriminant;
	double		t;
	t_cylinder	*cy;

	cy = (t_cylinder *) ob->object;
	ob->tran = cy->tran;
	r.dir = unit_vector(r.dir);
	oc = v_sub(r.orig, cy->center);
	coe[0] = -(pow(dot(r.dir, cy->normal), 2) - 1);
	coe[1] = -(dot(oc, cy->normal) * dot(r.dir, cy->normal) - dot(oc, r.dir));
	coe[2] = -(pow(cy->radius, 2) - dot(oc, oc) + pow(dot(oc, cy->normal), 2));
	discriminant = coe[1] * coe[1] - coe[0] * coe[2];
	t = (-coe[1] - sqrt(discriminant)) / coe[0];
	if (t < 0)
		t = (-coe[1] + sqrt(discriminant)) / coe[0];
	if ((discriminant > 0 && \
	fabs(dot(v_sub(cy->center, at(r, t)), cy->normal)) <= cy->height / 2))
		return (t);
	else
		return (-1.0);
}

double	hit_circle(t_object *ob, t_ray r)
{
	t_circle	*cir;
	double		coe;
	double		constant;
	double		t;

	cir = (t_circle *) ob->object;
	ob->tran = cir->tran;
	coe = dot(r.dir, cir->normal);
	if (coe == 0)
		return (-1);
	constant = dot(cir->normal, v_sub(cir->center, r.orig));
	t = constant / coe;
	if (cir->radius >= length(v_sub(cir->center, at(r, t))))
		return (t);
	return (-1);
}
