
#include "minirt_bonus.h"

int	hit_cone_bottom(t_cone *cn, t_object *ob, t_ray ray, double *t)
{
	t_vec	center;
	double	coe;
	double	constant;

	coe = dot(ray.dir, cn->normal);
	if (coe == 0)
		return (0);
	center = v_add(cn->center, v_mul_n(cn->normal, -cn->height));
	constant = dot(cn->normal, v_sub(center, ray.orig));
	if (close_cylinder_cap(ray, constant / coe, \
			dot(cn->normal, v_sub(cn->center, ray.orig)) / coe))
	{
		ob->hit_part = 1;
		if (cn->radius >= length(v_sub(center, at(ray, constant / coe))))
			return (*t = constant / coe);
	}
	return (0);
}

double	hit_cone(t_object *ob, t_ray r)
{
	t_vec		oc;
	double		coe[3];
	double		discriminant;
	double		t;
	t_cone		*cn;

	cn = (t_cone *) ob->object;
	r.dir = unit_vector(r.dir);
	oc = v_sub(r.orig, cn->center);
	coe[0] = -(pow(dot(r.dir, cn->normal), 2) - pow(cn->height, 2) \
			/ (pow(cn->height, 2) + pow(cn->radius, 2)));
	coe[1] = -(dot(oc, cn->normal) * dot(r.dir, cn->normal) - dot(oc, r.dir) \
			* pow(cn->height, 2) / (pow(cn->height, 2) + pow(cn->radius, 2)));
	coe[2] = -(pow(dot(oc, cn->normal), 2) - dot(oc, oc) * pow(cn->height, 2) \
			/ (pow(cn->height, 2) + pow(cn->radius, 2)));
	discriminant = coe[1] * coe[1] - coe[0] * coe[2];
	t = (-coe[1] - sqrt(discriminant)) / coe[0];
	if (hit_cone_bottom(cn, ob, r, &t))
		return (t);
	ob->hit_part = 0;
	if (t < 0)
		t = (-coe[1] - sqrt(discriminant)) / coe[0];
	if (discriminant > 0 && \
		dot(v_sub(cn->center, at(r, t)), cn->normal) <= cn->height && \
		dot(v_sub(cn->center, at(r, t)), cn->normal) >= 0)
		return (t);
	else
		return (-1.0);
}

double	hit_hyper(t_object *ob, t_ray r)
{
	t_vec		oc;
	double		coe[3];
	double		discriminant;
	double		t;
	t_hyper		*hy;

	t = 0.0;
	hy = (t_hyper *) ob->object;
	r.dir = unit_vector(r.dir);
	oc = v_sub(r.orig, hy->center);
	coe[0] = (pow(hy->a * r.dir.x, 2) + pow(hy->a * r.dir.y, 2) - pow(hy->c * r.dir.z, 2));
	coe[1] = (2 * (hy->b * hy->b * oc.x * r.dir.x + hy->a * hy->a * oc.y * r.dir.y - hy->c * hy->c * oc.z * r.dir.z));
	coe[2] = (pow(hy->b * oc.x, 2) + pow(hy->a * oc.y, 2) - pow(hy->c * oc.z, 2) - pow(hy->a * hy->b, 2));
	discriminant = coe[1] * coe[1] - 4 * coe[0] * coe[2];
	t = (-coe[1] - sqrt(discriminant)) / (2 * coe[0]);
	if ((discriminant > 0 && \
	 fabs(dot(v_sub(hy->center, at(r, t)), hy->normal)) <= hy->height / 2))
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
	ob->refl = pl->refl;
	ob->refr = pl->refr;
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
	ob->refl = s->refl;
	ob->refr = s->refr;
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
	ob->refl = cy->refl;
	ob->refr = cy->refr;
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
	ob->refl = cir->refl;
	ob->refr = cir->refr;
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