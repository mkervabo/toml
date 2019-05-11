/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toml.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 13:33:40 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/11 17:08:21 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"

void				skip_ws(t_reader *r, bool newline)
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

t_toml_error		read_toml_value(t_reader *r, t_toml *tom)
{
	int16_t			c;
	t_toml_error	err;

	c = reader_peek(r);
	err = NO_ERROR;
	if ((c >= '0' && c <= '9') || c == '+' || c == '-')
		*tom = read_digit(r);
	else if (c == '\'' || c == '"')
		err = read_string(r, tom);
	else if (c == '[')
		err = read_array(r, tom);
	else if (c == 't' || c == 'f')
		err = read_boolean(r, tom);
	else
		return (INVALID_TOML_VALUE);
	return (err);
}

static t_toml_error	read_key_val(t_reader *r, t_toml_table **gros_poisson,
	char **key)
{
	int16_t			c;
	t_toml_error	err;
	t_toml			value;
	t_toml_table	*petit_poisson;

	petit_poisson = *gros_poisson;
	if ((err = read_dotted_key(r, &petit_poisson, key)) != NO_ERROR)
		return (err);
	c = reader_peek(r);
	if (c != '=')
		return (INVALID_FORMAT_KEY_VALUE);
	reader_next(r);
	skip_ws(r, false);
	if ((err = read_toml_value(r, &value)) != NO_ERROR)
		return (err);
	if (!append_table(petit_poisson, *key, value))
		return (ERROR_MALLOC);
	skip_ws(r, true);
	return (NO_ERROR);
}

t_toml_error		read_toml(t_reader *r, t_toml_table **gros_poisson,
	bool read_tables)
{
	char			*key;
	int16_t			c;
	t_toml_error	err;

	if (!(*gros_poisson = create_table(10)))
		return (ERROR_MALLOC);
	skip_ws(r, true);
	while (reader_peek(r) != -1)
	{
		if (reader_peek(r) == '[')
		{
			if (read_tables)
			{
				if ((err = read_table(r, *gros_poisson)) != NO_ERROR)
					return (err);
			}
			else
				break ;
		}
		else if ((err = read_key_val(r, gros_poisson, &key)) != NO_ERROR)
			return (err);
	}
	return (NO_ERROR);
}
