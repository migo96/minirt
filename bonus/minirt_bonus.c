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

void	anti_ali(t_data *img)
{
	int		i;
	int		j;
	t_vec	color;

	j = 1;
	while (j < 800)
	{
		i = 1;
		while (i < 1200)
		{
			color = ch_color(gain_color(img, i, j));
			color = v_add(color, ch_color(gain_color(img, i + 1, j)));
			color = v_add(color, ch_color(gain_color(img, i - 1, j)));
			color = v_add(color, ch_color(gain_color(img, i, j + 1)));
			color = v_add(color, ch_color(gain_color(img, i, j - 1)));
			color = v_mul_n(color, 0.2);
			my_mlx_pixel_put(img, i, j, fl_color(color));
			i++;
		}
		j++;
	}
}

int	main_loop(t_data *img)
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

void	xpm_map(t_data *img)
{
	int		img_width;
	int		img_height;
	int		i;
	char	*map[3];

	i = 0;
	map[0] = "earth.xpm";
	map[1] = "earth2.xpm";
	map[2] = "tile.xpm";
	img->set.img2 = malloc(sizeof(t_data) * 3);
	while (i < 3)
	{
		img->set.img2[i].mlx = mlx_init();
		img->set.img2[i].img = mlx_xpm_file_to_image(img->set.img2[i].mlx, \
							map[i], &img_width, &img_height);
		img->set.img2[i].addr = mlx_get_data_addr(img->set.img2[i].img, \
				&img->set.img2[i].bits_per_pixel, \
				&img->set.img2[i].line_length, \
				&img->set.img2[i].endian);
		img->set.img2[i].width = img_width;
		img->set.img2[i].height = img_height;
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_data	img;

	if (argc != 2)
		return (printf("parameter error"));
	checkmap(argv, &(img.set));
	img.mlx = mlx_init();
	xpm_map(&img);
	img.win = mlx_new_window(img.mlx, WIDTH, HEIGHT, "ray tracing");
	img.img = mlx_new_image(img.mlx, WIDTH, HEIGHT);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel,
			&img.line_length, &img.endian);
	rt_hook(&img);
	mlx_loop_hook(img.mlx, main_loop, &img);
	mlx_loop(img.mlx);
}
