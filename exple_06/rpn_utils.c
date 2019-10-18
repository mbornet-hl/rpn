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
 *   rpn_utils.c
 *
 *   @(#)  [MB] cy_rpn_utils.c Version 1.9 du 17/11/13 - 
 */

#include  "rpn_header.h"
#include  "rpn_proto.h"

struct global_struct     G         = { 0 };

/******************************************************************************

                         RPN_NEW_STACK

******************************************************************************/
rpn_stack *rpn_new_stack(const char *stack_name)
{
     rpn_stack           *_stack;

     if ((_stack = (rpn_stack *) malloc(sizeof(*_stack))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

	_stack->name		= strdup(stack_name);
     _stack->num_elts    = 0;
     _stack->lastx       = rpn_new_elt();
	_stack->clear_x	= FALSE;
     rpn_set_type(_stack->lastx, RPN_TYPE_INT);
     _stack->base_elt    = 0;
     _stack->top_elt     = 0;

     return _stack;
}

/******************************************************************************

                         RPN_NEW_ELT

******************************************************************************/
rpn_elt *rpn_new_elt()
{
     rpn_elt             *_elt;

     if ((_elt = (rpn_elt *) malloc(sizeof(*_elt))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _elt->type          = RPN_TYPE_UNDEFINED;
     _elt->value.i       = 0;
     _elt->previous_elt  = NULL;
     _elt->next_elt      = NULL;

     return _elt;
}

/******************************************************************************

					RPN_FREE_ELT

******************************************************************************/
void rpn_free_elt(rpn_elt *elt)
{
	int				 _type;

	switch ((_type = rpn_get_type(elt))) {

	case	RPN_TYPE_INT:
	case	RPN_TYPE_FLOAT:
	case	RPN_TYPE_STRING:
		free(elt);
		break;

	case	RPN_TYPE_MATRIX:
		fprintf(stderr, "%s : (%s) element type %d : not managed yet !\n",
		        G.progname, __func__, _type);
	}
}

/******************************************************************************

                         RPN_CLONE_ELT

******************************************************************************/
rpn_elt *rpn_clone_elt(rpn_elt *elt)
{
     rpn_elt             *_clone;
	int				 _idx;

	_clone			= rpn_new_elt();
     _clone->type        = elt->type;
     _clone->value.f     = elt->value.f;

	switch (elt->type) {

	case	RPN_TYPE_MATRIX:
		{
			struct rpn_elt			*_sub_elt, *_clone_sub;
			struct rpn_matrix		*_mat, *_clone_mat;
			int					 _size;

			_mat				= (struct rpn_matrix *) elt->value.obj;
			_size			= sizeof(struct rpn_matrix)
							+ (((_mat->n *_mat->p) - 1) * sizeof(struct rpn_elt *));
			_clone->value.obj	= malloc(_size);
			_clone_mat		= _clone->value.obj;
			_clone_mat->n		= _mat->n;
			_clone_mat->p		= _mat->p;

			for (_idx = 0; _idx < (_mat->n * _mat->p); _idx++) {
				_sub_elt				= (struct rpn_elt *) _mat->base[_idx];
				_clone_sub			= rpn_clone_elt(_sub_elt);
				_clone_mat->base[_idx]	= _clone_sub;
			}
		}
		break;

	case	RPN_TYPE_INT:
	case	RPN_TYPE_FLOAT:
	case	RPN_TYPE_STRING:
	case	RPN_TYPE_LITTERAL:
		break;

	default:
		fprintf(stderr, "%s : unknown type (%d)\n", __func__, elt->type);
		break;
	}

     return _clone;
}

/******************************************************************************

                         RPN_PUSH

******************************************************************************/
int rpn_push(rpn_stack *stack, rpn_elt *elt)
{
     int             _retcode;
	rpn_elt		*_stk_x;

     if (stack != 0) {
		if (stack->clear_x) {
			stack->clear_x		= FALSE;
			_stk_x		= rpn_pop(stack);
			rpn_free_elt(_stk_x);
		}

          if (stack->base_elt == 0) {
               stack->base_elt          = elt;
               stack->top_elt           = elt;

               elt->previous_elt        = 0;
               elt->next_elt            = 0;
          }
          else {
               stack->top_elt->next_elt = elt;

               elt->previous_elt        = stack->top_elt;
               elt->next_elt            = 0;

               stack->top_elt           = elt;
          }
          stack->num_elts++;
          _retcode                 = RPN_RET_OK;
     }
     else {
          _retcode                 = RPN_RET_INVALID_STACK;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_POP

******************************************************************************/
rpn_elt *rpn_pop(rpn_stack *stack)
{
     rpn_elt        *_elt     = 0;

     if ((stack != 0) && (stack->num_elts != 0)) {
          _elt                     = stack->top_elt;
          stack->top_elt           = _elt->previous_elt;
          if (stack->top_elt != 0) {
               stack->top_elt->next_elt = 0;
          }
          else {
               stack->base_elt     = 0;
          }

          _elt->previous_elt       = 0;
          _elt->next_elt           = 0;

          stack->num_elts--;
     }
     else {
          _elt                     = rpn_new_elt();
          rpn_set_type(_elt, RPN_TYPE_INT);
     }

     return _elt;
}

/******************************************************************************

                         RPN_BASE_PUSH

******************************************************************************/
int rpn_base_push(rpn_stack *stack, rpn_elt *elt)
{
     int             _retcode;

     if (stack != 0) {
          if (stack->base_elt == 0) {
               stack->base_elt               = elt;
               stack->top_elt                = elt;

               elt->previous_elt             = 0;
               elt->next_elt                 = 0;
          }
          else {
               stack->base_elt->previous_elt = elt;

               elt->previous_elt             = 0;
               elt->next_elt                 = stack->base_elt;

               stack->base_elt                = elt;
          }
          stack->num_elts++;
          _retcode                 = RPN_RET_OK;
     }
     else {
          _retcode                 = RPN_RET_INVALID_STACK;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_BASE_POP

******************************************************************************/
rpn_elt *rpn_base_pop(rpn_stack *stack)
{
     rpn_elt        *_elt     = 0;

     if ((stack != 0) && (stack->num_elts != 0)) {
          _elt                     = stack->base_elt;
          stack->base_elt          = _elt->next_elt;
          if (stack->base_elt != 0) {
               stack->base_elt->previous_elt = 0;
          }
          else {
               stack->top_elt      = 0;
          }

          _elt->previous_elt       = 0;
          _elt->next_elt           = 0;

          stack->num_elts--;
     }
     else {
          _elt                     = rpn_new_elt();
          rpn_set_type(_elt, RPN_TYPE_INT);
     }

     return _elt;
}

/******************************************************************************

                         RPN_ROLL_DOWN

******************************************************************************/
int rpn_op_roll_down(rpn_stack *stack)
{
     int                       _retcode;
     struct rpn_elt           *_elt;

     _elt                     = rpn_pop(stack);
     rpn_base_push(stack, _elt);
     _retcode                 = RPN_RET_OK;

     return _retcode;
}

/******************************************************************************

                         RPN_ROLL_UP

******************************************************************************/
int rpn_op_roll_up(rpn_stack *stack)
{
     int                       _retcode;
     struct rpn_elt           *_elt;

     _elt                     = rpn_base_pop(stack);
     rpn_push(stack, _elt);

     _retcode                 = RPN_RET_OK;

     return _retcode;
}

/******************************************************************************

                         RPN_OP_SWAP_XY

******************************************************************************/
int rpn_op_swap_xy(rpn_stack *stack)
{
     struct rpn_elt           *_stk_x, *_stk_y;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 2) {
          _stk_x	    = rpn_pop(stack);
          _stk_y	    = rpn_pop(stack);

          rpn_push(stack, _stk_x);
          rpn_push(stack, _stk_y);
     }

     return _retcode;
}

/******************************************************************************

                         RPN_OP_SWAP_XZ

******************************************************************************/
int rpn_op_swap_xz(rpn_stack *stack)
{
     struct rpn_elt           *_stk_x, *_stk_y,
                              *_stk_z;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 3) {
          _stk_x    = rpn_pop(stack);
          _stk_y    = rpn_pop(stack);
          _stk_z    = rpn_pop(stack);

          rpn_push(stack, _stk_x);
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_z);
     }

     return _retcode;
}

/******************************************************************************

                         RPN_OP_SWAP_XT

******************************************************************************/
int rpn_op_swap_xt(rpn_stack *stack)
{
     struct rpn_elt           *_stk_x, *_stk_y,
                              *_stk_z, *_stk_t;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 4) {
          _stk_x    = rpn_pop(stack);
          _stk_y    = rpn_pop(stack);
          _stk_z    = rpn_pop(stack);
          _stk_t    = rpn_pop(stack);

          rpn_push(stack, _stk_x);
          rpn_push(stack, _stk_z);
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_t);
     }

     return _retcode;
}

/******************************************************************************

                         RPN_OP_SWAP_XL

******************************************************************************/
int rpn_op_swap_xl(rpn_stack *stack)
{
     struct rpn_elt           *_stk_x;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 1) {
          _stk_x    = rpn_pop(stack);

          rpn_push(stack, stack->lastx);
          stack->lastx   = _stk_x;
     }

     return _retcode;
}

/******************************************************************************

					RPN_OP

******************************************************************************/
int rpn_op(rpn_stack *stack, int (*func)(rpn_stack *))
{
	stack->clear_x		= FALSE;

	return (*func)(stack);
}

/******************************************************************************

                         RPN_OP_ENTER

******************************************************************************/
int rpn_op_enter(rpn_stack *stack)
{
     struct rpn_elt           *_stk_x, *_elt;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 1) {
          _stk_x    = rpn_pop(stack);
		_elt		= rpn_clone_elt(_stk_x);
          rpn_push(stack, _stk_x);
          rpn_push(stack, _elt);
     }

     return _retcode;
}

/******************************************************************************

					RPN_OP_CLX

******************************************************************************/
int rpn_op_clx(rpn_stack *stack)
{
	struct rpn_elt			*_stk_x, *_zero;

	_stk_x		= rpn_pop(stack);
	rpn_free_elt(_stk_x);

	_zero	     = rpn_new_elt();
	rpn_set_type(_zero, RPN_TYPE_INT);
	rpn_push(stack, _zero);

	stack->clear_x	= TRUE;

	return RPN_RET_OK;
}

/******************************************************************************

					RPN_SET_LASTX

******************************************************************************/
void rpn_set_lastx(rpn_stack *stack, rpn_elt *elt)
{
	if (stack->lastx) {
		rpn_free_elt(stack->lastx);
	}

	stack->lastx		= rpn_clone_elt(elt);
}

/******************************************************************************

                         RPN_OP_LASTX

******************************************************************************/
int rpn_op_lastx(rpn_stack *stack)
{
     struct rpn_elt           *_stk_x;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 1) {
          rpn_push(stack, rpn_clone_elt(stack->lastx));
     }

     return _retcode;
}

/******************************************************************************

                         RPN_GET_TYPE

******************************************************************************/
int rpn_get_type(rpn_elt *elt)
{
     int        _retcode;

     if (elt) {
          _retcode  = elt->type;
     }
     else {
          _retcode  = RPN_RET_INVALID_ELT;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_SET_TYPE

******************************************************************************/
int rpn_set_type(rpn_elt *elt, int type)
{
     int             _retcode;

     if (elt) {
          elt->type      = type;
          _retcode       = RPN_RET_OK;
     }
     else {
          _retcode       = RPN_RET_INVALID_ELT;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_SEARCH_OP

******************************************************************************/
rpn_operator *rpn_search_op(rpn_operator *operators, char *operator)
{
     rpn_operator        *_op;

     for (_op = operators; _op->op_name != 0; _op++) {
          if (!strcmp(_op->op_name, operator)) {
               /* Operator found
                  ~~~~~~~~~~~~~~ */
               break;
          }
     }

     if (_op->op_name == 0) {
          _op       = 0;
     }

     return _op;
}

/******************************************************************************

                         RPN_CATALOG

******************************************************************************/
int rpn_catalog(struct rpn_operator *ops)
{
     struct rpn_operator      *op;

     for (op = ops; op->op_name !=  NULL; op++) {
          printf("%-15s %2d %2d\n", op->op_name, op->nb_args_in, op->nb_args_out);
     }
}

/******************************************************************************

                         RPN_ERR_MSG_NO_MEM

******************************************************************************/
void rpn_err_msg_no_mem()
{
     fprintf(stderr, "%s: not enough memory !\n", G.progname);
}

/******************************************************************************

                         RPN_ERR_MSG_OP_NOT_FOUND

******************************************************************************/
void rpn_err_msg_op_not_found(char *operator)
{
     fprintf(stderr, "%s: operator \"%s\" not found !\n", G.progname, operator);
}

/******************************************************************************

                         RPN_ERR_MSG_OP_ERROR

******************************************************************************/
void rpn_err_msg_op_error(char *operator, int retcode)
{
     fprintf(stderr, "%s: operator \"%s\" returned %d !\n",
             G.progname, operator, retcode);
}


/******************************************************************************

                         RPN_ERR_MSG_INVALID_STRING

******************************************************************************/
void rpn_err_msg_invalid_string(char *str)
{
     fprintf(stderr, "%s: invalid string (%s)\n", G.progname, str);
}

/******************************************************************************

                         RPN_STRIP_QUOTES

******************************************************************************/
int rpn_strip_quotes(char **str)
{
     int                 _lg, _retcode;
     char                *_new_str;

     _lg            = strlen(*str);
     if ((*str)[0] != '"' || (*str)[_lg - 1] != '"') {
          _retcode       = RPN_RET_INVALID_STR;
     }
     else {
          _new_str            = strdup(*str);
          _new_str[_lg - 1]   = 0;
          _new_str++;

          *str                = _new_str;

          _retcode            = RPN_RET_OK;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_STRIP_SINGLEQUOTES

******************************************************************************/
int rpn_strip_single_quotes(char **str)
{
     int                 _lg, _retcode;
     char                *_new_str;

     _lg            = strlen(*str);
     if ((*str)[0] != '\'' || (*str)[_lg - 1] != '\'') {
          _retcode       = RPN_RET_INVALID_STR;
     }
     else {
          _new_str            = strdup(*str);
          _new_str[_lg - 1]   = 0;
          _new_str++;

          *str                = _new_str;

          _retcode            = RPN_RET_OK;
     }

     return _retcode;
}
