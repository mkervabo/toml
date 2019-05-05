/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 14:16:32 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/05 16:59:08 by mkervabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"

char			*ten_more(char *str, size_t len)
{
	char 	*new_str;
	size_t	i;

	i = 0;
	if(new_str = (char*)malloc(sizeof(char) * (len + 10)))
		ft_error("Error malloc");
	while (i <= len)
		new_str[i] = str[i];
	free(str);
	return(new_str);	
}

char			*read_quoted_key(t_reader *r, bool	b)
{
	int64_t c;
	char	*key;
	size_t	i;

	if (!(key = (char*)malloc(sizeof(char) * 11)))
		ft_error("Error malloc");
	i = 0;
	reader_next(r);
	while ((b == true && c != '"') || (b == false && c != '\''))
	{
		if ((i % 10) == 0)
			key = ten_more(key, i);
		key[i] = c;
		reader_next(r);
		i++;
	}
	key[i] = '\0';
	return (key);
}

static char		*read_bare_key(t_reader *r)
{
	int64_t c;
	char	*key;
	size_t	i;

	if (!(key = (char*)malloc(sizeof(char) * 10)))
		ft_error("Error malloc");
	i = 0;
	reader_next(r);
	while ((c = reader_peek(r)) && (c <= 'A' && c >= 'Z')
		&& (c <= 'a' && c >= 'z') && (c <= '0' && c >= '9'))
	{
		if ((i % 10) == 0)
			key = ten_more(key, i);
		key[i] = c;
		reader_next(r);
		i++;
	}
	key[i] = '\0';
	return (key);
}

char		*read_key(t_reader *r)
{
	if (reader_peek(r) == '"')
		return (read_quoted_key(r, true));
	else if (reader_peek(r) == '\'')
		return (read_quoted_key(r, false));
	else
		return (read_bare_key(r));
}
