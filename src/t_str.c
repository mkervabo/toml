#include "toml.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	const char	*c_src = src;
	char		*c_dst;

	c_dst = dst;
	while (n--)
		*c_dst++ = *c_src++;
	return (dst);
}

t_str create_str(size_t capacity)
{
	return ((t_str) {
		.len = 0,
		.capacity = capacity,
		.inner = malloc(capacity * sizeof(char))
	});
}

void	append_char(t_str *str, char c)
{
	size_t	new_capacity;
	char	*old;
	
	if (str->len == str->capacity)
	{
		new_capacity = str->capacity * 2;
		old = str->inner;
		str->inner = malloc(new_capacity * sizeof(char));
		ft_memcpy(str->inner, old, str->capacity);
		str->capacity = new_capacity;
	}
	str->inner[str->len++] = c;
}

t_tab create_tab(size_t capacity)
{
	return ((t_tab) {
		.len = 0,
		.capacity = capacity,
		.inner = malloc(capacity * sizeof(t_toml))
	});
}

void	append_tab(t_tab *tab, t_toml tom)
{
	size_t	new_capacity;
	t_toml	*old;
	
	if (tab->len == tab->capacity)
	{
		new_capacity = tab->capacity * 2;
		old = tab->inner;
		tab->inner = malloc(new_capacity * sizeof(t_toml*));
		ft_memcpy(tab->inner, old, tab->capacity);
		tab->capacity = new_capacity;
	}
	tab->inner[tab->len++] = tom;
}