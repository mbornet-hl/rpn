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
 *   @(#)  [MB] dn_mod_stats.c Version 1.5 du 19/10/12 - 
 */

#include  <math.h>
#include  "../cc/cc_types.h"
#include  "../cy/cy_rpn_header.h"
#include  "../cy/cy_rpn_proto.h"
#include	"../cy/cy_rpn_epub.h"
#include  "../dl/dl_cpub.h"
#include	"../dl/dl_epub.h"
#include  "dn_serial.h"
#include  "dn_cpub.h"
#include	"dn_epub.h"
#include	"dn_epri.h"

/* Operators from external modules, needed by this module
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
struct dn_operators {
	dl_operator					*roll_down,
								*swap_xy,
								*lastx;
};
static struct dn_operators			 dn_ops = { 0 };

static dl_op_desc					 dn_ops_array[];

/* Module descriptor {{{ */
struct dl_module         stats_module = {
     "stats",
     "2.0",
     DN_LINK_ID,
     0,
	dn_ops_array,
	dn_init
};

/* Module descriptor }}} */
/* Operator parameters descriptions {{{ */
static dl_op_params					 dn_params_sum_add[] = {
	DL_OP_DEF2(dn_op_stats_sum_add, 1, INT, INT),
	DL_OP_DEF2(dn_op_stats_sum_add, 1, INT, DOUBLE),
	DL_OP_DEF2(dn_op_stats_sum_add, 1, DOUBLE, INT),
	DL_OP_DEF2(dn_op_stats_sum_add, 1, DOUBLE, DOUBLE),
	DL_OP_DEF_END
};

static dl_op_params					 dn_params_sum_sub[] = {
	DL_OP_DEF2(dn_op_stats_sum_sub, 1, INT, INT),
	DL_OP_DEF2(dn_op_stats_sum_sub, 1, INT, DOUBLE),
	DL_OP_DEF2(dn_op_stats_sum_sub, 1, DOUBLE, INT),
	DL_OP_DEF2(dn_op_stats_sum_sub, 1, DOUBLE, DOUBLE),
	DL_OP_DEF_END
};

static dl_op_params					 dn_params_mean[] = {
	DL_OP_DEF0(dn_op_stats_mean, 1),
	DL_OP_DEF_END
};

static dl_op_params					 dn_params_sdev[] = {
	DL_OP_DEF0(dn_op_stats_sdev, 1),
	DL_OP_DEF_END
};

static dl_op_params					 dn_params_meanx[] = {
	DL_OP_DEF1(dn_op_stats_meanx, 1, INT),
	DL_OP_DEF1(dn_op_stats_meanx, 1, DOUBLE),
	DL_OP_DEF_END
};

static dl_op_params					 dn_params_mmm[] = {
	DL_OP_DEF0(dn_op_stats_mmm, 1),
	DL_OP_DEF_END
};

static dl_op_params					 dn_params_line_data_set[] = {
	DL_OP_DEF4(dn_op_stats_line_data_set, 1, INT, DOUBLE, MIN_MAX, COEF_A_B),
	DL_OP_DEF_END
};

static dl_op_params					 dn_params_exp_data_set[] = {
	DL_OP_DEF4(dn_op_stats_exp_data_set, 1, INT, DOUBLE, MIN_MAX, COEF_A_B),
	DL_OP_DEF_END
};

static dl_op_params					 dn_params_log_data_set[] = {
	DL_OP_DEF4(dn_op_stats_log_data_set, 1, INT, DOUBLE, MIN_MAX, COEF_A_B),
	DL_OP_DEF_END
};

static dl_op_params					 dn_params_pow_data_set[] = {
	DL_OP_DEF4(dn_op_stats_pow_data_set, 1, INT, DOUBLE, MIN_MAX, COEF_A_B),
	DL_OP_DEF_END
};

/* Operator parameters descriptions }}} */
/* Operators list {{{ */
static dl_op_desc					 dn_ops_array[] = {
	{	"sigma+",				dn_params_sum_add					},
	{	"sigma-",				dn_params_sum_sub					},
	{	"mean",				dn_params_mean						},
	{	"sdev",				dn_params_sdev						},
	{	"meanx",				dn_params_meanx					},
	{	"mmm",				dn_params_mmm						},
	{	"line_data_set",		dn_params_line_data_set				},
	{	"exp_data_set",		dn_params_exp_data_set				},
	{	"log_data_set",		dn_params_log_data_set				},
	{	"pow_data_set",		dn_params_pow_data_set				},
	{	0,					0								}
};

/* Operators list }}} */
/* dn_op_stats_sum_add() {{{ */

/******************************************************************************

                              DN_OP_SUM_ADD

******************************************************************************/
RPN_DEF_OP(dn_op_stats_sum_add)
{
     rpn_elt                  *_stk_x, *_stk_y,
                              *_sum_x, *_sum_x2,
                              *_sum_y, *_sum_y2,
                              *_sum_xy, *_N, *_constant;
     rpn_stack                *_stack;
     int                       _retcode;
	dl_operator			*_op;

//X
//rpn_disp_sigma(G.sigma);
     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     _stack                   = rpn_new_stack("sigma+ sum(x)");

//X
//rpn_disp_sigma(G.sigma);
     /* Sum(x)
        ~~~~~~ */
     _sum_x                                  = G.sigma->reg[RPN_SIGMA_IDX_SUM_X];
     G.sigma->reg[RPN_SIGMA_IDX_SUM_X]       = 0;
//printf("_sum_x = %p\n", _sum_x);
     rpn_push(_stack, _sum_x);
     rpn_push(_stack, _stk_x);

//X
//rpn_disp_stk(_stack);
//X
//rpn_disp_sigma(G.sigma);
     if ((_op = rpn_search_op(stack, RPN_OP_NAME_ADD)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }
//X
//rpn_disp_sigma(G.sigma);
//X
//rpn_disp_stk(_stack);

     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_SUM_X]);

//X
//rpn_disp_sigma(G.sigma);
     /* Sum(x^2)
        ~~~~~~ */
     (*dn_ops.lastx->func)(_stack, dn_ops.lastx);
     _sum_x2                                 = G.sigma->reg[RPN_SIGMA_IDX_SUM_X2];
     G.sigma->reg[RPN_SIGMA_IDX_SUM_X2]      = 0;
     rpn_push(_stack, _sum_x2);
     (*dn_ops.lastx->func)(_stack, dn_ops.lastx);
     (*dn_ops.lastx->func)(_stack, dn_ops.lastx);

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_MATH_MUL)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_SUM_X2]);
     (*dn_ops.swap_xy->func)(_stack, dn_ops.swap_xy);

