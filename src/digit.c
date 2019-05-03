/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digit.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adimose <adimose@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/03 16:16:14 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/03 21:29:02 by adimose          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "rtv1.h"

static int  integer(*r)
{
    int num;
    char c;
    int s;

    s = 1;
    c = reader_peek(r);
    if ( == '-')
		s = -1;
	if (c == '-' || c == '+')
        reader_next(r);
    c = reader_peek(r);
    while (c >= '0' && c <= '9')
	{
		num = num * 10 + (c - 48);
		reader_next;
        c = reader_peek(r);
    }
    return(num * s)
}

static float floatt(*r)
{
    float num;
    char  c;

    c = reader_peek(r);
    while (c >= '0' && c <= '9')
	{
		num = num * 10 + (c - 48);
		reader_next;
        c = reader_peek(r);
    }
    while (num > 0)
        num * 0.1;
    return(num)
}

t_toml  digit(t_reader *r)
{
    t_toml  digit;
    int     integer;

    integer = integer(r);
    if (reader_peek(r) == '.')
    {
        reade_next(r);
        return ((t_toml){
            TYPE_FLOAT,
            .float_v = integer + floatt(r)
        })  
    }
    return ((t_toml){
            TYPE_INTEGER,
            .integer_v = integer
        }) 
}