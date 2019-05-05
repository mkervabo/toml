/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toml.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 13:33:40 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/05 17:00:35 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"

void			ft_error(char *msg)
{
	size_t i;
	while (msg[i])
		i++;
	write(0, msg, i);
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

t_toml			read_t_toml(t_reader *r)
{
	char c;
	t_toml tom;

	c = reader_peek(r);
	if ((c > '0' && c < '9') || c == '+' || c == '-')
		tom = read_digit(r);
	else if (c == '\'' || c == '"')
		tom =  read_string(r);
	else if (c == '[')
		tom = (t_toml) {
			TOML_TABLE,
			{.table_v = read_table(r)}
		};
	/*else if ((c > 'a' && c < 'z') || (c > 'A' && c < 'Z'))
		tom = read_array(r);*/
	return(tom);
}

t_toml_table	*read_toml(t_reader *r)
{
	t_toml_table	petit_poisson;
	char 			c;

	skip_ws(r, true);
	if (reader_peek(r) == '[')
	{
		// TODO read table
	}
	else
	{
		petit_poisson.key = read_key(r);
		skip_ws(r, false);
		c = reader_peek(r);
		if (c != '=')
			ft_error("Wrong format");
		reader_next(r);
		skip_ws(r, false);
		petit_poisson.value = read_t_toml(r);
		
		

	}
}