//X
//rpn_disp_sigma(G.sigma);
     /* Sum(y)
        ~~~~~~ */
     _sum_y                                  = G.sigma->reg[RPN_SIGMA_IDX_SUM_Y];
     G.sigma->reg[RPN_SIGMA_IDX_SUM_Y]       = 0;
     rpn_push(_stack, _sum_y);
     rpn_push(_stack, _stk_y);

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_ADD)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_SUM_Y]);

//X
//rpn_disp_sigma(G.sigma);
     /* Sum(y^2)
        ~~~~~~ */
     (*dn_ops.swap_xy->func)(_stack, dn_ops.swap_xy);
     (*dn_ops.lastx->func)(_stack, dn_ops.lastx);
     _sum_y2                                 = G.sigma->reg[RPN_SIGMA_IDX_SUM_Y2];
     G.sigma->reg[RPN_SIGMA_IDX_SUM_Y2]      = 0;
     rpn_push(_stack, _sum_y2);
     (*dn_ops.lastx->func)(_stack, dn_ops.lastx);
     (*dn_ops.lastx->func)(_stack, dn_ops.lastx);

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_MATH_MUL)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_ADD)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_SUM_Y2]);
     (*dn_ops.roll_down->func)(_stack, dn_ops.roll_down);

//X
//rpn_disp_sigma(G.sigma);
     /* Sum(x*y)
        ~~~~~~~~ */
     if ((_op = rpn_search_op(stack, RPN_OP_NAME_MATH_MUL)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

//X
//rpn_disp_sigma(G.sigma);
     _sum_xy                                 = G.sigma->reg[RPN_SIGMA_IDX_SUM_XY];
     G.sigma->reg[RPN_SIGMA_IDX_SUM_XY]      = 0;
     rpn_push(_stack, _sum_xy);

//X
//rpn_disp_sigma(G.sigma);
     if ((_op = rpn_search_op(stack, RPN_OP_NAME_ADD)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }
//X
//rpn_disp_sigma(G.sigma);

//rpn_disp_stk(_stack);
//X
//rpn_disp_sigma(G.sigma);
     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_SUM_XY]);
//X
//rpn_disp_sigma(G.sigma);

     /* N
        ~ */
     _N                                      = G.sigma->reg[RPN_SIGMA_IDX_N];
     G.sigma->reg[RPN_SIGMA_IDX_N]           = 0;
     rpn_push(_stack, _N);
     _constant                = rpn_new_int(1);
     rpn_push(_stack, _constant);

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_ADD)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }
     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_N]);

