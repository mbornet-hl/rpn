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
 *	@(#)	[MB] 	Version 1.5 du 19/09/20 - 
 */

#include	"../cy/cy_rpn_header.h"
#include	"../cy/cy_rpn_proto.h"
#include	"do_cpri.h"
#include	"do_epri.h"
#include	"do_iperf_out_yacc.h"

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
bps					(bits\/sec)
kbps                     (Kbits\/sec)
mbps                     (Mbits\/sec)
gbps                     (Gbits\/sec)
bits                     (bits)
bytes				(Bytes)
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
                              // DO_TRACE_LEX_OUT(stderr, "========== %s ==========\n", yytext);
                         }

{space}                  {
                         }

{integer}                {
                              DO_TRACE_LEX_OUT("INTEGER : %s\n", yytext);
                              yylval.i_value           = atoi(yytext);
                              return DO_INT;
                         }

{first}                  {
                              DO_TRACE_LEX_OUT("FIRST   : %s\n", yytext);
                              return DO_FIRST;
                         }

{double}                 {
                              DO_TRACE_LEX_OUT("DOUBLE  : %s\n", yytext);
                              yylval.d_value           = atof(yytext);
                              return DO_DOUBLE;
                         }

^{sum}                   {
                              DO_TRACE_LEX_OUT("SUM     : %s\n", yytext);
                              return DO_SUM;
                         }

^{id}                    {
                              DO_TRACE_LEX_OUT("ID      : %s\n", yytext);
						yylval.i_value			= atoi(yytext);
                              return DO_ID;
                         }

{local}                  {
                              DO_TRACE_LEX_OUT("LOCAL   : %s\n", yytext);
                              return DO_LOCAL;
                         }

{kbps}                   {
                              DO_TRACE_LEX_OUT("Kbps    : %s\n", yytext);
                              return DO_KBPS;
                         }

{mbps}                   {
                              DO_TRACE_LEX_OUT("Mbps    : %s\n", yytext);
                              return DO_MBPS;
                         }

{gbps}                   {
                              DO_TRACE_LEX_OUT("Gbps    : %s\n", yytext);
                              return DO_GBPS;
                         }

{bps}				{
                              DO_TRACE_LEX_OUT("Bps    : %s\n", yytext);
                              return DO_BPS;
					};


{kbytes}                 {
                              DO_TRACE_LEX_OUT("KBytes  : %s\n", yytext);
                              return DO_KBYTES;
                         }

{mbytes}                 {
                              DO_TRACE_LEX_OUT("MBytes  : %s\n", yytext);
                              return DO_MBYTES;
                         }

{gbytes}                 {
                              DO_TRACE_LEX_OUT("GBytes  : %s\n", yytext);
                              return DO_GBYTES;
                         }

{bytes}                  {
                              DO_TRACE_LEX_OUT("Bytes   : %s\n", yytext);
                              return DO_BYTES;
                         }

{bits}                   {
                              DO_TRACE_LEX_OUT("bits    : %s\n", yytext);
                              return DO_BITS;
                         }

{garbage}				{
                              DO_TRACE_LEX_OUT("Garbage  : %s\n", yytext);
                              return DO_GARBAGE;
					}

{sec}                    {
                              DO_TRACE_LEX_OUT("sec     : %s\n", yytext);
                              return DO_SEC;
                         }

{ip}                     {
                              DO_TRACE_LEX_OUT("IP     : %s\n", yytext);
//						return DO_IP;
                         }

^{txt_tcp}               |
^{txt_server}            |
^{txt_client}            |
^{txt_ID}                {
                         }

{bind_failed}            {
                              // DO_TRACE_LEX_OUT("BIND_FAILED : %s\n", yytext);
                         }

{cnx_failed}             {
                              // DO_TRACE_LEX_OUT("CNX_FAILED : %s\n", yytext);
                         }

{ssh_err}				{
					}

{null}				{
                              DO_TRACE_LEX_OUT("NULL     : %s\n", yytext);
                              return DO_NULL;
					}

.                        {
                              printf("<%s>", yytext);
                         }

\n                       {
                              // DO_TRACE_LEX_OUT("\n");
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

						REMOVE_WARN_UNUSED

******************************************************************************/
static void remove_warn_unused(int always_false)
{
	if (always_false == TRUE) {
		yyunput(0, 0);
		input();
	}
}

/******************************************************************************

                              YYERROR

******************************************************************************/
void RPN_NAME(error)(char *errmsg)
{
     fprintf(stderr, "Parser error: %s\n", errmsg);
	remove_warn_unused(FALSE);
     exit(RPN_EXIT_SYNTAX);
}

/******************************************************************************

                              YYWRAP

******************************************************************************/
int RPN_NAME(wrap)()
{
     return 1;
}
