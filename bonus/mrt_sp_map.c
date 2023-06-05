/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   mrt_sp_map.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: dmin <dmin@student.42seoul.kr>			 +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/06/02 15:11:41 by dmin			  #+#	#+#			 */
/*   Updated: 2023/06/02 15:11:43 by dmin			 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minirt_bonus.h"

double	pump_map(t_set *set, int *c, int i, t_vec color)
{
	t_vec	color1;

	if (c[0] + 1 <= set->img2[i].width)
		color1 = v_sub(ch_color(gain_color \
			(&set->img2[i], c[0] + 1, c[1])), color);
	else
		color1 = v_sub(ch_color(gain_color \
			(&set->img2[i], 0, c[1])), color);
	return (length(v_mul_n(color1, 0.004)));
}

t_vec	sp_map(t_ray con, t_sphere *s, t_set *set, t_vec *normal)
{
	t_vec	check;
	int		c[2];
	int		i;
	t_vec	color[2];
	t_vec	show;

	i = s->mty - 1;
	check = v_sub(con.orig, s->center);
	c[0] = (int)((atan2(-check.y, check.x) + 3.14) \
			* (set->img2[i].width / 6.28));
	c[1] = (int)(acos(-check.z / (s->radius + 1)) \
			* (set->img2[i].height / 3.14));
	c[1] = set->img2[i].height - c[1];
	c[0] = set->img2[i].width - c[0];
	color[0] = ch_color(gain_color(&set->img2[i], c[0], c[1]));
	*normal = v_add(*normal, v_mul_n(unit_vector(v_sub(set->light->loc, \
			con.orig)), pump_map(set, c, i, color[0])));
	return (color[0]);
}

t_vec	pl_map(t_ray con, t_plane *pl, t_set *set, t_vec *normal)
{
	int		a[2];
	t_vec	hor;
	t_vec	ver;
	t_vec	color;
	int		i;

	i = pl->mty - 1;
	hor = v_mul(cross(pl->normal, make_vec(0, 0, 1)), con.orig);
	ver = v_mul(cross(hor, pl->normal), con.orig);
	if (pl->normal.x == 0 && pl->normal.y == 0 && pl->normal.z == 1)
	{
		hor = v_mul(make_vec(1, 0, 0), con.orig);
		ver = v_mul(make_vec(0, 1, 0), con.orig);
	}
	a[0] = (int)(hor.x + hor.y + hor.z) % set->img2[i].width;
	a[1] = (int)(ver.x + ver.y + ver.z) % set->img2[i].height;
	if (a[0] < 0)
		a[0] *= -1;
	if (a[1] < 0)
		a[1] *= -1;
	color = ch_color(gain_color(&set->img2[i], a[0], a[1]));
	*normal = v_add(*normal, v_mul_n(unit_vector(v_sub(set->light->loc, \
			con.orig)), pump_map(set, a, i, color)));
	return (color);
}
