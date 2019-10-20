%{
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
 *   @(#)  [MB] do_iperf_in_yacc.y Version 1.15 du 19/10/16 - 
 */

#include "../cy/cy_rpn_header.h"
#include "../cy/cy_rpn_proto.h"
#include  "do_cpri.h"
#include  "do_epri.h"
#include	"do_epub.h"

extern struct rpn_operator          rpn_operators[];   // XXX : a supprimer

int                       do_iperf_in_lex();
void                      do_iperf_in_error(char *);
int                       do_iperf_in_input(void);

%}

%union {
     char                *s_value;
}

%token    <s_value>      DO_VAR_C_OPTS DO_VAR_S_OPTS DO_VAR_S_PORT DO_SERVER
%token    <s_value>      DO_VAR_S_TIMEOUT DO_OPT
%token    <s_value>      DO_INT_PARAM DO_STR_PARAM
%token                   DO_COMMENT DO_EQ DO_SEP

%type     <s_value>      opt opts param server

%start                   desc

%%

desc           : line
               {
                    DO_TRACE_YACC_IN("line\n");
               }
               | desc line
               {
                    DO_TRACE_YACC_IN("desc line\n");
               }
               ;

line           : var_assign
               {
                    DO_TRACE_YACC_IN("var_assign\n");
               }
               | server_desc
               {
                    DO_TRACE_YACC_IN("server_desc\n");
               }
               | comment
               {
                    DO_TRACE_YACC_IN("comment\n");
               }
               ;

var_assign     : DO_VAR_C_OPTS DO_EQ opts
               {
                    rpn_elt             **_elt_list;

                    DO_TRACE_YACC_IN("VAR_C_OPTS EQ opts\n");
                    if (do_G.list_deflt_client_opts != 0) {
                         fprintf(stderr, "%s: duplicate variable for client options !\n",
                                 G.progname);
                         exit(RPN_EXIT_DUPLICATE);
                    }

                    _elt_list                     = do_get_curr_list(FALSE, 0);
				if (do_G.debug_level & DO_DEBUG_YACC_IN) {
					rpn_dump_elt(*_elt_list, 0);
				}
                    do_G.list_deflt_client_opts   = *_elt_list;
                    *_elt_list                    = 0;
               }
               | DO_VAR_S_OPTS DO_EQ opts
               {
                    rpn_elt             **_elt_list;

                    DO_TRACE_YACC_IN("VAR_S_OPTS EQ opts\n");
                    if (do_G.list_deflt_server_opts != 0) {
                         fprintf(stderr, "%s: duplicate variable for server options !\n",
                                 G.progname);
                         exit(RPN_EXIT_DUPLICATE);
                    }

                    _elt_list                     = do_get_curr_list(FALSE, 1);
				if (do_G.debug_level & DO_DEBUG_YACC_IN) {
					rpn_dump_elt(*_elt_list, 0);
				}
                    do_G.list_deflt_server_opts   = *_elt_list;
                    *_elt_list                    = 0;
               }
               | DO_VAR_S_PORT DO_EQ opts
               {
                    rpn_elt             **_elt_list;

                    DO_TRACE_YACC_IN("VAR_S_PORT EQ opts\n");
                    if (do_G.list_deflt_server_port != 0) {
                         fprintf(stderr, "%s: duplicate variable for server port !\n",
                                 G.progname);
                         exit(RPN_EXIT_DUPLICATE);
                    }

                    _elt_list                     = do_get_curr_list(FALSE, 2);
				if (do_G.debug_level & DO_DEBUG_YACC_IN) {
					rpn_dump_elt(*_elt_list, 0);
				}
                    do_G.list_deflt_server_port   = *_elt_list;
                    *_elt_list                    = 0;
               }
               | DO_VAR_S_TIMEOUT DO_EQ DO_INT_PARAM
               {
                    DO_TRACE_YACC_IN("VAR_S_TIMEOUT EQ opts\n");
                    if (do_G.server_timeout != 0) {
                         fprintf(stderr, "%s: duplicate variable for server timeout !\n",
                                 G.progname);
                         exit(RPN_EXIT_DUPLICATE);
                    }

                    do_G.server_timeout				= rpn_new_elt(RPN_TYPE_STRING);
                    do_G.server_timeout->value.s		= strdup($3);
               }
               ;

comment        : DO_COMMENT
               {
                    DO_TRACE_YACC_IN("COMMENT\n");
               }
               ;

server_desc    : server DO_SEP opts DO_SEP opts DO_SEP opts
               {
                    do_iperf_host_desc            *_host_desc;
                    int                            _i;

                    DO_TRACE_YACC_IN("server DO_SEP opts DO_SEP opts DO_SEP opts\n");

                    _host_desc                         = do_new_iperf_host_desc();
                    _host_desc->elt_server             = rpn_new_elt(RPN_TYPE_STRING);
                    _host_desc->elt_server->value.s    = strdup($1);

                    for (_i = 0; _i <= 2; _i++) {
                         rpn_set_list_name(do_G.tmp_opts[_i], do_list_label(_i));
                    }

                    _host_desc->list_client_opts       = do_G.tmp_opts[0];
                    _host_desc->list_server_opts       = do_G.tmp_opts[1];
                    _host_desc->list_server_port       = do_G.tmp_opts[2];

				if (do_G.debug_level & DO_DEBUG_YACC_IN) {
					do_dump_host_desc(_host_desc);
				}

                    for (_i = 0; _i <= 2; _i++) {
                         do_G.tmp_opts[_i]                  = 0;
                    }

                    do_add_host_desc(_host_desc);
               }
               | server DO_SEP opts DO_SEP opts sep
               {
                    do_iperf_host_desc            *_host_desc;
                    int                            _i;

                    DO_TRACE_YACC_IN("server DO_SEP opts DO_SEP opts\n");

                    _host_desc                         = do_new_iperf_host_desc();
                    _host_desc->elt_server             = rpn_new_elt(RPN_TYPE_STRING);
                    _host_desc->elt_server->value.s    = strdup($1);

                    do_G.tmp_opts[2]                   = do_empty_list();

                    for (_i = 0; _i <= 2; _i++) {
                         rpn_set_list_name(do_G.tmp_opts[_i], do_list_label(_i));
                    }

                    _host_desc->list_client_opts       = do_G.tmp_opts[0];
                    _host_desc->list_server_opts       = do_G.tmp_opts[1];
                    _host_desc->list_server_port       = do_G.tmp_opts[2];

				if (do_G.debug_level & DO_DEBUG_YACC_IN) {
					do_dump_host_desc(_host_desc);
				}

                    for (_i = 0; _i <= 2; _i++) {
                         do_G.tmp_opts[_i]                  = 0;
                    }

                    do_add_host_desc(_host_desc);
               }
               | server DO_SEP opts sep
               {
                    do_iperf_host_desc            *_host_desc;
                    int                            _i;

                    DO_TRACE_YACC_IN("server DO_SEP opts\n");

                    _host_desc                         = do_new_iperf_host_desc();
                    _host_desc->elt_server             = rpn_new_elt(RPN_TYPE_STRING);
                    _host_desc->elt_server->value.s    = strdup($1);

                    for (_i = 1; _i <= 2; _i++) {
                         do_G.tmp_opts[_i]                  = do_empty_list();
                    }

                    for (_i = 0; _i <= 2; _i++) {
                         rpn_set_list_name(do_G.tmp_opts[_i], do_list_label(_i));
                    }

                    _host_desc->list_client_opts       = do_G.tmp_opts[0];
                    _host_desc->list_server_opts       = do_G.tmp_opts[1];
                    _host_desc->list_server_port       = do_G.tmp_opts[2];

				if (do_G.debug_level & DO_DEBUG_YACC_IN) {
					do_dump_host_desc(_host_desc);
				}

                    for (_i = 0; _i <= 2; _i++) {
                         do_G.tmp_opts[_i]                  = 0;
                    }

                    do_add_host_desc(_host_desc);
               }
               | server sep
               {
                    do_iperf_host_desc            *_host_desc;
                    int                            _i;

                    DO_TRACE_YACC_IN("server\n");

                    _host_desc                         = do_new_iperf_host_desc();
                    _host_desc->elt_server             = rpn_new_elt(RPN_TYPE_STRING);
                    _host_desc->elt_server->value.s    = strdup($1);

                    for (_i = 0; _i <= 2; _i++) {
                         do_G.tmp_opts[_i]                  = do_empty_list();
                    }

                    for (_i = 0; _i <= 2; _i++) {
                         rpn_set_list_name(do_G.tmp_opts[_i], do_list_label(_i));
                    }

                    _host_desc->list_client_opts       = do_G.tmp_opts[0];
                    _host_desc->list_server_opts       = do_G.tmp_opts[1];
                    _host_desc->list_server_port       = do_G.tmp_opts[2];

				if (do_G.debug_level & DO_DEBUG_YACC_IN) {
					do_dump_host_desc(_host_desc);
				}

                    for (_i = 0; _i <= 2; _i++) {
                         do_G.tmp_opts[_i]                  = 0;
                    }

                    do_add_host_desc(_host_desc);
               }
               ;

sep            : DO_SEP
               | /* empty */
               ;

server         : DO_SERVER
               {
                    /*
                    rpn_elt                  *_elt;

                    if (do_G.tmp_host_desc == 0) {
                         do_G.tmp_host_desc                 = do_new_iperf_host_desc();
                         _elt                               = rpn_new_elt(RPN_TYPE_STRING);
                         _elt->value.s                      = strdup($1);
                         do_G.tmp_host_desc->elt_server     = _elt;
                    }
                    else {
                         RPN_INTERNAL_ERROR;
                    }
                    */

                    DO_TRACE_YACC_IN("DO_SERVER : [%s]\n", $1);
                    $$                  = $1;
               }

opts           : opt
               {
                    DO_TRACE_YACC_IN("opt\n");
               }
               | opts opt
               {
                    DO_TRACE_YACC_IN("opts opt\n");
               }
               ;

opt            : DO_OPT param
               {
                    struct rpn_elt           *_elt, **_elt_list;
                    struct rpn_list          *_list;

                    DO_TRACE_YACC_IN("DO_OPT param : [%s] [%s]\n", $1, $2);
                    _elt_list           = do_get_curr_list(FALSE, 3);
                    _list               = (*_elt_list)->value.obj;

                    _elt                = rpn_new_elt(RPN_TYPE_STRING);
                    _elt->value.s       = $1;
                    rpn_list_push_tail(_list, _elt);

                    _elt                = rpn_new_elt(RPN_TYPE_STRING);
                    _elt->value.s       = $2;
                    rpn_list_push_tail(_list, _elt);
               }
               | /* empty */
               {
                    struct rpn_elt           *_elt, **_elt_list;
                    struct rpn_list          *_list;

                    DO_TRACE_YACC_IN("empty opt => \"\"\n");

                    _elt_list           = do_get_curr_list(TRUE, 3);
                    _list               = (*_elt_list)->value.obj;

                    _elt                = rpn_new_elt(RPN_TYPE_STRING);
                    _elt->value.s       = strdup("");
                    rpn_list_push_tail(_list, _elt);
               }
               ;

param          : DO_STR_PARAM
               {
                    DO_TRACE_YACC_IN("DO_STR_PARAM : [%s]\n", $1);
                    $$        = $1;
               }
               | DO_INT_PARAM
               {
                    DO_TRACE_YACC_IN("DO_INT_PARAM : [%s]\n", $1);
                    $$        = $1;
               }
               ;

%%

/* do_get_curr_list() {{{ */
/******************************************************************************

                         DO_GET_CURR_LIST

******************************************************************************/
rpn_elt **do_get_curr_list(int empty_field_detected, int label_idx)
{
     int                   _idx, _label_idx;
     rpn_elt             **_elt_list;
     char                  _list_name[32];

     DO_TRACE_YACC_IN("global field_no = %d %s\n",
                          do_G.field_no,
                          empty_field_detected ? "(empty field detected)" : "");

     /* Initialize list pointer according to the current field number
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (empty_field_detected) {
          if (do_G.field_no == 1) {
               _idx           = 2;      // 4th field is empty
          }
          else {
               _idx           = do_G.field_no - 3;
          }
     }
     else {
          if (do_G.field_no == 0) {
               _idx           = 0;
          }
          else if (do_G.field_no >= 2 && do_G.field_no <= 4) {
               _idx           = do_G.field_no - 2;
          }
          else {
               /* Syntax error
                  ~~~~~~~~~~~~ */
               DO_TRACE_YACC_IN("Empty 4th field detected\n");
               _idx           = 2; // Due to empty 4th field
               yyerror("invalid field number !");
               RPN_INTERNAL_ERROR;
          }
     }

     DO_TRACE_YACC_IN("_idx = %d\n", _idx);
     if (label_idx != 0) {
          _label_idx     = label_idx;
      }
      else {
          _label_idx     = _idx;
      }
     strcpy(_list_name, do_list_label(_label_idx));

     _elt_list      = &do_G.tmp_opts[_idx];
     if (*_elt_list == 0) {
          /* Allocate a new list
             ~~~~~~~~~~~~~~~~~~~ */
          *_elt_list               = rpn_new_elt(RPN_TYPE_LIST);
          (*_elt_list)->value.obj  = rpn_new_list(strdup(_list_name));
     }
     else {
          /* Reset list name
             ~~~~~~~~~~~~~~~ */
          rpn_list                 *_list;

          _list                    = (*_elt_list)->value.obj;
          free(_list->name);
          _list->name              = strdup(_list_name);
     }

     return _elt_list;
}

