/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 14:54:10 by vfries            #+#    #+#             */
/*   Updated: 2022/11/05 23:42:02 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>
#define MAX_FD 1023

void	update_buffer(char *buffer)
{
	ssize_t	max;
	ssize_t	min;

	max = 0;
	while (max < BUFFER_SIZE)
	{
		if (buffer[max] && buffer[max] != '\n')
			max++;
		else
			break ;
	}
	if (max == BUFFER_SIZE)
	{
		ft_bzero(buffer, BUFFER_SIZE);
		return ;
	}
	if (buffer[max] == '\n')
		max++;
	min = 0;
	while (max < BUFFER_SIZE)
		buffer[min++] = buffer[max++];
	ft_bzero(&buffer[min], BUFFER_SIZE - min);
}

char	*dup_before_new_line(char *buffer, t_bool *new_line_found)
{
	ssize_t	len;
	char	*result;

	len = 0;
	while (len < BUFFER_SIZE - 1 && buffer[len] != '\n' && buffer[len])
		len++;
	if (buffer[len] == '\n')
		*new_line_found = TRUE;
	result = malloc(len + 2);
	if (result == NULL)
		return (NULL);
	result[len + 1] = '\0';
	while (len > -1)
	{
		result[len] = buffer[len];
		len--;
	}
	update_buffer(buffer);
	return (result);
}

size_t	get_line_len(t_lst *line_lst)
{
	size_t	len;
	size_t	i;

	len = 0;
	while (line_lst)
	{
		i = 0;
		while (line_lst->data[i])
			i++;
		len += i;
		line_lst = line_lst->next;
	}
	return (len);
}

char	*get_line(t_lst *line_lst)
{
	char	*line;
	char	*line_start;
	size_t	i;
	t_lst	*current;

	lst_reverse(&line_lst);
	line = malloc(get_line_len(line_lst) + 1);
	if (line == NULL)
		return (lst_free(line_lst));
	line_start = line;
	current = line_lst;
	while (current)
	{
		i = 0;
		while (current->data[i])
			*line++ = current->data[i++];
		current = current->next;
	}
	*line = '\0';
	lst_free(line_lst);
	return (line_start);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE];
	t_lst		*line;
	t_bool		new_line_found;
	ssize_t		read_output;

	new_line_found = FALSE;
	if (BUFFER_SIZE < 1 || fd < 0 || fd > MAX_FD)
		return (NULL);
	if (buffer[0] == '\0')
		if (read(fd, buffer, BUFFER_SIZE) == -1)
			return (NULL);
	if (buffer[0] == '\0')
		return (NULL);
	line = NULL;
	lst_add_front(&line, dup_before_new_line(buffer, &new_line_found));
	while (new_line_found == FALSE)
	{
		read_output = read(fd, buffer, BUFFER_SIZE);
		if (read_output == -1)
			return (lst_free(line));
		if (read_output == 0)
			break ;
		lst_add_front(&line, dup_before_new_line(buffer, &new_line_found));
	}
	return (get_line(line));
}
