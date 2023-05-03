/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utlis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 13:33:24 by migo              #+#    #+#             */
/*   Updated: 2023/05/03 13:49:17 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_object	*ft_lstnew(int nb)
{
	t_object	*str;

	str = (t_object *)malloc(sizeof(t_stack) * 1);
	if (str == 0)
		return (0);
	str->type = nb;
	str->object = NULL;
	str->next = NULL;
	return (str);
}

void	ft_lstadd_front(t_object **lst, t_object *new)
{
	if (new == 0)
		return ;
	else if (*lst == 0)
		*lst = new;
	else
	{
		new->next = *lst;
		*lst = new;
	}
}
