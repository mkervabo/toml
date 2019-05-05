/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/03 21:39:06 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/05 17:00:00 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"

static char *read_multi_string(t_reader *r, bool b)
{
	int		i;
	char	c;
	char	*str;

	i = 0;
	reader_next(r);
	if(reader_peek(r) != '\n')
		ft_error("Invalid multi string");
	if (b == false)
    	return(read_quoted_key(r, b));
	if (!(str = (char*)malloc(sizeof(char) * 11)))
		ft_error("Error malloc");
	while ((c = reader_peek(r)) && c != '"')
	{
		if ((i % 10) == 0)
			str = ten_more(str, i);
		if (c == '\\')
			skip_ws(r, false);
		else
		{
			str[i] = c;
			reader_next(r);
			i++;
		}
		return (str);
	}
}

static char *read_literal_string(t_reader *r)
{
    int     c;
    char    *str;
    size_t	i;
    
    reader_next(r);
    c = reader_peek(r);
    if (c = '\'')
    {
        reader_next(r);
        if (reader_peek(r) == '\'')
            return (read_multi_string(r, false));
        return ("\0");
    }
	return(read_quoted_key(r, false));
}

static char *read_basic_string(t_reader *r)
{
    int c;
    
    reader_next(r);
    c = reader_peek(r);
    if (c = '"')
    {
        reader_next(r);
        if (reader_peek(r) == '"')
            return (read_multi_string(r, true));
        return ("\0");
    }
    return(read_quoted_key(r, true));
}

t_toml        read_string(t_reader *r)
{
    int c;
	t_toml tom;

    c = reader_peek(r);
    if (c = '"')
		return(tom = (t_toml) {
			TOML_STRING,
			{.string_v = read_basic_string(r)}
		});
    else if (c = '\'')
		return(tom = (t_toml) {
			TOML_STRING,
			{.string_v = read_literal_string(r)}
		});
}