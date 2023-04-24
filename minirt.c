/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 11:53:10 by migo              #+#    #+#             */
/*   Updated: 2023/04/24 18:37:41 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	set_camera(t_camera *camera, char *map)
{
	camera->fov = tan(degrees_to_radians(45)) * 2;
	camera->location.x = 10;
	camera->location.y = 0;
	camera->location.z = 0;
	camera->view_point.x = -1;
	camera->view_point.y = 0;
	camera->view_point.z = 0;
}

void	set_light(t_light *light, char *map)
{
	light->location.x = 0;
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
	sphere[1].color.x = 0;
	sphere[1].color.y = 0;
	sphere[1].color.z = 255;
	sphere[1].radius = 90;
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
	z_axis = make_vec(1,1,1);
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

int	ray_color(t_ray r, t_set set)
{
	double	t;
	t_ray	contact;
	t_vec	normal;
	double	ratio;

	t = hit_sphere(set.sphere[0], r);
	if (t > 0.0)
	{
		contact.orig = at(r,t);
		normal = unit_vector(v_sub(at(r, t), set.sphere[0].center));
		contact.dir = unit_vector(v_sub(set.light.location, contact.orig));
		ratio = dot(contact.dir, normal) / length(normal) * length(contact.dir);
		if (ratio < 0)
			ratio *= -1;
		// printf("ori %f %f %f\n dir %f %f %f\n t %f\n", contact.orig.x, contact.orig.y, contact.orig.z, contact.dir.x, contact.dir.y, contact.dir.z, t);
		// if (hit_sphere(set.sphere[0], contact) != -1)
		// 	return (0);
		// if (hit_sphere(set.sphere[1], contact) != -1)
		// 	return (0);
		// printf("color %f\n", set.sphere[0].color.x * 256 * 256 * ratio + set.sphere[0].color.y * 256 * ratio + set.sphere[0].color.z * ratio);
		return (set.sphere[0].color.x * 256 * 256 * ratio + set.sphere[0].color.y * 256 * ratio + set.sphere[0].color.z * ratio);
	}
	t = hit_sphere(set.sphere[1], r);
	if (t > 0.0)
	{
		contact.orig = at(r,t);
		normal = unit_vector(v_sub(at(r, t), set.sphere[0].center));
		contact.dir = unit_vector(v_sub(set.light.location, contact.orig));
		ratio = dot(contact.dir, normal) / length(normal) * length(contact.dir);
		if (ratio < 0)
			ratio *= -1;
		if (hit_sphere(set.sphere[0], contact) != -1)
			return (0);
		// if (hit_sphere(set.sphere[1], contact) != -1)
		// 	return (0);
		return (set.sphere[1].color.x * 256 * 256 * ratio + set.sphere[1].color.y * 256 * ratio + set.sphere[1].color.z * ratio);
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
	printf("%f %f %f\n", set.camera.lower_left_corner.x, set.camera.lower_left_corner.y, set.camera.location.z);
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
			printf("%f %f %f\n", r.dir.x, r.dir.y, r.dir.z);
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