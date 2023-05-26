/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:41:53 by dmin              #+#    #+#             */
/*   Updated: 2023/05/23 17:10:32 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_object	*ft_lstnew(int nb, char *map)
{
	t_object	*ob;
	static int	i;

	ob = (t_object *)malloc(sizeof(t_object) * 1);
	if (ob == 0)
		exit(printf("malloc error\n"));
	ob->type = nb;
	if (nb == 0)
		ob->object = (void *)set_sphere(map, ob);
	else if (nb == 1)
		ob->object = (void *)set_cylinder(map, ob);
	else if (nb == 2)
		ob->object = (void *)set_plane(map, ob);
	ob->rank = i;
	i++;
	ob->refl = 0;
	ob->next = NULL;
	return (ob);
}

void	ft_lstadd_front(t_object **lst, t_object *new)
{
	if (new == 0)
		return ;
	else if (*lst == NULL)
		*lst = new;
	else
	{
		new->next = *lst;
		*lst = new;
	}
}

void	ft_lstclear(t_object **lst)
{
	t_object	*tmp;

	if (*lst == 0)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free ((*lst)->object);
		free(*lst);
		*lst = tmp;
	}
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

double	degrees_to_radians(double degrees)
{
	return (degrees * 3.141529 / 180.0);
}
