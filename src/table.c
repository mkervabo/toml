/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/03 15:28:34 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/05 18:00:22 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"

static  char  *read_table_key(t_reader *r)
{
    int64_t c;
	char	*table;
	size_t	i;

	if (!(table = (char*)malloc(sizeof(char) * 11)))
		ft_error("Error malloc");
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

t_toml   read_table(t_reader *r)
{
    t_toml  tom;
	t_toml	next;
    char	c;

	tom.type = TOML_TABLE;
    tom.value.table_v.key = table_key(r);
    skip_ws(r, true);
	c = reader_peek(r);
	while (c != '[')
	{
		tom.value.table_v.value = read_t_toml(r);
		skip_ws(r);
	} 
    return(tom);
}