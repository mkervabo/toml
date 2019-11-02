# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mkervabo <mkervabo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/05/12 13:29:47 by mkervabo          #+#    #+#              #
#    Updated: 2019/11/02 16:37:40 by dde-jesu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SECONDARY:

libtoml.rootdir := $(dir $(lastword $(MAKEFILE_LIST)))

libtoml.objects := append.o array.o bool.o create.o digit.o dotted_key.o key.o \
		multi_string.o reader.o string.o table.o toml.o free.o error.o
libtoml.objects := $(addprefix $(libtoml.rootdir)src/, $(libtoml.objects))

$(libtoml.objects): CC       = gcc
$(libtoml.objects): CFLAGS   ?= -Wall -Wextra -Werror
$(libtoml.objects): CPPFLAGS += -MMD -MP -I$(libtoml.rootdir)include

.PHONY: all
all: libtoml.a

libtoml.a: libtoml.a($(libtoml.objects))

.PHONY: clean
clean::
	$(RM) $(libtoml.objects:.o=.{o,d,gcno,gcna})

.PHONY: fclean
fclean:: clean
	$(RM) libtoml.a

.PHNOY: re
re: fclean all

-include $(wildcard $(libtoml.rootdir)src/*.d)
