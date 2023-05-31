/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_shadow_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:42:57 by dmin              #+#    #+#             */
/*   Updated: 2023/05/31 16:41:40 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

void	hit_something(t_set *set, t_ray contact, t_object *obj)
{
	t_object	*ob;
	double		t;
	double		length;

	ob = set->objects;
	while (ob)
	{
		t = ob->hit_f(ob, contact);
		length = length_squared(at(contact, t));
		if (obj->rank == ob->rank)
			length = 184467440737095516;
		if (t > 0 && length != 184467440737095516)
		{
			obj->tran = ob->tran;
			hit_range(obj, set, contact, t);
		}
		ob = ob->next;
	}
}
