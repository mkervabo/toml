/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 17:56:17 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/11 18:04:45 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"

static t_toml	create_toml_table(t_toml_table *value)
{
	return ((t_toml) {
		TOML_TABLE,
		{
			.table_v = value
		}
	});
}

static bool		expected_key(const char *inner_key, const char *key)
{
	size_t i;

	i = 0;
	while (inner_key[i] && key[i] && inner_key[i] == key[i])
		i++;
	if (inner_key[i] != key[i])
		return (false);
	return (true);
}

static t_toml	*table_get(t_toml_table *table, const char *key)
{
	size_t i;

	i = 0;
	while (i < table->len)
	{
		if (expected_key(table->inner[i].key, key))
			return (&table->inner[i].value);
		i++;
	}
	return (NULL);
}

static t_toml_error	read_array_table(t_reader *r, t_toml_table *petit_poisson,
	char *key)
{
	t_toml			*value;
	t_toml_array	*tom_array;
	t_toml_table	*table;
	t_toml_error	err;

	if (reader_peek(r) != ']')
		return (INVALID_TABLE_HEADER);
	reader_next(r);
	if ((value = table_get(petit_poisson, key)))
	{
		if (value->type != TOML_ARRAY)
			return (INVALID_ARRAY);
		tom_array = value->value.array_v;
	}
	else
	{
		if (!(tom_array = create_array(10)))
			return (ERROR_MALLOC);
		if (!append_table(petit_poisson, key, (t_toml) {
			TOML_ARRAY,
			{
				.array_v = tom_array
			}
		}))
			return (ERROR_MALLOC);
	}
	if ((err = read_toml(r, &table, false)))
		return (err);
	if (!append_array(tom_array, create_toml_table(table)))
		return (ERROR_MALLOC);
	return (NO_ERROR);
}

t_toml_error	read_dotted_key(t_reader *r, t_toml_table **petit_poisson,
	char **key)
{
	t_toml			*value;
	t_toml_table	*table;
	t_toml_error	err;

	skip_ws(r, false);
	if ((err = read_key(r, key)) != NO_ERROR)
		return (err);
	skip_ws(r, false);
	while (reader_peek(r) == '.')
	{
		reader_next(r);
		skip_ws(r, false);
		if ((value = table_get(*petit_poisson, *key)))
		{
			if (value->type == TOML_ARRAY)
			{
				if (value->value.array_v->len)
					value = &value->value.array_v->inner[value->value.array_v->len - 1];
				else
					return (INVALID_TABLE);
			}
			if (value->type != TOML_TABLE)
				return (INVALID_TABLE);
			*petit_poisson = value->value.table_v;
		}
		else
		{
			if (!(table = create_table(10)))
				return (ERROR_MALLOC);
			if (!append_table(*petit_poisson, *key, create_toml_table(table)))
				return (ERROR_MALLOC);
			*petit_poisson = table;
		}
		if ((err = read_key(r, key)) != NO_ERROR)
			return (err);
		skip_ws(r, false);
	}
	return (NO_ERROR);
}

t_toml_error	read_table(t_reader *r, t_toml_table *gros_poisson)
{
	bool			array;
	t_toml_table	*petit_poisson;
	char			*key;
	t_toml_error	err;
	t_toml_table	*value;

	array = false;
	reader_next(r);
	if (reader_peek(r) == '[')
	{
		array = true;
		reader_next(r);
	}
	petit_poisson = gros_poisson;
	if ((err = read_dotted_key(r, &petit_poisson, &key)) != NO_ERROR)
		return (err);
	if (reader_peek(r) != ']')
		return (INVALID_TABLE_HEADER);
	reader_next(r);
	if (array)
		read_array_table(r, petit_poisson, key);
	else
	{
		if ((err = read_toml(r, &value, false)) != NO_ERROR)
			return (err);
		if (!append_table(petit_poisson, key, create_toml_table(value)))
			return (ERROR_MALLOC);
	}
	return (NO_ERROR);
}
