#include "mlx.h"
#include <stdlib.h>

#include <math.h>

typedef struct	s_vec3
{
	float x;
	float y;
	float z;
}		t_vec3;

typedef	struct s_mlx
{
	void *mlx_ptr;
	void *win_ptr;
	void *img_ptr;
	int  *data;
	int		bpp;
	int	size_l;
	int	endian;

	t_vec3	color;
	int		int_color;

} t_mlx;

t_vec3	make_vec(float n)
{
	t_vec3 result;

	result.x = n;
	result.y = n;
	result.z = n;
	return (result);
}
t_vec3	v_mul_n(t_vec3 v1, float n)
{
	t_vec3	result;

	result.x = v1.x * n;
	result.y = v1.y * n;
	result.z = v1.z * n;
	return (result);
}

t_vec3	v_mul(t_vec3 v1, t_vec3 v2)
{
	t_vec3	result;

	result.x = v1.x * v2.x;
	result.y = v1.y * v2.y;
	result.z = v1.z * v2.z;
	return (result);
}

t_vec3	v_sub(t_vec3 v1, t_vec3 v2)
{
	t_vec3	result;

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return (result);
}

t_vec3	v_add(t_vec3 v1, t_vec3 v2)
{
	t_vec3	result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return (result);
}

t_vec3	v_div_n(t_vec3 v1, float n)
{
	t_vec3	result;

	result.x = v1.x / n;
	result.y = v1.y / n;
	result.z = v1.z / n;
	return (result);
}
float	dot(t_vec3 v1, t_vec3 v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

float	length_squared(t_vec3 e)
{
	return (e.x * e.x + e.y * e.y + e.z * e.z);
}

float	length(t_vec3 e)
{
	return (sqrt(length_squared(e)));
}

t_vec3	unit_vector(t_vec3 v)
{
	return (v_div_n(v, length(v)));
}

t_vec3 cross(t_vec3 v1, t_vec3 v2)
{
	t_vec3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return (result);
}

void	write_color(t_mlx *app, t_vec3 c)
{
	int	ir = 255.999 * c.x;
	int	ig = 255.999 * c.y;
	int	ib = 255.999 * c.z;

	app->color.x = ir * 256 * 256;
	app->color.y = ig * 256;
	app->color.z = ib;
	app->int_color = app->color.x + app->color.y + app->color.z;
}

// ray.h

typedef struct	s_ray
{
	t_vec3	orig;
	t_vec3	dir;
}		t_ray;


//hittable
// #include "ray.h"

// sphere.h

typedef struct 	s_sphere
{
	t_vec3	center;
	double	radius;

}		t_sphere;

double	hit_sphere(t_sphere s, t_ray r)
{	
	t_vec3 oc = v_sub(r.orig, s.center);
	float a = length_squared(r.dir);
	float b = 2 * dot(oc, r.dir);
	float c = length_squared(oc) - s.radius * s.radius;
	float discriminant = b * b - (a * c * 4);
	if (discriminant < 0)
		return (-1.0);
	else
		return ((- b - sqrt(discriminant)) / (2.0) * a);
}

//hittable.h
typedef struct s_hit_record
{
	t_vec3	p;
	t_vec3	normal;
	double	t;
	int	front_face;
}		t_hit_record;

//ray.h function
//
t_vec3	origin(t_ray r)
{
	return (r.orig);
}

t_vec3	direction(t_ray r)
{
	return (r.dir);
}

t_vec3	at(t_ray r, double t)
{
	return (v_add(r.orig, v_mul_n(r.dir, t)));
}

int	hit(t_ray r, double t_min, double t_max, t_hit_record rec, t_sphere s);

typedef struct	s_hittable_list
{
	t_sphere	sp[2];
}		t_hittable_list;

int	hittable_list_hit(t_ray r, double t_min, double t_max, t_hit_record rec, t_hittable_list world);

t_vec3	ray_color(t_ray r, t_hittable_list world)
{
	float t = hit_sphere(world.sp[0], r);
	if (t > 0.0)
	{
		t_vec3 n = unit_vector(v_sub(at(r, t), world.sp[0].center));
		t_vec3 color;
		color.x = n.x + 1;
		color.y = n.y + 1;
		color.z = n.z + 1;

		return (v_mul_n(color, 0.5));

	}

	t = hit_sphere(world.sp[1], r);
	if (t > 0.0)
	{
		t_vec3 n = unit_vector(v_sub(at(r, t), world.sp[1].center));
		t_vec3 color;
		color.x = n.x + 1;
		color.y = n.y + 1;
		color.z = n.z + 1;

		return (v_mul_n(color, 0.5));

	}
	t_vec3 unit_direction = unit_vector(r.dir);
	t = 0.5 * (unit_direction.y + 1.0);
	t_vec3 a= make_vec(1.0);
	t_vec3 b; b.x = 0.5; b.y = 0.7; b.z = 1.0;

	return (v_add(v_mul_n(a, 1.0 - t), v_mul_n(b, t)));
}


//hittable.h function
void	set_face_normal(t_vec3 direction, t_vec3 normal, t_vec3 outward_normal, int front_face)
{
	front_face = dot(direction, outward_normal) < 0 ? 1 : 0;
	normal = front_face ? outward_normal : v_mul_n(outward_normal, -1);
}


int	hit(t_ray r, double t_min, double t_max, t_hit_record rec, t_sphere s)
{
	t_vec3 oc = v_sub(r.orig, s.center);
	float a = length_squared(r.dir);
	float half_b = dot(oc, r.dir);
	float c = length_squared(oc) - s.radius * s.radius;
	float discriminant = half_b * half_b - a * c;

	if (discriminant > 0)
	{
		float root = sqrt(discriminant);
		float temp = (-half_b - root) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = at(r, rec.t);
			t_vec3 outward_normal  = v_div_n(v_sub(rec.p, s.center), s.radius);
			set_face_normal(r.dir, rec.normal, outward_normal, rec.front_face);
			return (1);
		}
		temp = (-half_b + root) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = at(r, rec.t);
			t_vec3 outward_normal = v_div_n(v_sub(rec.p, s.center), s.radius);
			set_face_normal(r.dir, rec.normal, outward_normal, rec.front_face);
			return (1);
		}
	}
	return (0);
}

