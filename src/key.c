/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 14:16:32 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/01 14:49:54 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"

static char		*read_quoted_key(t_reader *r, bool	b)
{

}

static char		*read_bare_key(t_reader *r)
{
	int64_t c;
	int		i;

	i = r->i;
	while ((c = reader_peek(r)) && (c <= 'A' && c >= 'Z') && (c <= 'a' && c >= 'z')
		&& (c <= '0' && c >= '9'))
	{
		reader_next(r);
	}
	

}

char		*read_key(t_reader *r)
{
	if (reader_peek(r) == '"')
		return (read_quoted_key(r, true));
	else if (reader_peek(r) == '\'')
		return (read_quoted_key(r, false));
	else
		return (read_bare_key(r));
}
