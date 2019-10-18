#
#	Makefile for RPN tests
#	~~~~~~~~~~~~~~~~~~~~~~
#
#	@(#)	[MB] cy_Makefile	Version 1.1 du 17/09/27 - 
#
#	:vim:se ts=5 sw=5:

PROGS		= a001

RPN_HEADERS	= rpn_types.h rpn_header.h

RPN_OBJS		= rpn_custom.o rpn_disp.o rpn_utils.o

RPN_FILES		= $(RPN_HEADERS) $(RPN_OBJS)

a001			: a001.c $(RPN_FILES)
			$(CC) -o a001 a001.c $(RPN_FILES)
