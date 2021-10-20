%{
/*
 * Copyright (C) 2019-2020, Martial Bornet
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
<<<<<<< HEAD
 *	@(#)	[MB] du_log_lex.l	Version 1.5 du 21/07/23 - 
=======
 *	@(#)	[MB] du_log_lex.l	Version 1.4 du 21/07/21 - 
>>>>>>> d9987656174c5b946bd2d1be7c6d1ee85f575d29
 */

#include <stdlib.h>
#include "../cy/cy_rpn_header.h"
#include "../cy/cy_rpn_proto.h"
#include "du_cpub.h"
#include "du_epub.h"
#include "du_cpri.h"
#include "du_epri.h"
#include "du_log_yacc.h"

%}

osb				(\[)
csb				(\])
IP				(([0-9]{1,3}\.){3}[0-9]{1,3})
time				(([01][0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9])
date				(([0-2][0-9]|3[01])\/(Jan|Feb|Mar|Apr|May|Ju[nl]|Aug|Sep|Oct|Nov|Dec)\/[0-9]{4})
dquote			(\")
<<<<<<< HEAD
GET				(GET\ ([^"]|\\\")+)
PUT				(PUT\ ([^"]|\\\")+)
POST				(POST\ ([^"]|\\\")+)
HEAD				(HEAD\ ([^"]|\\\")+)
OPTIONS			(OPTIONS\ ([^"]|\\\")+)
PROPFIND			(PROPFIND\ ([^"]|\\\")+)
num				([0-9][0-9]*)
=======
GET				(GET\ [^"]+)
PUT				(PUT\ [^"]+)
POST				(POST\ [^"]+)
HEAD				(HEAD\ [^"]+)
OPTIONS			(OPTIONS\ [^"]+)
PROPFIND			(PROPFIND\ [^"]+)
num				([1-9][0-9]*)
>>>>>>> d9987656174c5b946bd2d1be7c6d1ee85f575d29
dash				(-)

%%

{osb}			{
					/* Opening Square Bracket
					   ~~~~~~~~~~~~~~~~~~~~~~ */
					DU_TRACE_LEX("OSB      : %s\n", yytext);
					return DU_OSB;
				}

{csb}			{
					/* Closing Square Bracket
					   ~~~~~~~~~~~~~~~~~~~~~~ */
					DU_TRACE_LEX("CSB      : %s\n", yytext);
					return DU_CSB;
				}

{dquote}			{
					/* Double quote
					   ~~~~~~~~~~~~ */
					DU_TRACE_LEX("dquote   : %s\n", yytext);
					return DU_DQUOTE;
				}

{IP}				{
					/* DEBUG ONLY !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
<<<<<<< HEAD
					// du_G.debug_level		= 0xFFFF;
=======
					du_G.debug_level		= 0xFFFF;
>>>>>>> d9987656174c5b946bd2d1be7c6d1ee85f575d29

					DU_TRACE_LEX("IP       : %s\n", yytext);
					yylval.s_value		= strdup(yytext);
					return DU_IP;
				}

{time}			{
					DU_TRACE_LEX("time     : %s\n", yytext);
					yylval.s_value		= strdup(yytext);
					return DU_TIME;
				}

{date}			{
					DU_TRACE_LEX("date     : %s\n", yytext);
					yylval.s_value		= strdup(yytext);
					return DU_DATE;
				}

{GET}			{
					DU_TRACE_LEX("GET      : %s\n", yytext);
					yylval.s_value		= strdup(yytext);
					return DU_GET;
				}

{POST}			{
					DU_TRACE_LEX("POST     : %s\n", yytext);
					yylval.s_value		= strdup(yytext);
					return DU_POST;
				}

{PUT}			{
					DU_TRACE_LEX("PUT      : %s\n", yytext);
					yylval.s_value		= strdup(yytext);
					return DU_PUT;
				}

{HEAD}			{
					DU_TRACE_LEX("HEAD     : %s\n", yytext);
					yylval.s_value		= strdup(yytext);
					return DU_HEAD;
				}

{OPTIONS}			{
					DU_TRACE_LEX("OPTIONS  : %s\n", yytext);
					yylval.s_value		= strdup(yytext);
					return DU_OPTIONS;
				}

{PROPFIND}		{
					DU_TRACE_LEX("PROPFIND : %s\n", yytext);
					yylval.s_value		= strdup(yytext);
					return DU_PROPFIND;
				}

{num}			{
					DU_TRACE_LEX("num      : %s\n", yytext);
					yylval.d_value		= atol(yytext);
					return DU_NUM;
				}

{dash}			{
					DU_TRACE_LEX("dash     : %s\n", yytext);
					return DU_DASH;
				}

<<<<<<< HEAD
.				{
					// fprintf(stderr, "<%s>",  yytext);
				}

\n				{
					du_G.num_line++;
					DU_TRACE_LEX("Line %10ld\n", du_G.num_line);
=======
.|\n				{
					// fprintf(stderr, "<%s>",  yytext);
					du_G.num_line++;
>>>>>>> d9987656174c5b946bd2d1be7c6d1ee85f575d29
				}

%%

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
void yyerror(char *errmsg)
{
	fprintf(stderr, "%s: line %ld : parser error: %s\n", G.progname,
	        du_G.num_line,errmsg);
	remove_warn_unused(TRUE);
	exit(RPN_EXIT_SYNTAX);
}

/******************************************************************************

						YYWRAP

******************************************************************************/
int yywrap()
{
	return 1;
}

