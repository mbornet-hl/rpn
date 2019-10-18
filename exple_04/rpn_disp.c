/*
 *	@(#)	[MB] cy_rpn_disp.c	Version 1.4 du 17/09/29 - 
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

	case	RPN_TYPE_STRING :
		printf("STRING    [%s]\n", elt->value.s);
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
