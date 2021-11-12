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
 *	@(#)	[MB] dm_mod_str.c	Version 1.24 du 21/11/12 - 
 */

#include	"../cc/cc_types.h"
#include	"../cy/cy_rpn_header.h"
#include	"../cy/cy_rpn_proto.h"
#include	"../ci/ci_cpub.h"
#include	"../dl/dl_cpub.h"
#include	"dm_serial.h"
#include	"dm_cpub.h"

/* Strings
   ~~~~~~~ */
RPN_DECL_OP(dm_op_str_length);
RPN_DECL_OP(dm_op_str_catenate);
RPN_DECL_OP(dm_op_str_dupl);

static dl_op_desc					 dm_ops_array[];

/* Module description {{{ */
static char						*dm_module_label[] = {
	"Strings functions",
	"Manipulation de chaines de caracteres",
	0
};

/* Module description }}} */
/* Help messages {{{ */
char							*dm_help_dupl[] = {
	"Concatenate string Y X times",
	0
};

char							*dm_help_cat[] = {
	"Concatenate string X to string Y",
	0
};

char							*dm_help_length[] = {
	"Replace X with length of string in X",
	0
};

/* Help messages }}} */
/* Module descriptor {{{ */
struct dl_module		strings_module = {
	"strings",
	"2.0",
	DM_LINK_ID,
	0, //"dm_operators",
	dm_ops_array,
	DL_MODULE_NO_INIT,
	dm_module_label
};

/* Module descriptor }}} */
/* Operator parameters descriptions {{{ */
static dl_op_params					 dm_params_cat[] = {
	DL_OP_DEF2H(dm_op_str_catenate, 1, STRING, STRING, dm_help_cat),
	DL_OP_DEF_END
};

static dl_op_params					 dm_params_dupl[] = {
	DL_OP_DEF2H(dm_op_str_dupl, 1, INT, STRING, dm_help_dupl),
	DL_OP_DEF_END
};

static dl_op_params					 dm_params_length[] = {
	DL_OP_DEF1H(dm_op_str_length, 1, STRING, dm_help_length),
	DL_OP_DEF_END
};
/* Operator parameters descriptions }}} */
/* Operators list {{{ */
static dl_op_desc					 dm_ops_array[] = {
	{	"cat",				dm_params_cat						},
	{	"+",					dm_params_cat						},
	{	"dupl",				dm_params_dupl						},
	{	"*",					dm_params_dupl						},
	{	"length",				dm_params_length					},
	{	0,					0								}
};

/* Operators list }}} */

/* GROUP : Strings {{{ */
/* dm_op_str_length() {{{ */

/******************************************************************************

						DM_OP_STR_LENGTH

******************************************************************************/
RPN_DEF_OP(dm_op_str_length)
{
     rpn_elt                  *_stk_x, *_stk_result;
     char                     *_s;
     int                       _result;

     _stk_x                   = rpn_pop(stack);

     _s                       = _stk_x->value.s;

     _result                  = strlen(_s);

     rpn_set_lastx(stack, _stk_x);

     _stk_result              = rpn_new_elt(RPN_TYPE_INT);
     _stk_result->value.i     = _result;

     rpn_push(stack, _stk_result);

     return RPN_RET_OK;
}
/* dm_op_str_length() }}} */
/* dm_op_str_catenate() {{{ */

/******************************************************************************

						DM_OP_STR_CATENATE

******************************************************************************/
RPN_DEF_OP(dm_op_str_catenate)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     char                     *_s1, *_s2;
     char                     *_result;
     size_t                    _lg;
	int					 _retcode      = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     if (rpn_get_type(_stk_x) == RPN_TYPE_STRING) {
          if (rpn_get_type(_stk_y) == RPN_TYPE_STRING) {
               _s1                      = _stk_y->value.s;
               _s2                      = _stk_x->value.s;

               _lg                      = strlen(_s1) + strlen(_s2);
               _result                  = RPN_MALLOC(_lg + 1);

               strcpy(_result, _s1);
               strcat(_result, _s2);

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_STRING);
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
/* dm_op_str_catenate() }}} */
/* dm_op_str_dupl() {{{ */

/******************************************************************************

						DM_OP_STR_DUPL

******************************************************************************/
RPN_DEF_OP(dm_op_str_dupl)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     char                     *_s;
     int                       _n, _i;
	size_t				 _total_size;
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
                    _result                  = RPN_MALLOC(_total_size + 1);

                    strcpy(_result, _s);
                    for (_i = 0; _i < (_n - 1); _i++) {
                         strcat(_result, _s);
                    }

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt(RPN_TYPE_STRING);
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
/* dm_op_str_dupl() }}} */
/* GROUP : Strings }}} */