//X
//rpn_disp_sigma(G.sigma);

     rpn_free_stack(_stack);

     _retcode                 = RPN_RET_OK;

end:
     return _retcode;
}
/* dn_op_stats_sum_add() }}} */
/* dn_op_stats_sum_sub() {{{ */

/******************************************************************************

                              DN_OP_SUM_SUB

******************************************************************************/
RPN_DEF_OP(dn_op_stats_sum_sub)
{
     rpn_elt                  *_stk_x;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     rpn_push(stack, _stk_x);

     _retcode                 = RPN_RET_INVALID_TYPES;

     return _retcode;
}
/* dn_op_stats_sum_sub() }}} */
/* dn_op_stats_mean() {{{ */

/******************************************************************************

                              DN_OP_MEAN

******************************************************************************/
RPN_DEF_OP(dn_op_stats_mean)
{
     rpn_elt                  *_stk_result,
                              *_sum_x, *_sum_y, *_N;
     rpn_stack                *_stack;
     int                       _retcode;
     dl_operator		     *_op;

     _retcode                 = RPN_RET_OK;

rpn_disp_sigma(G.sigma);
     _stack                   = rpn_new_stack("mean");

     /* Y mean
        ~~~~~~ */
     _sum_y                   = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_Y]);
     rpn_push(_stack, _sum_y);

     _N                       = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_N]);
     rpn_push(_stack, _N);

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_DIV)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _stk_result              = rpn_pop(_stack);
     rpn_push(stack, _stk_result);

     /* X mean
        ~~~~~ */
     _sum_x                   = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_X]);
     rpn_push(_stack, _sum_x);

     (*dn_ops.lastx->func)(_stack, dn_ops.lastx);

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_DIV)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _stk_result              = rpn_pop(_stack);
     rpn_free_stack(_stack);

     _retcode                 = RPN_RET_OK;

end:
     rpn_push(stack, _stk_result);

     return _retcode;
}
/* dn_op_stats_mean() }}} */
/* dn_op_stats_sdev() {{{ */

