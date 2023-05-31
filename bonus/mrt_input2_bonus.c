/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_input2_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmin <dmin@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 16:33:02 by dmin              #+#    #+#             */
/*   Updated: 2023/05/16 16:33:04 by dmin             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	*t1;
	unsigned char	*t2;

	t1 = (unsigned char *)s1;
	t2 = (unsigned char *)s2;
	while (*t1)
	{
		if (*t1 != *t2 || !*t1 || !*t2)
			return (*t1 - *t2);
		++t1;
		++t2;
	}
	return (*t1 - *t2);
}

int	is_file_name_ok(char *filename)
{
	int		file_name_len;
	char	*file_extension;

	file_name_len = ft_strlen(filename);
	if (file_name_len < 3)
		return (0);
	file_extension = filename + file_name_len - 3;
	return (!ft_strcmp(file_extension, ".rt"));
}

double	skip_space_comma(char **map)
{
	int		flag;
	double	m_flag;

	m_flag = 1;
	flag = 0;
	while (*map[0] == ' ' || (*map[0] >= 9 && *map[0] < 13) || *map[0] == ',')
	{
		if (*map[0] == ',')
			flag++;
		(*map)++;
	}
	if (flag > 1)
		exit(printf(", error"));
	if (*map[0] == '-')
	{
		m_flag = -1;
		(*map)++;
	}
	return (m_flag);
}
