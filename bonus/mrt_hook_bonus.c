/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_hook_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:58:32 by dmin              #+#    #+#             */
/*   Updated: 2023/05/31 12:30:38 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

void	cam_rotate(int keycode, t_data *img)
{
	if (keycode == 13)
		img->set.cam.vec = \
		unit_vector(v_add(img->set.cam.vec, v_mul_n(img->set.cam.ver, 0.1)));
	else if (keycode == 0)
		img->set.cam.vec = \
		unit_vector(v_sub(img->set.cam.vec, v_mul_n(img->set.cam.hor, 0.1)));
	else if (keycode == 1)
		img->set.cam.vec = \
		unit_vector(v_sub(img->set.cam.vec, v_mul_n(img->set.cam.ver, 0.1)));
	else if (keycode == 2)
		img->set.cam.vec = \
		unit_vector(v_add(img->set.cam.vec, v_mul_n(img->set.cam.hor, 0.1)));
}

void	cam_move(int keycode, t_data *img)
{
	if (keycode == 123)
		img->set.cam.loc = \
			v_sub(img->set.cam.loc, v_mul_n(img->set.cam.hor, 10));
	else if (keycode == 124)
		img->set.cam.loc = \
			v_add(img->set.cam.loc, v_mul_n(img->set.cam.hor, 10));
	else if (keycode == 126)
		img->set.cam.loc = \
			v_add(img->set.cam.loc, v_mul_n(img->set.cam.vec, 10));
	else if (keycode == 125)
		img->set.cam.loc = \
			v_sub(img->set.cam.loc, v_mul_n(img->set.cam.vec, 10));
}

int	key_hook_mng(int keycode, t_data *img)
{
	if (keycode == 53)
		rt_close(img);
	else if (keycode >= 123 && keycode <= 126)
		cam_move(keycode, img);
	else if (keycode == 13 || keycode == 0 || keycode == 1 || keycode == 2)
		cam_rotate(keycode, img);
	else if (keycode == 6)
		img->set.cam.loc.z -= 10;
	else if (keycode == 7)
		img->set.cam.loc.z += 10;
	return (0);
}

int	rt_close(void *param)
{
	t_data	*img;

	img = (t_data *) param;
	free(img->set.img2);
	l_lstclear(&img->set.light);
	ft_lstclear(&img->set.objects);
	exit(0);
}

void	rt_hook(t_data *img)
{
	mlx_hook(img->win, 2, 0, key_hook_mng, img);
	mlx_hook(img->win, 17, 0, rt_close, img);
}
