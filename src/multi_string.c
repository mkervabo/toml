/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 12:56:25 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/12 12:58:00 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"

static t_toml_error	multi_string_end(t_reader *r, t_str *str, bool quote,
	bool *end)
{
	char	q;
	int16_t	c;

	if (reader_peek(r) == '"')
		q = '"';
	else
		q = '\'';
	reader_next(r);
	c = reader_peek(r);
	if ((c == '"' && quote == true) || (c == '\'' && quote == false))
	{
		reader_next(r);
		c = reader_peek(r);
		if ((c == '"' && quote == true) || (c == '\'' && quote == false))
		{
			*end = true;
			return (NO_ERROR);
		}
		if (!append_char(str, q))
			return (ERROR_MALLOC);
	}
	if (!append_char(str, q))
		return (ERROR_MALLOC);
	*end = false;
	return (NO_ERROR);
}

static t_toml_error	read_multi_string_char(t_reader *r, t_str *str, bool quote,
	bool *end)
{
	int16_t			c;
	t_toml_error	err;

	err = NO_ERROR;
	if ((c = reader_peek(r)) == '\\' && quote == true)
	{
		reader_next(r);
		if (reader_peek(r) == '\n')
			skip_ws(r, true);
		else
		{
			if (!append_char(str, read_escape(r)))
				return (ERROR_MALLOC);
			reader_next(r);
		}
	}
	else if ((c == '"' && quote == true) || (c == '\'' && quote == false))
		err = multi_string_end(r, str, quote, end);
	else
	{
		if (!append_char(str, c))
			return (ERROR_MALLOC);
		reader_next(r);
	}
	return (err);
}

t_toml_error		read_multi_string(t_reader *r, bool quote, char **s)
{
	t_str			str;
	int64_t			c;
	t_toml_error	err;
	bool			end;

	end = false;
	reader_next(r);
	if (reader_peek(r) == '\n')
		reader_next(r);
	if (!(str = create_str(10)).inner)
		return (ERROR_MALLOC);
	while ((c = reader_peek(r)) != -1 && end == false)
		if ((err = read_multi_string_char(r, &str, quote, &end)) != NO_ERROR)
			return (err);
	if (!append_char(&str, '\0'))
		return (ERROR_MALLOC);
	reader_next(r);
	*s = str.inner;
	return (NO_ERROR);
}