/*
 *	:vim:se ts=5 sw=5:
 *
 *	@(#)	[MB] cy_rpn_custom.c	Version 1.3 du 17/09/29 - 
 */

#include	"rpn_header.h"

int				my_op_plus(rpn_stack *);
int				my_op_minus(rpn_stack *);
int				my_op_mult(rpn_stack *);
int				my_op_div(rpn_stack *);

int				my_op_str_length(rpn_stack *);
int				my_op_str_catenate(rpn_stack *);
int				my_op_str_dupl(rpn_stack *);

/* Operators descriptors
   ~~~~~~~~~~~~~~~~~~~~~ */
struct rpn_operator		my_operators[]			= {
	{	RPN_OP_NAME_PLUS,				2,			1,		my_op_plus		},
	{	RPN_OP_NAME_MINUS,				2,			1,		my_op_minus		},
	{	RPN_OP_NAME_MULT,				2,			1,		my_op_mult		},
	{	RPN_OP_NAME_DIV,				2,			1,		my_op_div			},

	{	RPN_OP_NAME_STR_LENGTH,			1,			1,		my_op_str_length	},
	{	RPN_OP_NAME_STR_CATENATE,		2,			1,		my_op_str_catenate	},
	{	RPN_OP_NAME_STR_DUPL,			2,			1,		my_op_str_dupl		},

	{	NULL,						0,			0,		0				}
};

/******************************************************************************

					MY_OP_PLUS

******************************************************************************/
int my_op_plus(rpn_stack *stack)
{
	rpn_elt				*_stk_x, *_stk_y, *_stk_result;
	int					 _x, _y, _result;

	_stk_x				= rpn_pop(stack);
	_stk_y				= rpn_pop(stack);

	_x					= _stk_x->value.i;
	_y					= _stk_y->value.i;

	_result				= _y + _x;

	_stk_result			= rpn_new_elt();

	_stk_result->type		= RPN_TYPE_INT;
	_stk_result->value.i	= _result;

	rpn_push(stack, _stk_result);

	return RPN_RET_OK;
}

/******************************************************************************

					MY_OP_MINUS

******************************************************************************/
int my_op_minus(rpn_stack *stack)
{
	rpn_elt				*_stk_x, *_stk_y, *_stk_result;
	int					 _x, _y, _result;

	_stk_x				= rpn_pop(stack);
	_stk_y				= rpn_pop(stack);

	_x					= _stk_x->value.i;
	_y					= _stk_y->value.i;

	_result				= _y - _x;

	_stk_result			= rpn_new_elt();

	_stk_result->type		= RPN_TYPE_INT;
	_stk_result->value.i	= _result;

	rpn_push(stack, _stk_result);

	return RPN_RET_OK;
}

/******************************************************************************

					MY_OP_MULT

******************************************************************************/
int my_op_mult(rpn_stack *stack)
{
	rpn_elt				*_stk_x, *_stk_y, *_stk_result;
	int					 _x, _y, _result;

	_stk_x				= rpn_pop(stack);
	_stk_y				= rpn_pop(stack);

	_x					= _stk_x->value.i;
	_y					= _stk_y->value.i;

	_result				= _y * _x;

	_stk_result			= rpn_new_elt();

	_stk_result->type		= RPN_TYPE_INT;
	_stk_result->value.i	= _result;

	rpn_push(stack, _stk_result);

	return RPN_RET_OK;
}

/******************************************************************************

					MY_OP_DIV

******************************************************************************/
int my_op_div(rpn_stack *stack)
{
	rpn_elt				*_stk_x, *_stk_y, *_stk_result;
	int					 _x, _y, _result;

	_stk_x				= rpn_pop(stack);
	_stk_y				= rpn_pop(stack);

	_x					= _stk_x->value.i;
	_y					= _stk_y->value.i;

	_result				= _y / _x;

	_stk_result			= rpn_new_elt();

	_stk_result->type		= RPN_TYPE_INT;
	_stk_result->value.i	= _result;

	rpn_push(stack, _stk_result);

	return RPN_RET_OK;
}

