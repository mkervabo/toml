/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/03 15:28:34 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/08 10:41:42 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"


t_toml   read_table(t_reader *r)
{
    t_toml  tom;
	t_toml	next;
    int16_t	c;

	tom.type = TOML_TABLE;
    tom.value.table_v.key = read_key(r);
    skip_ws(r, true);
	while ((c = reader_peek(r)) != -1 && c != '[')
	{
		tom.value.table_v.value = read_toml_value(r);
		skip_ws(r, false);
	} 
    return(tom);
}