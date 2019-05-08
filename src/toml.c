/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toml.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 13:33:40 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/08 18:22:20 by mkervabo         ###   ########.fr       */
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

t_toml_table	*read_toml(t_reader *r, bool read_tables)
{
	t_toml_table	*gros_poisson;
	char 			*key;
	int16_t 		c;

	gros_poisson = create_table(10);
	while (reader_peek(r) != -1)
	{
		skip_ws(r, true);
		if (reader_peek(r) == '[')
		{
			if (read_tables)
			{
				reader_next(r);
				skip_ws(r, false);
				key = read_key(r);
				skip_ws(r, false);
				if (reader_peek(r) != ']')
					ft_error("Invalid table header");
				reader_next(r);
				append_table(gros_poisson, key, (t_toml) {
					TOML_TABLE,
					{ .table_v = read_toml(r, false) }
				});
			} else
				break ;
		}
		else
		{
			key = read_key(r);
			skip_ws(r, false);
			c = reader_peek(r);
			if (c != '=')
				ft_error("Wrong format");
			reader_next(r);
			skip_ws(r, false);
			append_table(gros_poisson, key, read_toml_value(r));
		}
	}
}