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
 *   @(#)  [MB] ej_mod_hosts.c Version 1.18 du 22/08/04 - 
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

/* Module description {{{ */
static char						*ej_module_label[] = {
	"Hosts files manipulation",
	"Manipulation de fichiers hosts",
	0
};

/* Module description }}} */
/* Help messages {{{ */
static char                             *ej_help_hostsfile[] = {
     "Convert a filename into a hosts filename",
     0
},
		                             *ej_help_diff[] = {
     "Display differences between two hosts files",
     0
},
		                             *ej_help_diff_list[] = {
     "Display differences between a list of hosts files",
     0
},

		                             *ej_help_disp[] = {
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
     ej_init,
	ej_module_label
};

/* Module descriptor }}} */
/* Operator parameters descriptions {{{ */
static dl_op_params                      ej_params_hostsfile[] = {
     DL_OP_DEF1H(ej_op_hostsfile, 1, FILENAME, ej_help_hostsfile),
     DL_OP_DEF_END
};

static dl_op_params                      ej_params_diff[] = {
     DL_OP_DEF2H(ej_op_diff, 1, HOSTSFILE, HOSTSFILE, ej_help_diff),
     DL_OP_DEF2H(ej_op_diff, 1, HOSTS, HOSTS,         ej_help_diff),
     DL_OP_DEF1H(ej_op_diff, 1, LIST,                 ej_help_diff_list),
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
/* ej_dump_hosts_tree() {{{ */

/******************************************************************************

					EJ_DUMP_HOSTS_TREE

******************************************************************************/
void ej_dump_hosts_tree(ej_hosts_tree *hosts_tree, char *file, int line, const char *func)
{
     int                            _i;

     printf("%-20s (%4d) %s() : tree               : %p\n",      file, line, func,
            hosts_tree);
     printf("%-20s (%4d) %s() : tree->filename     : %p [%s]\n", file, line, func,
            hosts_tree->filename, hosts_tree->filename);
     printf("%-20s (%4d) %s() : tree->path_width   : %d\n",      file, line, func,
            hosts_tree->path_width);
     printf("%-20s (%4d) %s() : tree->name_width   : %d\n",      file, line, func,
            hosts_tree->name_width);
     printf("%-20s (%4d) %s() : tree->dim          : %d\n",      file, line, func,
            hosts_tree->dim);
     printf("%-20s (%4d) %s() : tree->dim_idx      : %d\n",      file, line, func,
            hosts_tree->dim_idx);
     for (_i = 0; _i < hosts_tree->dim; _i++) {
          printf("%-20s (%4d) %s() : tree->filenames    : %p\n", file, line, func,
                 hosts_tree->filenames);

          if (hosts_tree->filenames) {
               printf("%-20s (%4d) %s() : tree->filenames[%d] : %p\n", file, line, func,
                      _i, hosts_tree->filenames[_i]);
          }
          
     }
     printf("\n");
}

/* ej_dump_hosts_tree() }}} */
/* ej_dump_host() {{{ */

/******************************************************************************

					EJ_DUMP_HOST

******************************************************************************/
void ej_dump_host(ej_host *host, char *file, int line, const char *func)
{
     printf("%-20s (%4d) %s() : host             : %p\n",      file, line, func, host);
     printf("%-20s (%4d) %s() : host->IP         : %p [%s]\n", file, line, func, host->IP, host->IP);
     printf("%-20s (%4d) %s() : host->node.data  : %p\n",      file, line, func, host->node.data);
     printf("%-20s (%4d) %s() : host->hosts_tree : %p\n",      file, line, func, host->hosts_tree);
}

/* ej_dump_host() }}} */
/* ej_dump_name() {{{ */

/******************************************************************************

                         EJ_DUMP_NAME

******************************************************************************/
void ej_dump_name(ej_name *name, char *file, int line, const char *func)
{
     printf("%-20s (%4d) %s() : name              : %p\n", file, line, func, name);
     printf("%-20s (%4d) %s() : name->name        : %p [%s]\n", file, line, func,
	       name->name, name->name);
     printf("%-20s (%4d) %s() : name->dim         : %p [%d]\n", file, line, func,
	       name->name, name->dim);
	printf("%-20s (%4d) %s() : name->hosts_tree  : %p [%s]\n", file, line, func,
	       name->hosts_tree, name->hosts_tree->filename);
     EJ_DUMP_HOSTS_TREE(name->hosts_tree);
}

/* ej_dump_name() }}} */
/* ej_disp_name() {{{ */

/******************************************************************************

					EJ_DISP_NAME

******************************************************************************/
void ej_disp_name(ci_node *n)
{
	ej_name				*_name;
	int					 _i, _name_width, _path_width;
	char					*_fmt_present = " %*s | [%2d] %-*s |     %-*s\n",
						*_fmt_absent  = " %*s |      %-*s | [%2d] %-*s\n";

	_name				= n->data;
	_path_width			= _name->hosts_tree->path_width;
	_name_width			= MAX(16, _name->hosts_tree->name_width);

	for (_i = 0; _i < _name->dim; _i++) {
		if (_name->present[_i]) {
			printf(_fmt_present, _name_width, _name->name, _i + 1, _path_width,
			       _name->hosts_tree->filenames[_i], _path_width, "");
		}
		else {
			printf(_fmt_absent,  _name_width, _name->name, _path_width, "",
			        _i + 1, _path_width, _name->hosts_tree->filenames[_i]);
		}
	}
	printf("\n");
}

/* ej_disp_name() }}} */
/* ej_disp_dims() {{{ */

/******************************************************************************

					EJ_DISP_DIMS

******************************************************************************/
void ej_disp_dims(char *file, int line)
{
	printf("%s(%4d) curr_dim = %d dim_idx = %d\n", file, line,
	       ej_G.curr_dim, ej_G.dim_idx);
}

/* ej_disp_dims() }}} */
/* ej_disp_host() {{{ */

/******************************************************************************

					EJ_DISP_HOST

******************************************************************************/
void ej_disp_host(ci_node *h)
{
	ej_host				*_host;
	int					 _path_width, _num_width = 5, _sz;

	_host				= h->data;
	_path_width			= _host->hosts_tree->path_width;
	_sz					= MAX(16, _host->hosts_tree->name_width);

	printf("\n%-*s", _sz, _host->IP);
	printf("  : %-*s : %-*s\n",
	       _num_width + _path_width, "PRESENT",
	       _num_width + _path_width, "ABSENT");

	ci_traversal(&_host->names_alphabetical, ej_disp_name, CI_T_LNR);
}

/* ej_disp_host() }}} */
/* ej_disp_hosts_tree() {{{ */

/******************************************************************************

					EJ_DISP_HOSTS_TREE

******************************************************************************/
void ej_disp_hosts_tree(ej_hosts_tree *hosts)
{
	int					 _i;

	ci_traversal(&hosts->hosts_by_IP, ej_disp_host, CI_T_LNR);
}

/* ej_disp_hosts_tree() }}} */
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
//	_hosts_tree->path_width		= 0;
//	_hosts_tree->name_width		= 0;
	_hosts_tree->dim			= dim;
//	_hosts_tree->dim_idx		= 0;

     ci_init_root(&_hosts_tree->hosts_by_IP);

     return _hosts_tree;
}

/* ej_new_hosts_tree() }}} */
/* ej_new_host() {{{ */

/******************************************************************************

                         EJ_NEW_HOST

******************************************************************************/
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
/* ej_move_names() {{{ */

/******************************************************************************

                         EJ_MOVE_NAMES

******************************************************************************/
void ej_move_names(ej_host *dst_host, ej_host *src_host)
{
	ci_trek				 _trek;
	ci_node				*_node;
	ej_name				*_name;

	ci_reset(&_trek, &src_host->names_alphabetical, CI_T_LNR);

	for (_node = ci_get_next(&_trek); _node != 0;
	     _node = ci_get_next(&_trek)) {
		_name				= _node->data;

		if (ci_add_node(&dst_host->names_alphabetical, &_name->node, ej_name_cmp, 0) != 0) {
			fprintf(stderr, "%s: %s(%d) ci_add_node_error !\n",
				   G.progname, __FILE__, __LINE__);
			fprintf(stderr, "%s: [%s] duplicate name \"%s\" for %s\n",
			        G.progname, ej_G.file, _name->name, dst_host->IP);

			/* TODO : purge name element */
		}
	}
}

/* ej_move_names() }}} */
/* ej_clone_host() {{{ */

/******************************************************************************

					EJ_CLONE_HOST

******************************************************************************/
ej_host *ej_clone_host(ej_host *host, ej_hosts_tree *hosts_tree)
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
	_clone_host->hosts_tree	= hosts_tree;

	ci_reset(&_trek, &host->names_alphabetical, CI_T_LNR);

	for (_node = ci_get_next(&_trek); _node != 0;
	     _node = ci_get_next(&_trek)) {
		_name				= _node->data;

		_clone_name			= ej_new_name();
		_clone_name->node.data	= _clone_name;
		_clone_name->name		= strdup(_name->name);
		if (hosts_tree == 0) {
			_clone_name->hosts_tree	= _name->hosts_tree;
		}
		else {
			_clone_name->hosts_tree	= hosts_tree;
		}
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

	_clone_hosts			= ej_new_hosts_tree(hosts->dim);
	_clone_hosts->filename	= strdup(hosts->filename);
	_clone_hosts->filenames	= hosts->filenames;
	_clone_hosts->path_width	= hosts->path_width;
	_clone_hosts->name_width	= hosts->name_width;
	_clone_hosts->dim   	= hosts->dim;

	ci_reset(&_trek, &hosts->hosts_by_IP, CI_T_LNR);

	for (_node = ci_get_next(&_trek); _node != 0; _node = ci_get_next(&_trek)) {
		_host				= _node->data;
		_clone_host			= ej_clone_host(_host, _clone_hosts);

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
/* ej_free_name() {{{ */

/******************************************************************************

					EJ_FREE_NAME

******************************************************************************/
void ej_free_name(ej_name *name)
{
//fprintf(stderr, "%s(%s)\n", __func__, name->name);
	RPN_FREE(name->name);
	RPN_FREE(name);
}

/* ej_free_name() }}} */
/* ej_free_host() {{{ */

/******************************************************************************

					EJ_FREE_HOST

******************************************************************************/
void ej_free_host(ej_host *host)
{
	ci_trek				 _trek;
	ci_node				*_node;
	ej_name				*_name;

//fprintf(stdout, "%s(%s)\n", __func__, host->IP);
	ci_reset(&_trek, &host->names_alphabetical, CI_T_LRN);

	for (_node = ci_get_next(&_trek); _node != 0;
	     _node = ci_get_next(&_trek)) {
		_name					= _node->data;

		ej_free_name(_name);
	}

	RPN_FREE(host->IP);
	RPN_FREE(host);
}

/* ej_free_host() }}} */
/* ej_free_hosts() {{{ */

/******************************************************************************

					EJ_FREE_HOSTS

******************************************************************************/
void ej_free_hosts(ej_hosts_tree *hosts)
{
	ci_trek				 _trek;
	ci_node				*_node;
	ej_host				*_host;

//fprintf(stderr, "%s(%s)\n", __func__, hosts->filename);
	ci_reset(&_trek, &hosts->hosts_by_IP, CI_T_LRN);

	for (_node = ci_get_next(&_trek); _node != 0;
	     _node = ci_get_next(&_trek)) {
		_host					= _node->data;

		ej_free_host(_host);
	}

	RPN_FREE(hosts->filename);
	RPN_FREE(hosts);
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
		}
		break;

     default:
          RPN_INTERNAL_ERROR;
          break;
     }
}

/* ej_free_elt() }}} */
/* --- Methods }}} */
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
/* ej_set_dims() {{{ */

/******************************************************************************

					EJ_SET_DIMS

******************************************************************************/
void ej_set_dims(int curr_dim, int dim_idx)
{
//Z
	ej_G.curr_dim			= curr_dim;
	ej_G.dim_idx			= dim_idx;
}

/* ej_set_dims() }}} */
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
/* ej_set_names_flags() {{{ */

/******************************************************************************

					EJ_SET_NAMES_FLAGS

TODO : replace function name : ej_merge_names

******************************************************************************/
void ej_set_names_flags(ej_hosts_tree *hosts_tree, ej_host *dst, ej_host *src, cc_uint16 dim_idx)
{
	ci_trek				 _trek;
	ci_node				*_src_node, *_dst_node;
	ej_name				*_src_name, *_merged_name, *_dst_name;
	int					 _lg;

	ci_reset(&_trek, &src->names_alphabetical, CI_T_LNR);

	/* For all names in source host :
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	for (_src_node = ci_get_next(&_trek); _src_node != 0;
		_src_node = ci_get_next(&_trek)) {
		_src_name					= _src_node->data;
		_lg						= strlen(_src_name->name);

// printf("_src_name :\n");
// EJ_DUMP_NAME(_src_name);

		/* Is the name already present into the destination tree ?
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		if ((_dst_node = ci_search(&dst->names_alphabetical, _src_node, ej_name_cmp))) {
			/* Yes => set flag for dim_idx
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			_dst_name						= _dst_node->data;
// printf("_dst_name :\n");
// EJ_DUMP_NAME(_dst_name);
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
			_merged_name->hosts_tree			= hosts_tree;

// printf("_merged_name :\n");
// EJ_DUMP_NAME(_merged_name);
			if (ci_add_node(&dst->names_alphabetical, &_merged_name->node,
			      ej_name_cmp, 0) != 0) {
				RPN_INTERNAL_ERROR;
			 }
			_dst_name						= _merged_name;
		}

		if (_dst_name->hosts_tree->name_width < _lg) {
			_dst_name->hosts_tree->name_width	= _lg;
		}
// printf("Final _dst_name :\n");
// EJ_DUMP_NAME(_dst_name);
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
//EJ_DISP_DIMS;
//fprintf(stderr, "%s(%4d) _dim = %d\n", __FILE__, __LINE__,  _dim);
	if (_dim > 0) {
		/* Set name flags for the host
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		ej_set_names_flags(hosts_tree, _host, host, _dim);
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
	int					 _dim_idx, _lg;

// X
// EJ_DISP_DIMS;
	_dim_idx				= ej_G.dim_idx;

	if (dst == 0) {
		/* Destination tree is not supplied : allocation
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		_dst					= ej_new_hosts_tree(ej_G.curr_dim);
		_dst->filename			= strdup("VOID");
		_dst->dim_idx			= 0;
		_dst->path_width		= strlen(_dst->filename);

		/* Allocation for the array of filenames
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		if ((_dst->filenames = RPN_MALLOC(sizeof(sizeof(_dst->filenames[0]) * _dst->dim))) == 0) {
			rpn_err_msg_no_mem();
			exit(RPN_EXIT_NO_MEM);
		}

		/* Copy filename into the array of filenames
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		_dst->filenames[0]		= strdup(src->filename);
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

		_clone_host				= ej_clone_host(_src_host, _dst);
		ej_inject_host(_dst, _clone_host);
	}

	/* Copy filename into the array of filenames
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	_dst->filenames[_dim_idx]	= strdup(src->filename);
	_lg						= strlen(_dst->filenames[0]);
	if (_dst->path_width < _lg) {
		_dst->path_width			= _lg;
// printf("path_width = %d\n", _dst->path_width);
	}

// printf("filenames[%d] = [%s]\n", _dim_idx, _dst->filenames[_dim_idx]);

	return _dst;
}

/* ej_pour_hosts() }}} */
/* ej_parse_hostsfile() {{{ */

/******************************************************************************

					EJ_PARSE_HOSTSFILE

******************************************************************************/
int ej_parse_hostsfile(rpn_elt *elt, rpn_elt **ref_stk_result)
{
	int				 _retcode;
	char				*_hostsfile;
	ej_hosts_tree		*_hosts_tree;

// X
	_retcode			= RPN_RET_OK;
	*ref_stk_result	= NULL;

	_hostsfile		= elt->value.s;

	/* Parse hosts file
	   ~~~~~~~~~~~~~~~~ */
	ej_G.file					= _hostsfile;
	assert(ej_G.tmp_host == 0);
	assert(ej_G.hosts_tree == 0);
	if ((ej_hosts_in = fopen(_hostsfile, "r")) == NULL) {
		fprintf(stderr, "%s: cannot open \"%s\" ! \n", G.progname, _hostsfile);
		// TODO: free memory
		_retcode			= RPN_RET_OPEN_ERROR;
		goto end;
	}

	ej_set_dims(1, 0);
	ej_hosts_parse();

	_hosts_tree				= ej_G.hosts_tree;
	ej_G.hosts_tree			= 0;
	_hosts_tree->filename		= strdup(_hostsfile);
	_hosts_tree->path_width		= strlen(_hostsfile);

// X
// printf("parse_hostfile(%s) :\n", _hosts_tree->filename);
// EJ_DUMP_HOSTS_TREE(_hosts_tree);
	*ref_stk_result			= rpn_new_elt(RPN_TYPE_HOSTS);
	(*ref_stk_result)->value.obj	= _hosts_tree;

end:
	return _retcode;
}
/* ej_parse_hostsfile() }}} */
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

// Z
// X
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

// ej_G.debug_level		|= (EJ_DEBUG_LEX | EJ_DEBUG_YACC);
//		ej_G.debug_level		|= (G.debug_level & RPN_DBG_LEX)  ? EJ_DEBUG_LEX  : 0;
//		ej_G.debug_level		|= (G.debug_level & RPN_DBG_YACC) ? EJ_DEBUG_YACC : 0;

		/* Parse hosts file Y
		   ~~~~~~~~~~~~~~~~~~ */
          ej_G.file                = _Y_hostsfile;
          assert(ej_G.tmp_host == 0);
          assert(ej_G.hosts_tree == 0);
          if ((ej_hosts_in = fopen(_Y_hostsfile, "r")) == NULL) {
               fprintf(stderr, "%s: cannot open \"%s\" ! \n", G.progname,
			        _Y_hostsfile);
			_retcode				= RPN_RET_OPEN_ERROR;
			// TODO: free memory
			rpn_push(stack, _stk_y);
			rpn_push(stack, _stk_x);
			goto end;
          }
		ej_set_dims(1, 0);
          ej_hosts_parse();

		_Y_hosts_tree				= ej_G.hosts_tree;
          ej_G.hosts_tree			= 0;
		_Y_hosts_tree->filename		= strdup(_Y_hostsfile);
		_Y_hosts_tree->path_width	= strlen(_Y_hostsfile);
// X
// printf("===================> _Y_hosts_tree :\n");
// EJ_DUMP_HOSTS_TREE(_Y_hosts_tree);
          _stk_result_Y				= rpn_new_elt(RPN_TYPE_HOSTS);
          _stk_result_Y->value.obj		= _Y_hosts_tree;

		/* Parse hosts file X
		   ~~~~~~~~~~~~~~~~~~ */
          ej_G.file					= _X_hostsfile;
          assert(ej_G.tmp_host == 0);
          assert(ej_G.hosts_tree == 0);
          if ((ej_hosts_in = fopen(_X_hostsfile, "r")) == NULL) {
               fprintf(stderr, "%s: cannot open \"%s\" ! \n", G.progname,
			        _X_hostsfile);
			// TODO: free memory
			rpn_push(stack, _stk_y);
			rpn_push(stack, _stk_x);
			goto end;
          }
		ej_set_dims(1, 0);
          ej_hosts_parse();

		_X_hosts_tree				= ej_G.hosts_tree;
          ej_G.hosts_tree			= 0;
		_X_hosts_tree->filename		= strdup(_X_hostsfile);
		_X_hosts_tree->path_width	= strlen(_X_hostsfile);
// X
// printf("===================> _X_hosts_tree :\n");
// EJ_DUMP_HOSTS_TREE(_X_hosts_tree);
          _stk_result_X				= rpn_new_elt(RPN_TYPE_HOSTS);
          _stk_result_X->value.obj		= _X_hosts_tree;

		/* Create merged hosts tree
		   ~~~~~~~~~~~~~~~~~~~~~~~~ */
		ej_set_dims(2, 0);

		_hosts_merge				= ej_pour_hosts(0, _Y_hosts_tree);
// X
// printf("===================> _hosts_merge (Y) : %p ==> %p\n", _Y_hosts_tree, _hosts_merge);
// EJ_DUMP_HOSTS_TREE(_hosts_merge);
		ej_next_dim();
		_hosts_merge				= ej_pour_hosts(_hosts_merge, _X_hosts_tree);
// X
// printf("===================> _hosts_merge (Y + X) : %p ==> %p\n", _X_hosts_tree, _hosts_merge);
// EJ_DUMP_HOSTS_TREE(_hosts_merge);
		ej_next_dim();

		/* Display merged hosts tree
		   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
		ej_disp_hosts_tree(_hosts_merge);

		rpn_push(stack, _stk_result_Y);
		rpn_push(stack, _stk_result_X);
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
	case RPN_TYPE_HOSTS:
// {{{
Z
          _stk_y					= rpn_pop(stack);
          _Y_type					= rpn_get_type(_stk_y);

          _X_hosts_tree				= _stk_x->value.obj;
          _Y_hosts_tree				= _stk_y->value.obj;

		/* Create merged hosts tree
		   ~~~~~~~~~~~~~~~~~~~~~~~~ */
		ej_set_dims(2, 0);

		_hosts_merge				= ej_pour_hosts(0, _Y_hosts_tree);
		ej_next_dim();
		_hosts_merge				= ej_pour_hosts(_hosts_merge, _X_hosts_tree);
		ej_next_dim();

		_stk_result_Y				= _stk_y;
		_stk_result_X				= _stk_x;

		/* Display merged hosts tree
		   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
		ej_disp_hosts_tree(_hosts_merge);

		rpn_push(stack, _stk_result_Y);
		rpn_push(stack, _stk_result_X);
		break;
// }}}
	case	RPN_TYPE_LIST:
// {{{
//X
		{
			rpn_list					*_list;
			rpn_elt					*_elt, *_clone_elt, *_stk_result;
			int						 _nb_pushed = 0, _i, _dim, _dim_idx;

			/* Clone list
			   ~~~~~~~~~~ */
			_clone_elt				= rpn_clone_elt(_stk_x);

			/* Parse all elements of the list
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			_list					= _stk_x->value.obj;
			_dim						= _list->num_elts;
			_dim_idx					= 0;
			_hosts_merge				= NULL;
//EJ_DISP_DIMS;
//X
//ej_G.debug_level		|= (EJ_DEBUG_LEX | EJ_DEBUG_YACC);
//X
			for (_list = _stk_x->value.obj; _list->num_elts > 0; ) {
//X
				_elt						= rpn_list_pop_head(_list);

				if ((_retcode = ej_parse_hostsfile(_elt, &_stk_result)) != RPN_RET_OK) {
					/* Pop elements from the stack
					   ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
					for (_i = 0; _i < _nb_pushed; _i++) {
						_elt					= rpn_pop(stack);
						rpn_free_elt(&_elt);
					}
					rpn_free_elt(&_stk_x);
					rpn_push(stack, _clone_elt);
					goto end;
				}
				rpn_push(stack, _stk_result);
				_nb_pushed++;

				ej_set_dims(_dim, _dim_idx++);
//EJ_DISP_DIMS;
				_hosts_merge				= ej_pour_hosts(_hosts_merge,
										                _stk_result->value.obj);
				ej_next_dim();
			}

			/* Display merged hosts tree
			   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
			ej_G.curr_dim				= _dim;

			ej_disp_hosts_tree(_hosts_merge);
			rpn_push(stack, _clone_elt);
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

     rpn_set_lastx(stack, _stk_x);

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
/* GROUP : HOSTS }}} */
