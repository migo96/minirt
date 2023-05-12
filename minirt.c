/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 11:53:10 by migo              #+#    #+#             */
/*   Updated: 2023/05/12 13:13:20 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_plane	*set_plane(char *map);
t_cylinder	*set_cylinder(char *map);
t_sphere	*set_sphere(char *map);

t_object	*ft_lstnew(int nb, char *map)
{
	t_object	*str;

	str = (t_object *)malloc(sizeof(t_object) * 1);
	if (str == 0)
		return (0);
	str->type = nb;
	if (nb == 0)
		str->object = (void *)set_sphere(map);
	if (nb == 1)
		str->object = (void *)set_cylinder(map);
	if (nb == 2)
		str->object = (void *)set_plane(map);
	str->next = NULL;
	return (str);
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

double	ft_atof(char **map)
{
	double	num;
	double	flag;
	double	m_flag;
	double	power;

	num = 0;
	flag = 0;
	m_flag = 1;
	power = 1;
	while (*map[0] == ' ')
		(*map)++;
	while (*map[0] == ',')
		(*map)++;
	if (*map[0] == '-')
	{
		m_flag = -1;	
		(*map)++;
	}
	while (1)
	{
		if (*map[0] <= '9' && *map[0] >= '0')
		{
			num = num * 10 + (*map[0] - 48);
			if (flag == 1)
				power *= 10;
		}
		else if (*map[0] == '.')
			flag++;
		else
			break ;
		if (flag == 2)
			break ;
		(*map)++;
	}
	if (*map[0] != ' ' && *map[0] != '\0' && *map[0] !='\n')
		exit(1);
	return (num * m_flag / power);
}

void	set_camera(t_camera *camera, char *map)
{
	map++;
	camera->location.x = ft_atof(&map);
	camera->location.y = ft_atof(&map);
	camera->location.z = ft_atof(&map);
	camera->view_point.x = ft_atof(&map);
	camera->view_point.y = ft_atof(&map);
	camera->view_point.z = ft_atof(&map);
	camera->fov = tan(degrees_to_radians(ft_atof(&map) / 2.0));
}

void	set_light(t_light *light, char *map)
{
	map++;
	light->location.x = ft_atof(&map);
	light->location.y = ft_atof(&map);
	light->location.z = ft_atof(&map);
	light->power = ft_atof(&map);
}

t_sphere	*set_sphere(char *map)
{
	t_sphere	*sphere;

	map = map + 2;
	sphere = malloc(sizeof(t_sphere) * 1);
	sphere->center = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	sphere->radius = ft_atof(&map);
	sphere->color = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	return (sphere);
}

t_plane	*set_plane(char *map)
{
	t_plane *plane;

	map = map + 2;
	plane = malloc(sizeof(t_plane) * 1);
	plane->center = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	plane->normal = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	plane->color = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	return (plane);
}

t_cylinder	*set_cylinder(char *map)
{
	t_cylinder *cy;

	map = map + 2;
	cy = malloc(sizeof(t_cylinder) * 1);
	cy->center = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	cy->normal = unit_vector(make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map)));
	cy->radius = ft_atof(&map);
	cy->height = ft_atof(&map);
	cy->color = make_vec(ft_atof(&map), ft_atof(&map), ft_atof(&map));
	cy->top = v_sub(cy->center, v_mul_n(cy->normal, cy->height / 2));
	cy->botton = v_add(cy->center, v_mul_n(cy->normal, cy->height / 2));
	cy->h = unit_vector(v_sub(cy->top, cy->botton));
	return (cy);
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
			ft_lstadd_front(&set->objects, ft_lstnew(0, map));
		if (map[0] == 'p' && map[1] == 'l')
			ft_lstadd_front(&set->objects, ft_lstnew(2, map));
		if (map[0] == 'c' && map[1] == 'y')
			ft_lstadd_front(&set->objects, ft_lstnew(1, map));
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
	horizontal = v_mul_n(horizontal, t * 1200 / 800);
	camera->hor = horizontal;
	vertical = cross(horizontal, camera->view_point);
	t = sqrt(pow(camera->fov, 2) / dot(vertical, vertical));
	vertical = v_mul_n(vertical, t);
	camera->ver = vertical;
	if (z_axis.x == 0 && z_axis.y == 0)
	{
		horizontal = make_vec(1, 0, 0);
		t = sqrt(pow(camera->fov, 2) / dot(horizontal, horizontal));
		horizontal = v_mul_n(horizontal, t);
		camera->hor = horizontal;
		vertical = make_vec(0, 1, 0);
		t = sqrt(pow(camera->fov, 2) / dot(vertical, vertical));
		vertical = v_mul_n(vertical, t * 800 / 1200);
		camera->ver = vertical;
	}
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

double hit_plane(t_plane *pl, t_ray r)
{
	double	coefficient;
	double	constant;

	coefficient = r.dir.x * pl->normal.x + r.dir.y * pl->normal.y + r.dir.z * pl->normal.z;
	constant = pl->normal.x * (pl->center.x - r.orig.x) + pl->normal.y * (pl->center.y - r.orig.y) + pl->normal.z * (pl->center.z - r.orig.z);
	if (coefficient == 0)
		return (-1);
	return (constant / coefficient);
}

double	hit_sphere(t_sphere *s, t_ray r)
{
	t_vec	oc;
	double	a;
	double	half_b;
	double	c;
	double	discriminant;

	oc = v_sub(r.orig, s->center);
	a = length_squared(r.dir);
	half_b = dot(oc, r.dir);
	c = length_squared(oc) - s->radius * s->radius;
	discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
		return (-1.0);
	else
		return ((- half_b - sqrt(discriminant)) / a);
}

int      hit_cylinder_cap(t_cylinder *cy, t_object *ob, t_ray ray, double *t)
{
    t_vec   center[2];
    double  coefficient;
    double  constant[2];
    double  ts[2];
	
    coefficient = dot(ray.dir, cy->normal);
    if (coefficient == 0)
        return (0);
    center[0] = v_add(cy->center, v_mul_n(cy->normal, cy->height / 2));
    constant[0] = dot(cy->normal, v_sub(center[0], ray.orig));
    ts[0] = constant[0] / coefficient;
	center[1] = v_add(cy->center, v_mul_n(cy->normal, -cy->height / 2));
    constant[1] = dot(cy->normal, v_sub(center[1], ray.orig));
    ts[1] = constant[1] / coefficient;
	if (length(v_sub(ray.orig, at(ray, ts[0]))) < length(v_sub(ray.orig, at(ray, ts[1]))))
	{
    	if (cy->radius >= length(v_sub(center[0], at(ray, ts[0]))))
		{
			ob->hit_part = 1;
    	    return (*t = constant[0] / coefficient);
		}
	}
	else
	{
    	if (cy->radius >= length(v_sub(center[1], at(ray, ts[1]))))
		{
			ob->hit_part = 2;
    	    return (*t = constant[1] / coefficient);
		}
	}
	return (0);
}
double  hit_cylinder(t_cylinder *cy, t_object *ob, t_ray r)
{
    t_vec   oc;
    double  a;
    double  half_b;
    double  c;
    double  discriminant;
    double  t;
    r.dir = unit_vector(r.dir);
    oc = v_sub(r.orig, cy->center);
    a = pow(dot(r.dir, cy->normal), 2) - 1;
    half_b =  dot(oc, cy->normal) * dot(r.dir, cy->normal) - dot(oc, r.dir);
    c = cy->radius * cy->radius - dot(oc, oc) +  pow(dot(oc, cy->normal), 2);
	a *= -1;
	half_b *= -1;
	c *= -1;
	// a = length_squared(cross(r.dir, cy->normal));
	// half_b = dot(cross(r.dir, cy->normal), cross(oc, cy->normal));
	// c = length_squared(cross(oc, cy->normal)) - pow(cy->radius, 2);
	// printf("%f %f %f\n", a, half_b, c);
	// a = (pow(dot(r.dir, cy->normal), 2) - dot(r.dir, r.dir)) * -1;
	// half_b =  (dot(oc, cy->normal) * dot(r.dir, cy->normal) - dot(oc, r.dir)) * -1;
	// c = (pow(dot(oc, cy->normal), 2) - dot(oc, oc) + cy->radius * cy->radius) * -1;
	// printf("%f %f %f\n", a, half_b, c);
    discriminant = half_b * half_b - a * c;
    t = (- half_b - sqrt(discriminant)) / a;
    if ((discriminant < 0 \
    || fabs(dot(v_sub(cy->center, at(r, t)), cy->normal)) > cy->height / 2\
    && !hit_cylinder_cap(cy, ob, r, &t)))
        return (-1.0);
    else
    {
        return (t);
    }
}

int	cy_boundary(t_cylinder *cy, t_vec contact)
{
	double	hit_height;
	double	max_height;

	hit_height = dot(v_sub(contact, cy->center), cy->normal);
	max_height = cy->height;
	if (fabs(hit_height) > max_height)
		return (0);
	return (1);
}

int	set_color(t_vec ob_color, double ratio, double light, double power)
{
	int	color_red;
	int color_green;
	int color_blue;
	int color;
	double ratio1;

	if (ratio < 0)
		ratio = 0;
	ratio1 = (ratio + light) * power;
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
	color = (color_red * 65536 + color_green * 256 + color_blue);
	return (color);
}

int	hit_something(t_set *set, t_ray contact)
{
	t_sphere	*sp;
	t_cylinder	*cy;
	t_object	*ob;
	t_plane		*pl;
	double		t;
	double		length;
	t_vec		near;

	ob = set->objects;
	while (ob)
	{
		if (ob->type == 0)
		{
			sp = ob->object;
			t = hit_sphere(sp, contact);
			if (t > 0)
			{
				ob->check = t;
				near = at(contact, t);
				ob ->length2 = length_squared(near);
			}
			else
				ob->length2 = 184467440737095516;
		}
		else if (ob->type == 1)
		{
			cy = ob->object;
			t = hit_cylinder(cy, ob, contact);
			if (t > 0)
			{
				ob->check = t;
				near = at(contact, t);
				ob ->length2 = length_squared(near);
			}
			else
				ob->length2 = 184467440737095516;
		}
		else if (ob->type == 2)
		{
			pl = ob->object;
			t = hit_plane(pl, contact);
			if (t > 0)
			{
				ob->check = t;
				near = at(contact, t);
				ob ->length2 = length_squared(near);
			}
			else
				ob->length2 = 184467440737095516;
		}
		ob = ob->next;
	}
	length = -1;
	ob = set->objects;
	while (ob)
	{
		if (length == -1)
		{
			length = ob->length2;
			t = ob->check;
		}
		else
		{
			if (length > ob->length2)
			{
				t = ob->check;
				length = ob->length2;
			}
		}
		ob = ob->next;
	}
	if (length == 184467440737095516)
		return (0);
	return (t);
}

double  ratio_cy(t_ray r, double t, t_object *ob, t_set *set)
{
    t_ray       contact;
    t_vec       normal;
    t_vec       center_vec;
    t_cylinder  *cylinder;
    double      ratio;
	t_vec		check;
	
    cylinder = ob->object;
	r.dir = unit_vector(r.dir);
	if (ob->hit_part == 0)
	{
    	contact.orig = at(r,t);
		printf("r.dir %f %f %f\n", r.dir.x, r.dir.y, r.dir.z);
		printf("r.orig %f %f %f\n", r.orig.x, r.orig.y, r.orig.z);
		printf("t %f\n", t);
    	center_vec = v_add(cylinder->center, \
    	v_mul_n(cylinder->normal, dot(v_sub(contact.orig, cylinder->center), cylinder->normal)));
    	normal = unit_vector(v_sub(contact.orig, center_vec));
    	contact.dir = unit_vector(v_sub(set->light.location, contact.orig));
    	ratio = dot(contact.dir, normal) / length(normal) * length(contact.dir);
    	ob->color = cylinder->color;
    	ob->ratio = ratio;
		printf("!!!!%f %f %f\n", contact.orig.x, contact.orig.y, contact.orig.z);
    	ob->length = length_squared(v_sub(set->camera.location, contact.orig));
	}
	else if (ob->hit_part == 1)
	{
		contact.orig = at(r,t);
		contact.dir = unit_vector(v_sub(set->light.location, contact.orig));
		ratio = dot(contact.dir, cylinder->normal) / length(cylinder->normal) * length(contact.dir);
		ob->color = cylinder->color;
		ob->ratio = ratio;
		ob->length = length_squared(v_sub(set->camera.location, contact.orig));

	}
	else
	{
		contact.orig = at(r,t);
		contact.dir = unit_vector(v_sub(set->light.location, contact.orig));
		normal = v_mul_n(cylinder->normal, -1);
		ratio = dot(contact.dir, normal) / length(normal) * length(contact.dir);
		ob->color = cylinder->color;
		ob->ratio = ratio;
		ob->length = length_squared(v_sub(set->camera.location, contact.orig));
	}
	// printf("@@%f %f %f\n", contact.orig.x ,contact.orig.y, contact.orig.z);
	// contact.orig.x += contact.dir.x;
	// contact.orig.y += contact.dir.y;
	// contact.orig.z += contact.dir.z;
	// t = hit_something(set, contact);
	// if (t != 0)
	// {
	// 	check = at(contact, t);
	// 	if (set->light.location.y > contact.orig.y)
	// 	{
	// 		if (contact.orig.y < check.y && check.y < set->light.location.y)
	// 			ob->ratio = 0;
	// 	}
	// 	else
	// 	{
	// 		if (set->light.location.y < check.y && check.y < contact.orig.y)
	// 			ob->ratio = 0;
	// 	}
	// }
    return (ratio);
}

double	ratio_sp(t_ray r, double t, t_object *ob, t_set *set)
{
	t_ray	contact;
	t_vec	normal;
	t_sphere *sphere;
	double	ratio;
	t_vec	check;

	sphere = ob->object;
	contact.orig = at(r,t);
	normal = unit_vector(v_sub(at(r, t), sphere->center));
	contact.dir = unit_vector(v_sub(set->light.location, contact.orig));
	ratio = dot(contact.dir, normal) / length(normal) * length(contact.dir);
	ob->color = sphere->color;
	ob->ratio = ratio;
	ob->length = length_squared(v_sub(set->camera.location, contact.orig));
	t = hit_something(set, contact);
	if (t != 0)
	{
		check = at(contact, t);
		if (set->light.location.y > contact.orig.y)
		{
			if (contact.orig.y < check.y && check.y < set->light.location.y)
				ob->ratio = 0;
		}
		else
		{
			if (set->light.location.y < check.y && check.y < contact.orig.y)
				ob->ratio = 0;
		}
	}
	return (ratio);
}

double	ratio_pl(t_ray r, double t, t_object *ob, t_set *set)
{
	t_ray	contact;
	t_vec	normal;
	double	ratio;
	t_vec	check;
	t_plane *pl;

	pl = ob->object;
	contact.orig = at(r,t);
	contact.dir = unit_vector(v_sub(set->light.location, contact.orig));
	ratio = dot(contact.dir, pl->normal) / length(pl->normal) * length(contact.dir);
	ob->color = pl->color;
	ob->ratio = ratio;
	ob->length = length_squared(v_sub(set->camera.location, contact.orig));
	contact.orig.x += contact.dir.x;
	contact.orig.y += contact.dir.y;
	contact.orig.z += contact.dir.z;
	t = hit_something(set, contact);
	if (t > 0)
	{
		check = at(contact, t);
		if (set->light.location.x > contact.orig.x)
		{
			if (contact.orig.x < check.x && check.x < set->light.location.x)
				ob->ratio = 0;
		}
		else
		{
			if (set->light.location.x < check.x && check.x < contact.orig.x)
				ob->ratio = 0;
		}
	}
	return (ratio);
}

int	ray_color(t_ray r, t_set *set)
{
	double	t;
	t_sphere	*sp;
	t_cylinder	*cy;
	t_object	*ob;
	t_plane		*pl;
	t_vec		color;
	t_vec		contact;
	double		ratio;
	double		length;

	ob = set->objects;
	while (ob)
	{
		if (ob->type == 0)
		{
			sp = ob->object;
			t = hit_sphere(sp, r);
			if (t > 0.0)
				ratio_sp(r, t, ob, set);
			else
				ob->length = 184467440737095516;
		}
		else if (ob->type == 1)
		{
			cy = ob->object;
			ob->hit_part = 0;
			t = hit_cylinder(cy, ob, r);
			ob->length = 184467440737095516;
			if (t != -1)
			{
				ratio_cy(r, t, ob ,set);
			}
		}
		else if (ob->type == 2)
		{
			pl = ob->object;
			t = hit_plane(pl, r);
			if (t > 0)
				ratio_pl(r, t, ob, set);
			else
			{
				color = make_vec(0, 0, 0);
				ob->length = 184467440737095516;
			}
		}
		ob = ob->next;
	}
	length = -1;
	ob = set->objects;
	while (ob)
	{
		if (length == -1)
		{
			ratio = ob->ratio;
			length = ob->length;
			color = ob->color;
		}
		else
		{
			if (length > ob->length)
			{
				ratio = ob->ratio;
				length = ob->length;
				color = ob->color;
			}
		}
		ob = ob->next;
	}
	if (length == 184467440737095516)
		return (0);
	return (set_color(color, ratio, 0.2, set->light.power));
}

void	render(t_data *data, t_set *set, double height, double width)
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
			r.orig = set->camera.location;
			r.dir = v_add(set->camera.lower_left_corner,
					v_sub(v_add(v_mul_n(set->camera.hor, u),
							v_mul_n(set->camera.ver, v)), set->camera.location));
			my_mlx_pixel_put(data, i, j, ray_color(r, set));
		}
	}
}

int	main(int argc, char **argv)
{
	t_set	set;
	t_data	img;
	t_vec	lower_left_corner;
	t_cylinder *cy;

	if (argc != 2)
		return (printf("parameter error"));
	checkmap(argv, &set);
	set.camera.lower_left_corner = set_lower_left_corner(&set.camera);
	img.mlx = mlx_init();
	img.win = mlx_new_window(img.mlx, 1200, 800, "ray tracing");
	img.img = mlx_new_image(img.mlx, 1200, 800);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel,
			&img.line_length, &img.endian);
	render(&img, &set, 800, 1200);
	mlx_put_image_to_window(img.mlx, img.win, img.img, 0, 0);
	mlx_loop(img.mlx);
}