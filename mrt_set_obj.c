/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_set_obj.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:20:24 by migo              #+#    #+#             */
/*   Updated: 2023/05/23 17:35:45 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_sphere	*set_sphere(char *map, t_object *ob)
{
	t_sphere	*sphere;

	ob->hit_f = hit_sphere;
	ob->ratio_f = ratio_sp;
	map = map + 2;
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	sphere = malloc(sizeof(t_sphere) * 1);
	if (!sphere)
		exit(printf("malloc error\n"));
	sphere->center = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	sphere->radius = ft_atof(&map);
	sphere->color = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	sphere->refl = ft_atof(&map);
	check_color(sphere->color);
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	if (map[0] != '\0')
		exit(printf("%c is wrong parameter\n", map[0]));
	return (sphere);
}

t_plane	*set_plane(char *map, t_object *ob)
{
	t_plane	*plane;

	ob->hit_f = hit_plane;
	ob->ratio_f = ratio_pl;
	map = map + 2;
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	plane = malloc(sizeof(t_plane) * 1);
	if (!plane)
		exit(printf("malloc error\n"));
	plane->center = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	plane->normal = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	plane->color = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	plane->refl = ft_atof(&map);
	check_viewpoint(plane->normal);
	check_color(plane->color);
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	if (map[0] != '\0')
		exit(printf("%c is wrong parameter\n", map[0]));
	return (plane);
}

t_cylinder	*set_cylinder(char *map, t_object *ob)
{
	t_cylinder	*cy;

	ob->hit_f = hit_cylinder;
	ob->ratio_f = ratio_cy;
	map = map + 2;
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	cy = malloc(sizeof(t_cylinder) * 1);
	if (!cy)
		exit(printf("malloc error\n"));
	cy->center = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	cy->normal = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	cy->radius = ft_atof(&map);
	cy->height = ft_atof(&map);
	cy->color = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	check_viewpoint(cy->normal);
	cy->normal = unit_vector(cy->normal);
	check_color(cy->color);
	cy->refl = ft_atof(&map);
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	if (map[0] != '\0')
		exit(printf("%c is wrong parameter\n", map[0]));
	return (cy);
}
