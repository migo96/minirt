/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 17:11:34 by migo              #+#    #+#             */
/*   Updated: 2022/12/15 15:34:54 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char	*ft_strdup(char *src, ssize_t rd_size)
{
	char	*dest;
	int		i;

	i = (int)rd_size;
	dest = (char *)malloc(sizeof(char) * i + 1);
	if (dest == 0)
		return (0);
	i = 0;
	while (i < rd_size)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (str == 0)
		return (0);
	while (str[i])
	{
		i++;
	}
	return (i);
}

char	*ft_strjoin(char *s1, char *s2, ssize_t	rd_size)
{
	int		i;
	int		j;
	char	*str;

	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + rd_size + 1));
	if (str == 0)
	{
		free (s1);
		return (0);
	}
	i = -1;
	while (s1[++i])
		str[i] = s1[i];
	j = i;
	i = 0;
	while (i < rd_size)
	{
		str[j] = s2[i];
		i++;
		j++;
	}
	str[j] = '\0';
	free (s1);
	return (str);
}

char	*ft_oneline(char *str)
{
	int		i;
	char	*src;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			break ;
		i++;
	}
	if (i == ft_strlen(str))
		i--;
	src = (char *)malloc(sizeof(char) * i + 2);
	if (src == 0)
		return (0);
	src[i + 1] = '\0';
	while (i)
	{
		src[i] = str[i];
		i--;
	}
	src[0] = str[0];
	return (src);
}
