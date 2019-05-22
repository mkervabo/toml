/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 17:09:19 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/22 13:14:26 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"

t_toml_error	read_array(t_reader *r, t_toml *tom)
{
	t_toml_array	*array;

	if (!(array = create_array(10)))
		return (error_malloc);
	reader_next(r);
	skip_ws(r, true);
	while (reader_peek(r) != -1 && reader_peek(r) != ']')
	{
		read_toml_value(r, tom);
		if (!append_array(array, *tom))
			return (error_malloc);
		if (array->inner[array->len - 1].type != array->inner[0].type)
			return (invalid_array);
		skip_ws(r, true);
		if (reader_peek(r) == ',')
			reader_next(r);
		else if (reader_peek(r) != ']')
			return (invalid_array);
		skip_ws(r, true);
	}
	tom->type = TOML_array;
	tom->value.array_v = array;
	reader_next(r);
	return (no_error);
}
