#include "toml.h"

t_toml		read_array(t_reader *r)
{
	t_toml	tom;
	t_tab	tab;
	int16_t	c;

	tom.type = TOML_ARRAY;
	tab = create_tab(10);
	reader_next(r);
	skip_ws(r, false);
	append_tab(&tab, read_toml_value(r));
	printf("type: %u\n", tab.inner[tab.len].type);
	while ((c = reader_peek(r)) != -1 && c != ']')
	{
		if (c == ',')
		{
			reader_next(r);
			skip_ws(r, false);
			append_tab(&tab, read_toml_value(r));
			printf("type: %u\n", tab.inner[tab.len].type);
			if (tab.inner[tab.len].type != tab.inner[0].type)
				ft_error("Wrong array");
		}
		else
			reader_next(r);
	}
	tom.value.array_v = tab.inner;
	reader_next(r);
	return(tom);
}