#include "toml.h"

t_toml		read_array(t_reader *r)
{
	t_toml			tom;
	t_toml_array	*array;
	int16_t	c;

	tom.type = TOML_ARRAY;
	array = create_array(10);
	reader_next(r);
	while (reader_peek(r) != -1)
	{
		skip_ws(r, true);
		if (reader_peek(r) == ']')
			break;
		append_array(array, read_toml_value(r));
		if (array->inner[array->len - 1].type != array->inner[0].type)
			ft_error("Wrong array");
		skip_ws(r, true);
		if (reader_peek(r) == ',')
			reader_next(r);
		else if (reader_peek(r) != ']')
			ft_error("Missing commad or unterminated array");
	}
	tom.value.array_v = array;
	reader_next(r);
	return (tom);
}