/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 16:34:25 by migo              #+#    #+#             */
/*   Updated: 2023/01/17 15:37:00 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <string.h>
# define BUFFER_SIZE 42

char	*ft_strdup(char *src, ssize_t rd_size);
int		ft_strlen(char *str);
char	*ft_strjoin(char *s1, char *s2, ssize_t	rd_size);
char	*ft_oneline(char *str);
char	*ft_free(char *str);
int		nlindex(char *str);
char	*ft_back(char *str, char *oneline);
int		find(char *str);
char	*get_next_line(int fd);

#endif
