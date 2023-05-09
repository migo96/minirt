/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 11:54:18 by migo              #+#    #+#             */
/*   Updated: 2023/05/09 14:39:08 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include "mlx.h"
# include "get_next_line.h"

typedef struct s_vec
{
	double	x;
	double	y;
	double	z;
}		t_vec;

typedef struct s_light
{
	t_vec	location;
	double	power;
}		t_light;

typedef struct s_ray
{
	t_vec	orig;
	t_vec	dir;
}		t_ray;

typedef struct s_sphere
{
	t_vec	center;
	double	radius;
	t_vec	color;
}		t_sphere;

typedef struct s_plane
{
	t_vec	center;
	t_vec	normal;
	t_vec	color;
}		t_plane;

typedef struct s_cylinder
{
	t_vec	center;
	t_vec	normal;
	double	height;
	double	radius;
	t_vec	top;
	t_vec	botton;
	t_vec	h;
	t_vec	color;
}		t_cylinder;

typedef struct s_camera
{
	t_vec	location;
	t_vec	view_point;
	t_vec	hor;
	t_vec	ver;
	t_vec	lower_left_corner;
	double	fov;
}		t_camera;

typedef struct s_object
{
	int				type;
	double			ratio;
	double			length;
	double			length2;
	double			check;
	t_vec			color;
	void			*object;
	struct s_object	*next;
}		t_object;

typedef struct s_set
{
	t_camera	camera;
	t_light		light;
	t_object	*objects;
}		t_set;

typedef struct s_data
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int	line_length;
	int	endian;
}t_data;

t_vec	make_vec(double x, double y, double z);
t_vec	v_mul_n(t_vec v1, float n);
t_vec	v_mul(t_vec v1, t_vec v2);
t_vec	v_sub(t_vec v1, t_vec v2);
t_vec	v_add(t_vec v1, t_vec v2);
t_vec	v_div_n(t_vec v1, float n);
t_vec	cross(t_vec v1, t_vec v2);
double	degrees_to_radians(double degrees);
float	dot(t_vec v1, t_vec v2);
float	length_squared(t_vec e);
t_vec	at(t_ray r, double t);
t_vec	unit_vector(t_vec v);
float	length(t_vec e);

#endif
