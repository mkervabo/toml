/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toml.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 13:33:40 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/08 14:09:16 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"

void			ft_error(char *msg)
{
	size_t i;

	i = 0;
	while (msg[i])
		i++;
	write(1, msg, i);
	exit(0);
}

void			skip_ws(t_reader *r, bool newline)
{
	int16_t	c;

	while ((c = reader_peek(r)) != -1 && (c == ' ' || c == '\t' || c == '#'
		|| (newline && c == '\n')))
	{
		if (c == '#')
		{
			while ((c = reader_peek(r)) != -1 && c != '\n')
				reader_next(r);
		}
		reader_next(r);
	}
}

t_toml			read_toml_value(t_reader *r)
{
	int16_t c;
	t_toml tom;

	c = reader_peek(r);
	if ((c >= '0' && c <= '9') || c == '+' || c == '-')
		tom = read_digit(r);
	else if (c == '\'' || c == '"')
		tom =  read_string(r);
	else if (c == '[')
		tom = read_array(r);
	else if (c == 't' || c == 'f')
		tom = read_boolean(r);
	return (tom);
}

t_toml_table	*read_toml(t_reader *r)
{
	t_toml_table	petit_poisson;
	int16_t 		c;

	skip_ws(r, true);
	if (reader_peek(r) == '[')
	{
		/*petit_poisson.value = (t_toml) {
			TOML_TABLE,
			{.table_v = read_table(r)}
		};*/
	}
	else
	{
		petit_poisson.key = read_key(r);
		skip_ws(r, false);
		c = reader_peek(r);
		printf("Key: %s\n", petit_poisson.key);
		if (c != '=')
			ft_error("Wrong format");
		reader_next(r);
		skip_ws(r, false);
		petit_poisson.value = read_toml_value(r);
		switch (petit_poisson.value.type) {
			case TOML_BOOLEAN:
				printf("Bool %s", petit_poisson.value.value.boolean_v ? "true" : "false");
				break;
			case TOML_FLOAT:
				printf("Float %f", petit_poisson.value.value.float_v);
				break;
			case TOML_INTEGER:
				printf("Int %lld", petit_poisson.value.value.integer_v);
				break;
			case TOML_STRING:
				printf("String %s", petit_poisson.value.value.string_v);
				break;
		}
		printf("\n");
	}
}