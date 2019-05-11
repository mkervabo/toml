/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/03 21:39:06 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/11 17:50:36 by mkervabo         ###   ########.fr       */
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

static t_toml_error	read_multi_string2(t_reader *r, t_str *str, bool quote,
	bool *end)
{
	int16_t			c;
	t_toml_error	err;

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
	else if (((c == '"' && quote == true) || (c == '\'' && quote == false))
		&& (err = multi_string_end(r, str, quote, end)) != NO_ERROR)
		return (err);
	else
	{
		if (!append_char(str, c))
			return (ERROR_MALLOC);
		reader_next(r);
	}
	return (NO_ERROR);
}

static t_toml_error	read_multi_string(t_reader *r, bool quote, char **s)
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
		if ((err = read_multi_strin2(r, &str, quote, &end)) != NO_ERROR)
			return (err);
	if (!append_char(&str, '\0'))
		return (ERROR_MALLOC);
	reader_next(r);
	*s = str.inner;
	return (NO_ERROR);
}

static t_toml_error	read_literal_string(t_reader *r, char **str)
{
	reader_next(r);
	if (reader_peek(r) == '\'')
	{
		reader_next(r);
		if (reader_peek(r) == '\'')
			return (read_multi_string(r, false, str));
		*str = "\0";
		return (NO_ERROR);
	}
	return (read_quoted_key(r, false, str));
}

static t_toml_error	read_basic_string(t_reader *r, char **str)
{
	reader_next(r);
	if (reader_peek(r) == '"')
	{
		reader_next(r);
		if (reader_peek(r) == '"')
			return (read_multi_string(r, true, str));
		*str = "\0";
		return (NO_ERROR);
	}
	return (read_quoted_key(r, true, str));
}

t_toml_error	read_string(t_reader *r, t_toml *tom)
{
	int				c;
	char			*str;
	t_toml_error	err;

	c = reader_peek(r);
	if (c == '"')
		err = read_basic_string(r, &str);
	else if (c == '\'')
		err = read_literal_string(r, &str);
	else
		return (INVALID_STRING);
	*tom = (t_toml) {
			TOML_STRING,
			{
				.string_v = str
			}
	};
	return (err);
}
