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
 *	@(#)	[MB] cy_rpn_disp.c	Version 1.3.1.1 du 17/11/03 - 
 */

#include	"rpn_header.h"


/******************************************************************************

					RPN_DISP_ELT

******************************************************************************/
void rpn_disp_elt(rpn_elt *elt)
{
	int				 _type;

	switch (_type = rpn_get_type(elt)) {

	case	RPN_TYPE_INT :
		printf("INT       %10d\n", elt->value.i);
		break;

	default :
		fprintf(stderr, "%s: unknown element type (%d)\n", G.progname, _type);
		exit(RPN_EXIT_INTERNAL_ERR);
	}
}

/******************************************************************************

					RPN_DISP_STACK

******************************************************************************/
void rpn_disp_stack(rpn_stack *stack)
{
	rpn_elt			*_elt;
	int				 _num	= 0;

	if (stack->base_elt == 0) {
		printf("STACK EMPTY\n");
	}
	else {
		for (_elt = stack->base_elt; _elt != 0; _elt = _elt->next_elt, _num++) {
			printf("STACK %3d   : ", _num);
			rpn_disp_elt(_elt);
		}
	}

	printf("\n");
}
