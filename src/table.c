#include "toml.h"

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
			return(&table->inner[i].value);
		i++;
	}
	return (NULL);
}

static void		read_array_table(t_reader *r, t_toml_table	*petit_poisson, char *key)
{
	t_toml			*value;
	t_toml_array	*tom_array;

	if (reader_peek(r) != ']')
		ft_error("Invalid table header");
	reader_next(r);
	if ((value = table_get(petit_poisson, key)))
	{
		if (value->type != TOML_ARRAY)
			ft_error("Not an array");
		tom_array = value->value.array_v;
	}
	else
	{
		tom_array = create_array(10);
		append_table(petit_poisson, key, (t_toml) {
			TOML_ARRAY,
			{ .array_v = tom_array }
		});
	}
	append_array(tom_array, (t_toml) {
		TOML_TABLE,
		{ .table_v = read_toml(r, false) }
	});	
}

t_toml_table *read_dotted_key(t_reader *r, t_toml_table *petit_poisson, char **key)
{
	t_toml			*value;
	t_toml_table	*table;

	skip_ws(r, false);
	*key = read_key(r);
	skip_ws(r, false);
	while (reader_peek(r) == '.')
	{
		reader_next(r);
		skip_ws(r, false);
		if ((value = table_get(petit_poisson, *key)))
		{
			if (value->type == TOML_ARRAY)
			{
				if (value->value.array_v->len)
					value = &value->value.array_v->inner[value->value.array_v->len - 1];
				else
					ft_error("Empty Array");
			}
			if (value->type != TOML_TABLE)
				ft_error("Not a table");
			petit_poisson = value->value.table_v;
		}
		else
		{
			table = create_table(10);
			append_table(petit_poisson, *key, (t_toml) {
				TOML_TABLE,
				{ .table_v = table }
			});
			petit_poisson = table;
		}
		*key = read_key(r);
		skip_ws(r, false);
	}
	return (petit_poisson);
}



void 	read_table(t_reader *r, t_toml_table *gros_poisson)
{
	bool			array;
	t_toml_table	*petit_poisson;
	char 			*key;

	array = false;
	reader_next(r);
	if (reader_peek(r) == '[')
	{
		array = true;
		reader_next(r);
	}
	petit_poisson = read_dotted_key(r, gros_poisson, &key);
	if (reader_peek(r) != ']')
		ft_error("Invalid table header");
	reader_next(r);
	if (array)
		read_array_table(r, petit_poisson, key);
	else
	{
		append_table(petit_poisson, key, (t_toml) {
			TOML_TABLE,
			{ .table_v = read_toml(r, false) }
		});
	}
}