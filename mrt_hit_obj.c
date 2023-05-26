/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_hit_obj.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:01:10 by migo              #+#    #+#             */
/*   Updated: 2023/05/23 17:11:43 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	hit_plane(t_object *ob, t_ray r)
{
	double	coefficient;
	double	constant;
	t_plane	*pl;

	pl = (t_plane *) ob->object;
	ob->refl = pl->refl;
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
	ob->refl = s->refl;
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
	ob->refl = cy->refl;
	r.dir = unit_vector(r.dir);
	oc = v_sub(r.orig, cy->center);
	coe[0] = -(pow(dot(r.dir, cy->normal), 2) - 1);
	coe[1] = -(dot(oc, cy->normal) * dot(r.dir, cy->normal) - dot(oc, r.dir));
	coe[2] = -(pow(cy->radius, 2) - dot(oc, oc) + pow(dot(oc, cy->normal), 2));
	discriminant = coe[1] * coe[1] - coe[0] * coe[2];
	t = (-coe[1] - sqrt(discriminant)) / coe[0];
	if (hit_cylinder_cap(cy, ob, r, &t))
		return (t);
	ob->hit_part = 0;
	if (t < 0)
		t = (-coe[1] + sqrt(discriminant)) / coe[0];
	if ((discriminant > 0 && \
	fabs(dot(v_sub(cy->center, at(r, t)), cy->normal)) <= cy->height / 2))
		return (t);
	else
		return (-1.0);
}

int	close_cylinder_cap(t_ray ray, double result1, double result2)
{
	if (result1 > 0 && result2 > 0)
	{
		if (length(v_sub(ray.orig, at(ray, result1))) < \
		length(v_sub(ray.orig, at(ray, result2))))
			return (1);
		else
			return (0);
	}
	else
	{
		if (result1 > 0)
			return (1);
		else
			return (0);
	}
}

int	hit_cylinder_cap(t_cylinder *cy, t_object *ob, t_ray ray, double *t)
{
	t_vec	center[2];
	double	coe;
	double	constant[2];

	coe = dot(ray.dir, cy->normal);
	if (coe == 0)
		return (0);
	center[0] = v_add(cy->center, v_mul_n(cy->normal, cy->height / 2));
	constant[0] = dot(cy->normal, v_sub(center[0], ray.orig));
	center[1] = v_add(cy->center, v_mul_n(cy->normal, -cy->height / 2));
	constant[1] = dot(cy->normal, v_sub(center[1], ray.orig));
	if (close_cylinder_cap(ray, constant[0] / coe, constant[1] / coe))
	{
		ob->hit_part = 1;
		if (cy->radius >= length(v_sub(center[0], at(ray, constant[0] / coe))))
			return (*t = constant[0] / coe);
	}
	else
	{
		ob->hit_part = 2;
		if (cy->radius >= length(v_sub(center[1], at(ray, constant[1] / coe))))
			return (*t = constant[1] / coe);
	}
	return (0);
}
