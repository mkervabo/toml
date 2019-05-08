/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toml.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 13:02:26 by mkervabo          #+#    #+#             */
/*   Updated: 2019/05/08 13:56:16 by mkervabo         ###   ########.fr       */
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

typedef struct	s_toml
{
	t_toml_type	type;
	union {
		int64_t			integer_v;
		float			float_v;
		char			*string_v;
		bool			boolean_v;
		void			*array_v;
		void			*table_v;
	}			value;
	
}				t_toml;

typedef	struct	s_toml_array 
{
	struct s_toml		value;
	struct s_toml_array *next;
}				t_toml_array;

typedef	struct	s_toml_table 
{
	char				*key;
	struct s_toml		value;
	struct s_toml_table *next;
}				t_toml_table;

typedef struct	s_str
{
	size_t				len;
	size_t				capacity;
	char				*inner;
}				t_str;

typedef struct	s_tab
{
	size_t				len;
	size_t				capacity;
	t_toml_type			type;
	t_toml				*inner;
}				t_tab;

t_reader		create_reader(int fd, char *buffer, size_t buffer_size);
int16_t			reader_peek(t_reader *self);
void			reader_next(t_reader *self);

void			ft_error(char *msg);

void			skip_ws(t_reader *r, bool newline);

char			*ten_more(char *str, size_t len);
char			read_escape(t_reader *r);

char			*read_key(t_reader *r);
char			*read_quoted_key(t_reader *r, bool	b);

t_str			create_str(size_t capacity);
void			append_char(t_str *str, char c);
t_tab			create_tab(size_t capacity);
void			append_tab(t_tab *tab, t_toml tom);

t_toml_table	*read_toml(t_reader *r);
t_toml			read_toml_value(t_reader *r);
t_toml     		read_digit(t_reader *r);
t_toml        	read_string(t_reader *r);
t_toml        	read_table(t_reader *r);
t_toml        	read_array(t_reader *r);
t_toml			read_boolean(t_reader *r);


#endif