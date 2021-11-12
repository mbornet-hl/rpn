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
 *   @(#)  [MB] dl_cpub.h Version 1.11 du 21/11/12 - 
 */

#define	DL_INTERFACE_VERSION		 1

#if ! defined(_DL_CPUB_H)
#define	_DL_CPUB_H

#include	"../ci/ci_cpub.h"
#include	"../dr/dr_cpub.h"

/* Languages management {{{ */
#define	DL_MAX_LANG				(2)
/* Languages management }}} */
/* Macros definitions {{{ */

/* The first resulting parameter (=> nb_args) indicates the number of elements whose types will be checked
 * to determine which definition of the operator to execute.
 *
 * The second resulting parameter (out => nb_args_out) is used to know how many stack elements should be
 * displayed after the execution of the operator.
 */
#define	DL_T(t)							RPN_TYPE_##t
#define	DL_UNUSED							DL_T(UNUSED)
#define	DL_OP_DEF0(f, out)					{ 0, out, DL_UNUSED, DL_UNUSED, DL_UNUSED, DL_UNUSED, f, 0}
#define	DL_OP_DEF1(f, out, t1)				{ 1, out, DL_T(t1),  DL_UNUSED, DL_UNUSED, DL_UNUSED, f, 0}
#define	DL_OP_DEF2(f, out, t1, t2)			{ 2, out, DL_T(t1),  DL_T(t2),  DL_UNUSED, DL_UNUSED, f, 0}
#define	DL_OP_DEF3(f, out, t1, t2, t3)		{ 3, out, DL_T(t1),  DL_T(t2),  DL_T(t3),  DL_UNUSED, f, 0}
#define	DL_OP_DEF4(f, out, t1, t2, t3, t4)		{ 4, out, DL_T(t1),  DL_T(t2),  DL_T(t3),  DL_T(t4),  f, 0}

#define	DL_OP_DEF0H(f, out, h)				{ 0, out, DL_UNUSED, DL_UNUSED, DL_UNUSED, DL_UNUSED, f, h}
#define	DL_OP_DEF1H(f, out, t1, h)			{ 1, out, DL_T(t1),  DL_UNUSED, DL_UNUSED, DL_UNUSED, f, h}
#define	DL_OP_DEF2H(f, out, t1, t2, h)		{ 2, out, DL_T(t1),  DL_T(t2),  DL_UNUSED, DL_UNUSED, f, h}
#define	DL_OP_DEF3H(f, out, t1, t2, t3, h)		{ 3, out, DL_T(t1),  DL_T(t2),  DL_T(t3),  DL_UNUSED, f, h}
#define	DL_OP_DEF4H(f, out, t1, t2, t3, t4, h)	{ 4, out, DL_T(t1),  DL_T(t2),  DL_T(t3),  DL_T(t4),  f, h}

#define	DL_OP_DEF_END						{ 0, 0,   DL_UNUSED, DL_UNUSED, DL_UNUSED, DL_UNUSED, 0, 0}

#define	DL_MODULE_NO_INIT					(0)

/* Macros definitions }}} */
/* Type definitions {{{ */
typedef struct dl_module				 dl_module;
typedef struct dl_operator			 dl_operator;
typedef struct dl_op_params			 dl_op_params;
typedef struct dl_op_desc			 dl_op_desc;
typedef struct dl_op_stk_types		 dl_op_stk_types;
typedef union  dl_op_types			 dl_op_types;
typedef struct dl_op_dyn_node			 dl_op_dyn_node;
typedef struct dl_params_def_node		 dl_params_def_node;
typedef struct dl_params_dyn_node		 dl_params_dyn_node;

/* Type definitions }}} */
/* Module descriptor {{{ */
struct dl_module {
	char							 *name,
								 *version;
	cc_uint32						  link_ID;
	char							 *ops_array_name;
	dl_op_desc					 *ops_array;
	int							(*init)(dl_module *);
	char							**label;
};

/* Module descriptor }}} */
/* Operator descriptor {{{ */
struct dl_operator {
	char							*op_name;
	char							*module_name;
	cc_uint32						 nb_args_out;
	int                                (*func)(rpn_stack *, dl_operator *);
};

/* Operator descriptor }}} */
/* Static parameter types descriptor {{{ */
struct dl_op_params {
	cc_uint32						 nb_args;
	cc_uint32						 nb_args_out;
	cc_uint16						 X_type,
								 Y_type,
								 Z_type,
								 T_type;
	int							(*func)(rpn_stack *, dl_operator *);
	char							**help;
};

/* Parameter types descriptor }}} */
/* Static operator descriptor {{{ */
struct dl_op_desc {
	char							*name;
	struct dl_op_params				*params;
};

/* Static operator descriptor }}} */
/* Stack element types descriptor {{{ */
struct dl_op_stk_types {
	cc_uint16						X_type,
								Y_type,
								Z_type,
								T_type;
};

/* Stack element types descriptor }}} */
/* Global types descriptor {{{ */
union dl_op_types {
	struct dl_op_stk_types			 stk;
	cc_uint64						 global;		/* Global types */
};

/* Global types descriptor }}} */
/* Dynamic operator node descriptor {{{ */
struct dl_op_dyn_node {
	char							*name;
	struct ci_root					 root[5];
	struct ci_node					 node;

#if 0
	/* For alias management
	   ~~~~~~~~~~~~~~~~~~~~ */
	char							*alias_original_name;
	dl_op_dyn_node					*orig_op_desc;
#endif
};

/* Dynamic operator node descriptor }}} */
/* Dynamic operator definitions {{{ */
struct dl_params_def_node {
	dl_op_types					 types;		/* Op types */
	struct ci_node					 node;
	dr_fifo						 fifo;
};
/* Dynamic operator definitions }}} */
/* Dynamic parameter types descriptor {{{ */
struct dl_params_dyn_node {
	dl_op_params					 *params;		// XXX TO BE REMOVED !!! XXX REPLACED BY A POINTER TO params_def_node
	int                                (*func)(rpn_stack *, dl_operator *);
	cc_uint32						  idx;
	dl_module						 *module;
	dr_elt						  link;		/* FIFO link */
	char							**help;
};

/* Dynamic parameter types descriptor }}} */

#endif	// _DL_CPUB_H
