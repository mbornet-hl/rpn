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
 *	@(#)	[MB] cy_rpn_import.c	Version 1.23 du 21/10/03 - 
 */

#include	<stdio.h>
#include	<unistd.h>
#include	<dlfcn.h>
#include	"cy_rpn_header.h"
#include	"cy_rpn_proto.h"
#include	"../dl/dl_cpub.h"
#include	"../dl/dl_epub.h"
#include	"../ci/ci_cpub.h"
#include	"../ci/ci_epub.h"

/* rpn_import() {{{ */

/******************************************************************************

						RPN_IMPORT

******************************************************************************/
int rpn_import(char *module_name)
{
	void					 *_handle;
	char					 _lib_path[128], _mod_desc[128];
	struct rpn_operator		 *_ops_array, *_op;
	struct rpn_module		 *_dyn_module;
	struct dl_module		 *_module;
	struct rpn_dyn_op		 *_dyn_op;
	int					  _ret;

	RPN_TRACE_VERBOSE("IMPORTING module \"%s\" ...\n", module_name);

	/* Search for the module in the modules tree
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (rpn_module_tree_search(module_name) != 0) {
		if (!G.silent) {
			fprintf(stderr, "%s: module \"%s\" has already been imported !\n",
				   G.progname, module_name);
		}
		_ret			= RPN_RET_DUPLICATE;
		goto end;
	}

	/* Generate library pathname
	   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (sprintf(_lib_path, "%s/lib%s.so", G.libpath, module_name) < 0) {
		fprintf(stderr, "sprintf error !\n");
		exit(1);
	}

	if (G.debug_level & RPN_DBG_DEBUG) {
		fprintf(stderr, "libpath = [%s]\n", _lib_path);
	}

	/* Check file existence
	   ~~~~~~~~~~~~~~~~~~~~ */
	if (access(_lib_path, F_OK) < 0) {
		fprintf(stderr, "File \"%s\" does not exist !\n", _lib_path);
//		perror("access");
		_ret			= RPN_RET_NONEXISTENT;
		goto end;
	}

	/* Check file access
	   ~~~~~~~~~~~~~~~~~ */
	if (access(_lib_path, R_OK) < 0) {
		fprintf(stderr, "File \"%s\" is not readable!\n", _lib_path);
//		perror("access");
		_ret			= RPN_RET_NOT_READABLE;
		goto end;
	}


	/* Try to link the library
	   ~~~~~~~~~~~~~~~~~~~~~~~ */
	if ((_handle = dlopen(_lib_path, RTLD_LAZY|RTLD_GLOBAL)) == 0) {
	   /* Fail to load the library
	      ~~~~~~~~~~~~~~~~~~~~~~~~ */
	   fprintf(stderr, "Cannot link library \"%s\" ! (%s)\n",
	           _lib_path, dlerror());
	   _ret			= RPN_RET_CANNOT_LINK;
	   goto end;
	}

	/* Generate the name of the module descriptor structure
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	sprintf(_mod_desc, "%s_module", module_name);
	if (!G.silent && (G.debug_level & RPN_DBG_VERBOSE)) {
		printf("module descriptor : \"%s\"\n", _mod_desc);
	}

	/* Get the pointer to the module descriptor
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	_module	= dlsym(_handle, _mod_desc);
	if (_module == 0) {
		fprintf(stderr, "%s: module descriptor \%s\" NOT FOUND !\n",
		        G.progname, _mod_desc);
		_ret			= RPN_RET_NOT_FOUND;
		goto end;
	}

	if (!G.silent && (G.debug_level & RPN_DBG_VERBOSE)) {
		printf("name    : [%s]\n", _module->name);
		printf("version : %s\n", _module->version);
		printf("link_ID : %d\n", _module->link_ID);
	}

	/* Add a module descriptor in the modules tree
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	_dyn_module				= rpn_new_module();
	_dyn_module->name			= strdup(_module->name);
	_dyn_module->version		= strdup(_module->version);
	_dyn_module->link_ID		= _module->link_ID;
	_dyn_module->node.data		= _dyn_module;

	if ((_ret = rpn_module_tree_add(_dyn_module)) != RPN_RET_OK) {
		if (_ret == RPN_RET_DUPLICATE) {
			fprintf(stderr, "%s: module \"%s\" has already been imported !\n",
			        G.progname, module_name);
			goto end;
		}
		else {
			fprintf(stderr, "%s: cannot add module \"%s\" in the module tree (retcode = %d)!\n",
				   G.progname, module_name, _ret);
			exit(RPN_EXIT_INTERNAL_ERR);
		}
	}

	if (_module->ops_array_name != 0) {
		/* Existing V1 operator definitions
		  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		printf("ops_array_name = %s\n", _module->ops_array_name);

		_ops_array	= dlsym(_handle, _module->ops_array_name);
		if (!_ops_array) {
		   /* No such symbol
			 ~~~~~~~~~~~~~~ */
		   fprintf(stderr, "Module \"%s\" : unknown variable (\"%s\")\n",
				 module_name, _module->ops_array_name);
		   dlclose(_handle);
		   exit(RPN_EXIT_OP_UNKNOWN);
		}

		/* Add the operators of the module into the V1 operators tree
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		for (_op = _ops_array; _op->func != 0; _op++) {
			if (G.debug_level & RPN_DBG_DYN_OP) {
				printf("Module \"%s\" : op \"%s\"\n", module_name, _op->op_name);
			}
			_dyn_op			= rpn_new_dyn_op();
			_dyn_op->node.data	= _dyn_op;
			_dyn_op->module	= _dyn_module;
			_dyn_op->operator	= _op;

			if (ci_add_node(&G.ops_tree, &_dyn_op->node, rpn_cmp_operators, 0)) {
				RPN_INTERNAL_ERROR;
			}
		}
	}

	/* Add the operators of the module into the V2 operators tree
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	dl_op_tree_add(&G.ops_tree_v2, _module);

	if (_module->init != 0) {
		/* Execute the initialization function of the module
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		if (!G.silent && (G.debug_level & RPN_DBG_VERBOSE)) {
			printf("Initializing module [%s] ...\n", _module->name);
		}

		(*_module->init)(_module);
	}

	/* Keep max module name length
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	G.module_lg			= MAX(G.module_lg, strlen(_module->name));

//	dlclose(_handle);
end:
	return _ret;
}

/* rpn_import() }}} */
