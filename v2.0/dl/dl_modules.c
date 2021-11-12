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
 *   @(#)  [MB] dl_modules.c Version 1.21 du 21/11/12 - 
 */

#include  "../cc/cc_types.h"
#include  "../dr/dr_cpub.h"
#include  "../dr/dr_epub.h"
#include  "../cy/cy_rpn_header.h"
#include  "../cy/cy_rpn_proto.h"
#include  "../ci/ci_cpub.h"
#include  "../ci/ci_epub.h"
#include  "dl_cpub.h"
#include  "dl_epub.h"

static cc_uint16					 dl_lang			= 0;
static cc_uint32					 dl_num_defs		= 0;

/* dl_set_lang() {{{ */

/******************************************************************************

					DL_SET_LANG

******************************************************************************/
void dl_set_lang(cc_uint16 lang)
{
	if (lang < DL_MAX_LANG) {
		dl_lang				= lang;
	}
}

/* dl_set_lang() }}} */
/* dl_type0() {{{ */

/******************************************************************************

                         DL_TYPE0

******************************************************************************/
cc_uint64 dl_type0()
{
     dl_op_types               _u;

     _u.stk.X_type            = DL_T(UNUSED);
     _u.stk.Y_type            = DL_T(UNUSED);
     _u.stk.Z_type            = DL_T(UNUSED);
     _u.stk.T_type            = DL_T(UNUSED);

     return _u.global;
}

/* dl_type0() }}} */
/* dl_type1() {{{ */

/******************************************************************************

                         DL_TYPE1

******************************************************************************/
cc_uint64 dl_type1(cc_uint16 X_type)
{
     dl_op_types               _u;

     _u.stk.X_type            = X_type;
     _u.stk.Y_type            = DL_T(UNUSED);
     _u.stk.Z_type            = DL_T(UNUSED);
     _u.stk.T_type            = DL_T(UNUSED);

     return _u.global;
}

/* dl_type1() }}} */
/* dl_type2() {{{ */

/******************************************************************************

                         DL_TYPE2

******************************************************************************/
cc_uint64 dl_type2(cc_uint16 X_type, cc_uint16 Y_type)
{
     dl_op_types               _u;

     _u.stk.X_type            = X_type;
     _u.stk.Y_type            = Y_type;
     _u.stk.Z_type            = DL_T(UNUSED);
     _u.stk.T_type            = DL_T(UNUSED);

     return _u.global;
}

/* dl_type2() }}} */
/* dl_type3() {{{ */

/******************************************************************************

                         DL_TYPE3

******************************************************************************/
cc_uint64 dl_type3(cc_uint16 X_type, cc_uint16 Y_type, cc_uint16 Z_type)
{
     dl_op_types               _u;

     _u.stk.X_type            = X_type;
     _u.stk.Y_type            = Y_type;
     _u.stk.Z_type            = Z_type;
     _u.stk.T_type            = DL_T(UNUSED);

     return _u.global;
}

/* dl_type3() }}} */
/* dl_type4() {{{ */

/******************************************************************************

                         DL_TYPE4

******************************************************************************/
cc_uint64 dl_type4(cc_uint16 X_type, cc_uint16 Y_type, cc_uint16 Z_type, cc_uint16 T_type)
{
     dl_op_types               _u;

     _u.stk.X_type            = X_type;
     _u.stk.Y_type            = Y_type;
     _u.stk.Z_type            = Z_type;
     _u.stk.T_type            = T_type;

     return _u.global;
}

/* dl_type4() }}} */
/* dl_global_types() {{{ */

/******************************************************************************

                         DL_GLOBAL_TYPES

******************************************************************************/
cc_uint64 dl_global_types(dl_op_params *params)
{
     cc_uint64                      _global;

     switch (params->nb_args) {

     case 0:
          _global                       = dl_type0();
          break;

     case 1:
          _global                       = dl_type1(params->X_type);
          break;

     case 2:
          _global                       = dl_type2(params->X_type,
                                                   params->Y_type);
          break;

     case 3:
          _global                       = dl_type3(params->X_type,
                                                   params->Y_type,
                                                   params->Z_type);
          break;

     case 4:
          _global                       = dl_type4(params->X_type,
                                                   params->Y_type,
                                                   params->Z_type,
                                                   params->T_type);
          break;

     default:
          RPN_INTERNAL_ERROR;
          break;
     }

     return _global;
}

/* dl_global_types() }}} */
/* dl_op_tree_add() {{{ */

/******************************************************************************

                         DL_OP_TREE_ADD

******************************************************************************/
void dl_op_tree_add(struct ci_root *op_root, dl_module *module)
{
     dl_op_desc                         *_op_desc;
     dl_op_dyn_node                     *_op_dyn_node;
     dl_op_params                       *_params;
     cc_uint32                           _params_idx, _idx_root;
     dl_params_def_node                 *_params_def;
     dl_params_dyn_node                 *_dyn_params;
     ci_root                            *_root;
	dl_op_types					*_types;

	/* Add all operators into the operators tree
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     for (_op_desc = module->ops_array; _op_desc->name != 0; _op_desc++) {
//          printf("%s() : adding operator [%s] ...\n", __func__, _op_desc->name);

          /* Search for an operator with that name
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          if ((_op_dyn_node = dl_op_tree_search(op_root, _op_desc->name)) == 0) {
               /* First operator with that name :
                  create a new node
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _op_dyn_node             = dl_new_op_dyn_node();
               _op_dyn_node->name       = strdup(_op_desc->name);

               if (ci_add_node(op_root, &_op_dyn_node->node, dl_op_dyn_node_cmp, 0) != 0) {
                    RPN_INTERNAL_ERROR;
               }
          }

		/* Initialize the definition counter
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          _params_idx		              = 1;

          /* Add types descriptions to the operator
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		_types						= dl_new_op_types();
          for (_params = _op_desc->params; _params->func != 0; _params++) {
#if 0
               printf("%s(): %d args : %-6s %-6s %-6s %-6s\n",
			       __func__,
                      _params->nb_args,
                      rpn_type_to_string(_params->X_type),
                      rpn_type_to_string(_params->Y_type),
                      rpn_type_to_string(_params->Z_type),
                      rpn_type_to_string(_params->T_type));
#endif

               if (_params->nb_args > 4) {
                    RPN_INTERNAL_ERROR;
               }

			_types->global					= dl_global_types(_params);
			_root						= &_op_dyn_node->root[_params->nb_args];

               /* Search for an identical definition
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if ((_params_def = dl_get_params_def(_root, _types)) == 0) {
                    /* No definition with theses types
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _params_def                        = dl_new_params_def_node();
                    _params_def->types.global          = _types->global;
//Z
//dl_dump_params_def_node(_params_def);

                    _idx_root                          = _params->nb_args;
                    _root                              = &_op_dyn_node->root[_idx_root];

				/* Add this definition into the tree
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    if (dl_add_params_def(_root, _params_def) != 0) {
                         RPN_INTERNAL_ERROR;
                    }
//Z
//dl_dump_params_def_node(_params_def);
               }
               else {
				/* This definition already exists in the tree
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				dr_elt						*_link;
				dl_params_dyn_node				*_hiding_params;

                    printf("%s(): params_def already present !\n", __func__);

//Z
				_link					= dr_fifo_read_elt(&_params_def->fifo, 1);
//Z
				_hiding_params				= _link->data;
#if O
dl_dump_params_dyn_node(_hiding_params);
Z
#endif

                    printf("%s(): definition %d of \"%s\" in module \"%s\" will hide definition %d in module \"%s\" !\n",
				       __func__, _hiding_params->idx, _op_desc->name, _hiding_params->module->name,
					  _params_idx, module->name);
               }

               _dyn_params                        = dl_new_params_dyn_node();
			_dyn_params->func				= _params->func;
               _dyn_params->params                = _params;
               _dyn_params->idx                   = _params_idx++;
               _dyn_params->module                = module;
			_dyn_params->help				= _params->help;

#if 0
Z
dl_dump_params_dyn_node(_dyn_params);
printf("%s(): FIFO = %p\n", __func__, &_params_def->fifo);
#endif
               dr_fifo_add(&_params_def->fifo, &_dyn_params->link);
#if 0
Z
dl_dump_params_dyn_node(_dyn_params);
#endif

          }
		free(_types);

//dl_dump_ops_tree(op_root);
     }
#if 0
Z
dl_dump_ops_tree(op_root);
#endif
}

/* dl_op_tree_add() }}} */
/* dl_op_tree_search() {{{ */

/******************************************************************************

                         DL_OP_TREE_SEARCH

******************************************************************************/
dl_op_dyn_node *dl_op_tree_search(struct ci_root *op_root, char *op_name)
{
     dl_op_dyn_node                *_op_dyn_node;
     ci_node                       *_tree_node;

     _op_dyn_node                  = dl_new_op_dyn_node();
     _op_dyn_node->name            = strdup(op_name);
     _op_dyn_node->node.data       = _op_dyn_node;

     if ((_tree_node = ci_search(op_root, &_op_dyn_node->node, dl_op_dyn_node_cmp)) == 0) {
          /* Operator not found
             ~~~~~~~~~~~~~~~~~~ */
          dl_free_op_dyn_node(_op_dyn_node);
          _op_dyn_node                  = 0;
     }
     else {
          /* Operator found
             ~~~~~~~~~~~~~~ */
          _op_dyn_node                  = _tree_node->data;
     }

     return _op_dyn_node;
}

/* dl_op_tree_search() }}} */
/* dl_new_op_dyn_node() {{{ */

/******************************************************************************

                         DL_NEW_OP_DYN_NODE

******************************************************************************/
dl_op_dyn_node *dl_new_op_dyn_node()
{
     dl_op_dyn_node                *_op_dyn_node;

     if ((_op_dyn_node = RPN_MALLOC(sizeof(*_op_dyn_node))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     bzero(_op_dyn_node, sizeof(*_op_dyn_node));
     _op_dyn_node->node.data       = _op_dyn_node;

     return _op_dyn_node;
}

/* dl_new_op_dyn_node() }}} */
/* dl_free_op_dyn_node() {{{ */

/******************************************************************************

                         DL_FREE_OP_DYN_NODE

******************************************************************************/
void dl_free_op_dyn_node(dl_op_dyn_node *dyn_node)
{
     if (dyn_node->name) {
          free(dyn_node->name);
     }

     free(dyn_node);
}

/* dl_free_op_dyn_node() }}} */
/* dl_op_dyn_node_cmp() {{{ */

/******************************************************************************

                         DL_OP_DYN_NODE_CMP

******************************************************************************/
cc_uint16 dl_op_dyn_node_cmp(struct ci_node *n1, struct ci_node *n2)
{
     int                            _result;
     cc_uint16                      _retcode;
     dl_op_dyn_node                *_dyn_node1, *_dyn_node2;

     _dyn_node1                    = n1->data;
     _dyn_node2                    = n2->data;

     if ((_result = strcmp(_dyn_node2->name, _dyn_node1->name)) < 0) {
          _retcode                      = CI_CMP_LT;
     }
     else if (_result > 0) {
          _retcode                      = CI_CMP_GT;
     }
     else {
          _retcode                      = CI_CMP_EQ;
     }

     return _retcode;
}

/* dl_op_dyn_node_cmp() }}} */
/* dl_new_params_def_node() {{{ */

/******************************************************************************

                         DL_NEW_PARAMS_DEF_NODE

******************************************************************************/
dl_params_def_node *dl_new_params_def_node()
{
     dl_params_def_node            *_params_def_node;

     if ((_params_def_node = RPN_MALLOC(sizeof(*_params_def_node))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     bzero(_params_def_node, sizeof(*_params_def_node));
     _params_def_node->node.data   = _params_def_node;
     dr_init_fifo(&_params_def_node->fifo);

     return _params_def_node;
}

/* dl_new_params_def_node() }}} */
/* dl_new_params_dyn_node() {{{ */

/******************************************************************************

                         DL_NEW_PARAMS_DYN_NODE

******************************************************************************/
dl_params_dyn_node *dl_new_params_dyn_node()
{
     dl_params_dyn_node            *_params_dyn_node;

     if ((_params_dyn_node = RPN_MALLOC(sizeof(*_params_dyn_node))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     bzero(_params_dyn_node, sizeof(*_params_dyn_node));
     dr_init_elt(&_params_dyn_node->link);
     _params_dyn_node->link.data   = _params_dyn_node;

     return _params_dyn_node;
}

/* dl_new_params_dyn_node() }}} */
/* dl_params_dyn_node_cmp() {{{ */

/******************************************************************************

                         DL_PARAMS_DYN_NODE_CMP

******************************************************************************/
cc_uint16 dl_params_dyn_node_cmp(struct ci_node *n1, struct ci_node *n2)
{
     cc_uint16                      _retcode;
     dl_params_dyn_node            *_dyn_node1, *_dyn_node2;
     dl_op_types                    _types_1, _types_2;

     _dyn_node1                    = n1->data;
     _dyn_node2                    = n2->data;

     _types_1.stk.X_type           = _dyn_node1->params->X_type;
     _types_1.stk.Y_type           = _dyn_node1->params->Y_type;
     _types_1.stk.Z_type           = _dyn_node1->params->Z_type;
     _types_1.stk.T_type           = _dyn_node1->params->T_type;

     _types_2.stk.X_type           = _dyn_node2->params->X_type;
     _types_2.stk.Y_type           = _dyn_node2->params->Y_type;
     _types_2.stk.Z_type           = _dyn_node2->params->Z_type;
     _types_2.stk.T_type           = _dyn_node2->params->T_type;

     if (_types_2.global < _types_1.global) {
          _retcode                      = CI_CMP_LT;
     }
     else if (_types_2.global > _types_1.global) {
          _retcode                      = CI_CMP_GT;
     }
     else {
          _retcode                      = CI_CMP_EQ;
     }

     return _retcode;
}

/* dl_params_dyn_node_cmp() }}} */
/* dl_search_matching_op() {{{ */

/******************************************************************************

                         DL_SEARCH_MATCHING_OP

******************************************************************************/
dl_operator *dl_search_matching_op(rpn_stack *stack, char *op_name)
{
     dl_operator                   *_op = 0;
     dl_op_dyn_node                *_dyn_op;
     dl_params_def_node            *_params_def;
     dl_params_dyn_node            *_params_dyn;
	dl_op_types				*_types;
	int						 _i, _found = FALSE;

     if ((_dyn_op = dl_op_tree_search(&G.ops_tree_v2, op_name)) == 0) {
		if (G.debug_level & RPN_DBG_DEBUG) {
			printf("%s: dynamic operator \"%s\" NOT FOUND !\n",
				  G.progname, op_name);
		}
     }
     else {
		/* Operator name found in the operators tree
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		_found				= TRUE;
		_types				= dl_new_op_types();

		for (_i = 0; _i <= 4; _i++) {
			/* Search operator descriptor
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			if (_dyn_op->root[_i].nb > 0) {
				if (G.debug_level & RPN_DBG_DEBUG) {
					printf("%s() : found op \"%s\" with %d parameter%s\n",
						  __func__, op_name, _i, _i > 0 ? "s" : "");
				}
				if (_i == 0 || stack->num_elts >= _i) {
					/* Search for the dynamic operator definition
					   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
					switch (_i) {

					case	0:
						_types->global			= dl_type0();
						break;

					case	1:
						_types->global			= dl_type1(rpn_get_X_type(stack));
						break;

					case	2:
						_types->global			= dl_type2(rpn_get_X_type(stack),
											           rpn_get_Y_type(stack));
						break;

					case	3:
						_types->global			= dl_type3(rpn_get_X_type(stack),
											           rpn_get_Y_type(stack),
											           rpn_get_Z_type(stack));
						break;

					case	4:
						_types->global			= dl_type4(rpn_get_X_type(stack),
											           rpn_get_Y_type(stack),
											           rpn_get_Z_type(stack),
											           rpn_get_T_type(stack));
						break;

					}

					if (G.debug_level & RPN_DBG_DEBUG) {
Y
						printf("Stack element types :\n");
						dl_disp_types(_types);
						rpn_disp_stk(stack);
					}

					if ((_params_def = dl_get_params_def(&_dyn_op->root[_i], _types)) == 0) {
						/* Definition not wanted
						   ~~~~~~~~~~~~~~~~~~~~~ */
						if (_i == 0) {
							/* Definition with 0 specified types overrides all others
							   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
							goto end;
						}
						else {
							/* Keep on searching the matching definition
							   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
							continue;
						}
					}

					if (G.debug_level & RPN_DBG_DEBUG) {
Y
						dl_dump_params_def_node(_params_def);
					}

					/* Get the first element of the FIFO
					   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
					_params_dyn              = _params_def->fifo.head->data;

					if (G.debug_level & RPN_DBG_DEBUG) {
Y
						dl_dump_params_dyn_node(_params_dyn);
						dl_disp_params_dyn_node(_params_dyn);
					}

					_op                      = dl_new_operator();
					_op->op_name             = strdup(op_name);
					_op->module_name         = strdup(_params_dyn->module->name);
					_op->nb_args_out         = _params_dyn->params->nb_args_out;
					_op->func                = _params_dyn->func;
					goto end;
				}
				else {
Y
				}
			}
			else {
				if (G.debug_level & RPN_DBG_DEBUG) {
					printf("%s(): %s(%d) : root[%d].nb = %llu\n",
					       __func__, __FILE__, __LINE__, _i, _dyn_op->root[_i].nb);
				}
			}
		}

     }

end:

	if (G.debug_level & RPN_DBG_DEBUG) {
Y
		dl_dump_operator(_op);
	}

	if (_op == 0 && _found == TRUE) {
		_op					= (dl_operator *) 0x1;
	}

     return _op;
}

/* dl_search_matching_op() }}} */
/* dl_node_op_disp() {{{ */

/******************************************************************************

                         DL_NODE_OP_DISP

******************************************************************************/
void dl_node_op_disp(struct ci_node *node)
{
     dl_params_def_node            *_params_def;
     dl_params_dyn_node            *_params_dyn;
	dr_elt					*_link;

     _params_def                   = node->data;

//     printf("%s() : types = 0x%016llX\n", __func__, _params_def->types.global);
	printf("%s(): %p %-6s %-6s %-6s %-6s\n",
		  __func__,
		  _params_def,
		  rpn_type_to_string(_params_def->types.stk.X_type),
		  rpn_type_to_string(_params_def->types.stk.Y_type),
		  rpn_type_to_string(_params_def->types.stk.Z_type),
		  rpn_type_to_string(_params_def->types.stk.T_type));
	_link					= dr_fifo_read_elt(&_params_def->fifo, 1);
	_params_dyn				= _link->data;
	dl_dump_params_dyn_node(_params_dyn);

}

/* dl_node_op_disp() }}} */
/* dl_node_params_cmp() {{{ */

/******************************************************************************

                         DL_NODE_PARAMS_CMP

******************************************************************************/
cc_uint16 dl_node_params_cmp(struct ci_node *n1, struct ci_node *n2)
{
     dl_params_def_node            *_def_node_1, *_def_node_2;
     cc_uint16                      _retcode;
     dl_op_types                    _types_1, _types_2;
	int						 _any_detected = FALSE;

     _def_node_1                   = n1->data;
     _def_node_2                   = n2->data;

	_types_1.global			= 0;
	_types_2.global			= 0;

	if (_def_node_1->types.stk.X_type != RPN_TYPE_ANY_TYPE
	&&  _def_node_2->types.stk.X_type != RPN_TYPE_ANY_TYPE) {
		_types_1.stk.X_type           = _def_node_1->types.stk.X_type;
		_types_2.stk.X_type           = _def_node_2->types.stk.X_type;
	}
	else {
		_any_detected				= TRUE;
	}

	if (_def_node_1->types.stk.Y_type != RPN_TYPE_ANY_TYPE
	&&  _def_node_2->types.stk.Y_type != RPN_TYPE_ANY_TYPE) {
		_types_1.stk.Y_type           = _def_node_1->types.stk.Y_type;
		_types_2.stk.Y_type           = _def_node_2->types.stk.Y_type;
	}
	else {
		_any_detected				= TRUE;
	}

	if (_def_node_1->types.stk.Z_type != RPN_TYPE_ANY_TYPE
	&&  _def_node_2->types.stk.Z_type != RPN_TYPE_ANY_TYPE) {
		_types_1.stk.Z_type           = _def_node_1->types.stk.Z_type;
		_types_2.stk.Z_type           = _def_node_2->types.stk.Z_type;
	}
	else {
		_any_detected				= TRUE;
	}

	if (_def_node_1->types.stk.T_type != RPN_TYPE_ANY_TYPE
	&&  _def_node_2->types.stk.T_type != RPN_TYPE_ANY_TYPE) {
		_types_1.stk.T_type           = _def_node_1->types.stk.T_type;
		_types_2.stk.T_type           = _def_node_2->types.stk.T_type;
	}
	else {
		_any_detected				= TRUE;
	}

     if (_types_2.global < _types_1.global) {
          _retcode                      = CI_CMP_LT;
     }
     else if (_types_2.global > _types_1.global) {
          _retcode                      = CI_CMP_GT;
     }
     else {
          _retcode                      = CI_CMP_EQ;
     }

	if (G.debug_level & RPN_DBG_DEBUG) {
		if (_any_detected) {
			printf("%s() : ANY_TYPE detected\n", __func__);
		}
	}

     return _retcode;
}

/* dl_node_params_cmp() }}} */
/* dl_get_params_def() {{{ */

/******************************************************************************

                         DL_GET_PARAMS_DEF

******************************************************************************/
//dl_params_def_node *dl_get_params_def(dl_op_dyn_node *op_dyn_node, dl_op_params *params)
dl_params_def_node *dl_get_params_def(ci_root *root, dl_op_types *types)
{
//     ci_root                       *_root;
     ci_node                       *_node;
     dl_params_def_node            *_params_def, *_retval;

     _params_def                   = dl_new_params_def_node();
     _params_def->types.global     = types->global;

	if (G.debug_level & RPN_DBG_DEBUG) {
		printf("%s(): searched types = 0x%016llX\n", __func__, types->global);
		dl_disp_types(types);
	}

     if ((_node = ci_search(root, &_params_def->node, dl_node_params_cmp)) == 0) {
		if (G.debug_level & RPN_DBG_DEBUG) {
			printf("%s() : definition NOT FOUND !\n", __func__);
		}
          _retval                       = 0;
     }
     else {
          _retval                       = _node->data;
     }

     return _retval;
}

/* dl_get_params_def() }}} */
/* dl_add_params_def() {{{ */

/******************************************************************************

                         DL_ADD_PARAMS_DEF

******************************************************************************/
dl_params_def_node *dl_add_params_def(ci_root *root, dl_params_def_node *params_def)
{
     ci_node                       *_node;
     dl_params_def_node            *_retval;

//     printf("root = %p\n", root);
     if ((_node = ci_add_node(root, &params_def->node, dl_params_def_node_cmp, 0)) == 0) {
//          printf("%s(): successfully added\n", __func__);
          _retval                       = 0;
     }
     else {
//          printf("Definition already present !\n");
          _retval                       = _node->data;
     }

     return _retval;
}

/* dl_add_params_def() }}} */
/* dl_params_def_node_cmp() {{{ */

/******************************************************************************

                         DL_PARAMS_DEF_NODE_CMP

******************************************************************************/
cc_uint16 dl_params_def_node_cmp(struct ci_node *n1, struct ci_node *n2)
{
     cc_uint16                      _retcode;
     dl_params_def_node            *_def_node1, *_def_node2;

     _def_node1                    = n1->data;
     _def_node2                    = n2->data;

     if (_def_node2->types.global < _def_node1->types.global) {
          _retcode                      = CI_CMP_LT;
     }
     else if (_def_node2->types.global > _def_node1->types.global) {
          _retcode                      = CI_CMP_GT;
     }
     else {
          _retcode                      = CI_CMP_EQ;
     }

     return _retcode;
}

/* dl_params_dyn_node_cmp() }}} */
/* dl_new_operator() {{{ */

/******************************************************************************

                         DL_NEW_OPERATOR

******************************************************************************/
dl_operator *dl_new_operator()
{
     dl_operator                   *_op;

     if ((_op = RPN_MALLOC(sizeof(*_op))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     bzero(_op, sizeof(*_op));

     return _op;
}
/* dl_new_operator() }}} */
/* dl_new_op_types() {{{ */

/******************************************************************************

					DL_NEW_OP_TYPES

******************************************************************************/
dl_op_types *dl_new_op_types()
{
     dl_op_types				*_types;

     if ((_types = RPN_MALLOC(sizeof(*_types))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     bzero(_types, sizeof(*_types));

     return _types;
}

/* dl_new_op_types() }}} */
/* dl_dump_params_def_node() {{{ */

/******************************************************************************

					DL_DUMP_PARAMS_DEF_NODE

******************************************************************************/
void dl_dump_params_def_node(dl_params_def_node *params_def)
{
	printf("%s(%p) :\n", __func__, params_def);
	printf("    types = 0x%016llX\n",
	       params_def->types.global);
}

/* dl_dump_params_def_node() }}} */
/* dl_dump_params_dyn_node() {{{ */

/******************************************************************************

					DL_DUMP_PARAMS_DYN_NODE

******************************************************************************/
void dl_dump_params_dyn_node(dl_params_dyn_node *dyn_node)
{
	printf("%s(%p):    IDX = %d module = [%s] elt: previous = %p next = %p data = %p func = %p\n",
	       __func__, dyn_node,
	       dyn_node->idx, dyn_node->module->name,
		  dyn_node->link.previous, dyn_node->link.next, dyn_node->link.data, dyn_node->func);
}

/* dl_dump_params_dyn_node() }}} */
/* dl_dump_ops_tree() {{{ */

/******************************************************************************

					DL_DUMP_OPS_TREE

******************************************************************************/
void dl_dump_ops_tree(ci_root *ops_tree)
{
	ci_traversal(ops_tree, dl_dump_op_dyn_node, CI_T_LNR);
}

/* dl_dump_ops_tree() }}} */
/* dl_dump_op_dyn_node() {{{ */

/******************************************************************************

					DL_DUMP_OP_DYN_NODE

******************************************************************************/
void dl_dump_op_dyn_node(ci_node *node)
{
	int							 _i;
	dl_op_dyn_node					*_dyn_op;

	_dyn_op						= node->data;

	printf("%s(): operator = [%s]\n", __func__, _dyn_op->name);

	for (_i = 0; _i < (sizeof(_dyn_op->root) / sizeof(_dyn_op->root[0])); _i++) {
		if (_dyn_op->root[_i].nb > 0) {
			printf("%s() : nb elts(root[%d]) = %llu\n", __func__, _i, _dyn_op->root[_i].nb);
			ci_traversal(&_dyn_op->root[_i], dl_node_op_disp, CI_T_LNR);
		}
	}

	printf("\n");
}
/* dl_dump_op_dyn_node() }}} */
/* dl_dump_operator() {{{ */

/******************************************************************************

					DL_DUMP_OPERATOR

******************************************************************************/
void dl_dump_operator(dl_operator *op)
{
	if (op != 0) {
		printf("%s(): operator = [%s] module = \"%s\" nb_args_out = %d func = %p\n",
			  __func__, op->op_name, op->module_name, op->nb_args_out, op->func);
	}
	else {
		printf("%s(): operator = NIL\n", __func__);
	}
}

/* dl_dump_operator() }}} */
/* dl_disp_op_dyn_node() {{{ */

/******************************************************************************

					DL_DISP_OP_DYN_NODE

******************************************************************************/
void dl_disp_op_dyn_node(ci_node *node)
{
	int							 _i, _num = 0, _sz = 20;
	char							 _op_label[128];
	dl_op_dyn_node					*_dyn_op;

	_dyn_op						= node->data;

	for (_i = 0; _i < (sizeof(_dyn_op->root) / sizeof(_dyn_op->root[0])); _i++) {
		_num			+= _dyn_op->root[_i].nb;
	}
	sprintf(_op_label, "[%s]", _dyn_op->name);
	printf("%-*s  %3d definition%s\n", _sz, _op_label, _num, _num > 1 ? "s" : "");

	for (_i = 0; _i < (sizeof(_dyn_op->root) / sizeof(_dyn_op->root[0])); _i++) {
		if (_dyn_op->root[_i].nb > 0) {
			ci_traversal(&_dyn_op->root[_i], dl_disp_op_def, CI_T_LNR);
		}
	}

	printf("\n");
}
/* dl_disp_op_dyn_node() }}} */
/* dl_disp_op_def() {{{ */

/******************************************************************************

                         DL_DISP_OP_DEF

******************************************************************************/
void dl_disp_op_def(struct ci_node *node)
{
     dl_params_def_node            *_params_def;
     dl_params_dyn_node            *_params_dyn;
	dr_elt					*_link;
	int						 _sz = 13;

     _params_def                   = node->data;

	printf("     %-*s %-*s %-*s %-*s",
	       _sz, rpn_type_to_string(_params_def->types.stk.X_type),
		  _sz, rpn_type_to_string(_params_def->types.stk.Y_type),
		  _sz, rpn_type_to_string(_params_def->types.stk.Z_type),
		  _sz, rpn_type_to_string(_params_def->types.stk.T_type));

	_link					= dr_fifo_read_elt(&_params_def->fifo, 1);
	_params_dyn				= _link->data;
	dl_disp_params_dyn_node(_params_dyn);

}

/* dl_disp_op_def() }}} */
/* dl_disp_params_dyn_node() {{{ */

/******************************************************************************

					DL_DISP_PARAMS_DYN_NODE

******************************************************************************/
void dl_disp_params_dyn_node(dl_params_dyn_node *dyn_node)
{
	char						 _module[64], _idx[16];

	sprintf(_module, "[%s]", dyn_node->module->name);
	sprintf(_idx, "(%d)", dyn_node->idx);
	printf(" %5s %-*s", _idx, G.module_lg + 2, _module);
	if (G.debug_level & RPN_DBG_DEBUG) {
		printf(" func = %p", dyn_node->func);
	}
	if (dyn_node->help) {
		printf(" %s", dyn_node->help[dl_lang]);
	}
	else {
//		printf(" --------------------");
	}
	printf("\n");
}

/* dl_disp_params_dyn_node() }}} */
/* dl_disp_module_node() {{{ */

/******************************************************************************

					DL_DISP_MODULE_NODE

******************************************************************************/
void dl_disp_module_node(ci_node *node)
{
	rpn_module				*_module;
//	dl_module					*_module;
	char						 _module_str[64];

	_module					= node->data;
	sprintf(_module_str, "[%s]", _module->name);
	printf("%-16s %-5s / %4d\n", _module_str, _module->version, _module->link_ID);
}

/* dl_disp_module_node() }}} */
/* dl_reset_count_op_defs() {{{ */

/******************************************************************************

					DL_RESET_COUNT_OP_DEFS

******************************************************************************/
void dl_reset_count_op_defs()
{
	dl_num_defs				= 0;
}

/* dl_reset_count_op_defs() }}} */
/* dl_get_count_op_defs() {{{ */

/******************************************************************************

					DL_GET_COUNT_OP_DEFS

******************************************************************************/
cc_uint32 dl_get_count_op_defs()
{
	return dl_num_defs;
}

/* dl_get_count_op_defs() }}} */
/* dl_count_op_defs() {{{ */

/******************************************************************************

					DL_COUNT_OP_DEFS

******************************************************************************/
void dl_count_op_defs(ci_node *node)
{
	int							 _i;
	dl_op_dyn_node					*_dyn_op;

	_dyn_op						= node->data;

	for (_i = 0; _i < (sizeof(_dyn_op->root) / sizeof(_dyn_op->root[0])); _i++) {
		dl_num_defs			+= _dyn_op->root[_i].nb;
	}
}

/* dl_count_op_defs() }}} */
/* dl_disp_types() {{{ */

/******************************************************************************

					DL_DISP_TYPES

******************************************************************************/
void dl_disp_types(dl_op_types *types)
{
	int						 _sz = 13;

	printf("     %-*s %-*s %-*s %-*s\n",
		  _sz, rpn_type_to_string(types->stk.X_type),
		  _sz, rpn_type_to_string(types->stk.Y_type),
		  _sz, rpn_type_to_string(types->stk.Z_type),
		  _sz, rpn_type_to_string(types->stk.T_type));
}

/* dl_disp_types() }}} */
/* dl_exec_op() {{{ */

/******************************************************************************

					DL_EXEC_OP

******************************************************************************/
int dl_exec_op(rpn_stack *stack, char *op_name)
{
	int						 _retcode;
	dl_operator				*_op;

	if ((_op = rpn_search_op(stack, op_name)) == 0) {
		exit(RPN_EXIT_OP_NOT_FOUND);
	}

	if ((_retcode = (*_op->func)(stack, _op)) != 0) {
		rpn_err_msg_op_error(_op->op_name, _retcode);
		exit(RPN_EXIT_OP_ERROR);
	}

	if (G.debug_level & RPN_DBG_DEBUG) {
		printf("%s(): retcode of \"%s\" is %d\n",
		       __func__, op_name, _retcode);
	}

	return _retcode;
}

/* dl_exec_op() }}} */
#if defined(OPS_DEFS_OK)
/* dl_disp_op_defs() {{{ */

/******************************************************************************

					DL_DISP_OP_DEFS

******************************************************************************/
void dl_disp_op_defs(char *op_name)
{
	/* Display available definitions for a specified operator
	  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     dl_operator                   *_op = 0;
     dl_op_dyn_node                *_dyn_op;
     dl_params_def_node            *_params_def;
     dl_params_dyn_node            *_params_dyn;
	dl_op_types				*_types;
	int						 _i, _found = FALSE;

     if ((_dyn_op = dl_op_tree_search(&G.ops_tree_v2, op_name)) == 0) {
		if (G.debug_level & RPN_DBG_DEBUG) {
			printf("%s: dynamic operator \"%s\" NOT FOUND !\n",
				  G.progname, op_name);
		}
     }
     else {
		/* Operator name found in the operators tree
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		_found				= TRUE;
		_types				= dl_new_op_types();

		for (_i = 0; _i <= 4; _i++) {
			/* Search operator descriptor
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			if (_dyn_op->root[_i].nb > 0) {
				if (G.debug_level & RPN_DBG_DEBUG) {
					printf("%s() : found op \"%s\" with %d parameter%s\n",
						  __func__, op_name, _i, _i > 0 ? "s" : "");
				}
				if (_i == 0 || stack->num_elts >= _i) {
					/* Search for the dynamic operator definition
					   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
					switch (_i) {

					case	0:
						_types->global			= dl_type0();
						break;

					case	1:
						_types->global			= dl_type1(rpn_get_X_type(stack));
						break;

					case	2:
						_types->global			= dl_type2(rpn_get_X_type(stack),
											           rpn_get_Y_type(stack));
						break;

					case	3:
						_types->global			= dl_type3(rpn_get_X_type(stack),
											           rpn_get_Y_type(stack),
											           rpn_get_Z_type(stack));
						break;

					case	4:
						_types->global			= dl_type4(rpn_get_X_type(stack),
											           rpn_get_Y_type(stack),
											           rpn_get_Z_type(stack),
											           rpn_get_T_type(stack));
						break;

					}

					if (G.debug_level & RPN_DBG_DEBUG) {
Y
						printf("Stack element types :\n");
						dl_disp_types(_types);
						rpn_disp_stk(stack);
					}

					if ((_params_def = dl_get_params_def(&_dyn_op->root[_i], _types)) == 0) {
						/* Definition not wanted
						   ~~~~~~~~~~~~~~~~~~~~~ */
						if (_i == 0) {
							/* Definition with 0 specified types overrides all others
							   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
							goto end;
						}
						else {
							/* Keep on searching the matching definition
							   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
							continue;
						}
					}

					if (G.debug_level & RPN_DBG_DEBUG) {
Y
						dl_dump_params_def_node(_params_def);
					}

					/* Get the first element of the FIFO
					   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
					_params_dyn              = _params_def->fifo.head->data;

					if (G.debug_level & RPN_DBG_DEBUG) {
Y
						dl_dump_params_dyn_node(_params_dyn);
						dl_disp_params_dyn_node(_params_dyn);
					}

					_op                      = dl_new_operator();
					_op->op_name             = strdup(op_name);
					_op->module_name         = strdup(_params_dyn->module->name);
					_op->nb_args_out         = _params_dyn->params->nb_args_out;
					_op->func                = _params_dyn->func;
					goto end;
				}
				else {
Y
				}
			}
			else {
				if (G.debug_level & RPN_DBG_DEBUG) {
					printf("%s(): %s(%d) : root[%d].nb = %llu\n",
					       __func__, __FILE__, __LINE__, _i, _dyn_op->root[_i].nb);
				}
			}
		}
     }
}

/* dl_disp_op_defs() }}} */
#endif
