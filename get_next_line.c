/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssfar <ssfar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/02 18:14:06 by ssfar             #+#    #+#             */
/*   Updated: 2018/12/07 10:18:14 by ssfar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_glist		*ft_glstnew(char *s, int fd, t_glist *first)
{
	t_glist			*list;

	if (!(list = (t_glist*)malloc(sizeof(t_glist))))
		return (NULL);
	list->s = s;
	list->fd = fd;
	list->next = first;
	return (list);
}

static int			get_line(t_glist **first, t_glist *lst, char **line)
{
	t_glist			*prev;

	if (!(*line = ft_strsub(lst->s, 0, ft_strclen(lst->s, '\n'))))
		return (-1);
	if ((ft_strchr(lst->s, '\n')))
		ft_strcpy(lst->s, ft_strchr(lst->s, '\n') + 1);
	else
	{
		if (*first == lst)
			*first = lst->next;
		else
		{
			prev = *first;
			while (prev->next != lst)
				prev = prev->next;
			prev->next = lst->next;
		}
		free(lst->s);
		free(lst);
	}
	return (1);
}

static int			fill_lst(t_glist **first, t_glist **lst, int fd, char *buf)
{
	int				ret;
	char			*to_free;

	while ((ret = read(fd, buf, BUFF_SIZE)) > 0)
	{
		buf[ret] = '\0';
		if (!*lst)
		{
			if (!(*lst = ft_glstnew(ft_strnew(0), fd, *first)))
				return (0);
			*first = *lst;
		}
		to_free = (*lst)->s;
		if (!((*lst)->s = ft_strjoin((*lst)->s, buf)))
			return (0);
		free(to_free);
		if (ft_strchr((*lst)->s, '\n'))
			break ;
	}
	return (1);
}

int					get_next_line(const int fd, char **line)
{
	static t_glist	*first = NULL;
	t_glist			*lst;
	char			buf[BUFF_SIZE + 1];

	lst = first;
	while (lst && lst->fd != fd)
		lst = lst->next;
	if (!line || BUFF_SIZE <= 0 || fd < 0 || read(fd, buf, 0) < 0
	|| !fill_lst(&first, &lst, fd, buf))
		return (-1);
	return (lst && ft_strlen(lst->s) ? get_line(&first, lst, line) : 0);
}
