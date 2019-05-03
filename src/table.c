/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adimose <adimose@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/03 15:28:34 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/03 21:30:21 by adimose          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static  char  *table_key(t_reader *r)
{
    int64_t c;
	char	*table;
	size_t	i;

	if (!(table = (char*)malloc(sizeof(char) * 10))
		return;
	i = r->i;
	reader_next(r);
	while (c != ']')
	{
		if ((i % 10) == 0)
			table = ten_more(table, i);
		table[i] = c;
		reader_next(r);
		i++;
	}
	return (table);
}

t_toml_table    table(t_reader *r)
{
    t_toml_table    table;
    char            c;

    table.key = table_key(r);
    skip_ws(r, true);
    c = reader_peek(r);
    if (c == '[')
		table.value = {
            TYPE_TABLE,
            .table_v  = table(r)
        }
	else if (c > '0' && c < '9')
        table.value = digit(r);
    return(table);
}