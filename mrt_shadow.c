/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_shadow.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:42:57 by dmin              #+#    #+#             */
/*   Updated: 2023/05/17 13:17:53 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	hit_something(t_set *set, t_ray contact, t_object *obj)
{
	t_object	*ob;
	double		t;
	double		near_t;
	double		length;
	double		near_length;

	near_length = 184467440737095516;
	ob = set->objects;
	while (ob)
	{
		t = ob->hit_f(ob, contact);
		length = length_squared(at(contact, t));
		if (obj->rank == ob->rank)
			length = 184467440737095516;
		if (t > 0 && near_length > length)
		{
			near_t = t;
			near_length = length;
		}
		ob = ob->next;
	}
	if (near_length == 184467440737095516)
		return (0);
	return (near_t);
}
