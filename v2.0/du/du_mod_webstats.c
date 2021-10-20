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
 *	@(#)	[MB] 	Version 1.6 du 21/07/30 - 
 */

#include	"../cc/cc_types.h"
#include	"../cy/cy_rpn_header.h"
#include	"../cy/cy_rpn_proto.h"
#include	"../ci/ci_cpub.h"
#include	"../dl/dl_cpub.h"
#include	"du_serial.h"
#include	"du_cpub.h"
#include	"du_epub.h"
#include	"du_cpri.h"
#include	"du_epri.h"

/* WWW
   ~~~ */
RPN_DECL_OP(du_op_log2mat);
RPN_DECL_OP(du_op_set_debug);
RPN_DECL_OP(du_op_get_debug);

static dl_op_desc					 du_ops_array[];

/* Help messages {{{ */
char							*du_help_log2mat[] = {
	"Convert a WWW logfile into a matrix",
	0
};

char							*du_help_set_debug[] = {
	"Set debug level for webstats module",
	0
};

char							*du_help_get_debug[] = {
	"Get debug level for webstats module",
	0
};

/* Help messages }}} */
/* Module descriptor {{{ */
struct dl_module		webstats_module = {
	"webstats",
	"2.0",
	DU_LINK_ID,
	0, //"du_operators",
	du_ops_array,
	0
};

/* Module descriptor }}} */
/* Operator parameters descriptions {{{ */
static dl_op_params					 du_params_log2mat[] = {
	DL_OP_DEF1H(du_op_log2mat, 1, FILENAME, du_help_log2mat),
	DL_OP_DEF_END
};

static dl_op_params					 du_params_set_debug[] = {
	DL_OP_DEF1H(du_op_set_debug, 1, INT, du_help_set_debug),
	DL_OP_DEF_END
};

static dl_op_params					 du_params_get_debug[] = {
	DL_OP_DEF0H(du_op_set_debug, 0, du_help_get_debug),
	DL_OP_DEF_END
};

/* Operator parameters descriptions }}} */
/* Operators list {{{ */
static dl_op_desc					 du_ops_array[] = {
	{	"log2mat",				du_params_log2mat				},
	{	"set_debug",				du_params_set_debug				},
	{	"get_debug",				du_params_get_debug				},
	{	0,					0								}
};

/* Operators list }}} */

/* GROUP : WWW {{{ */
/* du_new_log_entry() {{{ */

/******************************************************************************

                         DU_NEW_LOG_ENTRY

******************************************************************************/
RPN_PREF_NEW(du, log_entry)
/* du_new_log_entry() }}} */
/* du_add_to_list() {{{ */

/******************************************************************************

					DU_ADD_TO_LIST

******************************************************************************/
void du_add_to_list(struct du_list *list, struct du_log_entry *tmp_entry)
{
	if (list->insert == 0) {
		list->insert		= tmp_entry;
		list->extract		= tmp_entry;
	}
	else {
		list->insert->next	= tmp_entry;
		list->insert		= tmp_entry;
	}

	list->count++;
}

/* du_add_to_list() }}} */
/* du_extract_from_list() {{{ */

/******************************************************************************

					DU_EXTRACT_FROM_LIST

******************************************************************************/
struct du_log_entry *du_extract_from_list(struct du_list *list)
{
	struct du_log_entry		*_entry;

	if (list->extract == 0) {
		_entry			= 0;
	}
	else {
		_entry			= list->extract;
		list->extract		= list->extract->next;

		_entry->next		= 0;
		if (list->extract == 0) {
			list->insert		= 0;
		}

		list->count--;
	}

	return _entry;
}

/* du_add_to_list() }}} */
/* du_op_to_str() {{{ */

/******************************************************************************

						DU_OP_TO_STR

******************************************************************************/
char *du_op_to_str(int op)
{
	char				*_str_op;

	switch (op) {

	case	DU_OP_NIL:
		_str_op			= "NIL";
		break;

	case	DU_OP_GET:
		_str_op			= "GET";
		break;

	case	DU_OP_PUT:
		_str_op			= "PUT";
		break;

	case	DU_OP_POST:
		_str_op			= "POST";
		break;

	case	DU_OP_HEAD:
		_str_op			= "HEAD";
		break;

	case	DU_OP_OPTIONS:
		_str_op			= "OPTIONS";
		break;

	case	DU_OP_PROPFIND:
		_str_op			= "PROPFIND";
		break;

	default:
		_str_op			= "OP_UNKNOWN";
		break;
	}

	return strdup(_str_op);
}

/* du_op_to_str() }}} */
/* du_yyyymmdd() {{{ */

/******************************************************************************

					DU_YYYYMMDD

******************************************************************************/
char *du_yyyymmdd(struct tm *tm)
{
	char						 _Ymd[16];
	
	sprintf(_Ymd, "%04d-%02d-%02d",
	        tm->tm_year + 1900,
		   tm->tm_mon + 1,
		   tm->tm_mday);

	return strdup(_Ymd);
}

/* du_yyyymmdd() }}} */
/* du_HMS() {{{ */

/******************************************************************************

					DU_HMS

******************************************************************************/
char *du_HMS(struct tm *tm)
{
	char						 _HMS[16];
	
	sprintf(_HMS, "%02d:%02d:%02d",
	        tm->tm_hour,
		   tm->tm_min,
		   tm->tm_sec);

	return strdup(_HMS);
}

/* du_HMS() }}} */
/* du_fill_row() {{{ */

/******************************************************************************

						DU_FILL_ROW

******************************************************************************/
void du_fill_row(struct rpn_matrix *matrix, struct du_log_entry *entry,
                 int row, int n, int p)
{
	int					 _idx = 0, _j;
	rpn_elt				*_stk_elt;

	if (entry == 0) {
		for (_j = 1; _j <= DU_LOG_ENTRY_MEMBERS; _j++) {
			_idx					= RPN_MATRIX_IDX(row, _j, n, p);
			_stk_elt				= rpn_new_elt(RPN_TYPE_NIL);
			matrix->base[_idx]		= (void *) _stk_elt;
		}
	}
	else {
		/* IP */
		_j					= 1;
		_idx					= RPN_MATRIX_IDX(row, _j, n, p);
		_stk_elt				= rpn_new_elt(RPN_TYPE_IPv4);
		_stk_elt->value.i		= entry->IP.i;
		matrix->base[_idx]		= (void *) _stk_elt;

		/* Date (YYYY-mm-dd) */
		_j++;
		_idx					= RPN_MATRIX_IDX(row, _j, n, p);
		_stk_elt				= rpn_new_elt(RPN_TYPE_STRING);
		_stk_elt->value.s		= du_yyyymmdd(&entry->tm);
		matrix->base[_idx]		= (void *) _stk_elt;

		/* Time (HH:MM:SS) */
		_j++;
		_idx					= RPN_MATRIX_IDX(row, _j, n, p);
		_stk_elt				= rpn_new_elt(RPN_TYPE_STRING);
		_stk_elt->value.s		= du_HMS(&entry->tm);
		matrix->base[_idx]		= (void *) _stk_elt;

		/* Timestamp */
		_j++;
		_idx					= RPN_MATRIX_IDX(row, _j, n, p);
		_stk_elt				= rpn_new_elt(RPN_TYPE_INT);
		_stk_elt->value.i		= entry->time;
		matrix->base[_idx]		= (void *) _stk_elt;

		/* Error code */
		_j++;
		_idx					= RPN_MATRIX_IDX(row, _j, n, p);
		_stk_elt				= rpn_new_elt(RPN_TYPE_INT);
		_stk_elt->value.i		= entry->err_code;
		matrix->base[_idx]		= (void *) _stk_elt;

		/* Last field */
		_j++;
		_idx					= RPN_MATRIX_IDX(row, _j, n, p);
		_stk_elt				= rpn_new_elt(RPN_TYPE_INT);
		_stk_elt->value.i		= entry->last_field;
		matrix->base[_idx]		= (void *) _stk_elt;

		/* Operation */
		_j++;
		_idx					= RPN_MATRIX_IDX(row, _j, n, p);
		_stk_elt				= rpn_new_elt(RPN_TYPE_STRING);
		_stk_elt->value.s		= du_op_to_str(entry->op);
		matrix->base[_idx]		= (void *) _stk_elt;

		/* Pathname */
		_j++;
		_idx					= RPN_MATRIX_IDX(row, _j, n, p);
		_stk_elt				= rpn_new_elt(RPN_TYPE_STRING);
		_stk_elt->value.s		= entry->path;
		matrix->base[_idx]		= (void *) _stk_elt;
	}
}

/* du_fill_row() }}} */
/* du_op_log2mat() {{{ */

/******************************************************************************

						DU_OP_LOG2MAT

******************************************************************************/
RPN_DEF_OP(du_op_log2mat)
{
     rpn_elt                  *_stk_x, *_stk_result;
	rpn_matrix			*_matrix;
     char                     *_logfile;
     int                       _size, _i, _n = 0, _p = 0;
	FILE					*_fp;

     _stk_x                   = rpn_pop(stack);

     _logfile                 = _stk_x->value.s;

fprintf(stdout, "%s: %s():  logfile = [%s]\n", G.progname, __func__, _logfile);

     rpn_set_lastx(stack, _stk_x);


	if ((_fp = fopen(_logfile, "r")) == NULL) {
		fprintf(stderr, "%s: ERROR : cannot open \"%s\" !\n",
			   G.progname, _logfile);
		exit(RPN_EXIT_OPEN_ERROR);
	}
	else {
		du_log_restart(_fp);
		fprintf(stderr, "========== PARSE ==========\n");
		du_log_parse();
		fclose(_fp);
		printf("Nb lines : %ld\n", du_G.list.count);
	}

	_n					= du_G.list.count;
	_p					= DU_LOG_ENTRY_MEMBERS;

X
fprintf(stdout, "_n = %d _p = %d\n", _n, _p);

	_size                    = sizeof(*_matrix) + (((_n * _p) - 1) * sizeof (void *));

X
fprintf(stdout, "_size = %d\n", _size);
	_matrix                  = (struct rpn_matrix *) RPN_MALLOC(_size);
	_matrix->n               = _n;
	_matrix->p               = _p;

     _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
     _stk_result->value.obj   = _matrix;
X
fprintf(stdout, "matrix->n = %d matrix->p = %d\n", _matrix->n, _matrix->p);

	/* Store all log entries into the matrix
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	for (_i = 1; _i <= _n; _i++) {
		struct du_log_entry		*_log_entry;

		_log_entry			= du_extract_from_list(&du_G.list);
		du_fill_row(_matrix, _log_entry, _i, _n, _p);
	}
X
fprintf(stdout, "matrix->n = %d matrix->p = %d\n", _matrix->n, _matrix->p);

     rpn_push(stack, _stk_result);

     return RPN_RET_OK;
}
/* du_op_log2mat() }}} */
/* du op_set_debug() {{{ */

/******************************************************************************

					DU_OP_SET_DEBUG

******************************************************************************/
RPN_DEF_OP(du_op_set_debug)
{
	struct rpn_elt			*_stk_x;
	int					 _X_type;
	int					 _retcode;

	_retcode				= RPN_RET_OK;

	if (stack->num_elts >= 1) {
		_stk_x				= rpn_pop(stack);
		_X_type				= rpn_get_type(_stk_x);

		switch (_X_type) {

		case	RPN_TYPE_INT:
// {{{
			du_G.debug_level		= _stk_x->value.i;
			break;
// }}}
		default:
// {{{
			rpn_push(stack, _stk_x);
			_retcode			= RPN_RET_INVALID_X_TYPE;
			goto end;
			break;
// }}}
		}
	}
	else {
		/* Not enough elements in the stack
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		_retcode			= RPN_RET_NOT_ENOUGH_ELTS;
	}

end:
	return _retcode;
}

/* du op_set_debug() }}} */
/* du op_get_debug() {{{ */

/******************************************************************************

					DU_OP_GET_DEBUG

******************************************************************************/
RPN_DEF_OP(du_op_get_debug)
{
	struct rpn_elt			*_stk_result;
	int					 _retcode;

	_retcode				= RPN_RET_OK;

	_stk_result			= rpn_new_elt(RPN_TYPE_INT);
	_stk_result->value.i	= du_G.debug_level;

	return _retcode;
}

/* du op_get_debug() }}} */
/* du_IP_to_bytes() {{{ */

/******************************************************************************

					DU_IP_TO_BYTES

******************************************************************************/
unsigned int du_IP_to_bytes(char *IP_str)
{
	union rpn_IP			 _IP;
	char					*_ptr;
	int					 _idx;

	_ptr					= strtok(IP_str, ".");

	// Little endian !
	for (_idx = 3; _ptr != NULL; _idx--) {
		_IP.b[_idx]			= atoi(_ptr);
		_ptr					= strtok(NULL, ".");
	}

	return _IP.i;
}

/* du_IP_to_bytes() }}} */
/* GROUP : WWW }}} */
