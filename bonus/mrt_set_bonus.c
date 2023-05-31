/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_set_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:23:11 by migo              #+#    #+#             */
/*   Updated: 2023/05/16 14:23:40 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

void	check_color(t_vec color)
{
	if (color.x < 0 || color.x > 255)
		exit(printf("color error\n"));
	if (color.y < 0 || color.y > 255)
		exit(printf("color error\n"));
	if (color.z < 0 || color.z > 255)
		exit(printf("color error\n"));
}

void	check_viewpoint(t_vec vec)
{
	if (vec.x < -1 || vec.x > 1)
		exit(printf("vector error\n"));
	if (vec.y < -1 || vec.y > 1)
		exit(printf("vector error\n"));
	if (vec.z < -1 || vec.z > 1)
		exit(printf("vector error\n"));
	if (length(vec) == 0)
		exit(printf("vector error\n"));
}

void	set_cam(t_cam *cam, char *map)
{
	map++;
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	cam->loc = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	cam->vec = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	cam->fov = tan(degrees_to_radians(ft_atof(&map) / 2.0));
	if (cam->fov < 0 || cam->fov > 180)
		exit(printf("fov error\n"));
	check_viewpoint(cam->vec);
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	if (map[0] != '\0')
		exit(printf("%c is wrong parameter\n", map[0]));
}

void	set_light(t_light *light, char *map)
{
	map++;
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	light->loc = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	light->power = ft_atof(&map);
	if (light->power < 0 || light->power > 1)
		exit(printf("light power error\n"));
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	if (map[0] != '\0')
		exit(printf("%c is wrong parameter\n", map[0]));
}

void	set_am_light(t_am_light *am_light, char *map)
{
	map++;
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	am_light->am_light = ft_atof(&map);
	am_light->color = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	if (am_light->am_light < 0 || am_light->am_light > 1)
		exit(printf("light power error\n"));
	check_color(am_light->color);
	am_light->color = v_div_n(am_light->color, 255);
	while (map[0] == ' ' || (map[0] >= 9 && map[0] <= 13))
		map++;
	if (map[0] != '\0')
		exit(printf("%c is wrong parameter\n", map[0]));
}
