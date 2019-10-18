#
# Copyright (C) 2017, Martial Bornet
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#	Makefile for RPN tests
#	~~~~~~~~~~~~~~~~~~~~~~
#
#	%Z%	[%Y%] %M%	Version %I% du %E% - %Q%
#

LEX			= lex
CFLAGS		= -Werror

PROGS		= rpn

RPN_HEADERS	= rpn_types.h rpn_header.h rpn_proto.h

RPN_OBJS		= rpn_custom.o rpn_disp.o rpn_utils.o

RPN_FILES		= $(RPN_HEADERS) $(RPN_OBJS)

a006			: rpn_lex.l $(RPN_FILES)
			$(LEX) rpn_lex.l
			rm -f rpn_lex.c; mv lex.yy.c rpn_lex.c
			$(CC) $(CFLAGS) -o rpn rpn_lex.c $(RPN_OBJS)

rpn_custom.o	: rpn_custom.c $(RPN_HEADERS)
			$(CC) $(CFLAGS) -c rpn_custom.c

rpn_disp.o	: rpn_disp.c $(RPN_HEADERS)
			$(CC) $(CFLAGS) -c rpn_disp.c

rpn_utils.o	: rpn_utils.c $(RPN_HEADERS)
			$(CC) $(CFLAGS) -c rpn_utils.c