/******************************************************************************

                              DN_OP_SDEV

******************************************************************************/
RPN_DEF_OP(dn_op_stats_sdev)
{
     rpn_elt                  *_stk_result,
                              *_sum_x, *_sum_y,
                              *_sum_x2, *_sum_y2, *_N,
                              *_constant;
     rpn_stack                *_stack;
     dl_operator		     *_op;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stack                   = rpn_new_stack("sdev");

     /* Y sdev
        ~~~~~~ */
     _sum_y2                   = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_Y2]);
     rpn_push(_stack, _sum_y2);

     _sum_y                    = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_Y]);
     rpn_push(_stack, _sum_y);

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_SQUARE)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _N                       = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_N]);
     rpn_push(_stack, _N);

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_DIV)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_SUB)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _N                       = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_N]);
     rpn_push(_stack, _N);

     _constant                = rpn_new_int(1);
     rpn_push(_stack, _constant);

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_SUB)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_DIV)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_SQRT)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _stk_result              = rpn_pop(_stack);
     rpn_push(stack, _stk_result);

     /* X sdev
        ~~~~~~ */
     _sum_x2                   = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_X2]);
     rpn_push(_stack, _sum_x2);

     _sum_x                    = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_X]);
     rpn_push(_stack, _sum_x);

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_SQUARE)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _N                       = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_N]);
     rpn_push(_stack, _N);

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_DIV)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_SUB)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _N                       = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_N]);
     rpn_push(_stack, _N);

     _constant                = rpn_new_int(1);
     rpn_push(_stack, _constant);

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_SUB)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_DIV)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     if ((_op = rpn_search_op(stack, RPN_OP_NAME_SQRT)) == 0) {
		_retcode							= RPN_RET_OP_NOT_FOUND;
		goto end;
     }
     if ((_retcode = (*_op->func)(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _stk_result              = rpn_pop(_stack);
     rpn_free_stack(_stack);
     rpn_push(stack, _stk_result);

     _retcode                 = RPN_RET_OK;

end:
     return _retcode;
}
/* dn_op_stats_sdev() }}} */
/* dn_op_stats_meanx() {{{ */

/******************************************************************************

                              DN_OP_MEANX

******************************************************************************/
RPN_DEF_OP(dn_op_stats_meanx)
{
     rpn_elt                  *_stk_x, *_stk_result, *_sum_x, *_elt;
     int                       _retcode, _N, _X_type;
     rpn_list                 *_list;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_LIST:
          _list                    = _stk_x->value.obj;
          if (!_list->homogeneous) {
               _retcode                 = RPN_RET_NOT_HOMOGENEOUS;
               _stk_result              = _stk_x;
               goto end;
          }

          _N                       = _list->num_elts;
          _sum_x                   = rpn_new_elt(_list->type);

          for ( ; _list->num_elts > 0; ) {
               _elt                     = rpn_list_pop_head(_list);

               switch (_list->type) {

               case RPN_TYPE_INT:
                    _sum_x->value.i               += _elt->value.i;
                    break;

               case RPN_TYPE_DOUBLE:
                    _sum_x->value.d               += _elt->value.d;
                    break;

               default:
                    RPN_INTERNAL_ERROR;
                    break;

               }

               rpn_free_elt(&_elt);
          }
          rpn_free_elt(&_stk_x);

          switch (_list->type) {

          case RPN_TYPE_INT:
               _sum_x->value.i               /= _N;
               break;

          case RPN_TYPE_DOUBLE:
               _sum_x->value.d               /= _N;
               break;

          default:
               RPN_INTERNAL_ERROR;
               break;

          }

          _stk_result              = _sum_x;
          break;

     default:
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          _stk_result              = _stk_x;
          goto end;
          break;
     }

end:
     rpn_push(stack, _stk_result);

     return _retcode;
}
/* dn_op_stats_mean() }}} */
/* dn_op_stats_mmm() {{{ */

/******************************************************************************

                              DN_OP_MMM

******************************************************************************/
RPN_DEF_OP(dn_op_stats_mmm)
{
     rpn_elt                  *_stk_x, *_stk_result, *_sum_x, *_elt,
                              *_elt_min, *_elt_max;
     int                       _retcode, _N, _X_type;
     rpn_list                 *_list, *_result_list;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_LIST:
          _list                    = _stk_x->value.obj;
          if (!_list->homogeneous) {
               _retcode                 = RPN_RET_NOT_HOMOGENEOUS;
               _stk_result              = _stk_x;
               goto end;
          }

          _N                       = _list->num_elts;
          if (_N == 0) {
               /* Empty list
                  ~~~~~~~~~~ */
               _stk_result              = rpn_new_elt(RPN_TYPE_NIL);
               goto end;
          }

          _sum_x                   = rpn_new_elt(_list->type);

          _elt_min                 = NULL;
          _elt_max                 = NULL;

          for ( ; _list->num_elts > 0; ) {
               _elt                     = rpn_list_pop_head(_list);
               if (_elt_min == NULL) {
#if 0
                    _elt_min                 = rpn_new_elt(_list->type);
                    _elt_min->value          = _elt->value;
#else
                    _elt_min                 = rpn_clone_elt(_elt);
#endif
               }
               if (_elt_max == NULL) {
#if 0
                    _elt_max                 = rpn_new_elt(_list->type);
                    _elt_max->value          = _elt->value;
#else
                    _elt_max                 = rpn_clone_elt(_elt);
#endif
               }

               switch (_list->type) {

               case RPN_TYPE_INT:
                    _sum_x->value.i               += _elt->value.i;
                    if (_elt_max->value.i < _elt->value.i) {
                         _elt_max->value.i         = _elt->value.i;
                    }
                    if (_elt_min->value.i > _elt->value.i) {
                         _elt_min->value.i         = _elt->value.i;
                    }
                    break;

               case RPN_TYPE_DOUBLE:
                    _sum_x->value.d               += _elt->value.d;
                    if (_elt_max->value.d < _elt->value.d) {
                         _elt_max->value.d         = _elt->value.d;
                    }
                    if (_elt_min->value.d > _elt->value.d) {
                         _elt_min->value.d         = _elt->value.d;
                    }
                    break;

               default:
                    RPN_INTERNAL_ERROR;
                    break;

               }

               rpn_free_elt(&_elt);
          }

          rpn_set_lastx(stack, _stk_x);

          switch (_list->type) {

          case RPN_TYPE_INT:
               _sum_x->value.i               /= _N;
               break;

          case RPN_TYPE_DOUBLE:
               _sum_x->value.d               /= _N;
               break;

          default:
               RPN_INTERNAL_ERROR;
               break;

          }

          _stk_result              = _sum_x;
          break;

     default:
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          _stk_result              = _stk_x;
          goto end;
          break;
     }


     _stk_result              = rpn_new_elt(RPN_TYPE_LIST);
     _result_list             = rpn_new_list((char *) __func__);
     _stk_result->value.obj   = _result_list;

     rpn_list_push_tail(_result_list, _elt_min);
     rpn_list_push_tail(_result_list, _elt_max);
     rpn_list_push_tail(_result_list, _sum_x);

     rpn_free_elt(&_stk_x);
// XXX TODO : free _list

end:
     rpn_push(stack, _stk_result);

     return _retcode;
}
/* dn_op_stats_mean() }}} */
/* dn_set_mmm_names() {{{ */

