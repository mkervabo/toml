/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 17:56:17 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/12 12:54:58 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"

t_toml				create_toml_table(t_toml_table *value)
{
	return ((t_toml) {
		TOML_TABLE,
		{
			.table_v = value
		}
	});
}

static t_toml_error	which_array_table(t_toml_table *petit_poisson,
	t_toml_array **tom_array, char *key)
{
	t_toml			*value;

	if ((value = table_get(petit_poisson, key)))
	{
		if (value->type != TOML_ARRAY)
			return (INVALID_ARRAY);
		*tom_array = value->value.array_v;
	}
	else
	{
		if (!(*tom_array = create_array(10)))
			return (ERROR_MALLOC);
		if (!append_table(petit_poisson, key, (t_toml) {
			TOML_ARRAY,
			{
				.array_v = *tom_array
			}
		}))
			return (ERROR_MALLOC);
	}
	return (NO_ERROR);
}

static t_toml_error	read_array_table(t_reader *r, t_toml_table *petit_poisson,
	char *key)
{
	t_toml_array	*tom_array;
	t_toml_table	*table;
	t_toml_error	err;

	if (reader_peek(r) != ']')
		return (INVALID_TABLE_HEADER);
	reader_next(r);
	if ((err = which_array_table(petit_poisson, &tom_array, key)) != NO_ERROR)
		return (err);
	if ((err = read_toml(r, &table, false)) != NO_ERROR)
		return (err);
	if (!append_array(tom_array, create_toml_table(table)))
		return (ERROR_MALLOC);
	return (NO_ERROR);
}

t_toml_error		read_table(t_reader *r, t_toml_table *gros_poisson)
{
	bool			array;
	t_toml_table	*petit_poisson;
	char			*key;
	t_toml_error	err;
	t_toml_table	*value;

	err = NO_ERROR;
	reader_next(r);
	if ((array = (reader_peek(r) == '[')))
		reader_next(r);
	petit_poisson = gros_poisson;
	if ((err = read_dotted_key(r, &petit_poisson, &key)) != NO_ERROR)
		return (err);
	if (reader_peek(r) != ']')
		return (INVALID_TABLE_HEADER);
	reader_next(r);
	if (array)
		err = read_array_table(r, petit_poisson, key);
	else
	{
		if ((err = read_toml(r, &value, false)) == NO_ERROR)
			if (!append_table(petit_poisson, key, create_toml_table(value)))
				err = ERROR_MALLOC;
	}
	return (err);
}
