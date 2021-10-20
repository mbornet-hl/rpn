%{
/*
 *   Lexical analyzer for hosts file
 *   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *    @(#)  [MB] ej_hosts_lex.l Version 1.3 du 21/10/17 - 
 */

#include	"../cy/cy_rpn_header.h"
#include  "ej_epub.h"
#include	"ej_cpri.h"
#include	"ej_epri.h"
#include	"ej_hosts_yacc.h"

#if defined(getc)
#undef		    getc
#endif

%}

comment			(#.*)
hostname			([A-Za-z][A-Za-z0-9_\-\.]*)
IP				([0-9]{1,3}(\.[0-9]{1,3}){3})
spaces			([ \t]+)

%start 			CMT DECL

%%

{IP}           {
				EJ_TRACE_LEX("IP          [%s]\n", yytext);
				yylval.IP				= strdup(yytext);
				BEGIN DECL;
				return EJ_IP;
			}


<DECL>{hostname}	{
				EJ_TRACE_LEX("HOSTNAME    [%s]\n", yytext);
				yylval.hostname		= strdup(yytext);
				return EJ_HOSTNAME;
			}

{spaces}		;

{comment}		{
				EJ_TRACE_LEX("COMMENT\n");
			}

<DECL>\n       {
				BEGIN 0;
				EJ_TRACE_LEX("NEWLINE\n");
				return EJ_NEWLINE;
			}

\n			{
				BEGIN 0;
				EJ_TRACE_LEX("NEWLINE\n");
				return EJ_NEWLINE;
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
	        ej_G.line, errmsg);
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

