%{
/*
 * Copyright (C) 2017, Martial Bornet
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
 *	@(#)	[MB] cy_rpn_lex.l	Version 1.3 du 17/11/05 - 
 */

#include "rpn_header.h"

extern struct rpn_operator      my_operators[];

%}

integer			([-+]?(0|[1-9][0-9]*))
float			([-+]?(0|[1-9][0-9]*)\.([0-9]*)?)
string			(\"[^"]*\")
litteral			(\'[^']*\')
operator			([-+*/]|cat|dupl|length|mat)
spaces			([ 	]+)

%%

{integer}		{
				rpn_elt				*_elt_i;

				/* Allocate element descriptors
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				_elt_i		= rpn_new_elt();

				_elt_i->value.i     = atoi(yytext);
				rpn_set_type(_elt_i, RPN_TYPE_INT);

				/* Push element on the stack
				   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
				rpn_push(G.stack, _elt_i);

				/* Display stack
				   ~~~~~~~~~~~~~ */
				rpn_disp_stack(G.stack);
			}

{float}		{
				rpn_elt				*_elt_f;

				/* Allocate element descriptors
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				_elt_f		= rpn_new_elt();

				_elt_f->value.f     = atof(yytext);
				rpn_set_type(_elt_f, RPN_TYPE_FLOAT);

				/* Push element on the stack
				   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
				rpn_push(G.stack, _elt_f);

				/* Display stack
				   ~~~~~~~~~~~~~ */
				rpn_disp_stack(G.stack);
			}

{string}		{
				rpn_elt				*_elt_s;

				/* Allocate element descriptors
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				_elt_s		= rpn_new_elt();

				_elt_s->value.s	= yytext;
				if (rpn_strip_quotes(&_elt_s->value.s) != RPN_RET_OK) {
					rpn_err_msg_invalid_string(_elt_s->value.s);
					exit(RPN_EXIT_INVALID_ELT);
				}
				rpn_set_type(_elt_s, RPN_TYPE_STRING);

				/* Push element on the stack
				   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
				rpn_push(G.stack, _elt_s);

				/* Display stack
				   ~~~~~~~~~~~~~ */
				rpn_disp_stack(G.stack);
			}

{litteral}	{
				rpn_elt				*_elt_s;

				/* Allocate element descriptors
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				_elt_s		= rpn_new_elt();

				_elt_s->value.s	= yytext;
				if (rpn_strip_single_quotes(&_elt_s->value.s) != RPN_RET_OK) {
					rpn_err_msg_invalid_string(_elt_s->value.s);
					exit(RPN_EXIT_INVALID_ELT);
				}
				rpn_set_type(_elt_s, RPN_TYPE_LITTERAL);

				/* Push element on the stack
				   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
				rpn_push(G.stack, _elt_s);

				/* Display stack
				   ~~~~~~~~~~~~~ */
				rpn_disp_stack(G.stack);
			}

{operator}	{
				char					*_operator;
				rpn_operator			*_op;
				int					 _retcode;

				/* Initialize operator
				   ~~~~~~~~~~~~~~~~~~~ */
				_operator		= yytext;

				if ((_op = rpn_search_op(my_operators, _operator)) == 0) {
					rpn_err_msg_op_not_found(_operator);
					exit(RPN_EXIT_OP_UNKNOWN);
				}

				/* Execute operator
				   ~~~~~~~~~~~~~~~~ */
				if ((_retcode = (*_op->func)(G.stack)) != RPN_RET_OK) {
					rpn_err_msg_op_error(_op->op_name, _retcode);
					exit(RPN_EXIT_OP_ERROR);
				}

				/* Display stack
				   ~~~~~~~~~~~~~ */
				rpn_disp_stack(G.stack);
			}

{spaces}		;

.			{
				printf("[%s] ", yytext);
			}

\n			;

%%

/******************************************************************************

						MAIN

******************************************************************************/
int main(int argc, char *argv[])
{
	G.progname	= argv[0];

	/* Allocate stack descriptor
	   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
	G.stack		= rpn_new_stack();

	/* Display stack
	   ~~~~~~~~~~~~~ */
	rpn_disp_stack(G.stack);

	yylex();
	return 0;
}

/******************************************************************************

						YYERROR

******************************************************************************/
void yyerror(char *errmsg)
{
	fprintf(stderr, "Error: %s\n", errmsg);
	exit(RPN_EXIT_SYNTAX);
}

/******************************************************************************

						YYWRAP

******************************************************************************/
int yywrap()
{
	return 1;
}
