#include "minirt_bonus.h"

void	uv_checkers(t_checker* checkers, int r_in_w, int r_in_h, t_vec color_a, t_vec color_b)
{
	checkers->width = r_in_w;
	checkers->height = r_in_h;
	checkers->color_a = color_a;
	checkers->color_b = color_b;
}

t_vec uv_pattern_at(t_checker checkers, double u, double v)
{
	int u2;
	int v2;

	u2 = round(u * checkers.width);
	v2 = round(v * checkers.height);
	if ((u2 + v2) % 2)
		return checkers.color_a;
	else
		return checkers.color_b;
}
