/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 17:56:17 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/22 13:21:52 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"

t_toml				create_toml_table(t_toml_table *value)
{
	return ((t_toml) {
		TOML_table,
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
		if (value->type != TOML_array)
			return (invalid_array);
		*tom_array = value->value.array_v;
	}
	else
	{
		if (!(*tom_array = create_array(10)))
			return (error_malloc);
		if (!append_table(petit_poisson, key, (t_toml) {
			TOML_array,
			{
				.array_v = *tom_array
			}
		}))
			return (error_malloc);
	}
	return (no_error);
}

static t_toml_error	read_array_table(t_reader *r, t_toml_table *petit_poisson,
	char *key)
{
	t_toml_array	*tom_array;
	t_toml_table	*table;
	t_toml_error	err;

	if (reader_peek(r) != ']')
		return (invalid_table_header);
	reader_next(r);
	if ((err = which_array_table(petit_poisson, &tom_array, key)) != no_error)
		return (err);
	if ((err = read_toml(r, &table, false)) != no_error)
		return (err);
	if (!append_array(tom_array, create_toml_table(table)))
		return (error_malloc);
	return (no_error);
}

t_toml_error		read_table(t_reader *r, t_toml_table *gros_poisson)
{
	bool			array;
	t_toml_table	*petit_poisson;
	char			*key;
	t_toml_error	err;
	t_toml_table	*value;

	err = no_error;
	reader_next(r);
	if ((array = (reader_peek(r) == '[')))
		reader_next(r);
	petit_poisson = gros_poisson;
	if ((err = read_dotted_key(r, &petit_poisson, &key)) != no_error)
		return (err);
	if (reader_peek(r) != ']')
		return (invalid_table_header);
	reader_next(r);
	if (array)
		err = read_array_table(r, petit_poisson, key);
	else
	{
		if ((err = read_toml(r, &value, false)) == no_error)
			if (!append_table(petit_poisson, key, create_toml_table(value)))
				err = error_malloc;
	}
	return (err);
}

t_toml_error		read_inline_table(t_reader *r, t_toml *tom)
{
	t_toml_table	*gros_poisson;
	t_toml_error	err;
	char			*key;
	int16_t			c;

	if (!(gros_poisson = create_table(10)))
		return (error_malloc);
	reader_next(r);
	skip_ws(r, false);
	while ((c = reader_peek(r)) != -1 && c != '}')
	{
		if ((err = read_key_val(r, gros_poisson, &key)) != no_error)
			return (err);
		skip_ws(r, false);
		if (reader_peek(r) == ',')
			reader_next(r);
		else if (reader_peek(r) != '}')
			return (invalid_inline_table);
		skip_ws(r, false);
	}
	tom->type = TOML_table;
	tom->value.table_v = gros_poisson;
	reader_next(r);
	return (no_error);
}
