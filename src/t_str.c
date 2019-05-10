#include "toml.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	const char	*c_src = src;
	char		*c_dst;

	c_dst = dst;
	while (n--)
		*c_dst++ = *c_src++;
	return (dst);
}

t_str create_str(size_t capacity)
{
	return ((t_str) {
		.len = 0,
		.capacity = capacity,
		.inner = malloc(capacity * sizeof(char))
	});
}

void	append_char(t_str *str, char c)
{
	size_t	new_capacity;
	char	*old;
	
	if (str->len == str->capacity)
	{
		new_capacity = str->capacity * 2;
		old = str->inner;
		str->inner = malloc(new_capacity * sizeof(char));
		ft_memcpy(str->inner, old, str->capacity);
		str->capacity = new_capacity;
	}
	str->inner[str->len++] = c;
}

t_toml_array *create_array(size_t capacity)
{
	t_toml_array *array;

	array = malloc(sizeof(t_toml_array));
	*array = (t_toml_array) {
		.len = 0,
		.capacity = capacity,
		.inner = malloc(capacity * sizeof(t_toml))
	};
	return (array);
}

void	append_array(t_toml_array *array, t_toml tom)
{
	size_t	new_capacity;
	t_toml	*old;
	
	if (array->len == array->capacity)
	{
		new_capacity = array->capacity * 2;
		old = array->inner;
		array->inner = malloc(new_capacity * sizeof(t_toml));
		ft_memcpy(array->inner, old, array->capacity * sizeof(t_toml));
		array->capacity = new_capacity;
	}
	array->inner[array->len++] = tom;
}

t_toml_table *create_table(size_t capacity)
{
	t_toml_table *table;

	table = malloc(sizeof(t_toml_table));
	*table = (t_toml_table) {
		.len = 0,
		.capacity = capacity,
		.inner = malloc(capacity * sizeof(t_toml_entry))
	};
	return (table);
}

void	append_table(t_toml_table *table, char *key, t_toml tom)
{
	size_t			new_capacity;
	t_toml_entry	*old;
	
	if (table->len == table->capacity)
	{
		new_capacity = table->capacity * 2;
		old = table->inner;
		table->inner = malloc(new_capacity * sizeof(t_toml_entry));
		ft_memcpy(table->inner, old, table->capacity * sizeof(t_toml_entry));
		table->capacity = new_capacity;
	}
	table->inner[table->len++] = (t_toml_entry) {
		.key = key,
		.value = tom
	};
}