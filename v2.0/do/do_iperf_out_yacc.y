%{
/*
 * Copyright (C) 2017-2019, Martial Bornet
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	@(#)	[MB] 	Version 1.7 du 19/10/24 - 
 */

#include "../cy/cy_rpn_header.h"
#include "../cy/cy_rpn_proto.h"
#include	"do_cpri.h"
#include	"do_epri.h"

int					 do_iperf_out_lex();
void					 do_iperf_out_error(char *);
int					 do_iperf_out_input(void);

%}

%union {
     int                  i_value;
     double               d_value;
	rpn_elt			*elt;
}

%token    <i_value>      DO_INT DO_ID
%token    <d_value>      DO_DOUBLE
%token                   DO_SUM DO_FIRST DO_GBYTES DO_MBYTES DO_KBYTES DO_BYTES DO_BITS
%token                   DO_SEC DO_LOCAL DO_GBPS   DO_MBPS   DO_KBPS   DO_BPS
%token                   DO_NULL DO_GARBAGE 

%type     <d_value>      scalar unit
%type	<elt>		bandwidth

%start                   results

%%

results        : result
               {
                    DO_TRACE_YACC_OUT("result\n");
               }
               | results result
               {
                    DO_TRACE_YACC_OUT("results result\n");
               }
               | /* EMPTY */
               {
                    DO_TRACE_YACC_OUT("EMPTY\n");
               }
               ;

               ;

result         : DO_ID interval transfer bandwidth
               {
#if 0
				rpn_list				*_list;

Z
                    DO_TRACE_YACC_OUT("ID  interval transfer bandwidth\n");
                    DO_TRACE_YACC_OUT("bandwidth = %10e\n", $4->value.d);

Z
				_list				= do_G.ID_list->value.obj;
Z
				rpn_list_push_head(_list, $4);
#endif
               }
			| DO_SUM interval transfer bandwidth
			{
				rpn_list				*_list;

                    DO_TRACE_YACC_OUT("SUM interval transfer bandwidth\n");
                    DO_TRACE_YACC_OUT("bandwidth = %10e\n", $4->value.d);

				_list				=do_G.SUM_list->value.obj;
				rpn_list_push_head(_list, $4);
			}
               | DO_ID DO_LOCAL
               {
                    DO_TRACE_YACC_OUT("ID local\n");
               }
               ;

interval       : DO_FIRST DO_DOUBLE DO_SEC
               {
                    DO_TRACE_YACC_OUT("FIRST DOUBLE SEC\n");
               }
               ;

transfer       : scalar DO_GBYTES
               {
                    DO_TRACE_YACC_OUT("scalar GBYTES : [%e]\n", $1);
               }
               | scalar DO_MBYTES
               {
                    DO_TRACE_YACC_OUT("scalar MBYTES : [%e]\n", $1);
               }
               | scalar DO_KBYTES
               {
                    DO_TRACE_YACC_OUT("scalar KBYTES : [%e]\n", $1);
               }
               | scalar DO_GARBAGE
               {
                    DO_TRACE_YACC_OUT("scalar GARBAGE\n");
               }
               | scalar DO_BYTES
               {
                    DO_TRACE_YACC_OUT("scalar BYTES  : [%e]\n", $1);
               }
               | scalar DO_BITS
               {
                    DO_TRACE_YACC_OUT("scalar BYTES  : [%e]\n", $1);
               }
               | scalar DO_NULL
               {
                    DO_TRACE_YACC_OUT("scalar NULL   : [%e]\n", $1);
               }
               ;

scalar         : DO_INT
               {
                    DO_TRACE_YACC_OUT("INT\n");
                    $$             = (float) $1;
               }
               | DO_DOUBLE
               {
                    DO_TRACE_YACC_OUT("DOUBLE\n");
                    $$             = $1;
               }
               ;

bandwidth      : scalar unit
               {
                    DO_TRACE_YACC_OUT("scalar unit [%e]\n", $1 * $2);
				rpn_elt		*_elt;

				_elt           = rpn_new_elt(RPN_TYPE_DOUBLE);
				_elt->value.d  = $1 * $2;

				$$			= _elt;
               }
               ;

unit           : DO_KBPS
               {
               {
                    DO_TRACE_YACC_OUT("KBPS\n");
                    $$             = 1.0e3;
               }
               }
               | DO_MBPS
               {
                    DO_TRACE_YACC_OUT("MBPS\n");
                    $$             = 1.0e6;
               }
               | DO_GBPS
               {
                    DO_TRACE_YACC_OUT("GBPS\n");
                    $$             = 1.0e9;
               }
               | DO_BPS
               {
                    DO_TRACE_YACC_OUT("BPS\n");
                    $$             = 1.0;
               }
               ;
