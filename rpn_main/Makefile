#
# Copyright (C) 2017-2018, Martial Bornet
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
#	@(#)	[MB] cy_Makefile	Version 1.14 du 19/06/08 - 
#

CC				= gcc
LEX				= lex
YACC				= yacc
CFLAGS			= -Werror -rdynamic -g -Wuninitialized
LDFLAGS			= -lm

LIBS				= ../df/lib

# PROGS			= rpn iperf_out_lex iperf_in_lex
PROGS			= rpn

RPN_HEADERS		= rpn_types.h rpn_header.h rpn_proto.h

RPN_OBJS			= rpn_custom.o rpn_disp.o rpn_utils.o rpn_segv.o		\
				  iperf_out_lex.o iperf_out_yacc.o rpn_dump.o

RPN_FILES			= $(RPN_HEADERS) $(RPN_OBJS)

all				: $(PROGS)

clean			:
				rm -f *.o *_yacc.h

rpn				: rpn_lex.l $(RPN_FILES)
				$(LEX) rpn_lex.l
				rm -f rpn_lex.c; mv lex.yy.c rpn_lex.c
				$(CC) $(CFLAGS) -o rpn rpn_lex.c $(RPN_OBJS) $(LDFLAGS) $(LIBS)

rpn_custom.o		: rpn_custom.c $(RPN_HEADERS)
				$(CC) $(CFLAGS) -c rpn_custom.c

rpn_disp.o		: rpn_disp.c $(RPN_HEADERS)
				$(CC) $(CFLAGS) -c rpn_disp.c

rpn_dump.o		: rpn_dump.c $(RPN_HEADERS)
				$(CC) $(CFLAGS) -c rpn_dump.c

rpn_utils.o		: rpn_utils.c $(RPN_HEADERS)
				$(CC) $(CFLAGS) -c rpn_utils.c

rpn_segv.o		: rpn_segv.c $(RPN_HEADERS)
				$(CC) $(CFLAGS) -c rpn_segv.c

PREF_OUT			= iperf_out_
PREF_IN			= iperf_in_

iperf_out_lex		: iperf_out_lex.o
				$(CC) $(CFLAGS) -o iperf_out_lex $(PREF_OUT)lex.o $(RPN_OBJS) $(LDFLAGS) $(LIBS)

iperf_out_lex.o	: iperf_out_lex.c iperf_out_yacc.h
				$(CC) $(CFLAGS) -DPREF_OUT="$(PREF_OUT)" -c $(PREF_OUT)lex.c

iperf_out_lex.c	: rpn_iperf_out_lex.l
				$(LEX) -P $(PREF_OUT) rpn_iperf_out_lex.l
				rm -f rpn_iperf_out_lex.c; mv lex.$(PREF_OUT).c $(PREF_OUT)lex.c

iperf_out_yacc.h	: rpn_iperf_out_yacc.y
				$(YACC) -d -p $(PREF_OUT) rpn_iperf_out_yacc.y
				mv y.tab.h rpn_iperf_out_yacc.h

iperf_out_yacc		: iperf_out_yacc.o
				$(CC) $(CFLAGS) -o iperf_out_yacc $(PREF_OUT)yacc.o $(RPN_OBJS) $(LDFLAGS) $(LIBS)

iperf_out_yacc.o	: iperf_out_yacc.c
				$(CC) $(CFLAGS) -DPREF_OUT="$(PREF_OUT)" -c $(PREF_OUT)yacc.c

iperf_out_yacc.c	: rpn_iperf_out_yacc.y
				$(YACC) -p $(PREF_OUT) rpn_iperf_out_yacc.y
				rm -f rpn_iperf_out_yacc.c; mv y.tab.c $(PREF_OUT)yacc.c

iperf_in_lex		: rpn_iperf_in_lex.l
				$(LEX) -P $(PREF_IN) rpn_iperf_in_lex.l
				rm -f rpn_iperf_in_lex.c; mv lex.$(PREF_IN).c $(PREF_IN)lex.c
				$(CC) $(CFLAGS) -o iperf_in_lex $(PREF_IN)lex.c $(RPN_OBJS) $(LDFLAGS) $(LIBS)
