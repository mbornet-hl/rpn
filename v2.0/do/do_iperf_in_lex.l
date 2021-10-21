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
 *	@(#)	[MB] 	Version 1.8 du 19/10/16 - 
 */

/* Test configuration file syntax :
 *
 * # Comment
 *
 * SERVER_OPTS = server_options
 * CLIENT_OPTS = client_options
 *
 * server
 * server : client_specific_options
 * server : client_specific_options : server_specific_options
 * server : client_specific_options : server_specific_options : server_port
 */

 
#include	"../cy/cy_rpn_header.h"
#include	"../cy/cy_rpn_proto.h"
#include	"do_cpri.h"
#include	"do_epri.h"
#include	"do_iperf_in_yacc.h"

#define   CONCAT(A, B)        A##B
#define   EVAL_CONCAT(A, B)   CONCAT(A, B)
#define   RPN_NAME(suffix)    EVAL_CONCAT(PREF_IN, suffix)

#define   yylval              RPN_NAME(lval)

%}

comment				(#.*)
c_opts_var			(CLIENT_OPTS)
s_opts_var			(SERVER_OPTS)
s_port_var			(SERVER_PORT)
s_timeout				(SERVER_TIMEOUT)
equal				(=)
hostname				([a-zA-Z][-a-zA-Z0-9]*)
ip					(([1-9][0-9]{0,2}\.){3}[1-9][0-9]{0,2})
option				(-[a-zA-Z])
field_sep				(:)
space				([ \t]+)
num					([1-9][0-9]*)

%%

{comment}				{
						DO_TRACE_LEX_IN("========== %s ==========\n", yytext);
						return DO_COMMENT;
					}

^{c_opts_var}			{
						DO_TRACE_LEX_IN("variable  : [%s]\n", yytext);
						yylval.s_value		= strdup(yytext);
						return DO_VAR_C_OPTS;
					}

^{s_opts_var}			{
						DO_TRACE_LEX_IN("variable  : [%s]\n", yytext);
						yylval.s_value		= strdup(yytext);
						return DO_VAR_S_OPTS;
					}

^{s_port_var}			{
						DO_TRACE_LEX_IN("variable  : [%s]\n", yytext);
						yylval.s_value		= strdup(yytext);
						return DO_VAR_S_PORT;
					}

^{s_timeout}			{
						DO_TRACE_LEX_IN("variable  : [%s]\n", yytext);
						yylval.s_value		= strdup(yytext);
						return DO_VAR_S_TIMEOUT;
					}

{equal}				{
						DO_TRACE_LEX_IN("equal     : [%s]\n", yytext);
						return DO_EQ;
					}

{field_sep}			{
						DO_TRACE_LEX_IN("field sep : [%s]\n", yytext);
						do_G.field_no++;
						DO_TRACE_LEX_IN("field_no <== %d\n", do_G.field_no);
						return DO_SEP;
					}

{option}				{
						DO_TRACE_LEX_IN("option    : [%s]\n", yytext);
						yylval.s_value		= strdup(yytext);
						return DO_OPT;
					}

^{hostname}			|
^{ip}				{
						DO_TRACE_LEX_IN("server    : [%s]\n", yytext);
						do_G.field_no		= 1;
						DO_TRACE_LEX_IN("field_no <== %d\n", do_G.field_no);
						yylval.s_value		= strdup(yytext);
						return DO_SERVER;
					}

{num}				{
						DO_TRACE_LEX_IN("num       : [%s]\n", yytext);
						yylval.s_value		= strdup(yytext);
						return DO_INT_PARAM;
					}

{space}				{
						DO_TRACE_LEX_IN("space     : [%s]\n", yytext);
					}

.					{
						DO_TRACE_LEX_IN("<%s>", yytext);
					}

\n					{
						DO_TRACE_LEX_IN("\n");
						//do_G.field_no		= 0;
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
	fprintf(stderr, "%s: parser error: %s\n", G.progname, errmsg);
	remove_warn_unused(FALSE);
	exit(RPN_EXIT_SYNTAX);
}

/******************************************************************************

						YYWRAP

******************************************************************************/
int yywrap()
{
	return 1;
}
