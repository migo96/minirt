/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_set_ratio_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:08:11 by migo              #+#    #+#             */
/*   Updated: 2023/06/01 15:32:09 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

int	check_in_hy(t_hyper *hy, t_set *set, t_vec normal)
{
	double		check;
	t_vec		center;

	check = dot(v_sub(set->cam.loc, hy->center), normal);
	if (check < hy->height)
	{
		center = v_add(hy->center, v_mul_n(normal, check));
		if (length(v_sub(center, set->cam.loc)) < 1 - hy->height)
			return (1);
	}
	return (0);
}

int	check_in_cn(t_cone *cn, t_set *set, t_vec normal)
{
	double		check;
	t_vec		center;

	check = dot(v_sub(set->cam.loc, cn->center), normal);
	if (check < cn->height)
	{
		center = v_add(cn->center, v_mul_n(normal, check));
		if (length(v_sub(center, set->cam.loc)) < 1 - cn->height)
			return (1);
	}
	return (0);
}

int	check_in_sp(t_sphere *sphere, t_set *set, t_ray contact)
{
	if (length(v_sub(set->cam.loc, contact.orig)) < sphere->radius)
		return (1);
	return (0);
}

int	check_in_cy(t_cylinder *cy, t_set *set, t_vec normal)
{
	double		check;
	t_vec		center;

	check = dot(v_sub(set->cam.loc, cy->center), normal);
	if (check < cy->height / 2)
	{
		center = v_add(cy->center, v_mul_n(normal, check));
		if (length(v_sub(center, set->cam.loc)) < cy->radius)
			return (1);
	}
	return (0);
}

void	hit_range(t_object *ob, t_set *set, t_ray contact, double t)
{
	t_vec	check;

	if ((int)t > 0)
	{
		check = at(contact, t);
		if (set->light->loc.x > contact.orig.x)
		{
			if (contact.orig.x < check.x && check.x < set->light->loc.x)
				ob->ratio *= ob->tran;
		}
		else
		{
			if (set->light->loc.x < check.x && check.x < contact.orig.x)
				ob->ratio *= ob->tran;
		}
	}
}

t_vec	sp_map(t_ray con, t_sphere *sp, t_set *set, t_vec *normal)
{
	t_vec	check;
	double	a;
	double	b;
	int		c;
	int		d;
	int		color;
	int		color1;
	t_vec	show;
	t_vec	ray;

	ray = unit_vector(v_sub(set->light->loc, con.orig));
	check = v_sub(con.orig, sp->center);
	a = atan2(- check.y, check.x) + 3.14;
	b = acos(-check.z / (sp->radius + 1));
	c = (int)(a * (set->img2->width / 6.28));
	d = (int)(b * (set->img2->height / 3.14));
	d = set->img2->height - d;
	c = set->img2->width - c;
	color = gain_color(set->img2, c, d);
	if (c + 1 <= set->img2->width)
		color1 = gain_color(set->img2, c + 1, d) - color;
	else
		color1 = gain_color(set->img2, 0, d) - color;
	if (c - 1 >= 0)
		color1 += gain_color(set->img2, c - 1, d) - color;
	else
		color1 += gain_color(set->img2, 0, d);
	if (d + 1 <= set->img2->height)
		color1 += gain_color(set->img2, c, d + 1) - color;
	else
		color1 += gain_color(set->img2, c, 0) - color;
	if (d - 1 >= 0)
		color1 += gain_color(set->img2, c, d - 1) - color;
	else
		color1 += gain_color(set->img2, c, 0) - color;
	color1 /= 4;
	if (color1 > 0)
	{
		show = v_mul_n(ch_color(color1), 0.001);
		if (length(show) > 0.25)
			(*normal) = v_add((*normal), v_mul_n(ray, length(show)));
	}
	*normal = *normal;
	return (ch_color(color));
}

void	set_obj(t_object *ob, t_set *set, t_vec normal, t_ray con)
{
	t_sphere	*sp;
	t_cylinder	*cy;
	t_plane		*pl;
	t_cone		*cn;
	t_hyper		*hy;
	t_circle	*cir;

	if (ob->type == SPHERE)
	{
		sp = ob->object;
		ob->color = sp_map(con, sp, set, &normal);
		if (check_in_sp(sp, set, con))
			normal = v_mul_n(normal, -1);
	}
	else if (ob->type == CYLINDER)
	{
		cy = ob->object;
		ob->color = cy->color;
		if (check_in_cy(cy, set, normal))
			normal = v_mul_n(normal, -1);
	}
	else if (ob->type == PLANE)
	{
		pl = ob->object;
		ob->color = pl->color;
	}
	else if (ob->type == CONE)
	{
		cn = ob->object;
		ob->color = cn->color;
		if (check_in_cn(cn, set, normal))
			normal = v_mul_n(normal, -1);
	}
	else if (ob->type == HYPER)
	{
		hy = ob->object;
		ob->color = hy->color;
		if (check_in_hy(hy, set, normal))
			normal = v_mul_n(normal, -1);
	}
	else if (ob->type == 5 || ob->type == 6)
	{
		cir = ob->object;
		ob->color = cir->color;
	}	
	ob->ratio = dot(con.dir, normal) / length(normal) * length(con.dir);
}