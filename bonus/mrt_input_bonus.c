/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_input_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:25:15 by migo              #+#    #+#             */
/*   Updated: 2023/05/31 12:40:36 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

void	add_cylinder(char *map, t_set *set)
{
	ft_lstadd_front(&set->objects, ft_lstnew(1, map));
	ft_lstadd_front(&set->objects, ft_lstnew(5, map));
	ft_lstadd_front(&set->objects, ft_lstnew(6, map));
}

void	classification_map(char *map, int i, t_set *set)
{
	if (map[i] == 'C')
	{	
		if ((int)set->cam.fov != 0)
			exit(printf("cam must be only one\n"));
		set_cam(&set->cam, &map[i]);
	}
	else if (map[i] == 'A')
		set_am_light(&set->am_light, &map[i]);
	else if (map[i] == 'L')
		l_lstadd_front(&set->light, l_lstnew(&map[i]));
	else if (map[i] == 's' && map[i + 1] == 'p')
		ft_lstadd_front(&set->objects, ft_lstnew(0, &map[i]));
	else if (map[i] == 'p' && map[i + 1] == 'l')
		ft_lstadd_front(&set->objects, ft_lstnew(2, &map[i]));
	else if (map[i] == 'c' && map[i + 1] == 'y')
		add_cylinder(map + i, set);
	else if (map[i] == 'c' && map[i + 1] == 'n')
	{
		ft_lstadd_front(&set->objects, ft_lstnew(CONE, &map[i]));
		ft_lstadd_front(&set->objects, ft_lstnew(6, &map[i]));
	}
	else if (map[i] != '\0')
		exit(printf("%c is wrong parameter", map[i]));
}

double	check_dot(char **map, double flag)
{
	if ((*map)[-1] >= '0' && (*map)[-1] <= '9')
	{
		if ((*map)[-1] >= '0' && (*map)[-1] <= '9')
		{
			if (flag == 0)
				return (1);
		}
	}
	exit(printf("dot error\n"));
}

double	ft_atof(char **m)
{
	double	num;
	double	flag[2];
	double	power;

	num = 0;
	flag[0] = 0;
	flag[1] = skip_space_comma(m);
	power = 1;
	while (1)
	{
		if (**m <= '9' && **m >= '0')
		{
			num = num * 10 + (**m - 48);
			if (flag[0] == 1)
				power *= 10;
		}
		else if (**m == '.')
			flag[0] = check_dot(m, flag[0]);
		else
			break ;
		(*m)++;
	}
	if (**m != ' ' && **m != '\t' && **m != 0 && **m != '\n' && **m != ',')
		exit(printf("%c is wrong parameter", **m));
	return (num * flag[1] / power);
}

void	checkmap(char **argv, t_set	*set)
{
	char	*map;
	int		fd;
	int		i;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		exit(printf("open error\n"));
	if (!is_file_name_ok(argv[1]))
		exit(printf("file name error input file extention '.rt' required.\n"));
	while (1)
	{
		i = 0;
		map = get_next_line(fd);
		if (map == NULL)
			break ;
		while (map[i] == ' ' || (map[i] >= 9 && map[i] <= 13))
			i++;
		classification_map(map, i, set);
		free (map);
	}
	if (set->cam.fov == 0)
		exit(printf("cam must exist \n"));
	close(fd);
}
