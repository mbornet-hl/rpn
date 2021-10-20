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
 *   @(#)  [MB]  Version 1.6 du 19/06/08 - 
 */

#include "rpn_header.h"
#include "rpn_proto.h"

%}

%union {
     int                  i_value;
     double               d_value;
}

%token    <i_value>      CY_INT CY_ID
%token    <d_value>      CY_DOUBLE
%token                   CY_SUM CY_FIRST CY_GBYTES CY_MBYTES CY_KBYTES CY_BYTES 
%token                   CY_SEC CY_LOCAL CY_GBPS   CY_MBPS   CY_KBPS   CY_BPS
%token                   CY_NULL CY_GARBAGE 

%type     <d_value>      scalar unit

%start                   results

%%

results        : result
               {
                    RPN_IPERF_TRACE_YACC("result\n");
               }
               | results result
               {
                    RPN_IPERF_TRACE_YACC("results result\n");
               }
               | /* EMPTY */
               {
                    RPN_IPERF_TRACE_YACC("EMPTY\n");
               }
               ;

               ;

result         : ID interval transfer bandwidth
               {
                    RPN_IPERF_TRACE_YACC("ID interval transfer bandwidth\n");
               }
               | ID CY_LOCAL
               {
                    RPN_IPERF_TRACE_YACC("ID local\n");
               }
               ;

ID             : CY_ID
               {
                    RPN_IPERF_TRACE_YACC("ID       : %3d\n", $1);
                    G.iperf.sum_found        = FALSE;
               }
               | CY_SUM
               {
                    RPN_IPERF_TRACE_YACC("SUM\n");
                    G.iperf.sum_found        = TRUE;
               }
               ;

interval       : CY_FIRST CY_DOUBLE CY_SEC
               {
                    RPN_IPERF_TRACE_YACC("FIRST DOUBLE SEC\n");
               }
               ;

transfer       : scalar CY_GBYTES
               {
                    RPN_IPERF_TRACE_YACC("scalar GBYTES : [%e]\n", $1);
               }
               | scalar CY_MBYTES
               {
                    RPN_IPERF_TRACE_YACC("scalar MBYTES : [%e]\n", $1);
               }
               | scalar CY_KBYTES
               {
                    RPN_IPERF_TRACE_YACC("scalar KBYTES : [%e]\n", $1);
               }
               | scalar CY_GARBAGE
               {
                    RPN_IPERF_TRACE_YACC("scalar GARBAGE\n");
               }
               | scalar CY_BYTES
               {
                    RPN_IPERF_TRACE_YACC("scalar BYTES  : [%e]\n", $1);
               }
               | scalar CY_NULL
               {
                    RPN_IPERF_TRACE_YACC("scalar NULL   : [%e]\n", $1);
               }
               ;

scalar         : CY_INT
               {
                    RPN_IPERF_TRACE_YACC("INT\n");
                    $$             = (float) $1;
               }
               | CY_DOUBLE
               {
                    RPN_IPERF_TRACE_YACC("DOUBLE\n");
                    $$             = $1;
               }
               ;

bandwidth      : scalar unit
               {
                    RPN_IPERF_TRACE_YACC("scalar unit [%e]\n", $1 * $2);
                    if (G.iperf.sum_found) {
                         rpn_elt        *_elt;
                         rpn_list       *_list;

                         _elt           = rpn_new_elt(RPN_TYPE_DOUBLE);
                         _elt->value.d  = $1 * $2;
//                       printf("Bandwidth : %e\n", _elt->value.d);

                         _list          = G.iperf.elt_list->value.obj;
                         rpn_list_push_head(_list, _elt);
                    }
               }
               ;

unit           : CY_KBPS
               {
               {
                    RPN_IPERF_TRACE_YACC("KBPS\n");
                    $$             = 1.0e3;
               }
               }
               | CY_MBPS
               {
                    RPN_IPERF_TRACE_YACC("MBPS\n");
                    $$             = 1.0e6;
               }
               | CY_GBPS
               {
                    RPN_IPERF_TRACE_YACC("GBPS\n");
                    $$             = 1.0e9;
               }
               | CY_BPS
               {
                    RPN_IPERF_TRACE_YACC("BPS\n");
                    $$             = 1.0;
               }
               ;
