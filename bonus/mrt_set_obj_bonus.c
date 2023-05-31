/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_set_obj_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:20:24 by migo              #+#    #+#             */
/*   Updated: 2023/05/31 15:38:39 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

t_cone	*set_cone(char *map, t_object *ob)
{
	t_cone	*cn;

	ob->hit_f = hit_cone;
	ob->ratio_f = ratio_cn;
	map = map + 2;
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	cn = malloc(sizeof(t_cone) * 1);
	if (!cn)
		exit(printf("malloc error\n"));
	cn->center = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	cn->normal = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	cn->radius = ft_atof(&map);
	cn->height = ft_atof(&map);
	cn->color = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	check_viewpoint(cn->normal);
	cn->normal = unit_vector(cn->normal);
	check_color(cn->color);
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	if (map[0] != '\0')
		exit(printf("%c is wrong parameter\n", map[0]));
	return (cn);
}

t_hyper	*set_hyper(char *map, t_object *ob)
{
	t_hyper	*hy;

	ob->hit_f = hit_hyper;
	ob->ratio_f = ratio_hy;
	map = map + 2;
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	hy = malloc(sizeof(t_hyper) * 1);
	if (!hy)
		exit(printf("malloc error\n"));
	hy->center = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	hy->normal = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	hy->a = ft_atof(&map);
	hy->b = ft_atof(&map);
	hy->c = ft_atof(&map);
	hy->height = ft_atof(&map);
	hy->color = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	check_viewpoint(hy->normal);
	hy->normal = unit_vector(hy->normal);
	check_color(hy->color);
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	if (map[0] != '\0')
		exit(printf("%c is wrong parameter\n", map[0]));
	return (hy);
}

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
	sphere->refr = ft_atof(&map);
	sphere->tran = ft_atof(&map);
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
	plane->refr = ft_atof(&map);
	plane->tran = ft_atof(&map);
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
	cy->refr = ft_atof(&map);
	cy->tran = ft_atof(&map);
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	if (map[0] != '\0')
		exit(printf("%c is wrong parameter\n", map[0]));
	return (cy);
}

t_circle	*set_circle(char *map, t_object *ob, int nb)
{
	t_circle	*cir;

	ob->hit_f = hit_circle;
	ob->ratio_f = ratio_cir;
	map = map + 2;
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	cir = malloc(sizeof(t_cylinder) * 1);
	if (!cir)
		exit(printf("malloc error\n"));
	cir->center = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	cir->normal = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	cir->radius = ft_atof(&map);
	cir->height = ft_atof(&map);
	cir->color = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	check_viewpoint(cir->normal);
	cir->normal = unit_vector(cir->normal);
	if (nb == 5)
		cir->center = v_add(cir->center, v_mul_n(cir->normal, cir->height / 2));
	else if (nb == 6)
	{
		cir->normal = v_mul_n(cir->normal, -1);
		cir->center = v_add(cir->center, v_mul_n(cir->normal, cir->height / 2));
	}
	check_color(cir->color);
	cir->refl = ft_atof(&map);
	cir->refr = ft_atof(&map);
	cir->tran = ft_atof(&map);
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	if (map[0] != '\0')
		exit(printf("%c is wrong parameter\n", map[0]));
	return (cir);
}