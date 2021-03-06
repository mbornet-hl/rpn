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
 *	@(#)	[MB] ds_mod_core.c	Version 1.12 du 19/10/19 - 
 */

#include	<unistd.h>
#include  <math.h>
#include	<sys/stat.h>
#include	"../cc/cc_types.h"
#include	"../cy/cy_rpn_header.h"
#include	"../cy/cy_rpn_proto.h"
#include	"../ci/ci_cpub.h"
#include	"../dl/dl_cpub.h"
#include	"ds_serial.h"
#include	"ds_cpub.h"
#include	"ds_epub.h"

/* Help messages {{{ */
char							*ds_help_dupx[] = {
	"Creates X copies of Y in the stack",
	0
},
							*ds_help_int_double[] = {
	"Cast double to int",
	0
},
							*ds_help_double_int[] = {
	"Cast int to double",
	0
},
							*ds_help_chs[] = {
	"Change sign",
	0
},
							*ds_help_cat_list[] = {
	"Concatenate list X to list Y",
	0
},
							*ds_help_set_name[] = {
	"Set name of element Y",
	0
},
							*ds_help_reset_name[] = {
	"Clear name of element Y",
	0
},
							*ds_help_name[] = {
	"Push the name of X on the stack",
	0
},
							*ds_help_disp_name[] = {
	"Display the name of X",
	0
};


/* Help messages }}} */
/* Module descriptor {{{ */
struct dl_module		core_module = {
	"core",
	"2.0",
	DS_LINK_ID,
	0,
	ds_ops_array,
	0
};

/* Module descriptor }}} */
/* Operator parameters descriptions {{{ */
static dl_op_params					 ds_params_enter[] = {
	DL_OP_DEF0(ds_op_core_enter, 1),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_clx[] = {
	DL_OP_DEF0(ds_op_core_clx, 1),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_delx[] = {
	DL_OP_DEF0(ds_op_core_delx, 0),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_del_l[] = {
	DL_OP_DEF0(ds_op_core_del_l, 0),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_clst[] = {
	DL_OP_DEF0(ds_op_core_clst, 1),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_swap_xy[] = {
	DL_OP_DEF0(ds_op_core_swap_xy, 2),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_swap_xz[] = {
	DL_OP_DEF0(ds_op_core_swap_xz, 3),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_swap_xt[] = {
	DL_OP_DEF0(ds_op_core_swap_xt, 4),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_swap_xl[] = {
	DL_OP_DEF0(ds_op_core_swap_xl, 1),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_rdn[] = {
	DL_OP_DEF0(ds_op_core_roll_down, 1),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_rup[] = {
	DL_OP_DEF0(ds_op_core_roll_up, 1),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_lastx[] = {
	DL_OP_DEF0(ds_op_core_lastx, 1),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_rcl_x[] = {
	DL_OP_DEF0(ds_op_core_rcl_x, 1),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_prx[] = {
	DL_OP_DEF0(ds_op_core_prx, 1),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_prstk[] = {
	DL_OP_DEF0(ds_op_core_prstk, 0),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_stk[] = {
	DL_OP_DEF0(ds_op_core_stk, 0),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_dupx[] = {
	DL_OP_DEF2H(ds_op_core_dupx, 1, INT, ANY_TYPE, ds_help_dupx),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_dump[] = {
	DL_OP_DEF0(ds_op_core_dump, 0),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_chs[] = {
	DL_OP_DEF1H(ds_op_core_chs, 1, INT, ds_help_chs),
	DL_OP_DEF1H(ds_op_core_chs, 1, DOUBLE, ds_help_chs),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_sw_on[] = {
	DL_OP_DEF0(ds_op_core_sw_on, 0),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_sw_off[] = {
	DL_OP_DEF0(ds_op_core_sw_off, 0),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_filename[] = {
	DL_OP_DEF1(ds_op_core_filename, 1, STRING),
	DL_OP_DEF1(ds_op_core_filename, 1, FILENAME),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_int[] = {
	DL_OP_DEF1H(ds_op_core_int, 1, DOUBLE, ds_help_int_double),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_double[] = {
	DL_OP_DEF1H(ds_op_core_double, 1, INT, ds_help_double_int),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_string[] = {
	DL_OP_DEF1(ds_op_core_string, 1, LITTERAL),
	DL_OP_DEF1(ds_op_core_string, 1, FILENAME),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_litteral[] = {
	DL_OP_DEF1(ds_op_core_litteral, 1, STRING),
	DL_OP_DEF1(ds_op_core_litteral, 1, FILENAME),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_push[] = {
	DL_OP_DEF2(ds_op_core_push, 1, ANY_TYPE, LIST),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_pop[] = {
	DL_OP_DEF1(ds_op_core_pop, 1, LIST),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_list[] = {
	DL_OP_DEF0(ds_op_core_list, 1),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_pair[] = {
	DL_OP_DEF0(ds_op_core_pair, 1),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_tuple[] = {
	DL_OP_DEF0(ds_op_core_tuple, 1),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_coef_a_b[] = {
	DL_OP_DEF2(ds_op_core_coef_a_b, 1, DOUBLE, DOUBLE),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_min_max[] = {
	DL_OP_DEF2(ds_op_core_min_max, 1, DOUBLE, DOUBLE),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_cat[] = {
	DL_OP_DEF2H(ds_op_core_cat, 1, LIST, LIST, ds_help_cat_list),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_write[] = {
	DL_OP_DEF1(ds_op_core_write, 1, FILENAME),
	DL_OP_DEF1(ds_op_core_write, 1, TEXT_FILE),
	DL_OP_DEF_END
};

#if 0
static dl_op_params					 ds_params_push_extract[] = {
	DL_OP_DEF2(ds_op_core_push_extract, 1, ANY_TYPE, LIST),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_pop_insert[] = {
	DL_OP_DEF1(ds_op_core_pop_insert, 1, LIST),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_get_head[] = {
	DL_OP_DEF1(ds_op_core_get_head, 1, LIST),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_get_tail[] = {
	DL_OP_DEF1(ds_op_core_get_tail, 1, LIST),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_get[] = {
	DL_OP_DEF1(ds_op_core_get, 1, LIST),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_get_idx[] = {
	DL_OP_DEF1(ds_op_core_get_idx, 1, LIST),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_get_ignored[] = {
	DL_OP_DEF1(ds_op_core_get_ignored, 1, LIST),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_get_used[] = {
	DL_OP_DEF1(ds_op_core_get_used, 1, LIST),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_set_idx[] = {
	DL_OP_DEF1(ds_op_core_set_idx, 1, LIST),
	DL_OP_DEF_END
};
#endif

static dl_op_params					 ds_params_set_name[] = {
	DL_OP_DEF2H(ds_op_core_set_name, 1, STRING, ANY_TYPE, ds_help_set_name),
	DL_OP_DEF2H(ds_op_core_set_name, 1, NIL, ANY_TYPE, ds_help_reset_name),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_name[] = {
	DL_OP_DEF0H(ds_op_core_name, 1, ds_help_name),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_disp_name[] = {
	DL_OP_DEF0H(ds_op_core_disp_name, 0, ds_help_disp_name),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_explode[] = {
	DL_OP_DEF1(ds_op_core_explode, 1, LIST),
	DL_OP_DEF1(ds_op_core_explode, 1, TUPLE),
	DL_OP_DEF1(ds_op_core_explode, 1, OPAIR),
	DL_OP_DEF1(ds_op_core_explode, 1, COEF_A_B),
	DL_OP_DEF1(ds_op_core_explode, 1, MIN_MAX),
	DL_OP_DEF_END
};

static dl_op_params					 ds_params_clone[] = {
	DL_OP_DEF1(ds_op_core_clone, 1, MIN_MAX),
	DL_OP_DEF_END
};

/* Operator parameters descriptions }}} */
/* Operators list {{{ */
static dl_op_desc					 ds_ops_array[] = {
	{	"enter",				ds_params_enter					},
	{	"clx",				ds_params_clx						},
	{	"delx",				ds_params_delx						},
	{	"del_l",				ds_params_del_l					},
	{	"clst",				ds_params_clst						},
	{	"x<>y",				ds_params_swap_xy					},
	{	"x<>z",				ds_params_swap_xz					},
	{	"x<>t",				ds_params_swap_xt					},
	{	"x<>l",				ds_params_swap_xl					},
	{	"rdn",				ds_params_rdn						},
	{	"rup",				ds_params_rup						},
	{	"lastx",				ds_params_lastx					},
	{	"rcl_x",				ds_params_rcl_x					},
	{	"prx",				ds_params_prx						},
	{	"prstk",				ds_params_prstk					},
	{	"stk",				ds_params_stk						},
	{	"dupx",				ds_params_dupx						},
	{	"dump",				ds_params_dump						},
	{	"chs",				ds_params_chs						},
	{	"sw_on",				ds_params_sw_on					},
	{	"sw_off",				ds_params_sw_off					},
#if 00
	{	"rcl_off",			ds_params_rcl_sw					},
#endif
	{	"filename",			ds_params_filename					},
	{	"int",				ds_params_int						},
	{	"double",				ds_params_double					},
	{	"string",				ds_params_string					},
	{	"litteral",			ds_params_litteral					},
	{	"list",				ds_params_list						},
	{	"push",				ds_params_push						},
	{	"pop",				ds_params_pop						},
	{	"pair",				ds_params_pair						},
	{	"tuple",				ds_params_tuple					},
	{	"coef_a_b",			ds_params_coef_a_b					},
	{	"min_max",			ds_params_min_max					},
	{	"cat",				ds_params_cat						},
	{	"+",					ds_params_cat						},
	{	"write",				ds_params_write					},
	{	"set_name",			ds_params_set_name					},
	{	"name",				ds_params_name						},
	{	"disp_name",			ds_params_disp_name					},
	{	"expl",				ds_params_explode					},
	{	"clone",				ds_params_clone					},
	{	0,					0								}
};

/* Operators list }}} */

/* ds_op_core_enter() {{{ */
/******************************************************************************

                         DS_OP_CORE_ENTER

******************************************************************************/
RPN_DEF_OP(ds_op_core_enter)
{
     struct rpn_elt           *_stk_x, *_elt;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 1) {
          _stk_x    = rpn_pop(stack);
          _elt      = rpn_clone_elt(_stk_x);
          rpn_push(stack, _stk_x);
          rpn_push(stack, _elt);
     }

     stack->clear_x = TRUE;

     return _retcode;
}

/* ds_op_core_enter() }}} */
/* ds_op_core_clx() {{{ */
/******************************************************************************

                         DS_OP_CORE_CLX

******************************************************************************/
RPN_DEF_OP(ds_op_core_clx)
{
     struct rpn_elt           *_stk_x, *_nil;

     _stk_x         = rpn_pop(stack);
     rpn_free_elt(&_stk_x);

     _nil           = rpn_new_elt(RPN_TYPE_NIL);
     rpn_push(stack, _nil);

     stack->clear_x = TRUE;

     return RPN_RET_OK;
}

/* ds_op_core_clx() }}} */
/* ds_op_core_delx() {{{ */
/******************************************************************************

                         DS_OP_CORE_DELX

******************************************************************************/
RPN_DEF_OP(ds_op_core_delx)
{
     struct rpn_elt           *_stk_x;

     _stk_x         = rpn_pop(stack);
     rpn_free_elt(&_stk_x);

     return RPN_RET_OK;
}

/* ds_op_core_delx() }}} */
/* ds_op_core_del_l() {{{ */
/******************************************************************************

                         DS_OP_CORE_DEL_L

******************************************************************************/
RPN_DEF_OP(ds_op_core_del_l)
{
     rpn_free_elt(&stack->lastx);
     stack->lastx             = NULL;

     return RPN_RET_OK;
}

/* ds_op_core_del_l() }}} */
/* ds_op_core_clst() {{{ */
/******************************************************************************

                         DS_OP_CORE_CLST

******************************************************************************/
RPN_DEF_OP(ds_op_core_clst)
{
     struct rpn_elt           *_stk_x, *_nil;

     for (_stk_x = rpn_pop(stack); stack->num_elts != 0;_stk_x = rpn_pop(stack)) {
          rpn_free_elt(&_stk_x);
     }

     _nil           = rpn_new_elt(RPN_TYPE_NIL);
     rpn_push(stack, _nil);

     stack->clear_x = TRUE;

     return RPN_RET_OK;
}

/* ds_op_core_clst() }}} */
/* ds_op_core_swap_xy() {{{ */
/******************************************************************************

                         DS_OP_CORE_SWAP_XY

******************************************************************************/
RPN_DEF_OP(ds_op_core_swap_xy)
{
     struct rpn_elt           *_stk_x, *_stk_y;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 2) {
          _stk_x        = rpn_pop(stack);
          _stk_y        = rpn_pop(stack);

          rpn_push(stack, _stk_x);
          rpn_push(stack, _stk_y);
     }

     return _retcode;
}

/* ds_op_core_swap_xy() }}} */
/* ds_op_core_swap_xz() {{{ */
/******************************************************************************

                         DS_OP_CORE_SWAP_XZ

******************************************************************************/
RPN_DEF_OP(ds_op_core_swap_xz)
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

/* ds_op_core_swap_xz() }}} */
/* ds_op_core_swap_xt() {{{ */
/******************************************************************************

                         DS_OP_CORE_SWAP_XT

******************************************************************************/
RPN_DEF_OP(ds_op_core_swap_xt)
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

/* ds_op_core_swap_xt() }}} */
/* ds_op_core_swap_xl() {{{ */
/******************************************************************************

                         DS_OP_CORE_SWAP_XL

******************************************************************************/
RPN_DEF_OP(ds_op_core_swap_xl)
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

/* ds_op_core_swap_xl() }}} */
/* ds_op_core_roll_down() {{{ */
/******************************************************************************

                         DS_OP_CORE_ROLL_DOWN

******************************************************************************/
RPN_DEF_OP(ds_op_core_roll_down)
{
     int                       _retcode;
     struct rpn_elt           *_elt;

     _elt                     = rpn_pop(stack);
     rpn_base_push(stack, _elt);

     _retcode                 = RPN_RET_OK;

     return _retcode;
}

/* ds_op_core_roll_down() }}} */
/* ds_op_core_roll_up() {{{ */
/******************************************************************************

                         DS_OP_CORE_ROLL_UP

******************************************************************************/
RPN_DEF_OP(ds_op_core_roll_up)
{
     int                       _retcode;
     struct rpn_elt           *_elt;

     _elt                     = rpn_base_pop(stack);
     rpn_push(stack, _elt);

     _retcode                 = RPN_RET_OK;

     return _retcode;
}

/* ds_op_core_roll_up() }}} */
/* ds_op_core_lastx() {{{ */
/******************************************************************************

                         DS_OP_CORE_LASTX

******************************************************************************/
RPN_DEF_OP(ds_op_core_lastx)
{
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 1) {
          rpn_push(stack, rpn_clone_elt(stack->lastx));
     }

     return _retcode;
}

/* ds_op_core_lastx() }}} */
/* ds_op_core_rcl_x() {{{ */
/******************************************************************************

                         DS_OP_CORE_RCL_X

******************************************************************************/
RPN_DEF_OP(ds_op_core_rcl_x)
{
     struct rpn_elt           *_stk_x, *_elt;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 1) {
          _stk_x    = rpn_pop(stack);
          _elt      = rpn_clone_elt(_stk_x);
          rpn_push(stack, _stk_x);
          rpn_push(stack, _elt);
     }

     return _retcode;
}

/* ds_op_core_rcl_x() }}} */
/* ds_op_core_prx() {{{ */
/******************************************************************************

                         DS_OP_CORE_PRX

******************************************************************************/
RPN_DEF_OP(ds_op_core_prx)
{
     rpn_disp_elt(stack->top_elt, RPN_DISP_VALUE|RPN_DISP_INFOS);
     printf("%15s\n", op->op_name);

     return RPN_RET_OK;
}

/* ds_op_core_prx() }}} */
/* ds_op_core_prstk() {{{ */
/******************************************************************************

                         DS_OP_CORE_PRSTK

******************************************************************************/
RPN_DEF_OP(ds_op_core_prstk)
{
     /* Display stack
        ~~~~~~~~~~~~~ */
     rpn_disp_stack(stack);

     return RPN_RET_OK;
}

/* ds_op_core_prstk() }}} */
/* ds_op_core_stk() {{{ */
/******************************************************************************

                         DS_OP_CORE_STK

******************************************************************************/
RPN_DEF_OP(ds_op_core_stk)
{
     /* Display stack
        ~~~~~~~~~~~~~ */
//Z
     rpn_disp_stk(stack);
//Z

     return RPN_RET_OK;
}

/* ds_op_core_stk() }}} */
/* ds_op_core_dupx() {{{ */

/******************************************************************************

                         DS_OP_CORE_DUPX

******************************************************************************/
RPN_DEF_OP(ds_op_core_dupx)
{
     int                       _retcode, _X_type, _max, _i;
     rpn_elt                  *_stk_x, *_stk_y, *_clone;

     _retcode                 = RPN_RET_OK;

     /* Check minimum number of elements in the stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (stack->num_elts < 2) {
          _retcode                 = RPN_RET_NOT_ENOUGH_ELTS;
          goto end;
     }

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     if (_X_type != RPN_TYPE_INT) {
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          goto end;
     }

     _stk_y                   = rpn_pop(stack);

     /* Push Y back on the stack
        ~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_push(stack, _stk_y);

     _max                     = _stk_x->value.i;
     for (_i = 0; _i < _max - 1; _i++) {
          /* Clone Y
             ~~~~~~~ */
          _clone                   = rpn_clone_elt(_stk_y);

          /* Push clone on the stack
             ~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _clone);
     }

     /* Initialize lastx
        ~~~~~~~~~~~~~~~~ */
     rpn_set_lastx(stack, _stk_x);

     /* Free X
        ~~~~~~ */
     rpn_free_elt(&_stk_x);

end:
     return _retcode;
}

/* ds_op_core_dupx() }}} */
/* ds_op_core_dump() {{{ */
/******************************************************************************

                         DS_OP_CORE_DUMP

******************************************************************************/
RPN_DEF_OP(ds_op_core_dump)
{
     int                  _retcode;
     rpn_elt             *_elt;

     _retcode            = RPN_RET_OK;
     _elt                = stack->top_elt;

     /* Dump element with indentation level = 0
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_dump_elt(_elt, 0);

     return _retcode;
}

/* ds_op_core_dump() }}} */
/* ds_op_core_chs() {{{ */

/******************************************************************************

					DS_OP_CORE_CHS

******************************************************************************/
RPN_DEF_OP(ds_op_core_chs)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _x, _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _x                       = _stk_x->value.i;
		_stk_result              = rpn_new_elt(RPN_TYPE_INT);
		_stk_result->value.i     = - _x;
		break;

// }}}
	case	RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
		_stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
		_stk_result->value.d     = - _xd;
		break;

// }}}
	default:
// {{{
		fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
		_retcode				= RPN_RET_INVALID_X_TYPE;
		break;
// }}}
	}

	if (_retcode == RPN_RET_OK) {
		/* Set LastX and push result
		   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_set_lastx(stack, _stk_x);
          rpn_push(stack, _stk_result);
	}
	else {
		/* Restore X register
		   ~~~~~~~~~~~~~~~~~~ */
		rpn_push(stack, _stk_result);
	}

     return _retcode;
}

/* ds_op_core_chs() }}} */
/* ds_op_core_sw_on() {{{ */
/******************************************************************************

                         DS_OP_CORE_SW_ON

******************************************************************************/
RPN_DEF_OP(ds_op_core_sw_on)
{
     int             _retcode;

     _retcode            = RPN_RET_OK;

     G.sw_on             = TRUE;

     return _retcode;
}

/* ds_op_core_sw_on() }}} */
/* ds_op_core_sw_off() {{{ */
/******************************************************************************

                         DS_OP_CORE_SW_OFF

******************************************************************************/
RPN_DEF_OP(ds_op_core_sw_off)
{
     int             _retcode;

     _retcode            = RPN_RET_OK;

     G.sw_on             = FALSE;

     return _retcode;
}

/* ds_op_core_sw_off() }}} */
/* ds_op_core_filename {{{ */

/******************************************************************************

					DS_OP_CORE_FILENAME

******************************************************************************/
RPN_DEF_OP(ds_op_core_filename)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_STRING:
// {{{
		_stk_result              = rpn_new_elt(RPN_TYPE_FILENAME);
		_stk_result->value.s     = strdup(_stk_x->value.s);
		break;
// }}}

     case RPN_TYPE_FILENAME:
// {{{
		_stk_result              = rpn_new_elt(RPN_TYPE_FILENAME);
		_stk_result->value.s     = strdup(_stk_x->value.s);
		break;
// }}}
	default:
// {{{
          rpn_push(stack, _stk_x);
          _retcode                 = RPN_RET_INVALID_X_TYPE;
		goto end;
		break;
// }}}
	}

	rpn_set_lastx(stack, _stk_x);
	rpn_push(stack, _stk_result);

end:
     return _retcode;
}

/* ds_op_core_filename }}} */
/* ds_op_core_int {{{ */

/******************************************************************************

					DS_OP_CORE_INT

******************************************************************************/
RPN_DEF_OP(ds_op_core_int)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_DOUBLE:
// {{{
		_stk_result              = rpn_new_elt(RPN_TYPE_INT);
		_stk_result->value.i     = (int) _stk_x->value.d;
		break;
// }}}
	default:
// {{{
          rpn_push(stack, _stk_x);
          _retcode                 = RPN_RET_INVALID_X_TYPE;
		goto end;
		break;
// }}}
	}

	rpn_set_lastx(stack, _stk_x);
	rpn_push(stack, _stk_result);

end:
     return _retcode;
}

/* ds_op_core_int }}} */
/* ds_op_core_double {{{ */

/******************************************************************************

					DS_OP_CORE_DOUBLE

******************************************************************************/
RPN_DEF_OP(ds_op_core_double)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
		_stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
		_stk_result->value.d     = _stk_x->value.i;
		break;
// }}}
	default:
// {{{
          rpn_push(stack, _stk_x);
          _retcode                 = RPN_RET_INVALID_X_TYPE;
		goto end;
		break;
// }}}
	}

	rpn_set_lastx(stack, _stk_x);
	rpn_push(stack, _stk_result);

end:
     return _retcode;
}

/* ds_op_core_double }}} */
/* ds_op_core_string {{{ */

/******************************************************************************

					DS_OP_CORE_STRING

******************************************************************************/
RPN_DEF_OP(ds_op_core_string)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_LITTERAL:
// {{{
		_stk_result              = rpn_new_elt(RPN_TYPE_STRING);
		_stk_result->value.s     = strdup(_stk_x->value.s);
		break;
// }}}

     case RPN_TYPE_FILENAME:
// {{{
		_stk_result              = rpn_new_elt(RPN_TYPE_STRING);
		_stk_result->value.s     = strdup(_stk_x->value.s);
		break;
// }}}
	default:
// {{{
          rpn_push(stack, _stk_x);
          _retcode                 = RPN_RET_INVALID_X_TYPE;
		goto end;
		break;
// }}}
	}

	rpn_set_lastx(stack, _stk_x);
	rpn_push(stack, _stk_result);

end:
     return _retcode;
}

/* ds_op_core_string }}} */
/* ds_op_core_litteral {{{ */

/******************************************************************************

					DS_OP_CORE_LITTERAL

******************************************************************************/
RPN_DEF_OP(ds_op_core_litteral)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_STRING:
// {{{
		_stk_result              = rpn_new_elt(RPN_TYPE_LITTERAL);
		_stk_result->value.s     = strdup(_stk_x->value.s);
		break;
// }}}

     case RPN_TYPE_FILENAME:
// {{{
		_stk_result              = rpn_new_elt(RPN_TYPE_LITTERAL);
		_stk_result->value.s     = strdup(_stk_x->value.s);
		break;
// }}}
	default:
// {{{
          rpn_push(stack, _stk_x);
          _retcode                 = RPN_RET_INVALID_X_TYPE;
		goto end;
		break;
// }}}
	}

	rpn_set_lastx(stack, _stk_x);
	rpn_push(stack, _stk_result);

end:
     return _retcode;
}

/* ds_op_core_string }}} */
/* ds_op_core_list() {{{ */

/******************************************************************************

                         DS_OP_CORE_LIST

******************************************************************************/
RPN_DEF_OP(ds_op_core_list)
{
     int                  _retcode;
     rpn_elt             *_elt_list, *_elt;
     rpn_list            *_list;

     _retcode            = RPN_RET_OK;

     if (stack->nb_begin > 0) {
          _elt_list           = rpn_new_elt(RPN_TYPE_LIST);
          _list               = rpn_new_list((char *)__func__);

          for ( ; ; ) {
               _elt                = rpn_pop(stack);
               if (rpn_get_type(_elt) == RPN_TYPE_START_MARKER) {
                    rpn_dec_begin(stack);
                    break;
               }
               rpn_list_push_head(_list, _elt);
          }

          _elt_list->value.obj = _list;

          rpn_push(stack, _elt_list);
     }
     else {
          _retcode            = RPN_RET_NO_START_MARKER;
     }

     return _retcode;
}

/* ds_op_core_list() }}} */
/* ds_op_core_push() {{{ */

/******************************************************************************

                         DS_OP_CORE_PUSH

******************************************************************************/
RPN_DEF_OP(ds_op_core_push)
{
     int                       _retcode      = RPN_RET_OK,
                               _X_type, _Y_type;
     rpn_elt                  *_stk_x, *_stk_y;
     rpn_list                 *_list;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     _X_type                  = rpn_get_type(_stk_x);
     _Y_type                  = rpn_get_type(_stk_y);

     switch (_Y_type) {

     case RPN_TYPE_LIST:
          _list                    = _stk_y->value.obj;

          rpn_set_lastx(stack, _stk_x);

          /* Push X onto Y
             ~~~~~~~~~~~~~ */
          rpn_list_push_tail(_list, _stk_x);

          /* Push Y back on the stack
             ~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_y);

          break;

     default:
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));

          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_Y;
          break;
     }

     return _retcode;
}

/* ds_op_core_push() }}} */
/* ds_op_core_pop() {{{ */

/******************************************************************************

                         DS_OP_CORE_POP

******************************************************************************/
RPN_DEF_OP(ds_op_core_pop)
{
     int                       _retcode      = RPN_RET_OK,
                               _X_type;
     rpn_elt                  *_stk_x, *_elt;
     rpn_list                 *_list;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_LIST:
          _list                    = _stk_x->value.obj;

          rpn_set_lastx(stack, _stk_x);

          _elt                     = rpn_list_pop_head(_list);

          rpn_push(stack, _stk_x);
          rpn_push(stack, _elt);
          break;

     default:
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));

          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_X;
          break;
     }

     return _retcode;
}

/* ds_op_core_pop() }}} */
/* ds_op_core_pair() {{{ */

/******************************************************************************

                         DS_OP_CORE_PAIR

******************************************************************************/
RPN_DEF_OP(ds_op_core_pair)
{
     int                  _retcode;
     rpn_elt             *_elt_pair;
     rpn_pair            *_pair;

     _retcode            = RPN_RET_OK;

     if (stack->num_elts >= 2) {
          _elt_pair                = rpn_new_elt(RPN_TYPE_OPAIR);
          _pair                    = rpn_new_pair((char *)__func__);

          _pair->elt_b             = rpn_pop(stack);
          _pair->elt_a             = rpn_pop(stack);

          _elt_pair->value.obj     = _pair;

          rpn_push(stack, _elt_pair);
     }
     else {
          _retcode            = RPN_RET_NOT_ENOUGH_ELTS;
     }

     return _retcode;
}

/* ds_op_core_pair() }}} */
/* ds_op_core_tuple() {{{ */

/******************************************************************************

                         DS_OP_CORE_TUPLE

******************************************************************************/
RPN_DEF_OP(ds_op_core_tuple)
{
     int                  _retcode;
     rpn_elt             *_elt_list, *_elt;
     rpn_list            *_list;

     _retcode            = RPN_RET_OK;

     if (stack->nb_begin > 0) {
          _elt_list           = rpn_new_elt(RPN_TYPE_TUPLE);
          _list               = rpn_new_list((char *)__func__);

          for ( ; ; ) {
               _elt                = rpn_pop(stack);
               if (rpn_get_type(_elt) == RPN_TYPE_START_MARKER) {
                    rpn_dec_begin(stack);
                    break;
               }
               rpn_list_push_head(_list, _elt);
          }

          _elt_list->value.obj = _list;

          rpn_push(stack, _elt_list);
     }
     else {
          _retcode            = RPN_RET_NO_START_MARKER;
     }

     return _retcode;
}

/* ds_op_core_tuple() }}} */
/* ds_op_core_coef_a_b() {{{ */

/******************************************************************************

					DS_OP_CORE_COEF_A_B

******************************************************************************/
RPN_DEF_OP(ds_op_core_coef_a_b)
{
	rpn_elt					*_elt_coef_a_b, *_stk_x, *_stk_y;
	rpn_coef_a_b				*_coef_a_b;

	_stk_x					= rpn_pop(stack);
	_stk_y					= rpn_pop(stack);

	_coef_a_b					= rpn_new_coef_a_b();
	_coef_a_b->a				= _stk_y->value.d;
	_coef_a_b->b				= _stk_x->value.d;

	_elt_coef_a_b				= rpn_new_elt(RPN_TYPE_COEF_A_B);
	_elt_coef_a_b->value.obj		= _coef_a_b;

	rpn_free_elt(&_stk_x);
	rpn_free_elt(&_stk_y);

	rpn_push(stack, _elt_coef_a_b);

	return RPN_RET_OK;
}

/* ds_op_core_coef_a_b() }}} */
/* ds_op_core_min_max() {{{ */

/******************************************************************************

					DS_OP_CORE_MIN_MAX

******************************************************************************/
RPN_DEF_OP(ds_op_core_min_max)
{
	rpn_elt					*_elt_min_max, *_stk_x, *_stk_y;
	rpn_min_max				*_min_max;

	_stk_x					= rpn_pop(stack);
	_stk_y					= rpn_pop(stack);

	_min_max					= rpn_new_min_max();
	_min_max->min				= _stk_y->value.d;
	_min_max->max				= _stk_x->value.d;

	_elt_min_max				= rpn_new_elt(RPN_TYPE_MIN_MAX);
	_elt_min_max->value.obj		= _min_max;

	rpn_free_elt(&_stk_x);
	rpn_free_elt(&_stk_y);

	rpn_push(stack, _elt_min_max);

	return RPN_RET_OK;
}

/* ds_op_core_min_max() }}} */
/* ds_op_core_cat() {{{ */

/******************************************************************************

                         DS_OP_CORE_CAT

******************************************************************************/
RPN_DEF_OP(ds_op_core_cat)
{
     int					 _X_type, _Y_type, _retcode;
	rpn_elt				*_stk_x, *_stk_y, *_stk_result;
     rpn_list				*_list_x, *_list_y;

     _retcode            	= RPN_RET_OK;

	_stk_x				= rpn_pop(stack);
	_X_type				= rpn_get_type(_stk_x);

	switch (_X_type) {

	case	RPN_TYPE_LIST:
		_stk_y				= rpn_pop(stack);
		_Y_type				= rpn_get_type(_stk_y);

		switch (_Y_type) {
			
		case	RPN_TYPE_LIST:
			rpn_set_lastx(stack, _stk_x);

			_list_x				= _stk_x->value.obj;
			_list_y				= _stk_y->value.obj;

			if (_list_y->base_elt == 0) {
				_list_y->base_elt				= _list_x->base_elt;
				_list_y->top_elt				= _list_x->top_elt;
				_list_y->num_elts				= _list_x->num_elts;
				_list_y->type					= _list_x->type;
				_list_y->homogeneous			= _list_x->homogeneous;

				/* Clear X list
				   ~~~~~~~~~~~~ */
				_list_x->base_elt				= 0;
				_list_x->top_elt				= 0;
				_list_x->num_elts				= 0;
			}
			else {
				_list_y->base_elt->previous_elt	= _list_x->top_elt;
				_list_y->base_elt				= _list_x->base_elt;
				_list_y->num_elts				+= _list_x->num_elts;
				if (!_list_x->homogeneous
				|| (_list_y->homogeneous && _list_x->homogeneous && (_list_y->type != _list_x->type))) {
					_list_y->homogeneous			= FALSE;
				}

				/* Clear X list
				   ~~~~~~~~~~~~ */
				_list_x->base_elt				= 0;
				_list_x->top_elt				= 0;
				_list_x->num_elts				= 0;
			}

			rpn_free_elt(&_stk_x);
			_stk_result			= _stk_y;
			rpn_push(stack, _stk_result);
			break;

		default:
			rpn_push(stack, _stk_y);
			rpn_push(stack, _stk_x);

			_retcode				= RPN_RET_INVALID_Y_TYPE;
			break;
		}
		break;

	default:
		rpn_push(stack, _stk_x);

		_retcode				= RPN_RET_INVALID_X_TYPE;
		break;
	}

     return _retcode;
}

/* ds_op_core_cat() }}} */
/* ds_op_core_write() {{{ */

/******************************************************************************

                         DS_OP_CORE_WRITE

******************************************************************************/
RPN_DEF_OP(ds_op_core_write)
{
     rpn_elt                  *_stk_x, *_stk_y;
     int                       _X_type, _Y_type = RPN_TYPE_UNDEFINED;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_FILENAME:
          _stk_y                   = rpn_pop(stack);
          _Y_type                  = rpn_get_type(_stk_y);

          switch (_Y_type) {

          case RPN_TYPE_MATRIX:
               {
                    rpn_matrix               *_mat;
                    int                       _n, _p, _i, _j, _idx, _val, _type;
                    FILE                     *_fp;
                    char                     *_filename;
                    char                     *_fmt = "%3d";
                    char                     *_tab = "     ";
                    rpn_elt                  *_elt;

                    _filename                = _stk_x->value.s;
                    if ((_fp = fopen(_filename, "w")) == NULL) {
                         fprintf(stderr, "%s (%d) : cannot open \"%s\" !\n",
                                 __func__, __LINE__, _filename);
                         RPN_INTERNAL_ERROR;
                    }

                    _mat                     = (struct rpn_matrix *) _stk_y->value.obj;
                    _n                       = _mat->n;
                    _p                       = _mat->p;

                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              if (_j == 1 && _i > 1) {
                                   fprintf(_fp, "\n");
                              }
                              if (_j == 1) {
                                   fprintf(_fp, "%s", _tab);
                              }

                              _idx                     = RPN_MATRIX_IDX(_i, _j, _n, _p);
                              _elt                     = _mat->base[_idx];
                              if ((_type = rpn_get_type(_elt)) != RPN_TYPE_INT) {
                                   printf("Type = %d (%s) !\n",
                                         _type, rpn_type_to_string(_type));
                                   RPN_INTERNAL_ERROR;
                              }
                              _val                     = _elt->value.i;

                              if (_j > 1) {
                                   fprintf(_fp, " ");
                              }
                              fprintf(_fp, _fmt, _val);
                         }
                    }
                    fprintf(_fp, "\n");
// X
                    fclose(_fp);
               }

               rpn_free_elt(&_stk_x);
               rpn_free_elt(&_stk_y);
               break;

          default:
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
          }
          break;

     case RPN_TYPE_TEXT_FILE:
          {
               FILE                     *_fp;
               rpn_text_file            *_text_file;
               int                       _size, _i;
               char                     *_pathname, *_line;

               _text_file                    = _stk_x->value.obj;

               /* Eventually create the directory containing the file
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if ((_text_file->dirname != 0)) {
                    rpn_mkdir(_text_file->dirname);

                    _size                    = strlen(_text_file->dirname) + 1
                                             + strlen(_text_file->filename) + 1;
                    _pathname                = RPN_MALLOC(_size);
                    sprintf(_pathname, "%s/%s", _text_file->dirname, _text_file->filename);
               }
               else {
                    _pathname                = strdup(_text_file->filename);
               }

               if (access(_pathname, F_OK) < 0) {
                    if ((_fp = fopen(_pathname, "w")) == NULL) {
                         fprintf(stderr, "%s (%d) : cannot open \"%s\" !\n",
                                 __func__, __LINE__, _pathname);
                         RPN_INTERNAL_ERROR;
                    }

                    for (_i = 0; _text_file->text[_i] != NULL; _i++) {
                         _line                         = _text_file->text[_i];
                         fprintf(_fp, "%s\n", _line);
                    }
                    fclose(_fp);
               }

               /* Initialize file mode
                  ~~~~~~~~~~~~~~~~~~~~ */
               if (chmod(_pathname, _text_file->filemode) < 0) {
                    fprintf(stderr, "%s: cannot set mode of \"%s\" to %04o !\n",
                            G.progname, _pathname, _text_file->filemode);
               }

               RPN_FREE(_pathname);
          }

          /* TODO : initialize lastx
             ~~~~~~~~~~~~~~~~~~~~~~~ */
 //       rpn_set_lastx(stack, _stk_x);      // XXX

          rpn_free(_stk_x);
          break;

     default:
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
          RPN_INTERNAL_ERROR;
          break;
     }

//end:
     return _retcode;
}

/* ds_op_core_write() }}} */
/* ds_op_core_set_name() {{{ */

/******************************************************************************

					DS_OP_CORE_SET_NAME

******************************************************************************/
RPN_DEF_OP(ds_op_core_set_name)
{
	int						 _retcode, _X_type;
	rpn_elt					*_stk_x, *_stk_y, *_stk_result;

	_retcode					= RPN_RET_OK;

	_stk_x					= rpn_pop(stack);
	_X_type					= rpn_get_type(_stk_x);

	switch (_X_type) {

	case	RPN_TYPE_STRING:
		_stk_y					= rpn_pop(stack);
		if (_stk_y->name) {
			/* Free old name
			   ~~~~~~~~~~~~~ */
			rpn_free(_stk_y->name);
		}
		_stk_y->name				= strdup(_stk_x->value.s);

		rpn_set_lastx(stack, _stk_x);
		_stk_result				= _stk_y;
		break;

	case	RPN_TYPE_NIL:
		_stk_y					= rpn_pop(stack);
		if (_stk_y->name) {
			/* Free old name
			   ~~~~~~~~~~~~~ */
			rpn_free(_stk_y->name);
		}
		_stk_y->name				= 0;

		rpn_set_lastx(stack, _stk_x);
		_stk_result				= _stk_y;
		break;


	default:
		_retcode					= RPN_RET_INVALID_X_TYPE;
		_stk_result				= _stk_x;
		break;
	}

	rpn_push(stack, _stk_result);

	return _retcode;
}

/* ds_op_core_set_name() }}} */
/* ds_op_core_name() {{{ */

/******************************************************************************

					DS_OP_CORE_NAME

******************************************************************************/
RPN_DEF_OP(ds_op_core_name)
{
	int						 _retcode;
	rpn_elt					*_stk_x, *_elt_name;

	_retcode					= RPN_RET_OK;

	_stk_x					= rpn_pop(stack);
	if (_stk_x) {
		if (_stk_x->name) {
			_elt_name					= rpn_new_elt(RPN_TYPE_STRING);
			_elt_name->value.s			= strdup(_stk_x->name);

			rpn_push(stack, _stk_x);
			rpn_push(stack, _elt_name);
		}
		else {
			rpn_push(stack, _stk_x);
		}
	}

	return _retcode;
}

/* ds_op_core_name() }}} */
/* ds_op_core_disp_name() {{{ */

/******************************************************************************

					DS_OP_CORE_DISP_NAME

******************************************************************************/
RPN_DEF_OP(ds_op_core_disp_name)
{
	int						 _retcode;
	rpn_elt					*_stk_x;

	_retcode					= RPN_RET_OK;

	_stk_x					= rpn_pop(stack);
	if (_stk_x) {
		if (_stk_x->name) {
			printf("<%s>\n", _stk_x->name);
			rpn_push(stack, _stk_x);
		}
		else {
			rpn_push(stack, _stk_x);
		}
	}

	return _retcode;
}

/* ds_op_core_disp_name() }}} */
/* ds_op_core_explode() {{{ */

/******************************************************************************

                         DS_OP_CORE_EXPLODE

******************************************************************************/
RPN_DEF_OP(ds_op_core_explode)
{
     rpn_elt                  *_stk_x, *_elt;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     switch (_stk_x->type) {

     case RPN_TYPE_LIST:
     case RPN_TYPE_TUPLE:
          {
               rpn_list                 *_list;

               for (_list = _stk_x->value.obj; _list->num_elts > 0; ) {
                    _elt                     = rpn_list_pop_head(_list);
                    rpn_push(stack, _elt);
               }

               RPN_FREE(_list->name);
               RPN_FREE(_stk_x);
          }
          break;

	case	RPN_TYPE_OPAIR:
          {
               rpn_pair					*_pair;
Z

			_pair					= _stk_x->value.obj;
			_elt						= _pair->elt_a;
			_pair->elt_a				= 0;
			rpn_push(stack, _elt);

			_elt						= _pair->elt_b;
			_pair->elt_b				= 0;
			rpn_push(stack, _elt);

Z
               rpn_free_elt(&_stk_x);
Z
          }
		break;

	case	RPN_TYPE_COEF_A_B:
          {
               rpn_coef_a_b				*_coef_a_b;

			_coef_a_b					= _stk_x->value.obj;
			_elt						= rpn_new_double(_coef_a_b->a);
			rpn_push(stack, _elt);

			_elt						= rpn_new_double(_coef_a_b->b);
			rpn_push(stack, _elt);

               rpn_free_elt(&_stk_x);
          }
		break;

	case	RPN_TYPE_MIN_MAX:
          {
               rpn_min_max				*_min_max;

			_min_max					= _stk_x->value.obj;
			_elt						= rpn_new_double(_min_max->min);
			rpn_push(stack, _elt);

			_elt						= rpn_new_double(_min_max->max);
			rpn_push(stack, _elt);

               rpn_free_elt(&_stk_x);
          }
		break;

     default:
          rpn_push(stack, _stk_x);
          _retcode                 = RPN_RET_INVALID_X;
          break;
     }

     return _retcode;
}
/* ds_op_core_explode() }}} */
/* ds_op_core_clone() {{{ */

/******************************************************************************

					DS_OP_CORE_CLONE

******************************************************************************/
RPN_DEF_OP(ds_op_core_clone)
{
	rpn_elt					*_stk_x, *_clone;
	int						 _retcode, _X_type;

	_retcode					= RPN_RET_OK;

	_stk_x					= rpn_pop(stack);
	_X_type					= rpn_get_type(_stk_x);

	switch (_X_type) {

	case	RPN_TYPE_MIN_MAX:
		{
			rpn_min_max				*_min_max;
			_min_max					= rpn_new_min_max();
			_min_max->min				= rpn_get_min(_stk_x);
			_min_max->max				= rpn_get_max(_stk_x);

			_clone					= rpn_new_elt(RPN_TYPE_MIN_MAX);
			_clone->value.obj			= _min_max;
			rpn_set_elt_name(_clone, _stk_x->name);
		}
		break;

	default:
Z
		_retcode					= RPN_RET_INVALID_X_TYPE;
		break;
	}

	if (_retcode == RPN_RET_OK) {
		/* clone() does not set lastx (no need, and recursion problem)
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		rpn_push(stack, _stk_x);
		rpn_push(stack, _clone);
	}
	else {
		rpn_push(stack, _stk_x);
	}

	return _retcode;
}

/* ds_op_core_clone() }}} */
