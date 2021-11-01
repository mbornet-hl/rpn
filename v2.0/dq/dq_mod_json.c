/*
 * Copyright (C) 2019, Martial Bornet
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
 *   @(#)  [MB] dq_mod_json.c Version 1.14 du 21/11/01 - 
 */

#include	<stdio.h>
#include	<stdarg.h>

#include  "../cc/cc_types.h"
#include  "../cy/cy_rpn_header.h"
#include  "../cy/cy_rpn_proto.h"
#include	"../dt/dt_epub.h"
#include	"dq_serial.h"
#include	"dq_cpub.h"
#include	"dq_epub.h"

/* JSON
   ~~~~ */
RPN_DECL_OP(dq_write_json);

/* Module descriptor {{{ */
struct dl_module         json_module = {
     "json",
     "2.0",
     DQ_LINK_ID,
     0,
	dq_ops_array,
	0
};

/* Module descriptor }}} */
/* Help messages {{{ */
char							*dq_help_json_file[] = {
	"Write Y to file specified by X",
	0
},
							*dq_help_json_matrix[] = {
	"Write matrix X in JSON format",
	0
},
							*dq_help_json_list[] = {
	"Write list X in JSON format",
	0
},
							*dq_help_json_opair[] = {
	"Write opair X in JSON format",
	0
},
							*dq_help_json_string[] = {
	"Write string X in JSON format",
	0
},
							*dq_help_json_double[] = {
	"Write double X in JSON format",
	0
},
							*dq_help_json_nil[] = {
	"Write NIL in JSON format",
	0
};

/* Help messages }}} */
/* Operator parameters descriptions {{{ */
static dl_op_params					 dq_params_write_json[] = {
	DL_OP_DEF1H(dq_op_json_write_json, 1, DOUBLE,   dq_help_json_double),
	DL_OP_DEF1H(dq_op_json_write_json, 0, FILENAME, dq_help_json_file),
	DL_OP_DEF1H(dq_op_json_write_json, 1, MATRIX,   dq_help_json_matrix),
	DL_OP_DEF1H(dq_op_json_write_json, 1, NIL,      dq_help_json_nil),
	DL_OP_DEF1H(dq_op_json_write_json, 1, LIST,     dq_help_json_list),
	DL_OP_DEF1H(dq_op_json_write_json, 1, OPAIR,    dq_help_json_opair),
	DL_OP_DEF1H(dq_op_json_write_json, 1, STRING,   dq_help_json_string),
	DL_OP_DEF_END
};

/* Operator parameters descriptions }}} */
/* Operators list {{{ */
static dl_op_desc					 dq_ops_array[] = {
	{	"write_json",			dq_params_write_json				},
	{	0,					0								}
};

/* Operators list }}} */

// GROUP : JSON {{{
/* dq_fprintf() {{{ */

/******************************************************************************

					DQ_FPRINTF

******************************************************************************/
static int dq_fprintf(FILE *fp, int delta_before, int delta_after, char *fmt, ...)
{
	static int			 _level = 0;
	int					 _retcode, _i;
	char					*_indent_string = "     ";
	va_list				 _ap;

	_level				+= delta_before;

	for (_i = 0; _i < _level; _i++) {
		fprintf(fp, "%s", _indent_string);
	}

	va_start(_ap, fmt);

	_retcode				= vfprintf(fp, fmt, _ap);

	_level				+= delta_after;
	return _retcode;
}

/* dq_fprintf() }}} */
/* dq_assert_name() {{{ */

/******************************************************************************

					DQ_ASSERT_NAME

******************************************************************************/
static char *dq_assert_name(char *name)
{
	return name ? name : "NO_NAME";
}

/* dq_assert_name() }}} */
/* dq_op_json_write_json() {{{ */

