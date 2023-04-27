/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 11:53:10 by migo              #+#    #+#             */
/*   Updated: 2023/04/27 15:23:39 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	set_camera(t_camera *camera, char *map)
{
	camera->fov = tan(degrees_to_radians(45)) * 2;
	camera->location.x = 100;
	camera->location.y = 100;
	camera->location.z = 100;
	camera->view_point.x = -1;
	camera->view_point.y = -1;
	camera->view_point.z = -1;
}

void	set_light(t_light *light, char *map)
{
	light->location.x = 110;
	light->location.y = 0;
	light->location.z = 100;
	light->power = 0.7;
}

void	set_sphere(t_sphere *sphere, char *map)
{
	sphere[0].center.x = 0;
	sphere[0].center.y = 0;
	sphere[0].center.z = 0;
	sphere[0].color.x = 255;
	sphere[0].color.y = 0;
	sphere[0].color.z = 0;
	sphere[0].radius = 2;
	sphere[1].center.x = 0;
	sphere[1].center.y = 0;
	sphere[1].center.z = -100;
	sphere[1].color.x = 1;
	sphere[1].color.y = 1;
	sphere[1].color.z = 255;
	sphere[1].radius = 90;
}

void	set_plane(t_plane *plane, char *map)
{
	plane->center.x = 0;
	plane->center.y = 0;
	plane->center.z = -120;
	plane->normal.x = 1;
	plane->normal.y = 0;
	plane->normal.z = 1;
	plane->color.x = 0;
	plane->color.y = 255;
	plane->color.z = 0;
}

void	set_cylinder(t_cylinder *cy, char *map)
{
	cy->center.x = 30;
	cy->center.y = 30;
	cy->center.z = 0;
	cy->normal.x = 0;
	cy->normal.y = 0;
	cy->normal.z = 1;
	cy->radius = 10;
	cy->height = 100;
	cy->color.x = 255;
	cy->color.y = 255;
	cy->color.z = 0;
	cy->top = v_sub(cy->center, v_mul_n(cy->normal, cy->height / 2));
	cy->botton = v_add(cy->center, v_mul_n(cy->normal, cy->height / 2));
	cy->h = unit_vector(v_sub(cy->top, cy->botton));
}

void	checkmap(char **argv, t_set	*set)
{
	char	*map;
	int		fd;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		exit(1);
	}
	while (1)
	{
		map = get_next_line(fd);
		if (map == NULL)
			break ;
		if (map[0] == 'C')
			set_camera(&set->camera, map);
		if (map[0] == 'L')
			set_light(&set->light, map);
		if (map[0] == 's' && map[1] == 'p')
			set_sphere(set->sphere, map);
		if (map[0] == 'p' && map[1] == 'l')
			set_plane(&set->plane, map);
		if (map[0] == 'c' && map[1] == 'y')
			set_cylinder(&set->cy, map);
		free (map);
	}
	close(fd);
}

t_vec	set_lower_left_corner(t_camera *camera)
{
	t_vec	z;
	t_vec	z_axis;
	t_vec	horizontal;
	t_vec	vertical;
	double 	t;

	z = make_vec(0,0,1);
	z_axis = make_vec(- camera->view_point.x,- camera->view_point.y, - camera->view_point.z);
	horizontal = cross(camera->view_point, z);
	t = sqrt(pow(camera->fov, 2) / dot(horizontal, horizontal));
	horizontal = v_mul_n(horizontal, t);
	camera->hor = horizontal;
	vertical = cross(horizontal, camera->view_point);
	t = sqrt(pow(camera->fov, 2) / dot(vertical, vertical));
	vertical = v_mul_n(vertical, t);
	camera->ver = vertical;
	return (v_sub(camera->location, v_add(v_add(v_div_n(horizontal, 2),
					v_div_n(vertical, 2)), z_axis)));
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (x > 1200 || x < 0)
		return ;
	if (y > 1200 || y < 0)
		return ;
	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

double hit_plane(t_plane pl, t_ray r)
{
	double	coefficient;
	double	constant;

	coefficient = r.dir.x * pl.normal.x + r.dir.y * pl.normal.y + r.dir.z * pl.normal.z;
	constant = pl.normal.x * (pl.center.x - r.orig.x) + pl.normal.y * (pl.center.y - r.orig.y) + pl.normal.z * (pl.center.z - r.orig.z);
	if (coefficient == 0)
		return (0);
	return (constant / coefficient);
}

double	hit_sphere(t_sphere s, t_ray r)
{
	t_vec	oc;
	double	a;
	double	half_b;
	double	c;
	double	discriminant;

	oc = v_sub(r.orig, s.center);
	a = length_squared(r.dir);
	half_b = dot(oc, r.dir);
	c = length_squared(oc) - s.radius * s.radius;
	discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
		return (-1.0);
	else
		return ((- half_b - sqrt(discriminant)) / a);
}

double	hit_cylinder(t_cylinder cy, t_ray r)
{
	t_vec	oc;
	double	a;
	double	half_b;
	double	c;
	double	discriminant;

	oc = v_sub(r.orig, cy.center);
	a = dot(r.dir, r.dir) - pow(dot(r.dir, cy.h), 2);
	half_b = (dot(r.dir, oc) - (dot(r.dir, cy.h) * dot(oc, cy.h))) * 2;
	c = dot(oc, oc) - pow(dot(oc, cy.h), 2) - (cy.radius *  cy.radius);
	discriminant = half_b * half_b - a * c * 4;
	// printf("%f\n", discriminant);
	if (discriminant < 0)
		return (-1.0);
	else
		return (- half_b + sqrt(discriminant) / 2 * a);
}

int	set_color(t_vec ob_color, double ratio, double light)
{
	int	color_red;
	int color_green;
	int color_blue;
	int color;
	double ratio1;

	if (ratio < 0)
		ratio = 0;
	ratio1 = ratio + light;
	if (ratio1 < 1)
	{
		color_red = (int)(ob_color.x * ratio1);
		color_green = (int)(ob_color.y * ratio1);
		color_blue = (int)(ob_color.z * ratio1);
	}
	else
	{
		color_red = (int)(ob_color.x * (2 - ratio1) + 255 * (ratio1 - 1));
		color_green = (int)(ob_color.y * (2 - ratio1) + 255 * (ratio1 - 1));
		color_blue = (int)(ob_color.z * (2 - ratio1) + 255 * (ratio1 - 1));
	}
	color = color_red * 65536 + color_green * 256 + color_blue;
	return (color);
}

int	ray_color(t_ray r, t_set set)
{
	double	t;
	t_ray	contact;
	t_vec	normal;
	t_ray	reflection;
	double	ratio;

	t = hit_sphere(set.sphere[0], r);
	if (t > 0.0)
	{
		contact.orig = at(r,t);
		reflection.orig = at(r,t);
		normal = unit_vector(v_sub(at(r, t), set.sphere[0].center));
		contact.dir = unit_vector(v_sub(set.light.location, contact.orig));
		reflection.dir = unit_vector(v_sub(r.dir, v_mul_n(normal, 2 * dot(normal, r.dir))));
		ratio = dot(contact.dir, normal) / length(normal) * length(contact.dir);
		if (hit_sphere(set.sphere[1], contact) > 0)
			return (0);
		return (set_color(set.sphere[0].color, ratio, 0.2));
	}
	t = hit_sphere(set.sphere[1], r);
	if (t > 0.0)
	{
		contact.orig = at(r,t);
		reflection.orig = at(r,t);
		normal = unit_vector(v_sub(at(r, t), set.sphere[1].center));
		contact.dir = unit_vector(v_sub(set.light.location, contact.orig));
		reflection.dir = unit_vector(v_sub(r.dir, v_mul_n(normal, 2 * dot(normal, r.dir))));
		ratio = dot(contact.dir, normal) / length(normal) * length(contact.dir);
		if (hit_sphere(set.sphere[0], contact) > 0)
			return (set_color(set.sphere[1].color, 0, 0.2));
		return (set_color(set.sphere[1].color, ratio, 0.2));
	}
	t = hit_cylinder(set.cy, r);
	if (t > 0)
	{
		contact.orig = at(r,t);
		printf("%f %f %f\n", contact.orig.x, contact.orig.y, contact.orig.z);
		return (set_color(set.cy.color, 1, 0));
	}
	t = hit_plane(set.plane, r);
	if (t > 0)
	{
		contact.orig = at(r,t);
		contact.dir = unit_vector(v_sub(set.light.location, contact.orig));
		ratio = dot(contact.dir, set.plane.normal) / length(set.plane.normal) * length(contact.dir);
		if (hit_sphere(set.sphere[0], contact) > 0)
			return (set_color(set.plane.color, 0, 0.2));
		if (hit_sphere(set.sphere[1], contact) > 0)
			return (set_color(set.plane.color, 0, 0.2));
		return(set_color(set.plane.color, ratio, 0.2));
	}
	return (0);
}

void	render(t_data *data, t_set set, double height, double width)
{
	int		j;
	int		i;
	float	u;
	float	v;
	t_ray	r;

	j = -1;
	while (++j < height)
	{
		i = -1;
		while (++i < width)
		{
			u = (double)i / (width - 1);
			v = (height - (double)j - 1) / (height - 1);
			r.orig = set.camera.location;
			r.dir = v_add(set.camera.lower_left_corner,
					v_sub(v_add(v_mul_n(set.camera.hor, u),
							v_mul_n(set.camera.ver, v)), set.camera.location));
			my_mlx_pixel_put(data, i, j, ray_color(r, set));
		}
	}
}

int	main(int argc, char **argv)
{
	t_set	set;
	t_data	img;
	t_vec	lower_left_corner;

	if (argc != 2)
		return (printf("parameter error"));
	checkmap(argv, &set);
	set.camera.lower_left_corner = set_lower_left_corner(&set.camera);
	img.mlx = mlx_init();
	img.win = mlx_new_window(img.mlx, 800, 600, "ray tracing");
	img.img = mlx_new_image(img.mlx, 800, 600);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel,
			&img.line_length, &img.endian);
	render(&img, set, 600, 800);
	mlx_put_image_to_window(img.mlx, img.win, img.img, 0, 0);
	mlx_loop(img.mlx);
}