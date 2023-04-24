/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 17:17:29 by migo              #+#    #+#             */
/*   Updated: 2023/01/17 15:39:21 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"
#include<string.h> 
#include<unistd.h>
#include<stdlib.h>

char	*ft_free(char *str)
{
	free(str);
	return (0);
}

int	nlindex(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			break ;
		i++;
	}
	return (i);
}

char	*ft_back(char *str, char *oneline)
{
	char	*src;
	int		i;
	int		j;

	if (oneline == 0)
		return (ft_free(str));
	j = ft_strlen(str);
	if (find(str) == 0)
		return (ft_free(str));
	i = j - nlindex(str);
	src = (char *)malloc(sizeof(char) * i);
	if (src == 0)
		return (ft_free(str));
	while (i)
		src[i-- - 1] = str[j--];
	free (str);
	if (src[0] == '\0')
	{
		free(src);
		return (0);
	}
	return (src);
}

int	find(char *str)
{
	int	i;

	i = 0;
	if (str == 0)
		return (0);
	while (str[i])
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*get_next_line(int fd)
{
	char		buff[BUFFER_SIZE];
	ssize_t		rd_size;
	char		*oneline;
	static char	*storage;

	while (find(storage) == 0)
	{
		rd_size = read(fd, buff, BUFFER_SIZE);
		if (0 < (rd_size))
		{
			if (storage == 0)
				storage = ft_strdup(buff, rd_size);
			else
				storage = ft_strjoin(storage, buff, rd_size);
		}
		else
			break ;
	}
	if (rd_size < 0)
		storage = ft_free(storage);
	if (storage == 0)
		return (0);
	oneline = ft_oneline(storage);
	storage = ft_back(storage, oneline);
	return (oneline);
}
