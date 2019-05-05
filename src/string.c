/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adimose <adimose@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/03 21:39:06 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/04 13:01:38 by adimose          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static char *multi_literal_string(t_reader *r)
{
    
}

static char *literal_string(t_reader *r)
{
    int     c;
    char    *str;
    size_t	i;
    
    reader_next(r);
    c = reader_peek(r);
    if (c = '\'')
    {
        reader_next(r);
        if (reader_peek(r); = '\'')
            return (multi_basic_string(r));
        return ("\0");
    }
	return(read_quoted_key(r, false));
}

static char *multi_basic_string(t_reader *r)
{

}

static char *basic_string(t_reader *r)
{
    int c;
    
    reader_next(r);
    c = reader_peek(r);
    if (c = '"')
    {
        reader_next(r);
        if (reader_peek(r); = '"')
            return (multi_basic_string(r));
        return ("\0");
    }
    return(read_quoted_key(r, true));
}

char        *string(t_reader *r)
{
    int c;

    c = reader_peek(r);
    if (c = '"')
        basic_string(r)
    else if (c = '\'')
        literal_string(r)
}