/******************************************************************************

                              DN_SET_MMM_NAMES

******************************************************************************/
void dn_set_mmm_names(rpn_elt *elt_list,
                      char *min_name, char *max_name, char *mean_name)
{
     int                       _type;
     rpn_list                 *_list;
     rpn_elt                  *_elt;

     if ((_type = rpn_get_type(elt_list)) != RPN_TYPE_LIST) {
          fprintf(stderr, "%s: error : type (%d = %s) != LIST\n",
                  G.progname, _type, rpn_type_to_string(_type));
          RPN_INTERNAL_ERROR;
     }

     _list                    = rpn_get_list_addr(elt_list);

     if (_list->num_elts != 3) {
          RPN_INTERNAL_ERROR;
     }

     _elt                     = _list->top_elt;
     rpn_set_elt_name(_elt, min_name);
     _elt                     = _elt->previous_elt;
     rpn_set_elt_name(_elt, max_name);
     _elt                     = _elt->previous_elt;
     rpn_set_elt_name(_elt, mean_name);
}

/* dn_set_mmm_names() }}} */
/* dn_op_stats_line_data_set() {{{ */

/******************************************************************************

                              DN_OP_LINE_DATA_SET

******************************************************************************/
RPN_DEF_OP(dn_op_stats_line_data_set)
{
     /* Generate linear data set
        ~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_elt                  *_stk_n, *_stk_delta_y, *_elt_x, *_elt_y,
						*_begin, *_coef_a_b, *_min_max;
     double                    _a, _b, _x_min, _x_max, _delta_y, _delta_x,
                               _xd, _yd, _delta;
     int                       _n, _retcode, _i, _rnd, _sign;

     _retcode                 = RPN_RET_OK;

     _stk_n                   = rpn_pop(stack);
     _stk_delta_y             = rpn_pop(stack);
	_min_max				= rpn_pop(stack);
	_coef_a_b				= rpn_pop(stack);

     _a                       = rpn_coef_get_a(_coef_a_b);
     _b                       = rpn_coef_get_b(_coef_a_b);
     _x_min                   = rpn_get_min(_min_max);
     _x_max                   = rpn_get_max(_min_max);
     _delta_y                 = _stk_delta_y->value.d;
     _n                       = _stk_n->value.i;

     _delta_x                 = (_x_max - _x_min) / (_n - 1);
     _xd                      = _x_min;

     /* Place a start marker (BEGIN) on the stack
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	rpn_inc_begin(stack);
	_begin				= rpn_new_elt(RPN_TYPE_START_MARKER);
	stack->new_value		= TRUE;
	rpn_push(stack, _begin);

	/* Generate the data set
	   ~~~~~~~~~~~~~~~~~~~~~ */
     for (_i = 0; _i < _n; _i++, _xd += _delta_x) {
          _sign                    = rand() < (RAND_MAX / 2) ? -1 : 1;
          _rnd                     = rand();
          _delta                   = _delta_y * _rnd / RAND_MAX;

		_yd                      = (_a * _xd) + _b + (_sign * _delta);

		_elt_x				= rpn_new_elt(RPN_TYPE_DOUBLE);
		_elt_x->value.d		= _xd,
		
		_elt_y				= rpn_new_elt(RPN_TYPE_DOUBLE);
		_elt_y->value.d		= _yd;

          printf("%#16.10g %#16.10g\n", _xd, _yd);

		rpn_push(stack, _elt_y);
		rpn_push(stack, _elt_x);

		dl_exec_op(stack, "pair");
     }

	dl_exec_op(stack, "list");

     return _retcode;
}

