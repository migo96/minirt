/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_set_ratio_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:08:11 by migo              #+#    #+#             */
/*   Updated: 2023/06/05 13:33:16 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

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

t_vec	get_sp_norm(t_object *ob, t_set *set, t_vec normal, t_ray con)
{
	t_sphere	*sp;

	sp = ob->object;
	if (sp->mty != 0)
		ob->color = sp_map(con, sp, set, &normal);
	if (check_in_sp(sp, set, con))
		normal = v_mul_n(normal, -1);
	return (normal);
}

t_vec	get_cy_norm(t_object *ob, t_set *set, t_vec normal, t_ray con)
{
	t_cylinder	*cy;

	cy = ob->object;
	if (check_in_cy(cy, set, normal))
		normal = v_mul_n(normal, -1);
	return (normal);
}

t_vec	get_cn_norm(t_object *ob, t_set *set, t_vec normal, t_ray con)
{
	t_cone		*cn;

	cn = ob->object;
	if (check_in_cn(cn, set, normal))
		normal = v_mul_n(normal, -1);
	return (normal);
}

void	set_obj(t_object *ob, t_set *set, t_vec normal, t_ray con)
{
	t_cone		*cn;
	t_hyper		*hy;
	t_circle	*cir;
	t_plane		*pl;

	if (ob->type == SPHERE)
		normal = get_sp_norm(ob, set, normal, con);
	else if (ob->type == CYLINDER)
		normal = get_cy_norm(ob, set, normal, con);
	else if (ob->type == CONE)
		normal = get_cn_norm(ob, set, normal, con);
	else if (ob->type == PLANE)
	{
		pl = ob->object;
		if (pl->mty != 0)
			ob->color = pl_map(con, pl, set, &normal);
	}
	ob->ratio = dot(con.dir, normal) / length(normal) * length(con.dir);
}
