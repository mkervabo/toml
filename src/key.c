/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adimose <adimose@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 14:16:32 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/03 15:04:54 by adimose          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "toml.h"

char			*ten_more(char *str, size_t len)
{
	char 	*new_str;
	size_t	i;

	i = 0;
	if(new_str = (char*)malloc(sizeof(char) * (len + 10))
		return ;
	while (i <= len)
		new_str[i] = str[i];
	free(str);
	return(new_str);	
}

static char		*read_quoted_key(t_reader *r, bool	b)
{
	int64_t c;
	char	*key;
	size_t	i;

	if (!(key = (char*)malloc(sizeof(char) * 10))
		return;
	i = r->i;
	reader_next(r);
	while ((b == true && c != '"') ││ (b == false && c != '\''))
	{
		if ((i % 10) == 0)
			key = ten_more(key, i);
		key[i] = c;
		reader_next(r);
		i++;
	}
	return (key);
}

static char		*read_bare_key(t_reader *r)
{
	int64_t c;
	char	*key;
	size_t	i;

	if (!(key = (char*)malloc(sizeof(char) * 10))
		return;
	i = r->i;
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