/* dn_op_stats_line_data_set() }}} */
/* dn_op_stats_exp_data_set() {{{ */

/******************************************************************************

                              DN_OP_EXP_DATA_SET

******************************************************************************/
RPN_DEF_OP(dn_op_stats_exp_data_set)
{
     /* Generate exponential data set
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_elt                  *_stk_n, *_stk_delta_y, *_elt_x, *_elt_y,
						*_begin, *_coef_a_b, *_min_max;
     double                    _a, _b, _x_min, _x_max, _delta_y, _delta_x,
                               _xd, _yd, _delta;
     int                       _n, _retcode, _i, _rnd, _sign;

     _retcode                 = RPN_RET_OK;

     _stk_n                   = rpn_pop(stack);
     _stk_delta_y             = rpn_pop(stack);
     _min_max                 = rpn_pop(stack);
     _coef_a_b                = rpn_pop(stack);

     _a                       = rpn_coef_get_a(_coef_a_b);
     _b                       = rpn_coef_get_b(_coef_a_b);
     _x_min                   = rpn_get_min(_min_max);
     _x_max                   = rpn_get_max(_min_max);
     _delta_y                 = _stk_delta_y->value.d;
     _n                       = _stk_n->value.i;

     _delta_x                 = (_x_max - _x_min) / (_n - 1);
     _xd                      = _x_min;

     /* Place a start marker (BEGIN) on the stack
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	rpn_inc_begin(stack);
	_begin				= rpn_new_elt(RPN_TYPE_START_MARKER);
	stack->new_value		= TRUE;
	rpn_push(stack, _begin);

	/* Generate the data set
	   ~~~~~~~~~~~~~~~~~~~~~ */
     for (_i = 0; _i < _n; _i++, _xd += _delta_x) {
          _sign                    = rand() < (RAND_MAX / 2) ? -1 : 1;
          _rnd                     = rand();
          _delta                   = _delta_y * _rnd / RAND_MAX;

		_yd                      = (_b * exp(_a * _xd)) + (_sign * _delta);

		_elt_x				= rpn_new_elt(RPN_TYPE_DOUBLE);
		_elt_x->value.d		= _xd,
		
		_elt_y				= rpn_new_elt(RPN_TYPE_DOUBLE);
		_elt_y->value.d		= _yd;

          printf("%#16.10g %#16.10g\n", _xd, _yd);

		rpn_push(stack, _elt_y);
		rpn_push(stack, _elt_x);

		dl_exec_op(stack, "pair");
     }

	dl_exec_op(stack, "list");

     return _retcode;
}

/* dn_op_stats_exp_data_set() }}} */
/* dn_op_stats_log_data_set() {{{ */

/******************************************************************************

                              DN_OP_LOG_DATA_SET

******************************************************************************/
RPN_DEF_OP(dn_op_stats_log_data_set)
{
     /* Generate linear data set
        ~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_elt                  *_stk_n, *_stk_delta_y, *_elt_x, *_elt_y,
						*_begin, *_coef_a_b, *_min_max;
     double                    _a, _b, _x_min, _x_max, _delta_y, _delta_x,
                               _xd, _yd, _delta;
     int                       _n, _retcode, _i, _rnd, _sign;

     _retcode                 = RPN_RET_OK;

     _stk_n                   = rpn_pop(stack);
     _stk_delta_y             = rpn_pop(stack);
	_min_max				= rpn_pop(stack);
	_coef_a_b				= rpn_pop(stack);

     _a                       = rpn_coef_get_a(_coef_a_b);
     _b                       = rpn_coef_get_b(_coef_a_b);
     _x_min                   = rpn_get_min(_min_max);
     _x_max                   = rpn_get_max(_min_max);
     _delta_y                 = _stk_delta_y->value.d;
     _n                       = _stk_n->value.i;

     _delta_x                 = (_x_max - _x_min) / (_n - 1);
     _xd                      = _x_min;

     /* Place a start marker (BEGIN) on the stack
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	rpn_inc_begin(stack);
	_begin				= rpn_new_elt(RPN_TYPE_START_MARKER);
	stack->new_value		= TRUE;
	rpn_push(stack, _begin);

	/* Generate the data set
	   ~~~~~~~~~~~~~~~~~~~~~ */
     for (_i = 0; _i < _n; _i++, _xd += _delta_x) {
          _sign                    = rand() < (RAND_MAX / 2) ? -1 : 1;
          _rnd                     = rand();
          _delta                   = _delta_y * _rnd / RAND_MAX;

		_yd                      = (_a * log(_xd)) + _b + (_sign * _delta);

		_elt_x				= rpn_new_elt(RPN_TYPE_DOUBLE);
		_elt_x->value.d		= _xd,
		
		_elt_y				= rpn_new_elt(RPN_TYPE_DOUBLE);
		_elt_y->value.d		= _yd;

          printf("%#16.10g %#16.10g\n", _xd, _yd);

		rpn_push(stack, _elt_y);
		rpn_push(stack, _elt_x);

		dl_exec_op(stack, "pair");
     }

	dl_exec_op(stack, "list");

     return _retcode;
}

