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
 * along with this program.  If not, see <http://webstats.gnu.org/licenses/>.
 *
 *	@(#)	[MB] 	Version 1.4 du 20/07/31 - 
 */

#include	"../cc/cc_types.h"
#include	"../cy/cy_rpn_header.h"
#include	"../cy/cy_rpn_proto.h"
#include	"../ci/ci_cpub.h"
#include	"../dl/dl_cpub.h"
#include	"du_serial.h"
#include	"du_cpub.h"
#include	"du_epub.h"

/* WWW
   ~~~ */
RPN_DECL_OP(du_op_webstats_log2mat);

static dl_op_desc					 du_ops_array[];

/* Help messages {{{ */
char							*du_help_log2mat[] = {
	"Convert a WWW logfile into a matrix",
	0
};

/* Help messages }}} */
/* Module descriptor {{{ */
struct dl_module		webstats_module = {
	"webstats",
	"2.0",
	DU_LINK_ID,
	0, //"du_operators",
	du_ops_array
};

/* Module descriptor }}} */
/* Operator parameters descriptions {{{ */
static dl_op_params					 du_params_log2mat[] = {
	DL_OP_DEF1H(du_op_webstats_log2mat, 1, FILENAME, du_help_log2mat),
	DL_OP_DEF_END
};

/* Operator parameters descriptions }}} */
/* Operators list {{{ */
static dl_op_desc					 du_ops_array[] = {
	{	"webstatslog2mat",			du_params_log2mat					},
	{	0,					0								}
};

/* Operators list }}} */
/* GROUP : WWW {{{ */
/* du_op_webstats_log2mat() {{{ */

/******************************************************************************

						DU_OP_WWW_LOG2MAT

******************************************************************************/
RPN_DEF_OP(du_op_webstats_log2mat)
{
     rpn_elt                  *_stk_x, *_stk_result;
     char                     *_logfile;
     int                       _size, _n = 0, _p = 0;
	struct rpn_matrix        *_mat;
	FILE					*_fp;

     _stk_x                   = rpn_pop(stack);

     _logfile                 = _stk_x->value.s;

fprintf(stderr, "%s: %s():  logfile = [%s]\n", G.progname, __func__, _logfile);

     rpn_set_lastx(stack, _stk_x);

	/* TESTS ONLY !!!!! */
	_n					= 5;
	_p					= 10;


X
	if ((_fp = fopen(_logfile, "r")) == NULL) {
		fprintf(stderr, "%s: ERROR : cannot open \"%s\" !\n",
			   G.progname, _logfile);
		exit(RPN_EXIT_OPEN_ERROR);
	}
	else {
X
		du_log_restart(_fp);
X
X
		// du_log_lex();
		fprintf(stderr, "========== PARSE ==========\n");
X
		du_log_parse();
X
		fclose(_fp);
X
	}

X
	_size                    = sizeof(*_mat) + (((_n * _p) - 1) * sizeof (void *));
	_mat                     = (struct rpn_matrix *) RPN_MALLOC(_size);
	_mat->n                  = _n;
	_mat->p                  = _p;

     _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
     _stk_result->value.obj   = _mat;

     rpn_push(stack, _stk_result);

     return RPN_RET_OK;
}
/* du_op_webstats_log2mat() }}} */
/* GROUP : WWW }}} */
