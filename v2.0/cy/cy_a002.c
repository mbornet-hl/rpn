/*
 *	a002.c
 *
 *	@(#)	[MB] cy_a002.c	Version 1.1 du 17/09/28 - 
 */

#include	"rpn_header.h"

extern struct rpn_operator      my_operators[];

/******************************************************************************

						MAIN

******************************************************************************/
int main(int argc, char *argv[])
{
	rpn_stack				*_stack;
	rpn_elt				*_elt_x, *_elt_y;
	char					*_operator;
	rpn_operator			*_op;
	int					 _retcode;

	G.progname	= argv[0];

	/* Check arguments
	   ~~~~~~~~~~~~~~~ */
	if (argc != 3 && argc != 4) {
		fprintf(stderr, "Usage: %s arg1 arg2 operator\n", argv[0]);
		fprintf(stderr, "or :   %s arg operator\n", argv[0]);
		exit(RPN_EXIT_USAGE);
	}

	/* Allocate stack descriptor
	   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
	_stack		= rpn_new_stack();

	/* Display stack
	   ~~~~~~~~~~~~~ */
	rpn_disp_stack(_stack);

	if (argc == 3) {
		/* Allocate element descriptors
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		_elt_x		= rpn_new_elt();

		/* Initialize element (X)
		   ~~~~~~~~~~~~~~~~~~~~~~ */
		_elt_x->value.s	= argv[1];
		if (rpn_strip_quotes(&_elt_x->value.s) != RPN_RET_OK) {
			rpn_err_msg_invalid_string(_elt_x->value.s);
			exit(RPN_EXIT_INVALID_ELT);
		}
		rpn_set_type(_elt_x, RPN_TYPE_STRING);

		/* Push X on the stack
		   ~~~~~~~~~~~~~~~~~~~ */
		rpn_push(_stack, _elt_x);

		/* Display stack
		   ~~~~~~~~~~~~~ */
		rpn_disp_stack(_stack);

		/* Initialize operator
		   ~~~~~~~~~~~~~~~~~~~ */
		_operator		= argv[2];
	}
	else {
		/* Allocate element descriptors
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		_elt_y		= rpn_new_elt();
		_elt_x		= rpn_new_elt();

		/* Initialize first element (Y)
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		_elt_y->value.s	= argv[1];
		if (rpn_strip_quotes(&_elt_y->value.s) != RPN_RET_OK) {
			rpn_err_msg_invalid_string(_elt_y->value.s);
			exit(RPN_EXIT_INVALID_ELT);
		}
		rpn_set_type(_elt_y, RPN_TYPE_STRING);

		/* Push Y on the stack
		   ~~~~~~~~~~~~~~~~~~~ */
		rpn_push(_stack, _elt_y);

		/* Display stack
		   ~~~~~~~~~~~~~ */
		rpn_disp_stack(_stack);

		/* Initialize second element (X)
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		if (isdigit(argv[2][0])) {
			_elt_x->value.i	= atoi(argv[2]);
			rpn_set_type(_elt_x, RPN_TYPE_INT);
		}
		else {
			_elt_x->value.s	= argv[2];
			if (rpn_strip_quotes(&_elt_x->value.s) != RPN_RET_OK) {
				rpn_err_msg_invalid_string(_elt_x->value.s);
				exit(RPN_EXIT_INVALID_ELT);
			}
			rpn_set_type(_elt_x, RPN_TYPE_STRING);
		}

		/* Push X on the stack
		   ~~~~~~~~~~~~~~~~~~~ */
		rpn_push(_stack, _elt_x);

		/* Display stack
		   ~~~~~~~~~~~~~ */
		rpn_disp_stack(_stack);

		/* Initialize operator
		   ~~~~~~~~~~~~~~~~~~~ */
		_operator		= argv[3];
	}

	if ((_op = rpn_search_op(my_operators, _operator)) == 0) {
		rpn_err_msg_op_not_found(_operator);
		exit(RPN_EXIT_OP_UNKNOWN);
	}

	/* Execute operator
	   ~~~~~~~~~~~~~~~~ */
	if ((_retcode = (*_op->func)(_stack)) != RPN_RET_OK) {
		rpn_err_msg_op_error(_op->op_name, _retcode);
		exit(RPN_EXIT_OP_ERROR);
	}

	/* Display stack
	   ~~~~~~~~~~~~~ */
	rpn_disp_stack(_stack);

	return RPN_EXIT_OK;
}
