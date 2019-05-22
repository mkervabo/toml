/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dotted_key.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 12:42:35 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/22 13:16:39 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"

static bool			expected_key(const char *inner_key, const char *key)
{
	size_t i;

	i = 0;
	while (inner_key[i] && key[i] && inner_key[i] == key[i])
		i++;
	if (inner_key[i] != key[i])
		return (false);
	return (true);
}

t_toml				*table_get(t_toml_table *table, const char *key)
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

static t_toml_error	read_dotted_key_char(t_toml_table **petit_poisson,
	char **key)
{
	t_toml			*elem;
	t_toml_table	*table;

	if ((elem = table_get(*petit_poisson, *key)))
	{
		if (elem->type == TOML_array)
		{
			if (elem->value.array_v->len == 0)
				return (invalid_table);
			elem = &elem->value.array_v->inner[elem->value.array_v->len - 1];
		}
		if (elem->type != TOML_table)
			return (invalid_table);
		*petit_poisson = elem->value.table_v;
	}
	else
	{
		if (!(table = create_table(10)))
			return (error_malloc);
		if (!append_table(*petit_poisson, *key, create_toml_table(table)))
			return (error_malloc);
		*petit_poisson = table;
	}
	return (no_error);
}

t_toml_error		read_dotted_key(t_reader *r, t_toml_table **petit_poisson,
	char **key)
{
	t_toml_error	err;

	skip_ws(r, false);
	if ((err = read_key(r, key)) != no_error)
		return (err);
	skip_ws(r, false);
	while (reader_peek(r) == '.')
	{
		reader_next(r);
		skip_ws(r, false);
		if ((err = read_dotted_key_char(petit_poisson, key)) != no_error)
			return (err);
		if ((err = read_key(r, key)) != no_error)
			return (err);
		skip_ws(r, false);
	}
	return (no_error);
}