/* do_get_curr_list() }}} */
/* do_add_host_desc() {{{ */

/******************************************************************************

                         DO_ADD_HOST_DESC

******************************************************************************/
void do_add_host_desc(do_iperf_host_desc *host_desc)
{
     rpn_elt                       *_elt_list;
     rpn_list                      *_list;

     if (do_G.desc == 0) {
          do_G.desc                = rpn_new_elt(RPN_TYPE_LIST);
          do_G.desc->value.obj     = rpn_new_list(strdup("iperf test description"));
     }

     /* Allocate a list for the description of server parameters
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _elt_list                = rpn_new_elt(RPN_TYPE_LIST);
     _list                    = rpn_new_list(strdup("Host parameters"));
     _elt_list->value.obj     = _list;

     rpn_list_push_tail(_list, host_desc->elt_server);
     rpn_list_push_tail(_list, host_desc->list_client_opts);
     rpn_list_push_tail(_list, host_desc->list_server_opts);
     rpn_list_push_tail(_list, host_desc->list_server_port);

     rpn_list_push_tail(do_G.desc->value.obj, _elt_list);

	if (do_G.debug_level & DO_DEBUG_ELEMENTS) {
		do_dump_desc(do_G.desc);
	}
}

/* do_add_host_desc() }}} */
/* do_dump_desc() {{{ */

/******************************************************************************

                         DO_DUMP_DESC

******************************************************************************/
void do_dump_desc(rpn_elt *elt)
{
     rpn_dump_elt(elt, 1);
}

/* do_dump_desc() }}} */
/* do_dump_host_desc() {{{ */

/******************************************************************************

                         DO_DUMP_HOST_DESC

******************************************************************************/
void do_dump_host_desc(do_iperf_host_desc *host_desc)
{
	if (do_G.debug_level & DO_DEBUG_ELEMENTS) {
		rpn_dump_elt(host_desc->elt_server,       0);
		rpn_dump_elt(host_desc->list_client_opts, 0);
		rpn_dump_elt(host_desc->list_server_opts, 0);
		rpn_dump_elt(host_desc->list_server_port, 0);
	}
}

/* do_dump_host_desc() }}} */
/* do_empty_list() {{{ */

/******************************************************************************

                         DO_EMPTY_LIST

******************************************************************************/
rpn_elt *do_empty_list()
{
     rpn_elt                       *_elt_list, *_elt_str;

     /* Allocate a new list
        ~~~~~~~~~~~~~~~~~~~ */
     _elt_list                = rpn_new_elt(RPN_TYPE_LIST);
     _elt_list->value.obj     = rpn_new_list(strdup("empty list"));

     _elt_str                 = rpn_new_elt(RPN_TYPE_STRING);
     _elt_str->value.s        = strdup("");

     rpn_list_push_tail(_elt_list->value.obj, _elt_str);

     return _elt_list;
}

/* do_empty_list() }}} */
/* do_list_label() {{{ */

/******************************************************************************

                         DO_LIST_LABEL

******************************************************************************/
char *do_list_label(int label_idx)
{
     static char          *_labels[] = {
                              "Client options",   /* 0 */
                              "Server options",   /* 1 */
                              "Server port",      /* 2 */
                              "* partial *",      /* 3 */
                              0 };                /* 4 */
     if (label_idx > (sizeof(_labels) / sizeof(_labels[0]))) {
          RPN_INTERNAL_ERROR;
     }

     return _labels[label_idx];
}
/* do_list_label() }}} */
/* do_list_to_argp() {{{ */

/******************************************************************************

					DO_LIST_TO_ARGP

******************************************************************************/
char **do_list_to_argp(rpn_list *list)
{
	char				**_argp;
	rpn_elt			 *_elt;
	cc_uint32			  _i, _size;
	int				  _type;

	/* Verify that the list is homogeneous
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (!list->homogeneous) {
		fprintf(stderr, "%s: %s() : list is not homogeneous !\n",
		        G.progname, __func__);
		RPN_INTERNAL_ERROR;
	}

	/* Verify that the type of all element is STRING
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	_elt				= list->top_elt;
	if ((_type = rpn_get_type(_elt)) != RPN_TYPE_STRING) {
		fprintf(stderr, "%s: %s() : bad element type (%s) !\n",
		        G.progname, __func__, rpn_type_to_string(_type));
		RPN_INTERNAL_ERROR;
	}

	/* Allocate memory for _argp[]
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	_size			= (list->num_elts + 1) * sizeof(char *);
	if ((_argp = (char **) RPN_MALLOC(_size)) == 0) {
		rpn_err_msg_no_mem();
		exit(RPN_EXIT_NO_MEM);
	}

	_i				= 0;
	for (_elt = list->top_elt; _elt != 0; _elt = _elt->previous_elt) {
		_argp[_i++]		= strdup(_elt->value.s);
	}
	_argp[_i]			= 0;

	return _argp;

}

/* do_list_to_argp() }}} */
/* do_gen_matrix() {{{ */

/******************************************************************************

					DO_GEN_MATRIX

******************************************************************************/
rpn_elt *do_gen_matrix(do_host_desc *hosts_desc_list,
                       do_host_desc *deflt_params, int N)
{
	int					 _n, _p, _retcode;
	do_host_desc			*_h_client, *_h_server;
	rpn_stack				*_stack_main;
	rpn_elt				*_elt_mat, *_elt_N;
	char					**_argp_client, **_argp_server, **_argp_port;
	dl_operator			*_op;

	/* Allocate temporary stack
	   ~~~~~~~~~~~~~~~~~~~~~~~~ */
	_stack_main			= rpn_new_stack("iperf_matrix_main");

	/* Generate the matrix components
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	for (_n = 0; _n < N; _n++) {
		for (_p = 0; _p < N; _p++) {
			rpn_elt				*_elt_matrix;
			char					*_opts_server, *_opts_client, *_opts_port,
								*_opts_timeout;;
			rpn_stack				*_stack;

			_stack				= rpn_new_stack("iperf_matrix_loop");

			if (_n == _p) {
				/* Element on the diagonal
				   ~~~~~~~~~~~~~~~~~~~~~~~ */
				_elt_matrix		= rpn_new_elt(RPN_TYPE_NIL);
			}
			else {
				int				 _lg_opts_client, _lg_opts_server, _lg_opts_port,
								 _lg_server, _size_server, _size_client;
				rpn_elt			*_elt_string_server, *_elt_string_client,
								*_elt_cmd_server, *_elt_cmd_client;
				char				*_cmd_server, *_cmd_client;

				_h_client			= &hosts_desc_list[_n];
				_h_server			= &hosts_desc_list[_p];

//				printf("(%-10s, %-10s)\n", _h_client->server, _h_server->server);

				_argp_client		= do_get_client_argp(_h_client, deflt_params);
				_argp_server		= do_get_server_argp(_h_server, deflt_params);
				_argp_port		= do_get_port_argp(_h_server,   deflt_params);

				_opts_client		= do_argp_to_string(_argp_client);
				_opts_server		= do_argp_to_string(_argp_server);
				_opts_port		= do_argp_to_string(_argp_port);
				_opts_timeout		= do_get_timeout();

				_lg_opts_client	= strlen(_opts_client);
				_lg_opts_server	= strlen(_opts_server);
				_lg_opts_port		= strlen(_opts_port);

				_lg_server		= strlen(_h_server->server);

				_size_client		= 128 + _lg_server + _lg_opts_client + _lg_opts_port;
				_size_server		= 128              + _lg_opts_server + _lg_opts_port;

				if ((_cmd_server = (char *) RPN_MALLOC(_size_server)) == 0) {
					rpn_err_msg_no_mem();
					exit(RPN_EXIT_NO_MEM);
				}

				if ((_cmd_client = (char *) RPN_MALLOC(_size_client)) == 0) {
					rpn_err_msg_no_mem();
					exit(RPN_EXIT_NO_MEM);
				}

				/* Generate STRING element for client hostname
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				_elt_string_client			= rpn_new_elt(RPN_TYPE_STRING);
				_elt_string_client->value.s	= strdup(_h_client->server);

				/* Generate command for the client
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//				sprintf(_cmd_client, "iperf -c %s %s %s | tee -a /tmp/rpn_iperf.out",
//				        _h_server->server, _opts_client, _opts_port);        // XXX : tee !!! XXX
				sprintf(_cmd_client, "iperf -c %s %s %s",
				        _h_server->server, _opts_client, _opts_port);
				_elt_cmd_client			= rpn_new_elt(RPN_TYPE_STRING);
				_elt_cmd_client->value.s		= _cmd_client;

				/* Generate pair element for the client
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				rpn_push(_stack, _elt_string_client);
				rpn_push(_stack, _elt_cmd_client);

				(*do_ops.pair->func)(_stack, do_ops.pair);

				/* Generate STRING element for server hostname
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				_elt_string_server			= rpn_new_elt(RPN_TYPE_STRING);
				_elt_string_server->value.s	= strdup(_h_server->server);

				/* Generate command for the server
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//				sprintf(_cmd_server, "iperf -s %s %s > /dev/null 2>&1 & echo $!",
				sprintf(_cmd_server, "iperf -s %s %s & sleep %s; kill $!",
				        _opts_server, _opts_port, _opts_timeout);
				_elt_cmd_server			= rpn_new_elt(RPN_TYPE_STRING);
				_elt_cmd_server->value.s		= _cmd_server;


				/* Generate pair element for the server
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				rpn_push(_stack, _elt_string_server);
				rpn_push(_stack, _elt_cmd_server);

				(*do_ops.pair->func)(_stack, do_ops.pair);

				/* Generate pair for the matrix element
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				(*do_ops.pair->func)(_stack, do_ops.pair);

				_elt_matrix				= rpn_pop(_stack);
			}

			rpn_push(_stack_main, _elt_matrix);

			rpn_free_stack(_stack);
		}
	}

	_elt_N					= rpn_new_elt(RPN_TYPE_INT);
	_elt_N->value.i			= N;

	rpn_push(_stack_main, rpn_clone_elt(_elt_N));
	rpn_push(_stack_main, _elt_N);

#if 0
	/* DEBUG : stk
	   ~~~~~~~~~~~ */
     if ((_retcode = (*do_ops.stk->func)(_stack_main, do_ops.stk)) != 0) {
          rpn_err_msg_op_error(do_ops.stk->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }
#endif

	/* Generate matrix
	   ~~~~~~~~~~~~~~~ */
//printf("_stack_main->num_elts = %d\n", _stack_main->num_elts);
     if ((_op = rpn_search_op(_stack_main, RPN_OP_NAME_MATRIX)) == 0) {
		exit(RPN_EXIT_OP_NOT_FOUND);
//		_retcode							= RPN_RET_OP_NOT_FOUND;
//		goto end;
     }
     if ((_retcode = (*_op->func)(_stack_main, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

	_elt_mat					= rpn_pop(_stack_main);
	rpn_set_elt_name(_elt_mat, DO_LABEL_GLOBAL_RESULT);

	if (do_G.debug_level & DO_DEBUG_ELEMENTS) {
		rpn_dump_elt(_elt_mat, 0);
	}

	rpn_free_stack(_stack_main);

	return _elt_mat;
}


/* do_gen_matrix() }}} */
/* do_get_client_argp() {{{ */

/******************************************************************************

					DO_GET_CLIENT_ARGP

******************************************************************************/
char **do_get_client_argp(do_host_desc *host, do_host_desc *deflt_params)
{

	char					**_client_argp;

	/* Are there specific options for the host ?
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (host->list_client_opts    != 0
	&&  host->list_client_opts[0] != 0
	&& *host->list_client_opts[0] != 0) {
		_client_argp			= host->list_client_opts;
	}
	else {
		/* Are there default options ?
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		if (deflt_params->list_client_opts    != 0
		&&  deflt_params->list_client_opts[0] != 0
		&& *deflt_params->list_client_opts[0] != 0) {
			_client_argp			= deflt_params->list_client_opts;
		}
		else {
			/* No options specified for this host
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			fprintf(stderr, "%s: no options specified for client \"%s\"!\n",
			       G.progname, host->server);
			exit(RPN_EXIT_MISSING_PARAM);
		}
	}

	return _client_argp;
}

/* do_get_client_argp() }}} */
/* do_get_server_argp() {{{ */

/******************************************************************************

					DO_GET_SERVER_ARGP

******************************************************************************/
char **do_get_server_argp(do_host_desc *host, do_host_desc *deflt_params)
{

	char					**_server_argp;

	/* Are there specific options for the host ?
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (host->list_server_opts    != 0
	&&  host->list_server_opts[0] != 0
	&& *host->list_server_opts[0] != 0) {
		_server_argp			= host->list_server_opts;
	}
	else {
		/* Are there default options ?
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		if (deflt_params->list_server_opts    != 0
		&&  deflt_params->list_server_opts[0] != 0
		&& *deflt_params->list_server_opts[0] != 0) {
			_server_argp			= deflt_params->list_server_opts;
		}
		else {
			/* No options specified for this host
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			fprintf(stderr, "%s: no options specified for server \"%s\"!\n",
			       G.progname, host->server);
			exit(RPN_EXIT_MISSING_PARAM);
		}
	}

	return _server_argp;
}

/* do_get_server_argp() }}} */
/* do_get_port_argp() {{{ */

/******************************************************************************

					DO_GET_PORT_ARGP

******************************************************************************/
char **do_get_port_argp(do_host_desc *host, do_host_desc *deflt_params)
{
	char					**_port_argp;

	/* Is there a specific port for the host ?
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (host->list_server_port    != 0
	&&  host->list_server_port[0] != 0
	&& *host->list_server_port[0] != 0) {
		_port_argp			= host->list_server_port;
	}
	else {
		/* Is there a default port ?
		   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
		if (deflt_params->list_server_port    != 0
		&&  deflt_params->list_server_port[0] != 0
		&& *deflt_params->list_server_port[0] != 0) {
			_port_argp			= deflt_params->list_server_port;
		}
		else {
			/* No port specified for this server
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			fprintf(stderr, "%s: no port specified for \"%s\"!\n",
			       G.progname, host->server);
			exit(RPN_EXIT_MISSING_PARAM);
		}
	}

	return _port_argp;
}

/* do_get_port_argp() }}} */
/* do_new_host_desc() {{{ */

/******************************************************************************

					DO_NEW_HOST_DESC

******************************************************************************/
do_host_desc *do_new_host_desc()
{
	do_host_desc			*_host_desc;

	if ((_host_desc = (do_host_desc *) RPN_MALLOC(sizeof(*_host_desc)))== 0) {
		rpn_err_msg_no_mem();
		exit(RPN_EXIT_NO_MEM);
	}

	_host_desc->server				= 0;
	_host_desc->list_client_opts		= 0;
	_host_desc->list_server_opts		= 0;
	_host_desc->list_server_port		= 0;

	return _host_desc;
}

/* do_new_host_desc() }}} */
/* do_disp_argp() {{{ */

/******************************************************************************

					DO_DISP_ARGP

******************************************************************************/
void do_disp_argp(char **argp)
{
	char					*_p, **_argp, *_str;
	int					 _size = 0;

	/* Compute the size of the resulting string
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	_argp				= argp;
	for (_p = *_argp; _p != 0; _p = *(++_argp)) {
		_size				+= strlen(_p) + 1;
	}

	/* Allocate memory for the string
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if ((_str = (char *) RPN_MALLOC(_size + 1)) == 0) {
		rpn_err_msg_no_mem();
		exit(RPN_EXIT_NO_MEM);
	}
	_str[0]				= 0;

	/* Generate the string
	   ~~~~~~~~~~~~~~~~~~~ */
	_argp				= argp;
	for (_p = *_argp; _p != 0; _p = *(++_argp)) {
		strcat(_str, _p);
		strcat(_str, " ");
	}

	_str[strlen(_str) - 1]	= 0;

	printf("          STR : [%s]\n", _str);
}

/* do_disp_argp() }}} */
/* do_argp_to_string() {{{ */

/******************************************************************************

					DO_ARGP_TO_STRING

******************************************************************************/
char *do_argp_to_string(char **argp)
{
	char					*_p, **_argp, *_str;
	int					 _size = 0;

	/* Compute the size of the resulting string
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	_argp				= argp;
	for (_p = *_argp; _p != 0; _p = *(++_argp)) {
		_size				+= strlen(_p) + 1;
	}

	/* Allocate memory for the string
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if ((_str = (char *) RPN_MALLOC(_size + 1)) == 0) {
		rpn_err_msg_no_mem();
		exit(RPN_EXIT_NO_MEM);
	}
	_str[0]				= 0;

	/* Generate the string
	   ~~~~~~~~~~~~~~~~~~~ */
	_argp				= argp;
	for (_p = *_argp; _p != 0; _p = *(++_argp)) {
		strcat(_str, _p);
		strcat(_str, " ");
	}

	/* Remove trailing space
	   ~~~~~~~~~~~~~~~~~~~~~ */
	_str[strlen(_str) - 1]	= 0;

	return _str;
}

/* do_argp_to_string() }}} */
/* do_get_timeout() {{{ */

/******************************************************************************

					DO_GET_TIMEOUT

******************************************************************************/
char *do_get_timeout()
{
	char					*_timeout;

	if (do_G.server_timeout == 0) {
		_timeout			= DO_DEFLT_TIMEOUT;
	}
	else {
		_timeout			= do_G.server_timeout->value.s;
	}

	return _timeout;
}

/* do_get_timeout() }}} */
