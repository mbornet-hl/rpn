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
 *   @(#)  [MB] cy_rpn_custom.c Version 1.11 du 17/11/13 - 
 */

#include  "rpn_header.h"
#include  "rpn_proto.h"

int                 my_op_plus(rpn_stack *);
int                 my_op_minus(rpn_stack *);
int                 my_op_mult(rpn_stack *);
int                 my_op_div(rpn_stack *);

int                 my_op_str_length(rpn_stack *);
int                 my_op_str_catenate(rpn_stack *);
int                 my_op_str_dupl(rpn_stack *);

int                 my_op_matrix(rpn_stack *);

int                  rpn_mat_get_i(struct rpn_matrix *, int, int);
struct rpn_elt      *rpn_mat_get_elt(struct rpn_matrix *, int, int);

/* Operators descriptors
   ~~~~~~~~~~~~~~~~~~~~~ */
struct rpn_operator      my_operators[]           = {
     {    RPN_OP_NAME_PLUS,                  2,             1,        my_op_plus          },
     {    RPN_OP_NAME_MINUS,                 2,             1,        my_op_minus         },
     {    RPN_OP_NAME_MULT,                  2,             1,        my_op_mult          },
     {    RPN_OP_NAME_DIV,                   2,             1,        my_op_div           },

     {    RPN_OP_NAME_STR_LENGTH,            1,             1,        my_op_str_length    },
     {    RPN_OP_NAME_STR_CATENATE,          2,             1,        my_op_str_catenate  },
     {    RPN_OP_NAME_STR_DUPL,              2,             1,        my_op_str_dupl      },

     {    RPN_OP_NAME_MATRIX,                2,             1,        my_op_matrix        },
     {    RPN_OP_EXPLODE,                    1,             0,        my_op_explode       },
	{	RPN_OP_VEC2,					2,			1,		my_op_vec2		},
     {    RPN_OP_VEC3,                       2,             1,        my_op_vec3          },
     {    RPN_OP_TRANSPOSE,                  1,             1,        my_op_transpose     },
     {    RPN_OP_CROSS_PRODUCT,              2,             1,        my_op_cross_prdct   },

     {    RPN_OP_RDN,                        0,             0,        rpn_op_roll_down    },
     {    RPN_OP_RUP,                        0,             0,        rpn_op_roll_up      },
     {    RPN_OP_LASTX,                      0,             1,        rpn_op_lastx        },
     {    RPN_OP_SWAP_XY,                    2,             2,        rpn_op_swap_xy      },
     {    RPN_OP_SWAP_XZ,                    2,             2,        rpn_op_swap_xz      },
     {    RPN_OP_SWAP_XT,                    2,             2,        rpn_op_swap_xt      },
     {    RPN_OP_SWAP_XL,                    2,             2,        rpn_op_swap_xl      },
     {    RPN_OP_ENTER,                      1,             1,        rpn_op_enter        },
	{	RPN_OP_CLX,					1,			1,		rpn_op_clx		},

//	{	RPN_OP_CATALOG,				0,			0,		rpn_op_catalog		},

     {    NULL,                              0,             0,        0                   }
};

/******************************************************************************

                         MY_OP_PLUS

******************************************************************************/
int my_op_plus(rpn_stack *stack)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _x, _y, _result, _lg;
     double                    _xf, _yf, _resultf;
     char                     *_s1, *_s2, *_s;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     if ((_stk_x->type == RPN_TYPE_INT)
     &&  (_stk_y->type == RPN_TYPE_INT)) {
          _x                       = _stk_x->value.i;
          _y                       = _stk_y->value.i;

          _result                  = _y + _x;

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_INT;
          _stk_result->value.i     = _result;

          rpn_push(stack, _stk_result);
     }
     else if ((_stk_x->type == RPN_TYPE_FLOAT)
          &&  (_stk_y->type == RPN_TYPE_FLOAT)) {
          _xf                      = _stk_x->value.f;
          _yf                      = _stk_y->value.f;

          _resultf                 = _yf + _xf;

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_FLOAT;
          _stk_result->value.f     = _resultf;

          rpn_push(stack, _stk_result);
     }
     else if ((_stk_x->type == RPN_TYPE_STRING)
          &&  (_stk_y->type == RPN_TYPE_STRING)) {
          _s1                      = _stk_y->value.s;
          _s2                      = _stk_x->value.s;

          _lg                      = strlen(_s1) + strlen(_s2);
          _s                       = malloc(_lg + 1);

          strcpy(_s, _s1);
          strcat(_s, _s2);

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_STRING;
          _stk_result->value.s     = _s;

          rpn_push(stack, _stk_result);
     }
     else if ((_stk_x->type == RPN_TYPE_LITTERAL)
          &&  (_stk_y->type == RPN_TYPE_LITTERAL)) {
          _s1                      = _stk_y->value.s;
          _s2                      = _stk_x->value.s;

          _lg                      = strlen(_s1) + strlen(_s2);
          _s                       = malloc(_lg + 8);

          sprintf(_s, "(%s) + (%s)", _s1, _s2);

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_LITTERAL;
          _stk_result->value.s     = _s;

          rpn_push(stack, _stk_result);
     }
     else if (rpn_get_type(_stk_x) == RPN_TYPE_MATRIX) {
          if (rpn_get_type(_stk_y) == RPN_TYPE_MATRIX) {
               struct rpn_matrix        *_mat_A, *_mat_B, *_mat_C;
               int                       _n, _m, _p, _i, _j, _idx, _sum, _size;
               struct rpn_elt           *_elt, *_zero;
               struct rpn_stack         *_stack;

               _mat_A                   = (struct rpn_matrix *) _stk_y->value.obj;
               _mat_B                   = (struct rpn_matrix *) _stk_x->value.obj;

               /* Check dimensions : A : n x p;   B : n x p
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if ((_mat_A->n != _mat_B->n)
			||  (_mat_A->p != _mat_B->p)) {
                    // Incompatible dimensions
                    rpn_push(stack, _stk_y);
                    rpn_push(stack, _stk_x);

                    _retcode                 = RPN_RET_INCOMPATIBLE_ELTS;
                    goto end;
               }

               _n                       = _mat_A->n;
               _p                       = _mat_A->p;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();
               _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
               _mat_C                   = (struct rpn_matrix *) malloc(_size);
               _mat_C->n                = _n;
               _mat_C->p                = _p;
               _stk_result->value.obj   = _mat_C;
               rpn_set_type(_stk_result, RPN_TYPE_MATRIX);

               _stack                   = rpn_new_stack(__func__);

               for (_i = 1; _i <= _n; _i++) {
                    for (_j = 1; _j <= _p; _j++) {
                         _zero     = rpn_new_elt();
                         rpn_set_type(_zero, RPN_TYPE_INT);
                         rpn_push(_stack, _zero);

					_elt      = rpn_mat_get_elt(_mat_A, _i, _j);
					rpn_push(_stack, _elt);
					_elt      = rpn_mat_get_elt(_mat_B, _i, _j);
					rpn_push(_stack, _elt);
					if ((_retcode = my_op_plus(_stack)) != 0) {
						rpn_err_msg_op_error("+", _retcode);
						exit(RPN_EXIT_OP_ERROR);
					}
                         _elt                = rpn_pop(_stack);
                         _idx                = ((_j - 1) * _n) + (_i - 1);
                         _mat_C->base[_idx]  = (void *) _elt;
                    }
               }

               if ((_mat_C->n == 1)
               &&  (_mat_C->p == 1)) {
                    _stk_result              = rpn_mat_get_elt(_mat_C, 1, 1);
               }

               rpn_push(stack, _stk_result);
          }
     }
     else {
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_TYPES;
     }

end:
     return _retcode;
}

/******************************************************************************

                         MY_OP_MINUS

******************************************************************************/
int my_op_minus(rpn_stack *stack)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _x, _y, _result, _lg;
     double                    _xf, _yf, _resultf;
     char                     *_s1, *_s2, *_s;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     if ((rpn_get_type(_stk_x) == RPN_TYPE_INT)
     &&  (rpn_get_type(_stk_y) == RPN_TYPE_INT)) {
          _x                       = _stk_x->value.i;
          _y                       = _stk_y->value.i;

          _result                  = _y - _x;

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_INT;
          _stk_result->value.i     = _result;

          rpn_push(stack, _stk_result);
     }
     else if ((_stk_x->type == RPN_TYPE_FLOAT)
          &&  (_stk_y->type == RPN_TYPE_FLOAT)) {
          _xf                      = _stk_x->value.f;
          _yf                      = _stk_y->value.f;

          _resultf                 = _yf - _xf;

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_FLOAT;
          _stk_result->value.f     = _resultf;

          rpn_push(stack, _stk_result);
     }
     else if ((_stk_x->type == RPN_TYPE_LITTERAL)
          &&  (_stk_y->type == RPN_TYPE_LITTERAL)) {
          _s1                      = _stk_y->value.s;
          _s2                      = _stk_x->value.s;

          _lg                      = strlen(_s1) + strlen(_s2);
          _s                       = malloc(_lg + 8);

          sprintf(_s, "(%s) - (%s)", _s1, _s2);

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_LITTERAL;
          _stk_result->value.s     = _s;

          rpn_push(stack, _stk_result);
     }

     else {
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_TYPES;
     }

     return _retcode;
}

/******************************************************************************

                         MY_OP_MULT

******************************************************************************/
int my_op_mult(rpn_stack *stack)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _x, _y, _result, _lg, _n, _total_size, _i;
     double                    _xf, _yf, _resultf;
     char                     *_s1, *_s2, *_s, *_result_s;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     if ((rpn_get_type(_stk_x) == RPN_TYPE_INT)
     &&  (rpn_get_type(_stk_y) == RPN_TYPE_INT)) {
          _x                       = _stk_x->value.i;
          _y                       = _stk_y->value.i;

          _result                  = _y * _x;

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_INT;
          _stk_result->value.i     = _result;

          rpn_push(stack, _stk_result);
     }
     else if ((_stk_x->type == RPN_TYPE_FLOAT)
          &&  (_stk_y->type == RPN_TYPE_FLOAT)) {
          _xf                      = _stk_x->value.f;
          _yf                      = _stk_y->value.f;

          _resultf                 = _yf * _xf;

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_FLOAT;
          _stk_result->value.f     = _resultf;

          rpn_push(stack, _stk_result);
     }
     else if ((_stk_x->type == RPN_TYPE_LITTERAL)
          &&  (_stk_y->type == RPN_TYPE_LITTERAL)) {
          _s1                      = _stk_y->value.s;
          _s2                      = _stk_x->value.s;

          _lg                      = strlen(_s1) + strlen(_s2);
          _s                       = malloc(_lg + 8);

          sprintf(_s, "(%s) * (%s)", _s1, _s2);

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_LITTERAL;
          _stk_result->value.s     = _s;

          rpn_push(stack, _stk_result);
     }
     else if (rpn_get_type(_stk_x) == RPN_TYPE_INT) {
          if (rpn_get_type(_stk_y) == RPN_TYPE_STRING) {
               _s                       = _stk_y->value.s;
               _n                       = _stk_x->value.i;

               _lg                      = strlen(_s);
               _total_size              = _lg * _n;

#define   MAX_SIZE       (10240)
               if (_total_size > MAX_SIZE) {
                    /* Result would not be reasonable !
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _retcode                 = RPN_RET_TOO_BIG;
               }
               else {
                    _result_s                = malloc(_total_size + 1);

                    strcpy(_result_s, _s);
                    for (_i = 0; _i < (_n - 1); _i++) {
                         strcat(_result_s, _s);
                    }

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt();

                    _stk_result->type        = RPN_TYPE_STRING;
                    _stk_result->value.s     = _result_s;

                    rpn_push(stack, _stk_result);
               }
          }
          else {
               _retcode                 = RPN_RET_INVALID_Y;
          }
     }
     else if (rpn_get_type(_stk_x) == RPN_TYPE_MATRIX) {
          if (rpn_get_type(_stk_y) == RPN_TYPE_MATRIX) {
               struct rpn_matrix        *_mat_A, *_mat_B, *_mat_C;
               int                       _n, _m, _p, _i, _j, _k, _idx, _sum, _size;
               struct rpn_elt           *_elt, *_zero;
               struct rpn_stack         *_stack;

               _mat_A                   = (struct rpn_matrix *) _stk_y->value.obj;
               _mat_B                   = (struct rpn_matrix *) _stk_x->value.obj;

               /* Check dimensions : A : n x m;   B : m x p
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if (_mat_A->p != _mat_B->n) {
                    // Incompatible dimensions
                    rpn_push(stack, _stk_y);
                    rpn_push(stack, _stk_x);

                    _retcode                 = RPN_RET_INCOMPATIBLE_ELTS;
                    goto end;
               }

               _n                       = _mat_A->n;
               _m                       = _mat_A->p;
               _p                       = _mat_B->p;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();
               _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
               _mat_C                   = (struct rpn_matrix *) malloc(_size);
               _mat_C->n                = _n;
               _mat_C->p                = _p;
               _stk_result->value.obj   = _mat_C;
               rpn_set_type(_stk_result, RPN_TYPE_MATRIX);

               _stack                   = rpn_new_stack(__func__);

               for (_i = 1; _i <= _n; _i++) {
                    for (_j = 1; _j <= _p; _j++) {
                         _zero     = rpn_new_elt();
                         rpn_set_type(_zero, RPN_TYPE_INT);
                         rpn_push(_stack, _zero);

                         for (_k = 1; _k <= _m; _k++) {
                              _elt      = rpn_mat_get_elt(_mat_A, _i, _k);
                              rpn_push(_stack, _elt);
                              _elt      = rpn_mat_get_elt(_mat_B, _k, _j);
                              rpn_push(_stack, _elt);
rpn_disp_stack(_stack);
                              if ((_retcode = my_op_mult(_stack)) != 0) {
							rpn_err_msg_op_error("*", _retcode);
							exit(RPN_EXIT_OP_ERROR);
						}
                              if ((_retcode = my_op_plus(_stack)) != 0) {
							rpn_err_msg_op_error("+", _retcode);
							exit(RPN_EXIT_OP_ERROR);
						}
rpn_disp_stack(_stack);
                         }
                         _elt                = rpn_pop(_stack);
                         _idx                = ((_j - 1) * _n) + (_i - 1);
                         _mat_C->base[_idx]  = (void *) _elt;
                    }
               }

               if ((_mat_C->n == 1)
               &&  (_mat_C->p == 1)) {
                    _stk_result              = rpn_mat_get_elt(_mat_C, 1, 1);
               }

               rpn_push(stack, _stk_result);
          }
     }
     else {
          _retcode                 = RPN_RET_INVALID_X;
     }

end:
     return _retcode;
}

/******************************************************************************

                         MY_OP_DIV

******************************************************************************/
int my_op_div(rpn_stack *stack)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _x, _y, _result, _lg;
     double                    _xf, _yf, _resultf;
     char                     *_s1, *_s2, *_s;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     if ((rpn_get_type(_stk_x) == RPN_TYPE_INT)
     &&  (rpn_get_type(_stk_y) == RPN_TYPE_INT)) {
          _x                       = _stk_x->value.i;
          _y                       = _stk_y->value.i;

          _result                  = _y / _x;

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_INT;
          _stk_result->value.i     = _result;

          rpn_push(stack, _stk_result);
     }
     else if ((_stk_x->type == RPN_TYPE_FLOAT)
          &&  (_stk_y->type == RPN_TYPE_FLOAT)) {
          _xf                      = _stk_x->value.f;
          _yf                      = _stk_y->value.f;

          _resultf                 = _yf / _xf;

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_FLOAT;
          _stk_result->value.f     = _resultf;

          rpn_push(stack, _stk_result);
     }
     else if ((_stk_x->type == RPN_TYPE_LITTERAL)
          &&  (_stk_y->type == RPN_TYPE_LITTERAL)) {
          _s1                      = _stk_y->value.s;
          _s2                      = _stk_x->value.s;

          _lg                      = strlen(_s1) + strlen(_s2);
          _s                       = malloc(_lg + 8);

          sprintf(_s, "(%s) / (%s)", _s1, _s2);

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_LITTERAL;
          _stk_result->value.s     = _s;

          rpn_push(stack, _stk_result);
     }

     rpn_push(stack, _stk_result);

     return RPN_RET_OK;
}

/******************************************************************************

                         MY_OP_STR_LENGTH

******************************************************************************/
int my_op_str_length(rpn_stack *stack)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     char                     *_s;
     int                       _result;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     _s                       = _stk_x->value.s;

     _result                  = strlen(_s);

     rpn_set_lastx(stack, _stk_x);

     _stk_result              = rpn_new_elt();

     _stk_result->type        = RPN_TYPE_INT;
     _stk_result->value.i     = _result;

     rpn_push(stack, _stk_result);

     return RPN_RET_OK;
}

/******************************************************************************

                         MY_OP_STR_CATENATE

******************************************************************************/
int my_op_str_catenate(rpn_stack *stack)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     char                     *_s1, *_s2;
     char                     *_result;
     int                       _lg, _retcode      = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     if (rpn_get_type(_stk_x) == RPN_TYPE_STRING) {
          if (rpn_get_type(_stk_y) == RPN_TYPE_STRING) {
               _s1                      = _stk_y->value.s;
               _s2                      = _stk_x->value.s;

               _lg                      = strlen(_s1) + strlen(_s2);
               _result                  = malloc(_lg + 1);

               strcpy(_result, _s1);
               strcat(_result, _s2);

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();

               _stk_result->type        = RPN_TYPE_STRING;
               _stk_result->value.s     = _result;

               rpn_push(stack, _stk_result);
          }
          else {
               _retcode                 = RPN_RET_INVALID_Y;
          }
     }
     else {
          _retcode                 = RPN_RET_INVALID_X;
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
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     char                     *_s;
     int                       _n, _total_size, _i;
     char                     *_result;
     int                       _lg, _retcode      = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     if (rpn_get_type(_stk_x) == RPN_TYPE_INT) {
          if (rpn_get_type(_stk_y) == RPN_TYPE_STRING) {
               _s                       = _stk_y->value.s;
               _n                       = _stk_x->value.i;

               _lg                      = strlen(_s);
               _total_size              = _lg * _n;

#define   MAX_SIZE       (10240)
               if (_total_size > MAX_SIZE) {
                    /* Result would not be reasonable !
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _retcode                 = RPN_RET_TOO_BIG;
               }
               else {
                    _result                  = malloc(_total_size + 1);

                    strcpy(_result, _s);
                    for (_i = 0; _i < (_n - 1); _i++) {
                         strcat(_result, _s);
                    }

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt();

                    _stk_result->type        = RPN_TYPE_STRING;
                    _stk_result->value.s     = _result;

                    rpn_push(stack, _stk_result);
               }
          }
          else {
               _retcode                 = RPN_RET_INVALID_Y;
          }
     }
     else {
          _retcode                 = RPN_RET_INVALID_X;
     }

     if (_retcode != RPN_RET_OK) {
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);
     }

     return _retcode;
}

/******************************************************************************

                         MY_OP_MATRIX

******************************************************************************/
int my_op_matrix(rpn_stack *stack)
{
     rpn_elt                  *_stk_n, *_stk_p, *_stk_matrix, *_stk_elt;
     int                       _n, _p, _size, _i, _j, _idx;
     struct rpn_matrix        *_matrix;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_p                   = rpn_pop(stack);

     if ((_stk_p->type != RPN_TYPE_INT)
     || ((_p  = _stk_p->value.i) <= 0)) {
          /* p is invalid
             ~~~~~~~~~~~~ */
          rpn_push(stack, _stk_p);

          _retcode                 = RPN_RET_INVALID_X;
          goto end;
     }

     _stk_n                   = rpn_pop(stack);
     if ((_stk_n->type != RPN_TYPE_INT)
     || ((_n = _stk_n->value.i) <= 0)) {
          /* n is invalid
             ~~~~~~~~~~~~ */
          rpn_push(stack, _stk_n);
          rpn_push(stack, _stk_p);

          _retcode                 = RPN_RET_INVALID_Y;
          goto end;
     }

     /* Check number of elements in the stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (stack->num_elts < (_n * _p)) {
          /* Too few elements in the stack
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_n);
          rpn_push(stack, _stk_p);

          _retcode                 = RPN_RET_NOT_ENOUGH_ELTS;
          goto end;
     }

     /* Create matrix element
        ~~~~~~~~~~~~~~~~~~~~~ */
     _stk_matrix              = rpn_new_elt();
     _stk_matrix->type        = RPN_TYPE_MATRIX;

     _size                    = sizeof(*_matrix) + (((_n * _p) - 1) * sizeof (void *));
     _matrix                  = (struct rpn_matrix *) malloc(_size);
     _matrix->n               = _n;
     _matrix->p               = _p;
     _stk_matrix->value.obj   = _matrix;

     for (_i = _n; _i > 0; _i--) {
          for (_j = _p; _j > 0; _j--) {
               _stk_elt            = rpn_pop(stack);
               _idx                = ((_j - 1) * _n) + (_i - 1);
               _matrix->base[_idx] = (void *) _stk_elt;
          }
     }

     rpn_push(stack, _stk_matrix);

end:
     return _retcode;
}

/******************************************************************************

                         MY_OP_EXPLODE

******************************************************************************/
int my_op_explode(rpn_stack *stack)
{
     rpn_elt                  *_stk_x, *_elt;
     int                       _n, _p, _size, _i, _j, _idx;
     struct rpn_matrix        *_matrix;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     if (_stk_x->type == RPN_TYPE_MATRIX) {
          _matrix                  = (struct rpn_matrix *) _stk_x->value.obj;
          _n                       = _matrix->n;
          _p                       = _matrix->p;

          for (_i = 1; _i <= _n; _i++) {
               for (_j = 1; _j <= _p; _j++) {
                     _idx          = ((_j - 1) * _n) + (_i - 1);
                     _elt          = (struct rpn_elt *) _matrix->base[_idx];
                    rpn_push(stack, _elt);
               }
          }
     }

     return _retcode;
}

/******************************************************************************

                         MY_OP_VEC2

******************************************************************************/
int my_op_vec2(rpn_stack *stack)
{
     rpn_elt                  *_stk_n, *_stk_p;
     int                       _n, _p;

     /* Vectors of dimension 2 are defined as matrices 2x1
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _stk_n                   = rpn_new_elt();
     rpn_set_type(_stk_n, RPN_TYPE_INT);
     _stk_n->value.i          = 2;      // 2 lines
     rpn_push(stack, _stk_n);

     _stk_p                   = rpn_new_elt();
     rpn_set_type(_stk_p, RPN_TYPE_INT);
     _stk_p->value.i          = 1;      // 1 column
     rpn_push(stack, _stk_p);

     return my_op_matrix(stack);
}

/******************************************************************************

                         MY_OP_VEC3

******************************************************************************/
int my_op_vec3(rpn_stack *stack)
{
     rpn_elt                  *_stk_n, *_stk_p;
     int                       _n, _p;

     /* Vectors of dimension 3 are defined as matrices 3x1
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _stk_n                   = rpn_new_elt();
     rpn_set_type(_stk_n, RPN_TYPE_INT);
     _stk_n->value.i          = 3;      // 3 lines
     rpn_push(stack, _stk_n);

     _stk_p                   = rpn_new_elt();
     rpn_set_type(_stk_p, RPN_TYPE_INT);
     _stk_p->value.i          = 1;      // 1 column
     rpn_push(stack, _stk_p);

     return my_op_matrix(stack);
}

/******************************************************************************

                         MY_OP_TRANSPOSE

******************************************************************************/
int my_op_transpose(rpn_stack *stack)
{
     rpn_elt                  *_stk_x;
     int                       _n, _p;
     int                       _retcode;
     struct rpn_matrix        *_matrix;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     if (rpn_get_type(_stk_x) != RPN_TYPE_MATRIX) {
          _retcode       = RPN_RET_INVALID_X;
          goto end;
     }

     _matrix        = (struct rpn_matrix *) _stk_x->value.obj;
     if ((_matrix->n != 1) && (_matrix->p != 1)) {
          _retcode       = RPN_RET_INVALID_X;
          goto end;
     }

     _n             = _matrix->n;
     _p             = _matrix->p;

     _matrix->n     = _p;
     _matrix->p     = _n;

     rpn_push(stack, _stk_x);

end:
     return _retcode;
}

/******************************************************************************

                         MY_OP_CROSS_PRDCT

******************************************************************************/
int my_op_cross_prdct(rpn_stack *stack)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _n, _p;
     int                       _retcode;
     struct rpn_matrix        *_mat_x, *_mat_y;
     struct rpn_elt           *_a1, *_a2, *_a3,
                              *_b1, *_b2, *_b3,
                              *_c1, *_c2, *_c3;
	struct rpn_stack		*_stack;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     if (rpn_get_type(_stk_x) != RPN_TYPE_MATRIX) {
          rpn_push(stack, _stk_x);
          _retcode       = RPN_RET_INVALID_X;
          goto end;
     }

     _mat_x         = (struct rpn_matrix *) _stk_x->value.obj;
     if ((_mat_x->n != 3) && (_mat_x->p != 1)) {
          rpn_push(stack, _stk_x);
          _retcode       = RPN_RET_INVALID_X;
          goto end;
     }

     _stk_y                   = rpn_pop(stack);
     if (rpn_get_type(_stk_y) != RPN_TYPE_MATRIX) {
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);
          _retcode       = RPN_RET_INVALID_Y;
          goto end;
     }

     _mat_y         = (struct rpn_matrix *) _stk_y->value.obj;
     if ((_mat_y->n != 3) && (_mat_y->p != 1)) {
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);
          _retcode       = RPN_RET_INVALID_Y;
          goto end;
     }

	_stack                   = rpn_new_stack(__func__);

     _a1  = rpn_mat_get_elt(_mat_y, 1, 1);
     _a2  = rpn_mat_get_elt(_mat_y, 2, 1);
     _a3  = rpn_mat_get_elt(_mat_y, 3, 1);

     _b1  = rpn_mat_get_elt(_mat_x, 1, 1);
     _b2  = rpn_mat_get_elt(_mat_x, 2, 1);
     _b3  = rpn_mat_get_elt(_mat_x, 3, 1);

	rpn_push(_stack, _a2);
	rpn_push(_stack, _b3);
	my_op_mult(_stack);
	rpn_push(_stack, _a3);
	rpn_push(_stack, _b2);
	my_op_mult(_stack);
     my_op_minus(_stack);

	rpn_push(_stack, _a3);
	rpn_push(_stack, _b1);
	my_op_mult(_stack);
	rpn_push(_stack, _a1);
	rpn_push(_stack, _b3);
	my_op_mult(_stack);
     my_op_minus(_stack);

	rpn_push(_stack, _a1);
	rpn_push(_stack, _b2);
	my_op_mult(_stack);
	rpn_push(_stack, _a2);
	rpn_push(_stack, _b1);
	my_op_mult(_stack);
     my_op_minus(_stack);

	my_op_vec3(_stack);

     rpn_set_lastx(stack, _stk_x);
	_stk_result			= rpn_pop(_stack);

     rpn_free_elt(_stk_x);
     rpn_free_elt(_stk_y);

     rpn_push(stack, _stk_result);

end:
     return _retcode;
}

/******************************************************************************

                              RPN_MAT_GET_I

******************************************************************************/
int rpn_mat_get_i(struct rpn_matrix *matrix, int i, int j)
{
     struct rpn_elt      *_elt;
     int                  _idx;

     _idx                = ((j - 1) * matrix->n) + (i - 1);
// fprintf(stderr, "MAT [%d x %d] : i = %d, j = %d\n", matrix->n, matrix->p, i, j);
     _elt                = (struct rpn_elt *) matrix->base[_idx];

     return _elt->value.i;
}

/******************************************************************************

                              RPN_MAT_GET_ELT

******************************************************************************/
struct rpn_elt *rpn_mat_get_elt(struct rpn_matrix *matrix, int i, int j)
{
     struct rpn_elt      *_elt;
     int                  _idx;

     _idx                = ((j - 1) * matrix->n) + (i - 1);
     _elt                = (struct rpn_elt *) matrix->base[_idx];

     return _elt;
}
