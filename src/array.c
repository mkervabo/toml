#include "toml.h"

t_toml		read_array(t_reader *r)
{
	t_toml			tom;
	t_toml_array	*array;
	int16_t	c;

	tom.type = TOML_ARRAY;
	array = create_array(10);
	reader_next(r);
	skip_ws(r, false);
	append_array(&array, read_toml_value(r));
	printf("type: %u\n", array->inner[array->len].type);
	while ((c = reader_peek(r)) != -1 && c != ']')
	{
		if (c == ',')
		{
			reader_next(r);
			skip_ws(r, false);
			append_array(array, read_toml_value(r));
			if (array->inner[array->len - 1].type != array->inner[0].type)
				ft_error("Wrong array");
		}
		else
			reader_next(r);
	}
	tom.value.array_v = array;
	reader_next(r);
	return (tom);
}