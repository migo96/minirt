/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   minirt.c										   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: migo <migo@student.42seoul.kr>			 +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/04/24 11:53:10 by migo			  #+#	#+#			 */
/*   Updated: 2023/05/15 15:11:43 by migo			 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minirt_bonus.h"

void	render(t_data *data, t_set *set, double width, double height)
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
			r.orig = set->cam.loc;
			r.dir = v_add(set->cam.lower_left_corner, \
					v_sub(v_add(v_mul_n(set->cam.hor, u), \
					v_mul_n(set->cam.ver, v)), set->cam.loc));
			r.dir = unit_vector(r.dir);
			my_mlx_pixel_put(data, i, j, re_color(r, set));

		}
	}
}

int	main_loop( t_data *img)
{
	mlx_destroy_image(img->mlx, img->img);
	img->img = mlx_new_image(img->mlx, WIDTH, HEIGHT);
	img->set.cam.lower_left_corner = set_lower_left_corner(&img->set.cam);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel,
			&img->line_length, &img->endian);
	render(img, &(img->set), WIDTH, HEIGHT);
	mlx_put_image_to_window(img->mlx, img->win, img->img, 0, 0);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	img;
	int		img_width;
	int		img_height;

	if (argc != 2)
		return (printf("parameter error"));
	checkmap(argv, &(img.set));
	img.mlx = mlx_init();
	img.set.img2 = malloc(sizeof(t_data));
	img.set.img2->mlx = mlx_init();
	img.win = mlx_new_window(img.mlx, WIDTH, HEIGHT, "ray tracing");
	img.img = mlx_new_image(img.mlx, WIDTH, HEIGHT);
	img.set.img2->img = mlx_xpm_file_to_image(img.set.img2->mlx, "earth.xpm", &img_width, &img_height);
    img.set.img2->addr = mlx_get_data_addr(img.set.img2->img, &img.set.img2->bits_per_pixel, &img.set.img2->line_length,
                                 &img.set.img2->endian);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel,
			&img.line_length, &img.endian);
	rt_hook(&img);
	mlx_loop_hook(img.mlx, main_loop, &img);
	mlx_loop(img.mlx);
}