int	hittable_list_hit(t_ray r, double t_min, double t_max, t_hit_record rec, t_hittable_list world)
{
	t_hit_record	temp_rec;
	int	hit_anything = 0;
	float	closest_so_far = t_max;

	int	i = 0;

	while (i < 2)
	{
		if (hit(r, t_min, closest_so_far, temp_rec, world.sp[i]))
		{
			hit_anything = 1;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
		i++;
	}
	return (hit_anything);
}

// rtweekend.h

#define PI 3.1415926535897932385

double	degrees_to_radians(double degrees)
{
	return (degrees * PI / 180.0);
}

//


#include <stdio.h>

int	main()
{
	// Image
	const float	aspect_ratio = 4.0 / 3.0;
	const int	image_width = 800;
	const int	image_height = image_width / aspect_ratio;

	// Start mlx
	t_mlx	*app;
	if (!(app = (t_mlx*)malloc(sizeof(t_mlx))))
		return (-1);
	app->mlx_ptr = mlx_init();
	app->win_ptr = mlx_new_window(app->mlx_ptr, 800, 600, "raytracer");
	app->img_ptr = mlx_new_image(app->mlx_ptr, image_width, image_height);
	app->data = (int *)mlx_get_data_addr(app->img_ptr, &app->bpp, &app->size_l, &app->endian);

	// World
	t_hittable_list	world;

	world.sp[0].center.x = 5;
	world.sp[0].center.y = 5;
	world.sp[0].center.z = 5;
	world.sp[0].radius = 0.5;

	world.sp[1].center.x = 0;
	world.sp[1].center.y = 0;
	world.sp[1].center.z = -100;
	world.sp[1].radius = 100;

	// Camera
	float	viewport_height = tan(degrees_to_radians(45)) * 2;
	float	viewport_width = tan(degrees_to_radians(45)) * 2;;
	float	t = 0;

	t_vec3	z = {0,0,1};
	t_vec3	origin = {10,10,10};
	t_vec3 any = {1, 1, 1};
	t_vec3 z_axis = {-1, -1, -1};
	t_vec3	horizontal = cross(z_axis, z);
	t = sqrt(pow(viewport_width, 2) / dot(horizontal, horizontal));
	horizontal = v_mul_n(horizontal, t);
	t_vec3	vertical = cross(horizontal, z_axis);
	t = sqrt(pow(viewport_height, 2) / dot(vertical, vertical));
	vertical = v_mul_n(vertical, t);
	printf("%f %f %f\n", horizontal.x, horizontal.y, horizontal.z);
	printf("%f %f %f\n", vertical.x, vertical.y, vertical.z);
	t_vec3	lower_left_corner = v_sub(origin, v_add(v_add(v_div_n(horizontal, 2), v_div_n(vertical, 2)), any));

	// Render

	int j = 0;
	while (j < image_height)
	{
		int i = 0;
		while (i < image_width)
		{

			float u = (double)i / (image_width - 1);
			float v = (image_height - (double)j - 1)/ (image_height - 1);

			t_ray r;
			r.orig = origin;
			r.dir = v_add(lower_left_corner, v_sub(v_add(v_mul_n(horizontal, u), v_mul_n(vertical, v)), origin));
			t_vec3 pixel_color = ray_color(r, world);
			write_color(app, pixel_color);
			mlx_pixel_put(app->mlx_ptr, app->win_ptr, i, j, app->int_color);
			app->data[j * image_width + i] = app->int_color;

			++i;
		}
		++j;
	}
	mlx_put_image_to_window (app->mlx_ptr, app->win_ptr, app->img_ptr, 0, 0);
	mlx_loop(app->mlx_ptr);
}