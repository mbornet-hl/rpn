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
 *   @(#)  [MB]  Version 1.1 du 19/02/09 - 
 */

#include "rpn_header.h"
#include "rpn_proto.h"

%}

comment				(#.*)
hostname				([a-zA-Z][-a-zA-Z0-9]*)
ip					(([1-9][0-9]{0,2}\.){3}[1-9][0-9]{0,2})
option				(-[a-zA-Z])

%%

{comment}				{
						fprintf(stderr, "========== %s ==========\n", yytext);
					}

{option}				{
						printf("option : %s\n", yytext);
					}

{hostname}			|
{ip}					{
						printf("server : %s\n", yytext);
					}

.					{
						printf("<%s>", yytext);
					}

\n					{
						printf("\n");
					}

%%

int main()
{
	yylex();
	return 0;
}

void yyerror(char *errmsg)
{
	fprintf(stderr, "Parser error: %s\n", errmsg);
	exit(RPN_EXIT_SYNTAX);

}

int yywrap()
{
	return 1;
}
