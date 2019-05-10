/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 14:16:32 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/10 16:50:10 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"

char			read_escape(t_reader *r)
{
	char c;

	c = reader_peek(r);
	if (c == 'n')
		return('\n');
	else if (c == 't')
		return ('\t');
	else if (c == 'b')
		return ('\b');
	else if (c == 'f')
		return ('\f');
	else if (c == 'r')
		return ('\r');
	else
		return (c);
}

char			*read_quoted_key(t_reader *r, bool b)
{
	int16_t c;
	t_str	str;

	if (!(str = create_str(10)).inner)
		ft_error("Error malloc");
	while ((c = reader_peek(r)) != -1 && c != (b ? '"' : '\''))
	{
		if (c == '\n')
			ft_error("Invalid key");
		if (c == '\\' && b)
		{
			reader_next(r);
			c = read_escape(r);
		}
		append_char(&str, c);
		reader_next(r);
	}
	if (c != -1)
		reader_next(r);
	append_char(&str, '\0');
	return (str.inner);
}

static char		*read_bare_key(t_reader *r)
{
	int16_t c;
	t_str	str;

	if (!(str = create_str(10)).inner)
		ft_error("Error malloc");
	while ((c = reader_peek(r)) != -1 && ((c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')
		|| c == '-' || c == '_'))
	{
		append_char(&str, c);
		reader_next(r);
	}
	append_char(&str, '\0');
	return (str.inner);
}

char		*read_key(t_reader *r)
{
	if (reader_peek(r) == '"')
	{
		reader_next(r);
		return (read_quoted_key(r, true));
	}
	else if (reader_peek(r) == '\'')
	{
		reader_next(r);
		return (read_quoted_key(r, false));
	}
	else
		return (read_bare_key(r));
}
