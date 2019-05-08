/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/03 21:39:06 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/08 13:20:58 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"

static bool	multi_string_end(t_reader *r, t_str *str, bool quote)
{
	char q;
	int16_t c;

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
			return(true);
		append_char(str, q);
	}
	append_char(str, q);
	return(false);
}

static char *read_multi_string(t_reader *r, bool b)
{
	int64_t	c;
	bool	end;
	t_str	str;

	end = false;
	reader_next(r);
	if(reader_peek(r) == '\n')
		reader_next(r);
	if (!(str = create_str(10)).inner)
		ft_error("Error malloc");
	while ((c = reader_peek(r)) != -1 && end == false)
	{
		if (c == '\\' && b == true)
		{
			reader_next(r);
			skip_ws(r, true);
		}
		else if ((c == '"' && b == true) || (c == '\'' && b == false))
			end = multi_string_end(r, &str, b);
		else
		{
			append_char(&str, c);
			reader_next(r);
		}
	}
	append_char(&str, '\0');
	reader_next(r);
	return (str.inner);
}

static char *read_literal_string(t_reader *r)
{ 
    reader_next(r);
    if (reader_peek(r) == '\'')
    {
        reader_next(r);
        if (reader_peek(r) == '\'')
            return (read_multi_string(r, false));
        return ("\0");
    }
	return(read_quoted_key(r, false));
}

static char *read_basic_string(t_reader *r)
{
    reader_next(r);
    if (reader_peek(r) == '"')
    {
        reader_next(r);
        if (reader_peek(r) == '"')
            return (read_multi_string(r, true));
        return ("\0");
    }
    return(read_quoted_key(r, true));
}

t_toml        read_string(t_reader *r)
{
    int c;
	t_toml tom;

    c = reader_peek(r);
    if (c == '"')
		return(tom = (t_toml) {
			TOML_STRING,
			{.string_v = read_basic_string(r)}
		});
    else if (c == '\'')
		return(tom = (t_toml) {
			TOML_STRING,
			{.string_v = read_literal_string(r)}
		});
	else
		ft_error("Invalid string");
}