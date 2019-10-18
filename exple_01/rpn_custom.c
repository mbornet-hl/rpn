/*
 *	:vim:se ts=5 sw=5:
 *
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
 *	@(#)	[MB] cy_rpn_custom.c	Version 1.2.1.1 du 17/11/03 - 
 */

#include	<stdio.h>
#include	"rpn_header.h"

int				my_op_plus(rpn_stack *);
int				my_op_minus(rpn_stack *);
int				my_op_mult(rpn_stack *);
int				my_op_div(rpn_stack *);

/* Operators descriptors
   ~~~~~~~~~~~~~~~~~~~~~ */
struct rpn_operator		my_operators[]			= {
	{	RPN_OP_NAME_PLUS,				2,			1,		my_op_plus		},
	{	RPN_OP_NAME_MINUS,				2,			1,		my_op_minus		},
	{	RPN_OP_NAME_MULT,				2,			1,		my_op_mult		},
	{	RPN_OP_NAME_DIV,				2,			1,		my_op_div			},
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