/******************************************************************************

					MY_OP_STR_LENGTH

******************************************************************************/
int my_op_str_length(rpn_stack *stack)
{
	rpn_elt				*_stk_x, *_stk_y, *_stk_result;
	char					*_s;
	int					 _result;

	_stk_x				= rpn_pop(stack);
	_stk_y				= rpn_pop(stack);

	_s					= _stk_x->value.s;

	_result				= strlen(_s);

	_stk_result			= rpn_new_elt();

	_stk_result->type		= RPN_TYPE_INT;
	_stk_result->value.i	= _result;

	rpn_push(stack, _stk_result);

	return RPN_RET_OK;
}

/******************************************************************************

					MY_OP_STR_CATENATE

******************************************************************************/
int my_op_str_catenate(rpn_stack *stack)
{
	rpn_elt				*_stk_x, *_stk_y, *_stk_result;
	char					*_s1, *_s2;
	char					*_result;
	int					 _lg, _retcode		= RPN_RET_OK;

	_stk_x				= rpn_pop(stack);
	_stk_y				= rpn_pop(stack);

	if (rpn_get_type(_stk_x) == RPN_TYPE_STRING) {
		if (rpn_get_type(_stk_y) == RPN_TYPE_STRING) {
			_s1					= _stk_y->value.s;
			_s2					= _stk_x->value.s;

			_lg					= strlen(_s1) + strlen(_s2);
			_result				= malloc(_lg + 1);

			strcpy(_result, _s1);
			strcat(_result, _s2);

			_stk_result			= rpn_new_elt();

			_stk_result->type		= RPN_TYPE_STRING;
			_stk_result->value.s	= _result;

			rpn_push(stack, _stk_result);
		}
		else {
			_retcode				= RPN_RET_INVALID_Y;
		}
	}
	else {
		_retcode				= RPN_RET_INVALID_X;
	}

	if (_retcode != RPN_RET_OK) {
		rpn_push(stack, _stk_y);
		rpn_push(stack, _stk_x);
	}

	return _retcode;
}


/******************************************************************************

					MY_OP_STR_DUPL

******************************************************************************/
int my_op_str_dupl(rpn_stack *stack)
{
	rpn_elt				*_stk_x, *_stk_y, *_stk_result;
	char					*_s;
	int					 _n, _total_size, _i;
	char					*_result;
	int					 _lg, _retcode		= RPN_RET_OK;

	_stk_x				= rpn_pop(stack);
	_stk_y				= rpn_pop(stack);

	if (rpn_get_type(_stk_x) == RPN_TYPE_INT) {
		if (rpn_get_type(_stk_y) == RPN_TYPE_STRING) {
			_s					= _stk_y->value.s;
			_n					= _stk_x->value.i;

			_lg					= strlen(_s);
			_total_size			= _lg * _n;

#define	MAX_SIZE		(10240)
			if (_total_size > MAX_SIZE) {
				/* Result would not be reasonable !
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				_retcode				= RPN_RET_TOO_BIG;
			}
			else {
				_result				= malloc(_total_size + 1);

				strcpy(_result, _s);
				for (_i = 0; _i < (_n - 1); _i++) {
					strcat(_result, _s);
				}

				_stk_result			= rpn_new_elt();

				_stk_result->type		= RPN_TYPE_STRING;
				_stk_result->value.s	= _result;

				rpn_push(stack, _stk_result);
			}
		}
		else {
			_retcode				= RPN_RET_INVALID_Y;
		}
	}
	else {
		_retcode				= RPN_RET_INVALID_X;
	}

	if (_retcode != RPN_RET_OK) {
		rpn_push(stack, _stk_y);
		rpn_push(stack, _stk_x);
	}

	return _retcode;
}
