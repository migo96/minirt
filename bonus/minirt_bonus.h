/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:24:03 by dmin              #+#    #+#             */
/*   Updated: 2023/06/05 13:32:56 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_BONUS_H
# define MINIRT_BONUS_H

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include "mlx.h"
# include "get_next_line_bonus.h"

# define SPHERE 0
# define CYLINDER 1
# define PLANE 2
# define CONE 3
# define HYPER 4

# define WIDTH 1200
# define HEIGHT 800

typedef struct s_vec
{
	double	x;
	double	y;
	double	z;
}		t_vec;

typedef struct s_light
{
	t_vec			loc;
	double			power;
	struct s_light	*next;
}		t_light;

typedef struct s_ray
{
	t_vec	orig;
	t_vec	dir;
}		t_ray;

typedef struct s_cone
{
	t_vec	center;
	t_vec	normal;
	double	height;
	double	radius;
	double	tran;
}		t_cone;

typedef struct s_circle
{
	t_vec	center;
	t_vec	normal;
	double	height;
	double	radius;
	double	tran;
}		t_circle;

typedef struct s_hyper
{
	t_vec	center;
	t_vec	normal;
	double	a;
	double	b;
	double	c;
	double	height;
	double	tran;	
}		t_hyper;

typedef struct s_checker
{
	int		width;
	int		height;
	t_vec	color_a;
	t_vec	color_b;
}		t_checker;

typedef struct s_sphere
{
	t_vec	center;
	double	radius;
	double	tran;
	double	mty;
}		t_sphere;

typedef struct s_am_light
{
	double	am_light;
	t_vec	color;
}		t_am_light;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}		t_img;

typedef struct s_plane
{
	t_vec	center;
	t_vec	normal;
	double	tran;
	double	mty;
}		t_plane;

typedef struct s_cylinder
{
	t_vec	center;
	t_vec	normal;
	double	height;
	double	radius;
	double	tran;
}		t_cylinder;

typedef struct s_cam
{
	t_vec	loc;
	t_vec	vec;
	t_vec	hor;
	t_vec	ver;
	t_vec	lower_left_corner;
	double	fov;
}		t_cam;

typedef struct s_set
{
	t_cam			cam;
	t_am_light		am_light;
	struct s_light	*light;
	struct s_object	*objects;
	struct s_data	*img2;
}		t_set;

typedef struct s_object
{
	int				type;
	double			ratio;
	double			length;
	double			hit_part;
	t_vec			color;
	void			*object;
	double			(*hit_f)(struct s_object*, t_ray);
	void			(*ratio_f)(t_ray, double, struct s_object*, struct s_set*);
	double			refl;
	double			refr;
	double			tran;
	int				rank;
	struct s_object	*next;
}		t_object;

typedef struct s_data
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
	t_set	set;
}	t_data;

t_vec		make_vec(double x, double y, double z);
t_vec		v_mul_n(t_vec v1, float n);
t_vec		v_mul(t_vec v1, t_vec v2);
t_vec		v_sub(t_vec v1, t_vec v2);
t_vec		v_add(t_vec v1, t_vec v2);
t_vec		v_div_n(t_vec v1, float n);
t_vec		cross(t_vec v1, t_vec v2);
double		degrees_to_radians(double degrees);
double		dot(t_vec v1, t_vec v2);
double		length_squared(t_vec e);
t_vec		at(t_ray r, double t);
t_vec		unit_vector(t_vec v);
double		length(t_vec e);
t_vec		v_add_n(t_vec v1, double n);
int			fl_color(t_vec color);
t_vec		ch_color(int color);
void		set_fr_fl_color(char **map, t_object *ob);

t_object	*ft_lstnew(int nb, char *map);
t_light		*l_lstnew(char *map);
void		l_lstadd_front(t_light **lst, t_light *new);
void		ft_lstadd_front(t_object **lst, t_object *new);
void		ft_lstclear(t_object **lst);
void		l_lstclear(t_light **lst);

int			is_file_name_ok(char *filename);
double		skip_space_comma(char **map);
double		ft_atof(char **map);
void		checkmap(char **argv, t_set	*set);
void		my_mlx_pixel_put(t_data *data, int x, int y, int color);
int			gain_color(t_data *data, int x, int y);

t_vec		set_color(t_vec ob_color, double ratio, t_am_light am);
t_vec		ray_color(t_ray r, t_set *set, double light_power, t_object *check);
int			re_color(t_ray r, t_set *set);

void		check_color(t_vec color);
void		check_viewpoint(t_vec vec);
void		set_cam(t_cam *cam, char *map);
void		set_light(t_light *light, char *map);
void		set_am_light(t_am_light *am_light, char *map);

t_plane		*set_plane(char *map, t_object *ob);
t_cylinder	*set_cylinder(char *map, t_object *ob);
t_sphere	*set_sphere(char *map, t_object *ob);

t_vec		set_lower_left_corner(t_cam *cam);

double		hit_plane(t_object *ob, t_ray r);
double		hit_sphere(t_object *ob, t_ray r);
double		hit_cylinder(t_object *ob, t_ray r);
int			hit_cylinder_cap(t_cylinder *cy, t_object *ob, \
			t_ray ray, double *t);
void		hit_something(t_set *set, t_ray contact, t_object *obj);
int			close_cylinder_cap(t_ray ray, double result1, double result2);

void		ratio_cy(t_ray r, double t, t_object *ob, t_set *set);
void		ratio_sp(t_ray r, double t, t_object *ob, t_set *set);
void		ratio_pl(t_ray r, double t, t_object *ob, t_set *set);
void		ratio_cir(t_ray r, double t, t_object *ob, t_set *set);

void		hit_range(t_object *ob, t_set *set, t_ray contact, double t);
void		set_obj(t_object *ob, t_set *set, t_vec normal, t_ray con);

void		rt_hook(t_data *img);
int			rt_close(void *param);

t_circle	*set_circle(char *map, t_object *ob, int norm_dir);
t_cone		*set_cone(char *map, t_object *ob);
t_hyper		*set_hyper(char *map, t_object *ob);

double		hit_circle(t_object *ob, t_ray r);
double		hit_hyper(t_object *ob, t_ray r);
double		hit_cone(t_object *ob, t_ray r);			

void		ratio_cn(t_ray r, double t, t_object *ob, t_set *set);
void		ratio_hy(t_ray r, double t, t_object *ob, t_set *set);

void		set_obj_bo(t_object *ob, t_set *set, t_vec normal, t_ray con);
void		classification_map_bo(char *map, int i, t_set *set);

t_ray		refract_color(t_ray r, double t, t_object *ob);
t_vec		get_refr_sp_norm(t_ray r, double t, t_object *ob, t_ray contact);
t_vec		get_refr_cy_norm(t_ray r, double t, t_object *ob, t_ray contact);
t_vec		get_refr_pl_norm(t_ray r, double t, t_object *ob, t_ray contact);
t_vec		get_refr_cn_norm(t_ray r, double t, t_object *ob, t_ray contact);
t_vec		get_refr_cir_norm(t_ray r, double t, t_object *ob, t_ray contact);

t_ray		reflect_color(t_ray r, double t, t_object *ob);
t_vec		get_refl_sp_norm(t_ray r, double t, t_object *ob, t_ray contact);
t_vec		get_refl_cy_norm(t_ray r, double t, t_object *ob, t_ray contact);
t_vec		get_refl_pl_norm(t_ray r, double t, t_object *ob, t_ray contact);
t_vec		get_refl_cn_norm(t_ray r, double t, t_object *ob, t_ray contact);
t_vec		get_refl_cir_norm(t_ray r, double t, t_object *ob, t_ray contact);

int			check_in_sp(t_sphere *sp, t_set *set, t_ray contact);
int			check_in_cy(t_cylinder *cy, t_set *set, t_vec normal);
int			check_in_cn(t_cone *cn, t_set *set, t_vec normal);

t_vec		sp_map(t_ray con, t_sphere *sp, t_set *set, t_vec *normal);
t_vec		pl_map(t_ray con, t_plane *pl, t_set *set, t_vec *normal);

#endif
