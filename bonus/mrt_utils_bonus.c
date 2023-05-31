/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_utils_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:41:53 by dmin              #+#    #+#             */
/*   Updated: 2023/05/31 19:18:23 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

t_object	*ft_lstnew(int nb, char *map)
{
	t_object	*ob;
	static int	i;

	ob = (t_object *)malloc(sizeof(t_object) * 1);
	if (ob == 0)
		exit(printf("malloc error\n"));
	ob->type = nb;
	ob->ratio = 0;
	if (nb == SPHERE)
		ob->object = (void *)set_sphere(map, ob);
	else if (nb == CYLINDER)
		ob->object = (void *)set_cylinder(map, ob);
	else if (nb == PLANE)
		ob->object = (void *)set_plane(map, ob);
	else if (nb == CONE)
		ob->object = (void *)set_cone(map, ob);
	else if (nb == HYPER)
		ob->object = (void *)set_hyper(map, ob);
	else if (nb == 5 || nb == 6)
		ob->object = (void *)set_circle(map, ob, nb);
	ob->rank = i;
	i++;
	ob->next = NULL;
	return (ob);
}

t_light	*l_lstnew(char *map)
{
	t_light	*light;

	light = (t_light *)malloc(sizeof(t_light));
	if (light == 0)
		exit(printf("malloc error\n"));
	set_light(light, map);
	light->next = NULL;
	return (light);
}

void	l_lstadd_front(t_light **lst, t_light *new)
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

void	l_lstclear(t_light **lst)
{
	t_light	*tmp;

	if (*lst == 0)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free(*lst);
		*lst = tmp;
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

int	gain_color(t_data *data, int x, int y)
{
    char    *dst;
 
    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    return (*(unsigned int*)dst);
}