/* dn_op_stats_log_data_set() }}} */
/* dn_op_stats_pow_data_set() {{{ */

/******************************************************************************

                              DN_OP_POW_DATA_SET

******************************************************************************/
RPN_DEF_OP(dn_op_stats_pow_data_set)
{
     /* Generate linear data set
        ~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_elt                  *_stk_n, *_stk_delta_y, *_elt_x, *_elt_y,
						*_begin, *_coef_a_b, *_min_max;
     double                    _a, _b, _x_min, _x_max, _delta_y, _delta_x,
                               _xd, _yd, _delta;
     int                       _n, _retcode, _i, _rnd, _sign;

     _retcode                 = RPN_RET_OK;

     _stk_n                   = rpn_pop(stack);
     _stk_delta_y             = rpn_pop(stack);
	_min_max				= rpn_pop(stack);
	_coef_a_b				= rpn_pop(stack);

     _a                       = rpn_coef_get_a(_coef_a_b);
     _b                       = rpn_coef_get_b(_coef_a_b);
     _x_min                   = rpn_get_min(_min_max);
     _x_max                   = rpn_get_max(_min_max);
     _delta_y                 = _stk_delta_y->value.d;
     _n                       = _stk_n->value.i;

     _delta_x                 = (_x_max - _x_min) / (_n - 1);
     _xd                      = _x_min;

     /* Place a start marker (BEGIN) on the stack
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	rpn_inc_begin(stack);
	_begin				= rpn_new_elt(RPN_TYPE_START_MARKER);
	stack->new_value		= TRUE;
	rpn_push(stack, _begin);

	/* Generate the data set
	   ~~~~~~~~~~~~~~~~~~~~~ */
     for (_i = 0; _i < _n; _i++, _xd += _delta_x) {
          _sign                    = rand() < (RAND_MAX / 2) ? -1 : 1;
          _rnd                     = rand();
          _delta                   = _delta_y * _rnd / RAND_MAX;

		_yd                      = _b * pow(_xd, _a) + (_sign * _delta);

		_elt_x				= rpn_new_elt(RPN_TYPE_DOUBLE);
		_elt_x->value.d		= _xd,
		
		_elt_y				= rpn_new_elt(RPN_TYPE_DOUBLE);
		_elt_y->value.d		= _yd;

          printf("%#16.10g %#16.10g\n", _xd, _yd);

		rpn_push(stack, _elt_y);
		rpn_push(stack, _elt_x);

		dl_exec_op(stack, "pair");
     }

	dl_exec_op(stack, "list");

     return _retcode;
}

/* dn_op_stats_pow_data_set() }}} */
/* dn_init() {{{ */

/******************************************************************************

					DN_INIT

******************************************************************************/
int dn_init(dl_module *module)
{
	/* Initialize pointers to operators that are independant of the
	 * element types
	 */
	dn_ops.roll_down		= dl_search_matching_op(G.stack, "rdn");
	dn_ops.swap_xy			= dl_search_matching_op(G.stack, "swap_xy");
	dn_ops.lastx			= dl_search_matching_op(G.stack, "lastx");

	return RPN_RET_OK;
}

/* dn_init() }}} */

