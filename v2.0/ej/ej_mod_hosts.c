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
 *   @(#)  [MB] ej_mod_hosts.c Version 1.8 du 21/10/21 - 
 */

#include  <stdio.h>
#include  <assert.h>
#include  "../cc/cc_types.h"
#include  "../cy/cy_rpn_header.h"
#include  "../cy/cy_rpn_proto.h"
#include  "../cy/cy_rpn_epub.h"
#include  "../ci/ci_cpub.h"
#include  "../ci/ci_epub.h"
#include  "../dl/dl_cpub.h"
#include  "ej_serial.h"
#include  "ej_cpri.h"
#include  "ej_epri.h"
#include  "ej_hosts_yacc.h"

/* Methods
   ~~~~~~~ */
RPN_DEFN_METHODS(ej);
RPN_DECL_OP(ej_op_hostsfile);
RPN_DECL_OP(ej_op_diff);
RPN_DECL_OP(ej_op_disp);


// Creer fonction de duplication de l'arbre des hosts (dup ~ clone),
// + liberation (free), + affichage (disp)

/* List of types managed by the module
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static int                               ej_managed_types[] = {
     RPN_TYPE_HOSTSFILE,
     RPN_TYPE_HOSTS,
     0
};

static dl_op_desc                        ej_ops_array[];

/* Help messages {{{ */
static char                             *ej_help_hostsfile[] = {
     "Convert a filename into a hosts filename",
     0
};

static char                             *ej_help_diff[] = {
     "Display differences between two hosts files",
     0
};

static char                             *ej_help_disp[] = {
     "Display contents of an HOSTS element",
     0
};

/* Help messages }}} */
/* Module descriptor {{{ */
struct dl_module         hosts_module = {
     "hosts",
     "2.0",
     EJ_LINK_ID,
     0, //"ej_operators",
     ej_ops_array,
     ej_init
};

/* Module descriptor }}} */
/* Operator parameters descriptions {{{ */
static dl_op_params                      ej_params_hostsfile[] = {
     DL_OP_DEF1H(ej_op_hostsfile, 1, FILENAME, ej_help_hostsfile),
     DL_OP_DEF_END
};

static dl_op_params                      ej_params_diff[] = {
     DL_OP_DEF2H(ej_op_diff, 1, HOSTSFILE, HOSTSFILE, ej_help_diff),
     DL_OP_DEF2H(ej_op_diff, 1, HOSTS, HOSTS, ej_help_diff),
     DL_OP_DEF_END
};

static dl_op_params                      ej_params_disp[] = {
     DL_OP_DEF1H(ej_op_disp, 1, HOSTS, ej_help_disp),
     DL_OP_DEF_END
};

/* Operator parameters descriptions }}} */
/* Operators list {{{ */
static dl_op_desc                        ej_ops_array[] = {
     {    "hostsfile",                  ej_params_hostsfile                },
     {    "diff",                       ej_params_diff                     },
	{	"disp",					ej_params_disp					},
     {    0,                       0                                       }
};

/* Operators list }}} */

/* GROUP : HOSTS {{{ */
/* ej_init() {{{ */

/******************************************************************************

                              EJ_INIT

******************************************************************************/
RPN_DEFN_INIT(ej)

/* ej_init() }}} */
/* ej_new_hosts_tree() {{{ */

/******************************************************************************

                         EJ_NEW_HOSTS_TREE

******************************************************************************/
ej_hosts_tree *ej_new_hosts_tree(cc_uint16 dim)
{
     struct ej_hosts_tree          *_hosts_tree;

     if ((_hosts_tree = (struct ej_hosts_tree *) RPN_MALLOC(sizeof(*_hosts_tree))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     bzero(_hosts_tree, sizeof(*_hosts_tree));
	_hosts_tree->dim			= dim;
	_hosts_tree->dim_idx		= 0;

     ci_init_root(&_hosts_tree->hosts_by_IP);

     return _hosts_tree;
}

/* ej_new_hosts_tree() }}} */
/* ej_clone_host() {{{ */

/******************************************************************************

					EJ_CLONE_HOST

******************************************************************************/
ej_host *ej_clone_host(ej_host *host)
{
	ci_trek				 _trek;
	ci_node				*_node;
	ej_name				*_name, *_clone_name;
	ej_host				*_clone_host;
	int					 _i;

	_clone_host			= ej_new_host();
	_clone_host->seq_num	= host->seq_num;
	_clone_host->IP		= strdup(host->IP);
	_clone_host->IP_bytes	= host->IP_bytes;

	ci_reset(&_trek, &host->names_alphabetical, CI_T_LNR);

	for (_node = ci_get_next(&_trek); _node != 0;
	     _node = ci_get_next(&_trek)) {
		_name				= _node->data;

		_clone_name			= ej_new_name();
		_clone_name->node.data	= _clone_name;
		_clone_name->name		= strdup(_name->name);
		for (_i = 0; _i < _name->dim; _i++) {
			_clone_name->present[_i]		= _name->present[_i];
		}

		if (ci_add_node(&_clone_host->names_alphabetical, &_clone_name->node, ej_name_cmp, 0) != 0) {
			fprintf(stderr, "%s: %s(%d) ci_add_node_error !\n",
				   G.progname, __FILE__, __LINE__);
			exit(1);
		}
	}

	return _clone_host;
}

/* ej_clone_host() }}} */
/* ej_clone_hosts_tree() {{{ */

/******************************************************************************

					EJ_CLONE_HOSTS_TREE

******************************************************************************/
ej_hosts_tree *ej_clone_hosts_tree(ej_hosts_tree *hosts)
{
	ci_trek				 _trek;
	ci_node				*_node;
	ej_hosts_tree			*_clone_hosts;
	ej_host				*_host, *_clone_host;

X
	_clone_hosts			= ej_new_hosts_tree(hosts->dim);
	_clone_hosts->filename	= strdup(hosts->filename);

	ci_reset(&_trek, &hosts->hosts_by_IP, CI_T_LNR);

	for (_node = ci_get_next(&_trek); _node != 0; _node = ci_get_next(&_trek)) {
		_host				= _node->data;
		_clone_host			= ej_clone_host(_host);

		if (ci_add_node(&_clone_hosts->hosts_by_IP, &_clone_host->node, ej_host_IP_cmp, 0) != 0) {
			fprintf(stderr, "%s: %s(%d) ci_add_node_error !\n",
				   G.progname, __FILE__, __LINE__);
			exit(1);
		}
	}

	return _clone_hosts;
}

/* ej_clone_hosts_tree() }}} */
/* --- Methods {{{ */
/* ej_disp_elt() {{{ */

/******************************************************************************

                         EJ_DISP_ELT

******************************************************************************/
RPN_DEFN_DISP(ej)
{
     int                  _type;

     switch (_type = rpn_get_type(elt)) {

     case RPN_TYPE_HOSTSFILE :
          if (!(disp_flags & RPN_DISP_NO_TYPE)) {
               printf("%-*s ", RPN_DISP_SZ, "HOSTSFILE");
          }
          if (elt->value.s) {
               printf("\"%s\"", elt->value.s);
          }
          else {
               printf("<NIL>");
          }
          break;

     case RPN_TYPE_HOSTS :
          if (!(disp_flags & RPN_DISP_NO_TYPE)) {
               printf("%-*s ", RPN_DISP_SZ, "HOSTS");
          }
          if (elt->value.obj) {
			ej_hosts_tree			*_hosts;

			_hosts				= elt->value.obj;
               printf("[%s] (%4llu element%s)", _hosts->filename,
			       _hosts->hosts_by_IP.nb, _hosts->hosts_by_IP.nb > 1 ? "s" : "");
          }
          else {
               printf("<NIL>");
          }
          break;

     default :
          RPN_INTERNAL_ERROR;
          break;
     }
}

/* ej_disp_elt() }}} */
/* ej_clone_elt() {{{ */

/******************************************************************************

                         EJ_CLONE_ELT

******************************************************************************/
RPN_DEFN_CLONE(ej)
{
     switch (elt->type) {

     case RPN_TYPE_HOSTSFILE:
          clone->value.s     = strdup(elt->value.s);
          break;

     case RPN_TYPE_HOSTS:
		{
			ej_hosts_tree			*_hosts, *_clone_hosts;

			_hosts				= elt->value.obj;
			_clone_hosts			= ej_clone_hosts_tree(_hosts);

			clone->value.obj		= _clone_hosts;
		}
          break;

     default:
          RPN_INTERNAL_ERROR;
          break;
     }
}

/* ej_clone_elt() }}} */
/* ej_type_to_string() {{{ */

/******************************************************************************

                         EJ_TYPE_TO_STRING

******************************************************************************/
RPN_DEFN_TYPE_TO_STR(ej)
{
     char                     *_str_type;

     switch (type) {

     case RPN_TYPE_HOSTSFILE:
          _str_type      = "HOSTSFILE";
          break;

     case RPN_TYPE_HOSTS:
          _str_type      = "HOSTS";
          break;

     default:
          RPN_INTERNAL_ERROR;
          break;
     }

     return _str_type;
}

/* ej_type_to_string() }}} */
/* ej_free_hosts() {{{ */

/******************************************************************************

					EJ_FREE_HOSTS

******************************************************************************/
void ej_free_hosts(ej_hosts_tree *hosts)
{
	fprintf(stderr, "%s(%d) : %s() is unimplemented\n",
		   __FILE__, __LINE__, __func__);
}

/* ej_free_hosts() }}} */
/* ej_free_elt() {{{ */

/******************************************************************************

                         EJ_FREE_ELT

******************************************************************************/
RPN_DEFN_FREE(ej)
{
     switch (type) {

     case RPN_TYPE_HOSTSFILE:
          if (elt->value.s) {
               /* Free string */
               RPN_FREE(elt->value.s);
          }

          /* Free element */
          RPN_FREE(elt);
          break;

	case	RPN_TYPE_HOSTS:
		{
			if (elt->value.obj) {
				ej_hosts_tree				*_hosts;

				_hosts					= elt->value.obj;

				ej_free_hosts(_hosts);
			}

			fprintf(stderr, "%s(%d) : %s() is unimplemented for type %s !\n",
				   __FILE__, __LINE__, __func__, rpn_type_to_string(type));
		}
		break;

     default:
          RPN_INTERNAL_ERROR;
          break;
     }
}

/* ej_free_elt() }}} */
/* --- Methods }}} */
/* ej_new_host() {{{ */

/******************************************************************************

                         EJ_NEW_HOST

******************************************************************************/
//RPN_PREF_NEW(ej, host)
RPN_DEFN_NEW(ej, host)
{
     struct ej_host                *_host;

     if ((_host = (struct ej_host *) RPN_MALLOC(sizeof(*_host))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     bzero(_host, sizeof(*_host));

	_host->node.data			= _host;

     ci_init_root(&_host->names_alphabetical);

     return _host;
}

/* ej_new_host() }}} */
/* ej_new_name() {{{ */

/******************************************************************************

                         EJ_NEW_NAME

******************************************************************************/
RPN_DEFN_NEW(ej, name)
{
     struct ej_name                *_name;
	int						 _i;

     if ((_name = (struct ej_name *) RPN_MALLOC(sizeof(*_name))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     bzero(_name, sizeof(*_name));

     ci_init_node(&_name->node);
     _name->node.data              = _name;
	_name->dim				= ej_G.curr_dim;

     if ((_name->present = (bool *) RPN_MALLOC(sizeof(*_name->present) * _name->dim)) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
	for (_i = 0; _i < _name->dim; _i++) {
		_name->present[_i]				= FALSE;
	}

     return _name;
}

/* ej_new_node() }}} */
/* ej_name_cmp() {{{ */

/******************************************************************************

                         EJ_NAME_CMP

******************************************************************************/
cc_uint16 ej_name_cmp(ci_node *n1, ci_node *n2)
{
     ej_name                  *_name1, *_name2;
     char                     *_n1, *_n2;

     _name1                   = (ej_name *) n1->data;
     _name2                   = (ej_name *) n2->data;

//EJ_DUMP_NAME(_name1);
//EJ_DUMP_NAME(_name2);

     _n1                      = _name1->name;
     _n2                      = _name2->name;

     for  (;;) {
          if (*_n2 < *_n1)    return CI_CMP_LT;
          if (*_n2 > *_n1)    return CI_CMP_GT;
          if (*_n1 == 0)      return CI_CMP_EQ;
          _n1++;
          _n2++;
     }  
}

/* ej_name_cmp() }}} */
/* ej_IP_to_bytes() {{{ */

/******************************************************************************

                         EJ_IP_TO_BYTES

******************************************************************************/
void ej_IP_to_bytes(char *IP, ej_bytes_IP *bytes)
{
	char					*_ptr, *_IP;
	int					 _idx;

	_IP					= strdup(IP);
	_ptr					= strtok(_IP, ".");

	// Little endian !
	for (_idx = 3; _ptr != NULL; _idx--) {
		bytes->bytes[_idx]		= atoi(_ptr);
		_ptr					= strtok(NULL, ".");
	}

	free(_IP);

#if 0
     printf("%u.%u.%u.%u",
            bytes->bytes[3],
            bytes->bytes[2],
            bytes->bytes[1],
            bytes->bytes[0]);
#endif /* 0 */
}

/* ej_IP_to_bytes() }}} */
/* ej_host_IP_cmp() {{{ */

/******************************************************************************

                         EJ_HOST_IP_CMP

******************************************************************************/
cc_uint16 ej_host_IP_cmp(ci_node *n1, ci_node *n2)
{
     ej_host                  *_host1, *_host2;
     char                     *_IP1, *_IP2;
     ej_bytes_IP               _bytes1, _bytes2;
     int                       _i;

     _host1                   = (ej_host *) n1->data;
     _host2                   = (ej_host *) n2->data;

     _IP1                     = _host1->IP;
     _IP2                     = _host2->IP;

//fprintf(stdout, "%s <==> %s\n", _IP1, _IP2);

     ej_IP_to_bytes(_IP1, &_bytes1);
     ej_IP_to_bytes(_IP2, &_bytes2);

	// BIG ENDIAN  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//     for (_i = 0; _i < 4; _i++) {

	// LITTLE ENDIAN !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     for (_i = 3; _i >= 0; _i--) {
          if (_bytes2.bytes[_i] < _bytes1.bytes[_i])    return CI_CMP_LT;
          if (_bytes2.bytes[_i] > _bytes1.bytes[_i])    return CI_CMP_GT;
     }
     return CI_CMP_EQ;
}

/* ej_host_IP_cmp() }}} */
/* ej_new_elt_hosts() {{{ */


/******************************************************************************

                         EJ_NEW_ELT_HOSTS

******************************************************************************/
/* ej_new_elt_hosts() }}} */
/* ej_disp_name() {{{ */

/******************************************************************************

					EJ_DISP_NAME

******************************************************************************/
void ej_disp_name(ci_node *n)
{
	ej_name				*_name;
	int					 _i, _indent_sz = 8, _sz, _width = 30;
	bool					 _first = TRUE;

	_name				= n->data;

	for (_i = 0; _i < _name->dim; _i++) {
		if (_first) {
			printf("%*s", _indent_sz, "");
			if (_name->present[_i]) {
				_sz				= printf("%s", _name->name);
			}
			else {
				_sz				= printf("%*s", _width, "");
			}
		}
		else {
			printf("%*s | ", _width - _sz, "");
			if (_name->present[_i]) {
				_sz				= printf("%s", _name->name);
			}
			else {
				_sz				= printf("%*s", _width, "");
			}
		}
		_first				= FALSE;
	}

	printf("\n");
}

/* ej_disp_name() }}} */
/* ej_disp_host() {{{ */

/******************************************************************************

					EJ_DISP_HOST

******************************************************************************/
void ej_disp_host(ci_node *h)
{
	ej_host				*_host;

	_host				= h->data;

#if defined(DEBUG)
	printf("IP        : %s\n",  _host->IP);
	printf("  seq_num : %6d\n", _host->seq_num);
#else
	printf("\n%-16s", _host->IP);
#endif
	if (ej_G.curr_dim > 1) {
		printf(" :\n");
	}

	ci_traversal(&_host->names_alphabetical, ej_disp_name, CI_T_LNR);
}

/* ej_disp_host() }}} */
/* ej_disp_hosts_tree() {{{ */

/******************************************************************************

					EJ_DISP_HOSTS_TREE

******************************************************************************/
void ej_disp_hosts_tree(ej_hosts_tree *hosts)
{
	ci_traversal(&hosts->hosts_by_IP, ej_disp_host, CI_T_LNR);
}

/* ej_disp_hosts_tree() }}} */
/* ej_op_hostsfile() {{{ */

/******************************************************************************

                         EJ_OP_HOSTSFILE

******************************************************************************/
RPN_DEF_OP(ej_op_hostsfile)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_FILENAME:
// {{{
          _stk_result              = rpn_new_elt(RPN_TYPE_HOSTSFILE);
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

/* ej_op_hostsfile() }}} */
/* ej_reset_dim() {{{ */

/******************************************************************************

					EJ_RESET_DIM

******************************************************************************/
void ej_reset_dim(int dim)
{
//Z
	ej_G.curr_dim			= dim;
	ej_G.dim_idx			= 0;
}

/* ej_reset_dim() }}} */
/* ej_next_dim() {{{ */

/******************************************************************************

					EJ_NEXT_DIM

******************************************************************************/
void ej_next_dim(void)
{
//Z
	ej_G.dim_idx++;
}

/* ej_next_dim() }}} */
/* ej_get_dim() {{{ */

/******************************************************************************

					EJ_GET_DIM

******************************************************************************/
cc_uint16 ej_get_dim(void)
{
//Z
	return ej_G.dim_idx;
}

/* ej_get_dim() }}} */
/* ej_set_names_flags() {{{ */

/******************************************************************************

					EJ_SET_NAMES_FLAGS

******************************************************************************/
void ej_set_names_flags(ej_host *dst, ej_host *src, cc_uint16 dim_idx)
{
	ci_trek				 _trek;
	ci_node				*_src_node, *_dst_node;
	ej_name				*_src_name, *_merged_name, *_dst_name;

	ci_reset(&_trek, &src->names_alphabetical, CI_T_LNR);

	/* For all names in source host :
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	for (_src_node = ci_get_next(&_trek); _src_node != 0;
		_src_node = ci_get_next(&_trek)) {
		_src_name					= _src_node->data;

		/* Is the name already present into the destination tree ?
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		if ((_dst_node = ci_search(&dst->names_alphabetical, _src_node, ej_name_cmp))) {
			/* Yes => set flag for dim_idx
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			_dst_name						= _dst_node->data;
			_dst_name->present[dim_idx]		= _src_name->present[0];
			if (dst == src) {
				_dst_name->present[0]			= 0;
			}
		}
		else {
			/* No => add a multi-dimension copy in the destination tree
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			_merged_name					= ej_new_name();
			_merged_name->name				= strdup(_src_name->name);
			_merged_name->present[dim_idx]	= _src_name->present[0];

			if (ci_add_node(&dst->names_alphabetical, &_merged_name->node,
			      ej_name_cmp, 0) != 0) {
				RPN_INTERNAL_ERROR;
			 }
		}
	}
}

/* ej_set_names_flags() }}} */
/* ej_inject_host() {{{ */

/******************************************************************************

					EJ_INJECT_HOST

******************************************************************************/
void ej_inject_host(ej_hosts_tree *hosts_tree, ej_host *host)
{
	ci_node				*_node;
	ej_host				*_host;
	cc_uint16				 _dim;

	/* Insert the host into the hosts tree
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if ((_node = ci_add_node(&hosts_tree->hosts_by_IP, &host->node, ej_host_IP_cmp, 0)) != 0) {
//		fprintf(stderr, "%s(%d) : node already present in the tree (%p) %s\n",
//			  __FILE__, __LINE__, _node, host->IP);
		_host				= _node->data;
	}
	else {
//		fprintf(stderr, "%s(%d) : new host in the tree %s\n",
//			  __FILE__, __LINE__, host->IP);
		_host				= host;
	}

	_dim					= ej_get_dim();
	if (_dim > 0) {
		/* Set name flags for the host
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		ej_set_names_flags(_host, host, _dim);
	}
}
/* ej_inject_host() }}} */
/* ej_pour_hosts() {{{ */

/******************************************************************************

					EJ_POUR_HOSTS

******************************************************************************/
ej_hosts_tree *ej_pour_hosts(ej_hosts_tree *dst, ej_hosts_tree *src)
{
	ej_hosts_tree			*_dst;
	ci_trek				 _trek;
	ci_node				*_src_node;
	ej_host				*_src_host, *_clone_host;

	if (dst == 0) {
		/* Destination tree is not supplied : allocation
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		if ((_dst = RPN_MALLOC(sizeof(*_dst))) == 0) {
			rpn_err_msg_no_mem();
			exit(RPN_EXIT_NO_MEM);
		}

		bzero(_dst, sizeof(*_dst));
		_dst->filename			= strdup("VOID");
		_dst->dim				= ej_G.curr_dim;
		_dst->dim_idx			= ej_G.dim_idx;
	}
	else {
		/* Use supplied tree pointer
		   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
		_dst					= dst;
	}

	/* Pour source tree into the destination tree
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	ci_reset(&_trek, &src->hosts_by_IP, CI_T_LNR);

	for (_src_node = ci_get_next(&_trek); _src_node != 0;
	     _src_node = ci_get_next(&_trek)) {
		_src_host					= _src_node->data;

		_clone_host				= ej_clone_host(_src_host);
		ej_inject_host(_dst, _clone_host);
	}

	return _dst;
}

/* ej_pour_hosts() }}} */
/* ej_disp_mult_hosts_tree() {{{ */

/******************************************************************************

					EJ_DISP_MERGED_HOSTS_TREE

******************************************************************************/
void ej_disp_mult_hosts_tree(ej_hosts_tree *hosts)
{
	ci_trek				 _trek;
	ci_node				*_node;
	ej_host				*_host;
//	ej_name				*_name;

	ci_reset(&_trek, &hosts->hosts_by_IP, CI_T_LNR);

	for (_node = ci_get_next(&_trek); _node != 0;
	     _node = ci_get_next(&_trek)) {
		_host					= _node->data;
		printf("%s :\n", _host->IP);
	}
}

/* ej_disp_mult_hosts_tree() }}} */
/* ej_op_diff() {{{ */

/******************************************************************************

                         EJ_OP_DIFF

******************************************************************************/
RPN_DEF_OP(ej_op_diff)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result_X, *_stk_result_Y;
     int                       _X_type, _Y_type;
     char                     *_X_hostsfile, *_Y_hostsfile;
     int                       _retcode;
	ej_hosts_tree			*_X_hosts_tree, *_Y_hosts_tree, *_hosts_merge;

//Z
     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_HOSTSFILE:
// {{{
          _stk_y                   = rpn_pop(stack);
          _Y_type                  = rpn_get_type(_stk_y);

          switch (_Y_type) {

          case RPN_TYPE_HOSTSFILE:
// {{{
          _X_hostsfile             = _stk_x->value.s;
          _Y_hostsfile             = _stk_y->value.s;

//ej_G.debug_level    |= EJ_DEBUG_LEX | EJ_DEBUG_YACC;

		/* Parse hosts file Y
		   ~~~~~~~~~~~~~~~~~~ */
          ej_G.file                = _Y_hostsfile;
          assert(ej_G.tmp_host == 0);
          assert(ej_G.hosts_tree == 0);
          if ((ej_hosts_in = fopen(_Y_hostsfile, "r")) == NULL) {
               fprintf(stderr, "%s: cannot open \"%s\" ! \n", G.progname, _Y_hostsfile);
			_retcode				= RPN_RET_OPEN_ERROR;
			// TODO: free memory
			rpn_push(stack, _stk_y);
			rpn_push(stack, _stk_x);
			goto end;
          }
		ej_G.curr_dim				= 1;
		ej_G.dim_idx				= 0;
          ej_hosts_parse();

		_Y_hosts_tree				= ej_G.hosts_tree;
          ej_G.hosts_tree			= 0;
		_Y_hosts_tree->filename		= strdup(_Y_hostsfile);
          _stk_result_Y				= rpn_new_elt(RPN_TYPE_HOSTS);
          _stk_result_Y->value.obj		= _Y_hosts_tree;

		/* Parse hosts file X
		   ~~~~~~~~~~~~~~~~~~ */
          ej_G.file					= _X_hostsfile;
          assert(ej_G.tmp_host == 0);
          assert(ej_G.hosts_tree == 0);
          if ((ej_hosts_in = fopen(_X_hostsfile, "r")) == NULL) {
               fprintf(stderr, "%s: cannot open \"%s\" ! \n", G.progname, _X_hostsfile);
			// TODO: free memory
			rpn_push(stack, _stk_y);
			rpn_push(stack, _stk_x);
			goto end;
          }
		ej_G.curr_dim				= 1;
		ej_G.dim_idx				= 0;
          ej_hosts_parse();

		_X_hosts_tree				= ej_G.hosts_tree;
          ej_G.hosts_tree			= 0;
		_X_hosts_tree->filename		= strdup(_X_hostsfile);
          _stk_result_X				= rpn_new_elt(RPN_TYPE_HOSTS);
          _stk_result_X->value.obj		= _X_hosts_tree;

		/* Create merged hosts tree
		   ~~~~~~~~~~~~~~~~~~~~~~~~ */
#if defined(TESTS)
		ej_reset_dim(4);
#else
		ej_reset_dim(2);
#endif	/* TESTS */

		_hosts_merge				= ej_pour_hosts(0, _Y_hosts_tree);
		ej_next_dim();
		_hosts_merge				= ej_pour_hosts(_hosts_merge, _X_hosts_tree);
		ej_next_dim();

#if defined(TESTS)
		_hosts_merge				= ej_pour_hosts(_hosts_merge, _Y_hosts_tree);
		ej_next_dim();
		_hosts_merge				= ej_pour_hosts(_hosts_merge, _X_hosts_tree);
#endif	/* TESTS */

		/* Display merged hosts tree
		   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
//		ej_disp_mult_hosts_tree(_hosts_merge);
		ej_disp_hosts_tree(_hosts_merge);

          break;
// }}}

          default:
// {{{
               rpn_push(stack, _stk_y);
               rpn_push(stack, _stk_x);
               _retcode                 = RPN_RET_INVALID_Y_TYPE;
               goto end;
               break;
          }
          break;
// }}}
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
     rpn_push(stack, _stk_result_Y);
     rpn_push(stack, _stk_result_X);

end:
     return _retcode;
}

/* ej_op_diff() }}} */
/* ej_op_disp() {{{ */

/******************************************************************************

                         EJ_OP_DISP

******************************************************************************/
RPN_DEF_OP(ej_op_disp)
{
     rpn_elt                  *_stk_x;
     int                       _X_type;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_HOSTS:
// {{{
		{
			ej_hosts_tree			*_hosts;

			_hosts				= _stk_x->value.obj;

			ej_disp_hosts_tree(_hosts);
		}
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

#if 0
     rpn_set_lastx(stack, _stk_x);
     rpn_push(stack, _stk_result);
#else
     rpn_push(stack, _stk_x);
#endif /* 0 */

end:
     return _retcode;
}

/* ej_op_disp() }}} */
/* ej_dump_host() {{{ */

/******************************************************************************

					EJ_DUMP_HOST

******************************************************************************/
void ej_dump_host(ej_host *host, char *file, int line, const char *func)
{
     printf("%-20s (%4d) %s() : host           : %p\n",      file, line, func, host);
     printf("%-20s (%4d) %s() : host->IP       : %p [%s]\n", file, line, func, host->IP, host->IP);
     printf("%-20s (%4d) %s() : host->... data : %p\n",      file, line, func, host->node.data);
}

/* ej_dump_host() }}} */
/* ej_dump_name() {{{ */

/******************************************************************************

                         EJ_DUMP_NAME

******************************************************************************/
void ej_dump_name(ej_name *name, char *file, int line, const char *func)
{
     printf("%-20s (%4d) %s() : name              : %p\n", file, line, func, name);
     printf("%-20s (%4d) %s() : name->name        : %p [%s]\n", file, line, func, name->name, name->name);
}

/* ej_dump_name() }}} */
/* GROUP : HOSTS }}} */
