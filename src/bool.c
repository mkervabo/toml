#include "toml.h"

static bool		reader_expect(t_reader *r, const char *expected)
{
	while (*expected)
		if (reader_peek(r) == *expected)
		{
			reader_next(r);
			expected++;
		}
		else
			return (false);
	return (true);
}

t_toml		read_boolean(t_reader *r)
{
	t_toml	tom;

	tom.type = TOML_BOOLEAN;
	if (reader_peek(r) == 't')
	{
		if (reader_expect(r, "true"))
			tom.value.boolean_v = true;
		else
			ft_error("Invalid bool");
	}
	else
	{
		if (reader_expect(r, "false"))
			tom.value.boolean_v = false;
		else
			ft_error("Invalid bool");
	}
	return (tom);
}