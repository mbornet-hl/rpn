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
 *   @(#)  [MB]  Version 1.7 du 19/06/08 - 
 */

#include "rpn_header.h"
#include "rpn_proto.h"
#include "rpn_iperf_out_yacc.h"

#define   CONCAT(A, B)        A##B
#define   EVAL_CONCAT(A, B)   CONCAT(A, B)
#define   RPN_NAME(suffix)    EVAL_CONCAT(PREF_OUT, suffix)

#define   yylval              RPN_NAME(lval)

%}

comment                  (-+)
space                    ([ \t]+)
integer                  ([1-9][0-9]*|0)
first                    (([0-9]+\.[0-9]+)-)
double                   ([0-9]+\.[0-9]+)
hostname                 ([a-zA-Z][-a-zA-Z0-9]*)
sum                      (\[SUM\])
id                       (\[\ *[0-9]+\])
local                    (local\ .*)
bps					(Bytes\/sec)
kbps                     (Kbits\/sec)
mbps                     (Mbits\/sec)
gbps                     (Gbits\/sec)
kbytes                   (KBytes)
mbytes                   (MBytes)
gbytes                   (GBytes)
garbage				([^\001-\177]+\ *[^\001-\177]+s)
sec                      (sec)
sep                      (-)
ip                       (([1-9][0-9]{0,2}\.){3}[1-9][0-9]{0,2})
txt_tcp                  (TCP\ .+)
txt_server               (Server\ .+)
txt_client               (Client\ .+)
txt_ID                   (\[\ *ID\].+)
bind_failed              (bind\ failed.+)
cnx_failed               (connect\ failed.+)
ssh_err				(ssh:\ connect\ to\ host.*)
null					(\(null\)s?)

%%

^{comment}               {
                              // RPN_IPERF_TRACE_LEX(stderr, "========== %s ==========\n", yytext);
                         }

{space}                  {
                         }

{integer}                {
                              RPN_IPERF_TRACE_LEX("INTEGER : %s\n", yytext);
                              yylval.i_value           = atoi(yytext);
                              return CY_INT;
                         }

{first}                  {
                              RPN_IPERF_TRACE_LEX("FIRST   : %s\n", yytext);
                              return CY_FIRST;
                         }

{double}                 {
                              RPN_IPERF_TRACE_LEX("DOUBLE  : %s\n", yytext);
                              yylval.d_value           = atof(yytext);
                              return CY_DOUBLE;
                         }

^{sum}                   {
                              RPN_IPERF_TRACE_LEX("SUM     : %s\n", yytext);
                              return CY_SUM;
                         }

^{id}                    {
                              RPN_IPERF_TRACE_LEX("ID      : %s\n", yytext);
						yylval.i_value			= atoi(yytext);
                              return CY_ID;
                         }

{local}                  {
                              RPN_IPERF_TRACE_LEX("LOCAL   : %s\n", yytext);
                              return CY_LOCAL;
                         }

{kbps}                   {
                              RPN_IPERF_TRACE_LEX("Kbps    : %s\n", yytext);
                              return CY_KBPS;
                         }

{mbps}                   {
                              RPN_IPERF_TRACE_LEX("Mbps    : %s\n", yytext);
                              return CY_MBPS;
                         }

{gbps}                   {
                              RPN_IPERF_TRACE_LEX("Gbps    : %s\n", yytext);
                              return CY_GBPS;
                         }

{bps}				{
                              RPN_IPERF_TRACE_LEX("Bps    : %s\n", yytext);
                              return CY_BPS;
					};


{kbytes}                 {
                              RPN_IPERF_TRACE_LEX("Kbytes  : %s\n", yytext);
                              return CY_KBYTES;
                         }

{mbytes}                 {
                              RPN_IPERF_TRACE_LEX("Mbytes  : %s\n", yytext);
                              return CY_MBYTES;
                         }

{gbytes}                 {
                              RPN_IPERF_TRACE_LEX("Gbytes  : %s\n", yytext);
                              return CY_GBYTES;
                         }

{garbage}				{
                              RPN_IPERF_TRACE_LEX("Garbage  : %s\n", yytext);
                              return CY_GARBAGE;
					}

{sec}                    {
                              RPN_IPERF_TRACE_LEX("sec     : %s\n", yytext);
                              return CY_SEC;
                         }

{ip}                     {
                              RPN_IPERF_TRACE_LEX("IP     : %s\n", yytext);
                         }

^{txt_tcp}               |
^{txt_server}            |
^{txt_client}            |
^{txt_ID}                {
                         }

{bind_failed}            {
                              // RPN_IPERF_TRACE_LEX("BIND_FAILED : %s\n", yytext);
                         }

{cnx_failed}             {
                              // RPN_IPERF_TRACE_LEX("CNX_FAILED : %s\n", yytext);
                         }

{ssh_err}				{
					}

{null}				{
                              RPN_IPERF_TRACE_LEX("NULL     : %s\n", yytext);
                              return CY_NULL;
					}

.                        {
                              printf("<%s>", yytext);
                         }

\n                       {
                              // RPN_IPERF_TRACE_LEX("\n");
                         }

%%

#if 0
/******************************************************************************

                              MAIN

******************************************************************************/
int main()
{
     yylex();
     return 0;
}
#endif

/******************************************************************************

                              YYERROR

******************************************************************************/
void RPN_NAME(error)(char *errmsg)
{
     fprintf(stderr, "Parser error: %s\n", errmsg);
     exit(RPN_EXIT_SYNTAX);
}

/******************************************************************************

                              YYWRAP

******************************************************************************/
int RPN_NAME(wrap)()
{
     return 1;
}