/******************************************************************************

					DQ_OP_JSON_WRITE_JSON

******************************************************************************/
RPN_DEF_OP(dq_op_json_write_json)
{
     int                       _retcode, _type;
	rpn_elt				*_stk_x;
	static FILE			*_fp = 0;
	static int			 _calls_level = 0;

	if (_calls_level == 0 && _fp == 0) {
		_fp					= stdout;
	}
	_calls_level++;

     _retcode                 = RPN_RET_OK;
	_stk_x				= rpn_pop(stack);

	switch ((_type = rpn_get_type(_stk_x))) {

	case	RPN_TYPE_FILENAME:
		{
			char					*_filename;

			/* Create output file
			   ~~~~~~~~~~~~~~~~~~ */
			_filename				= _stk_x->value.s;
			if ((_fp = fopen(_filename, "w")) == NULL) {
				fprintf(stderr, "%s: %s (%d) : cannot open \"%s\" for writing !\n",
					   G.progname, __func__, __LINE__, _filename);
					   exit(RPN_EXIT_OPEN_ERROR);
			}

			/* Convert the current X to JSON
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			dq_op_json_write_json(stack, op);
		}
		break;

	case	RPN_TYPE_MATRIX:
		{
			rpn_stack				*_stack;
			int					 _i, _j, _n, _p;
			struct rpn_matrix		*_mat;

			_mat					= (struct rpn_matrix *) _stk_x->value.obj;

			_n					= _mat->n;
			_p					= _mat->p;

			_stack				= rpn_new_stack(__func__);
			dq_fprintf(_fp, 0, 1, "{\n");
			dq_fprintf(_fp, 0, 1, "\"%s\" : {\n", dq_assert_name(_stk_x->name));
			for (_i = 1; _i <= _n; _i++) {
				for (_j = 1; _j <= _p; _j++) {
					rpn_elt				*_elt, *_elt_clone;
					dq_fprintf(_fp, 0, 1,  "\"matrix_elt\" : {\n");
					dq_fprintf(_fp, 0, 0,  "\"row\" : %d,\n", _i);
					dq_fprintf(_fp, 0, 0,  "\"col\" : %d,\n", _j);

					_elt					= dt_mat_get_elt_ref(_mat, _i, _j);
					_elt_clone			= rpn_clone_elt(_elt);
					rpn_push(_stack, _elt_clone);
					dq_op_json_write_json(_stack, op);
					fprintf(_fp, "\n");
					dq_fprintf(_fp, -1, 0, "}%s\n", (_i != _n) || (_j != _p) ? "," : "" );
				}
			}
			dq_fprintf(_fp, -1, 0, "}\n");
			dq_fprintf(_fp, -1, 0, "}\n");
		}
		break;

	case	RPN_TYPE_LIST:
		{
			rpn_list			*_list;
			rpn_elt			*_elt;
			rpn_stack			*_stack;

			dq_fprintf(_fp, 0, 1, "\"%s\" : {", dq_assert_name(_stk_x->name));
			_list			= _stk_x->value.obj;

			_stack			= rpn_new_stack(__func__);

			if (_list->num_elts > 0) {
				int 				 _first = TRUE;

				for (_elt = _list->top_elt; _elt != 0; _elt = _elt-> previous_elt) {
					rpn_elt			*_elt_clone;

					fprintf(_fp, "%s\n", _first ? "" : "," );
					_first			= FALSE;
					_elt_clone		= rpn_clone_elt(_elt);
					rpn_push(_stack, _elt_clone);
					dq_op_json_write_json(_stack, op);
				}
			}

			fprintf(_fp, "\n");
			dq_fprintf(_fp, -1, 0, "}");
			rpn_free_stack(_stack);
		}
		break;

	case	RPN_TYPE_OPAIR:
		{
			rpn_pair			*_pair;
			rpn_elt			*_elt_a, *_elt_b, *_elt_clone;
			rpn_stack			*_stack;

			dq_fprintf(_fp, 0, 1, "\"%s\" : {\n", dq_assert_name(_stk_x->name));

			_stack			= rpn_new_stack(__func__);

			_pair			= _stk_x->value.obj;
			_elt_a			= _pair->elt_a;
			_elt_b			= _pair->elt_b;
			_elt_clone		= rpn_clone_elt(_elt_a);
			rpn_push(_stack, _elt_clone);
			dq_op_json_write_json(_stack, op);
			fprintf(_fp, ",\n");

			_elt_clone		= rpn_clone_elt(_elt_b);
			rpn_push(_stack, _elt_clone);
			dq_op_json_write_json(_stack, op);
			fprintf(_fp, "\n");

			rpn_free_stack(_stack);

			dq_fprintf(_fp, -1, 0, "}");
		}
		break;

	case	RPN_TYPE_STRING:
		{
			dq_fprintf(_fp, 0, 0, "\"%s\" : \"%s\"",
			       dq_assert_name(_stk_x->name), _stk_x->value.s);
		}
		break;

	case	RPN_TYPE_INT:
		{
			dq_fprintf(_fp, 0, 0, "\"%s\" : \"%d\"",
			       dq_assert_name(_stk_x->name), _stk_x->value.i);
		}
		break;

	case	RPN_TYPE_DOUBLE:
		{
			dq_fprintf(_fp, 0, 0, "\"%s\" : \"%e\"",
			       dq_assert_name(_stk_x->name), _stk_x->value.d);
		}
		break;

	case	RPN_TYPE_NIL:
		{
			dq_fprintf(_fp, 0, 0, "\"%s\" : \"<NIL>\"",
			       dq_assert_name(_stk_x->name));
		}
		break;

	default:
		fprintf(stderr, "%s: type = %d = %s\n", G.progname, _type,
		        rpn_type_to_string(_type));
		RPN_INTERNAL_ERROR;
		break;
	}

	rpn_push(stack, _stk_x);

	_calls_level--;

	if (_calls_level == 0 && _fp != stdout) {
		/* Final return of the operator : close the file
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		fclose(_fp);
	}

     return _retcode;
}

/* dq_op_json_write_json() }}} */
// GROUP : JSON }}}
