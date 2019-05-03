/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toml.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adimose <adimose@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 13:02:26 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/03 21:31:28 by adimose          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOML_H
#define TOML_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct s_reader
{
	int			fd;
	char		*buffer;
	size_t		buffer_size;
	size_t		len;
	size_t		i;
}				t_reader;

typedef enum	e_toml_type
{
	TOML_INTEGER,
	TOML_FLOAT,
	TOML_STRING,
	TOML_BOOLEAN,
	TOML_ARRAY,
	TOML_TABLE
}				t_toml_type;

typedef	struct	s_toml_array 
{
	struct s_toml		*value;
	struct s_toml_array *next;
}				t_toml_array;

typedef	struct	s_toml_table 
{
	char				*key;
	struct s_toml		*value;
	struct s_toml_table *next;
}				t_toml_table;

typedef struct	s_toml
{
	t_toml_type	type;
	union {
		int64_t			integer_v;
		float			float_v;
		char			*string_v;
		bool			boolean_v;
		t_toml_array	array_v;
		t_toml_table	table_v;
	}			value;
	
}				t_toml;

t_reader		create_reader(int fd, char *buffer, size_t buffer_size);
int16_t			reader_peek(t_reader *self);
void			reader_next(t_reader *self);

void			skip_ws(t_reader *r, bool newline);

char			*ten_more(char *str, size_t len);

char			*read_key(t_reader *r);

t_toml_table	*read_toml(t_reader *r);

#endif