/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 13:13:11 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/01 13:25:06 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"
#include <unistd.h>

t_reader	create_reader(int fd, char *buffer, size_t buffer_size)
{
	return ((t_reader){
		.fd = fd,
		.buffer = buffer,
		.buffer_size = buffer_size,
		.len = 0,
		.i = 0
	});
}

int16_t		reader_peek(t_reader *self)
{
	ssize_t	len;

	if (self->i == self->len)
	{
		len = read(self->fd, self->buffer, self->buffer_size);
		if (len <= 0)
			return (-1);
		self->i = 0;
		self->len = len;
	}
	return (self->buffer[self->i]);
}


void		reader_next(t_reader *self)
{
	self->i++;
}