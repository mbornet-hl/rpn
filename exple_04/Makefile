#
#	Makefile for RPN tests
#	~~~~~~~~~~~~~~~~~~~~~~
#
#	%Z%	[%Y%] %M%	Version %I% du %E% - %Q%
#

LEX			= lex
CFLAGS		= -Werror

PROGS		= a004

RPN_HEADERS	= rpn_types.h rpn_header.h

RPN_OBJS		= rpn_custom.o rpn_disp.o rpn_utils.o

RPN_FILES		= $(RPN_HEADERS) $(RPN_OBJS)

a004			: rpn_lex.l $(RPN_FILES)
			$(LEX) rpn_lex.l
			rm -f rpn_lex.c; mv lex.yy.c rpn_lex.c
			$(CC) $(CFLAGS) -o a004 rpn_lex.c $(RPN_FILES)

rpn_custom.o	: rpn_custom.c $(RPN_HEADERS)
			$(CC) $(CFLAGS) -c rpn_custom.c

rpn_disp.o	: rpn_disp.c $(RPN_HEADERS)
			$(CC) $(CFLAGS) -c rpn_disp.c

rpn_utils.o	: rpn_utils.c $(RPN_HEADERS)
			$(CC) $(CFLAGS) -c rpn_utils.c

