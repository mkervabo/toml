/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digit.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/03 16:16:14 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/08 10:56:26 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "toml.h"

static int64_t  read_integer(t_reader *r)
{
    int64_t num;
    int16_t c;
    int s;

    s = 1;
	num = 0;
    c = reader_peek(r);
    if (c == '-')
		s = -1;
	if (c == '-' || c == '+')
        reader_next(r);
    while ((c = reader_peek(r)) != -1 && ((c >= '0' && c <= '9') || c == '_'))
	{
        if (c >= '0' && c <= '9')
	    	num = num * 10 + (c - 48);
		reader_next(r);
    }
    return(num * s);
}

static float read_float(t_reader *r)
{
    float	num;
	size_t	i;
    int16_t c;

	i = 0;
    while ((c = reader_peek(r)) != -1 && ((c >= '0' && c <= '9') || c == '_'))
	{
        if (c >= '0' && c <= '9')
	    	num = num * 10 + (c - 48);
		reader_next(r);
		i++;
    }
    while (i-- > 0)
        num *= 0.1;
    return(num);
}

t_toml      read_digit(t_reader *r)
{
    t_toml  digit;
    int     integer;

    integer = read_integer(r);
    if (reader_peek(r) == '.')
    {
        reader_next(r);
        return ((t_toml) {
            TOML_FLOAT,
            {.float_v = integer + read_float(r)}
        });
    }
    return ((t_toml) {
            TOML_INTEGER,
            {.integer_v = integer}
        });
}