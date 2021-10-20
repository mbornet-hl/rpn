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
 *   @(#)  [MB] do_mod_system.c Version 1.26 du 21/08/29 - 
 */

#include  <unistd.h>
#include	<stdarg.h>			// XXX tests only ! => do_fprintf()
#include  <sys/types.h>
#include  <sys/wait.h>
#include  <math.h>
#include  "../cc/cc_types.h"
#include  "../cy/cy_rpn_header.h"
#include  "../cy/cy_rpn_proto.h"
#include	"../cy/cy_rpn_epub.h"
#include  "../dl/dl_cpub.h"
#include	"../dl/dl_epub.h"
#include	"../dn/dn_epub.h"
#include	"../dt/dt_epub.h"
#include  "do_serial.h"
#include  "do_cpri.h"
#include  "do_cpub.h"
#include  "do_epri.h"
#include	"do_epub.h"

extern FILE                        *do_iperf_out_in;
extern FILE                        *do_iperf_in_in;

int                                 do_iperf_in_lex();
int                                 do_iperf_in_parse();
int                                 do_iperf_out_parse();

struct do_operators				 do_ops = { 0 };

/* Help messages {{{ */
char							*do_help_iperf[] = {
	"Run iperf",
	0
},
							*do_help_ping[] = {
	"Run ping",
	0
},
							*do_help_better_units[] = {
	"Transforms bps into kbps, Mbps, Gbps",
	0
};

/* Help messages }}} */
/* Module descriptor {{{ */
struct dl_module         system_module = {
     "system",
     "2.0",
     DO_LINK_ID,
     0,
	do_ops_array,
	do_init,
};
/* Module descriptor }}} */
/* Operator parameters descriptions {{{ */
static dl_op_params					 do_params_memory[] = {
	DL_OP_DEF0(do_op_sys_memory, 0),
	DL_OP_DEF_END
};

static dl_op_params					 do_params_pause[] = {
	DL_OP_DEF0(do_op_sys_pause, 0),
	DL_OP_DEF_END
};

static dl_op_params					 do_params_ping[] = {
	DL_OP_DEF1(do_op_sys_ping, 1, MATRIX),
	DL_OP_DEF_END
};

static dl_op_params					 do_params_iperf[] = {
	DL_OP_DEF1(do_op_sys_iperf, 1, INT),
	DL_OP_DEF1(do_op_sys_iperf, 1, FILENAME),
	DL_OP_DEF1(do_op_sys_iperf, 1, MATRIX),
	DL_OP_DEF1(do_op_sys_iperf, 1, OPAIR),
	DL_OP_DEF_END
};

static dl_op_params					 do_params_better_units[] = {
	DL_OP_DEF1H(do_op_sys_better_units, 1, MATRIX, do_help_better_units),
	DL_OP_DEF_END
};

/* Operator parameters descriptions }}} */
/* Operators list {{{ */
static dl_op_desc					 do_ops_array[] = {
	{	"mem",				do_params_memory					},
	{	"pause",				do_params_pause					},
	{	"ping",				do_params_ping						},
	{	"iperf",				do_params_iperf					},
	{	"better_units",		do_params_better_units				},
	{	0,					0								}
};
/* Operators list }}} */

// GROUP : System {{{
/* do_new_iperf_host_desc() {{{ */
/******************************************************************************

                         DO_NEW_IPERF_HOST_DESC

******************************************************************************/
do_iperf_host_desc *do_new_iperf_host_desc()
{
     do_iperf_host_desc       *_h_desc;

     if ((_h_desc = (do_iperf_host_desc *) RPN_MALLOC(sizeof(*_h_desc))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _h_desc->elt_server        = 0;
     _h_desc->list_client_opts  = 0;
     _h_desc->list_server_opts  = 0;
     _h_desc->list_server_port  = 0;

     return _h_desc;
}

/* do_new_iperf_host_desc() }}} */
/* do_free_iperf_desc() {{{ */

/******************************************************************************

                         DO_FREE_IPERF_DESC

******************************************************************************/

/* do_free_iperf_desc() }}} */
/* do_op_sys_memory() {{{ */

/******************************************************************************

                         DO_OP_SYS_MEMORY

******************************************************************************/
RPN_DEF_OP(do_op_sys_memory)
{
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     M;

     return _retcode;
}

/* do_op_sys_memory() }}} */
/* do_op_sys_pause() {{{ */

/******************************************************************************

                         DO_OP_SYS_PAUSE

******************************************************************************/
RPN_DEF_OP(do_op_sys_pause)
{
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     pause();

     return _retcode;
}
/* do_op_sys_pause() }}} */
/* do_op_sys_ping() {{{ */

/******************************************************************************

                         DO_OP_SYS_PING

******************************************************************************/
RPN_DEF_OP(do_op_sys_ping)
{
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     RPN_UNIMPLEMENTED;

     return _retcode;
}
/* do_op_sys_ping() }}} */
/* do_op_sys_iperf() {{{ */

/******************************************************************************

                         DO_OP_SYS_IPERF

******************************************************************************/
RPN_DEF_OP(do_op_sys_iperf)
{
     int						 _retcode, _X_type;
     rpn_elt					*_stk_x, *_stk_result;
     static dl_operator			*_op_mmm = 0, *_op_list = 0;

     _retcode                 = RPN_RET_OK;

     /* Initialize operator pointers
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (_op_mmm == 0 && (_op_mmm = rpn_search_op(stack, RPN_OP_NAME_MMM)) == 0) {
          RPN_INTERNAL_ERROR;
     }
     if (_op_list == 0 && (_op_list = rpn_search_op(stack, RPN_OP_NAME_LIST)) == 0) {
          RPN_INTERNAL_ERROR;
     }

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_NIL:
// {{{
          _stk_result              = rpn_new_elt(RPN_TYPE_NIL);
		rpn_set_elt_name(_stk_result, DO_LABEL_RESULT);
          rpn_set_lastx(stack, _stk_x);
          break;

// }}}
     case RPN_TYPE_INT:
// {{{
		/* To set the debug level
		   ~~~~~~~~~~~~~~~~~~~~~~ */
          _stk_result              = rpn_clone_elt(_stk_x);;
          rpn_set_lastx(stack, _stk_x);

		do_G.debug_level		= _stk_x->value.i;
          break;

// }}}
     case RPN_TYPE_FILENAME:
// {{{
          {
               char                     *_filename;
               rpn_list                 *_list, *_list_desc;
               int                       _type, _num, _i;
			size_t				 _size;
               struct do_host_desc      *_hosts_desc_list, *_host_desc, *_deflt_params;
               rpn_elt                  *_elt_list, *_elt_server, *_elt_opts;
			rpn_list                 *_list_opts;

               rpn_set_lastx(stack, _stk_x);

               _filename                = _stk_x->value.s;
//               printf("%s() : filename = [%s]\n", __func__, _filename);
               if ((do_iperf_in_in = fopen(_filename, "r")) == NULL) {
                    fprintf(stderr, "%s: cannot open \"%s\" !\n",
                            G.progname, _filename);
                    exit(RPN_EXIT_OPEN_ERROR);
               }

               /* Allocate an element for the configuration description
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               do_G.desc                = rpn_new_elt(RPN_TYPE_LIST);
               _list                    = rpn_new_list("iperf test configuration");
               do_G.desc->value.obj     = _list;

               /* Parse the test description file : result is stored in do_G
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               do_iperf_in_parse();
			if (do_G.debug_level & DO_DEBUG_POINTERS) {
				printf("List_default client opts : %p\n", do_G.list_deflt_client_opts);
				printf("List_default server opts : %p\n", do_G.list_deflt_server_opts);
				printf("List_default server port : %p\n", do_G.list_deflt_server_port);
			}

			/* ======================================================================================= */

			/* Store default parameters
			   ~~~~~~~~~~~~~~~~~~~~~~~~ */
               _deflt_params = do_new_host_desc();

			/* Get default client's options
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			_elt_opts                = do_G.list_deflt_client_opts;
			if (_elt_opts) {
				_type                    = rpn_get_type(_elt_opts);
				_list_opts               = rpn_get_list_addr(_elt_opts);
				_deflt_params->list_client_opts	= do_list_to_argp(_list_opts);

				if (G.debug_level & RPN_DBG_VERBOSE) {
					printf("Type(_elt_opts)   = %s\n", rpn_type_to_string(_type));
					printf("List name         = [%s]\n", rpn_get_list_name(_elt_opts));
					printf("Default client options :\n");
				}
			}

			/* Get default server's options
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			_elt_opts				= do_G.list_deflt_server_opts;
			if (_elt_opts) {
				_type                    = rpn_get_type(_elt_opts);
				_list_opts               = rpn_get_list_addr(_elt_opts);
				_deflt_params->list_server_opts	= do_list_to_argp(_list_opts);

				if (G.debug_level & RPN_DBG_VERBOSE) {
					printf("Type(_elt_opts)   = %s\n", rpn_type_to_string(_type));
					printf("List name         = [%s]\n", rpn_get_list_name(_elt_opts));
					printf("Default server options :\n");
				}
			}

			/* Get default server's port
			   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
			_elt_opts				= do_G.list_deflt_server_port;
			if (_elt_opts) {
				_type                    = rpn_get_type(_elt_opts);
				_list_opts               = rpn_get_list_addr(_elt_opts);
				_deflt_params->list_server_port	= do_list_to_argp(_list_opts);
				if (_elt_opts && (G.debug_level & RPN_DBG_VERBOSE)) {
					printf("Type(_elt_opts)   = %s\n", rpn_type_to_string(_type));
					printf("List name         = [%s]\n", rpn_get_list_name(_elt_opts));
					printf("Default server port :\n");
				}
			}

			/* ======================================================================================= */

			/* Store hosts descriptions
			   ~~~~~~~~~~~~~~~~~~~~~~~~ */
               _type                    = rpn_get_type(do_G.desc);
			if (G.debug_level & RPN_DBG_VERBOSE) {
				printf("type(do_G.desc) = %s\n", rpn_type_to_string(_type));
				printf("list name = [%s]\n", rpn_get_list_name(do_G.desc));
				printf("list num  = [%d]\n", rpn_get_list_num(do_G.desc));
			}
               _num                     = rpn_get_list_num(do_G.desc);
               _size                    = _num * sizeof(struct do_host_desc);

               if ((_hosts_desc_list = (struct do_host_desc *) RPN_MALLOC(_size)) == 0) {
                    rpn_err_msg_no_mem();
                    exit(RPN_EXIT_NO_MEM);
               }
               _list_desc               = do_G.desc->value.obj;
               _elt_list                = _list_desc->top_elt;
               for (_i = 0, _elt_list = _list_desc->top_elt; _elt_list != 0;
				_elt_list = _elt_list->previous_elt) {
                    rpn_elt                  *_elt_opts;
                    rpn_list                 *_list_params, *_list_opts;

				/* Initialize host descriptor pointer
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _host_desc               = &_hosts_desc_list[_i++];

//                    _type                    = rpn_get_type(_elt_list);
//                    printf("type(_elt_list) = %s\n", rpn_type_to_string(_type));
//                    printf("list name       = [%s]\n", rpn_get_list_name(_elt_list));

                    _list_params             = rpn_get_list_addr(_elt_list);

				/* Get host name
				   ~~~~~~~~~~~~~ */
                    _elt_server              = _list_params->top_elt;
//                    _type                    = rpn_get_type(_elt_server);
//                    printf("    type(_elt_server) = %s\n", rpn_type_to_string(_type));
//                    printf("    str(_elt_server)  = [%s]\n", _elt_server->value.s);

                    _host_desc->server       = strdup(_elt_server->value.s);

				/* Get client's options
				   ~~~~~~~~~~~~~~~~~~~~ */
                    _elt_opts                = _elt_server->previous_elt;
//                    _type                    = rpn_get_type(_elt_opts);

                    _list_opts               = rpn_get_list_addr(_elt_opts);
//                    printf("    type(_elt_opts)   = %s\n", rpn_type_to_string(_type));
//                    printf("    list name         = [%s]\n", rpn_get_list_name(_elt_opts));

				_host_desc->list_client_opts	= do_list_to_argp(_list_opts);

				/* Get server's options
				   ~~~~~~~~~~~~~~~~~~~~ */
				_elt_opts				= _elt_opts->previous_elt;
//                    _type                    = rpn_get_type(_elt_opts);

                    _list_opts               = rpn_get_list_addr(_elt_opts);
//                    printf("    type(_elt_opts)   = %s\n", rpn_type_to_string(_type));
//                    printf("    list name         = [%s]\n", rpn_get_list_name(_elt_opts));

				_host_desc->list_server_opts	= do_list_to_argp(_list_opts);

				/* Get server's port
				   ~~~~~~~~~~~~~~~~~ */
				_elt_opts				= _elt_opts->previous_elt;
//                    _type                    = rpn_get_type(_elt_opts);

                    _list_opts               = rpn_get_list_addr(_elt_opts);
//                    printf("    type(_elt_opts)   = %s\n", rpn_type_to_string(_type));
//                    printf("    list name         = [%s]\n", rpn_get_list_name(_elt_opts));

				_host_desc->list_server_port	= do_list_to_argp(_list_opts);
               }


//               _stk_result         = do_G.desc;
               _stk_result         = do_gen_matrix(_hosts_desc_list, _deflt_params, rpn_get_list_num(do_G.desc));

			rpn_push(stack, _stk_result);
//X
			if ((_retcode = (*op->func)(stack, op)) != 0) {
				rpn_err_msg_op_error(op->op_name, _retcode);
				exit(RPN_EXIT_OP_ERROR);
			}
//X

			_stk_result		= rpn_pop(stack);
//X
          }
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          {
               struct rpn_matrix        *_mat_A, *_mat_C;
               int                       _n, _p, _i, _j, _idx;
			size_t				 _size;
               struct rpn_elt           *_elt, *_elt_clone;
               struct rpn_stack         *_stack;

               rpn_set_lastx(stack, _stk_x);

               _mat_A                   = (struct rpn_matrix *) _stk_x->value.obj;

               _n                       = _mat_A->n;
               _p                       = _mat_A->p;

               _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
               _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
               _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
               _mat_C->n                = _n;
               _mat_C->p                = _p;
               _stk_result->value.obj   = _mat_C;

               _stack                   = rpn_new_stack(__func__);

               for (_i = 1; _i <= _n; _i++) {
                    for (_j = 1; _j <= _p; _j++) {
                         _elt                = dt_mat_get_elt_ref(_mat_A, _i, _j);
                         _elt_clone          = rpn_clone_elt(_elt);
                         rpn_push(_stack, _elt_clone);

                         if ((_retcode = (*op->func)(_stack, op)) != 0) {
                              rpn_err_msg_op_error(op->op_name, _retcode);
                              exit(RPN_EXIT_OP_ERROR);
                         }
//X

                         _elt                = rpn_pop(_stack);
                         _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
                         _mat_C->base[_idx]  = (void *) _elt;
//rpn_dump_elt(_elt, 1);
                    }
               }

               if ((_mat_C->n == 1)
               &&  (_mat_C->p == 1)) {
                    _stk_result              = dt_mat_get_elt_ref(_mat_C, 1, 1);
               }

			rpn_set_elt_name(_stk_result, DO_LABEL_GLOBAL_RESULT);
          }
//X
          break;

// }}}
     case RPN_TYPE_OPAIR:
// {{{
          {
			/* Element should be formatted liked this :
			 * OPAIR
			 *      OPAIR
			 *         STRING client_hostname
			 *         STRING cmd_client
			 *      OPAIR
			 *         STRING server_hostname
			 *         STRING cmd_server
			 */
               rpn_elt                  *_elt_A, *_elt_B, *_elt_client, *_elt_server,
								*_elt_cmd_client, *_elt_cmd_server;
               rpn_pair                 *_pair, *_pair_A, *_pair_B;
               rpn_list                 *_result_list;
               int                       _A_type, _B_type, _A_a_type, _A_b_type, _B_a_type, _B_b_type,
                                         _pid_a, _pid_b, _status_a, _status_b;
               char                     *_argp_a[8], *_argp_b[8];
               rpn_elt                  *_elt_res_pair, *_begin;
               rpn_pair                 *_res_pair;
               int                       _pipe[2], _i, _fd_to_close[10];

               rpn_set_lastx(stack, _stk_x);

               _pair                    = _stk_x->value.obj;
               _elt_A                   = _pair->elt_a;
               _elt_B                   = _pair->elt_b;

               _A_type                  = rpn_get_type(_elt_A);
               _B_type                  = rpn_get_type(_elt_B);

               /* Host A (client)
                  ~~~~~~~~~~~~~~~ */
               switch (_A_type) {

			case	RPN_TYPE_OPAIR:
// {{{
				_pair_A                  = _elt_A->value.obj;
				_elt_client              = _pair_A->elt_a;
				_elt_cmd_client          = _pair_A->elt_b;

				_A_a_type                = rpn_get_type(_elt_client);
				_A_b_type                = rpn_get_type(_elt_cmd_client);
				if (_A_a_type != RPN_TYPE_STRING
				||  _A_b_type != RPN_TYPE_STRING) {
					RPN_INTERNAL_ERROR;
				}
				break;

// }}}
               default:
// {{{
				fprintf(stderr, "Type of _elt_A = %d (%s)\n", _A_type, rpn_type_to_string(_A_type));
                    RPN_INTERNAL_ERROR;
                    break;
// }}}
               }

               /* Host B (server)
                  ~~~~~~~~~~~~~~~ */
               switch (_B_type) {

			case	RPN_TYPE_OPAIR:
// {{{
				_pair_B                  = _elt_B->value.obj;
				_elt_server              = _pair_B->elt_a;
				_elt_cmd_server          = _pair_B->elt_b;

				_B_a_type                = rpn_get_type(_elt_server);
				_B_b_type                = rpn_get_type(_elt_cmd_client);
				if (_B_a_type != RPN_TYPE_STRING
				||  _B_b_type != RPN_TYPE_STRING) {
					RPN_INTERNAL_ERROR;
				}
				break;

// }}}
               default:
// {{{
                    RPN_INTERNAL_ERROR;
                    break;
// }}}
               }

               /* Generate arguments list for client command
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _i                            = 0;
               _argp_a[_i++]                 = "ssh";
               _argp_a[_i++]                 = _elt_client->value.s;
               _argp_a[_i++]				= _elt_cmd_client->value.s;
               _argp_a[_i++]                 = (char *) 0;

               /* Generate arguments list for server command
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _i                            = 0;
               _argp_b[_i++]                 = "ssh";
               _argp_b[_i++]                 = _elt_server->value.s;
               _argp_b[_i++]				= _elt_cmd_server->value.s;
               _argp_b[_i++]                 = (char *) 0;

               /* Display commands
                  ~~~~~~~~~~~~~~~~ */
			printf("iperf : %-10s ==> %s\n", _elt_client->value.s, _elt_server->value.s);

			if (do_G.debug_level & DO_DEBUG_CMD) {
				rpn_disp_argp((char *) __func__, _argp_a);
				rpn_disp_argp((char *) __func__, _argp_b);
			}

               /* Create a pipe to analyze the server output
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if (pipe(_pipe) == -1) {
                    fprintf(stderr, "%s: pipe error !\n", G.progname);
                    exit(RPN_EXIT_PIPE_ERROR);
               }

			_i						=  0;
			_fd_to_close[_i++]			=  0;  // Close stdin in the child
			_fd_to_close[_i++]			= -1;

               /* Run server
                  ~~~~~~~~~~ */
			if (do_G.debug_level & DO_DEBUG_DISP_CMDS) {
				do_disp_argp(_argp_b);
			}
               _pid_b                        = rpn_exec("/usr/bin/ssh", _argp_b, _pipe, _fd_to_close);
               close(_pipe[1]);

#if 0
               /* Create a pipe to analyze the client output
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if (pipe(_pipe) == -1) {
                    fprintf(stderr, "%s: pipe error !\n", G.progname);
                    exit(RPN_EXIT_PIPE_ERROR);
               }
#endif

//             usleep(500000L);
			sleep(1);

			_i						= 0;
			_fd_to_close[_i++]			= 0;        // Close stdin in the child
			_fd_to_close[_i++]			= 1;        // Close stdout in the child
			_fd_to_close[_i++]			= _pipe[0]; // Close pipe read in the child
			_fd_to_close[_i++]			= -1;

               /* Run client
                  ~~~~~~~~~~ */
			if (do_G.debug_level & DO_DEBUG_DISP_CMDS) {
				do_disp_argp(_argp_a);
			}
               _pid_a                        = rpn_exec("/usr/bin/ssh", _argp_a, NULL, _fd_to_close);

#if 0
               if (do_G.ID_list == NULL) {
                    do_G.ID_list                     = rpn_new_elt(RPN_TYPE_LIST);
                    _result_list	                  = rpn_new_list("result_ID");
                    do_G.ID_list->value.obj          = _result_list;
               }
               else {
                    RPN_INTERNAL_ERROR;
               }
#endif

               if (do_G.SUM_list == NULL) {
                    do_G.SUM_list                    = rpn_new_elt(RPN_TYPE_LIST);
                    _result_list	                  = rpn_new_list("result_SUM");
                    do_G.SUM_list->value.obj         = _result_list;
               }
               else {
                    RPN_INTERNAL_ERROR;
               }

               /* Run parser
                  ~~~~~~~~~~ */
               do_iperf_out_in			= fdopen(_pipe[0], "r");
               do_iperf_out_parse();

               /* Wait for termination of processes
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               waitpid(_pid_a, &_status_a, 0);
               waitpid(_pid_b, &_status_b, 0);

               close(_pipe[0]);

               /* Create the resulting list :
                * - start marker
                * - OPAIR(server A, server B)
                * - list(min throughput, max throughput, mean throughput)
                * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

               /* Push start marker on the stack
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_inc_begin(stack);
               _begin                   = rpn_new_elt(RPN_TYPE_START_MARKER);
               stack->new_value         = TRUE;
               rpn_push(stack, _begin);

               _elt_res_pair            = rpn_new_elt(RPN_TYPE_OPAIR);
			rpn_set_elt_name(_elt_res_pair, DO_LABEL_HOSTS);
               _res_pair                = rpn_new_pair(DO_LABEL_RESULT);
               _res_pair->elt_a         = rpn_clone_elt(_elt_client);
			rpn_set_elt_name(_res_pair->elt_a, DO_LABEL_ELT_CLIENT);

               _res_pair->elt_b         = rpn_clone_elt(_elt_server);
			rpn_set_elt_name(_res_pair->elt_b, DO_LABEL_ELT_SERVER);
               _elt_res_pair->value.obj = _res_pair;
               rpn_push(stack, _elt_res_pair);

#if 0
               /* Push the list of throughputs on the stack
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, do_G.ID_list);
               do_G.ID_list			= 0;

			/* Determine the min, max and mean of the throughputs
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if ((rpn_get_X_type(stack) == RPN_TYPE_LIST)
			&&  (rpn_get_list_num(stack->top_elt)) > 0) {
                    if ((_retcode = (*_op_mmm->func)(stack, _op_mmm)) != 0) {
                         rpn_err_msg_op_error(_op_mmm->op_name, _retcode);
                         exit(RPN_EXIT_OP_ERROR);
                    }
				rpn_set_elt_name(stack->top_elt, DO_LABEL_IPERF_ID);
//				rpn_dump_elt(stack->top_elt, 0);
				dn_set_mmm_names(stack->top_elt, DO_LABEL_MIN, DO_LABEL_MAX, DO_LABEL_MEAN);
               }
#endif

               /* Push the list of throughputs on the stack
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, do_G.SUM_list);
               do_G.SUM_list			= 0;

			/* Determine the min, max and mean of the throughputs
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if (rpn_get_X_type(stack) == RPN_TYPE_LIST) {
				if (rpn_get_list_num(stack->top_elt) > 0) {
					if ((_retcode = (*_op_mmm->func)(stack, _op_mmm)) != 0) {
						rpn_err_msg_op_error(_op_mmm->op_name, _retcode);
						exit(RPN_EXIT_OP_ERROR);
					}
					rpn_set_elt_name(stack->top_elt, DO_LABEL_IPERF_SUM);
					dn_set_mmm_names(stack->top_elt, DO_LABEL_MIN, DO_LABEL_MAX, DO_LABEL_MEAN);
				}
				else {
					rpn_set_elt_name(stack->top_elt, DO_LABEL_NO_DATA);
				}
               }

               /* Create resulting list
                  ~~~~~~~~~~~~~~~~~~~~~ */
               if ((_retcode = (*_op_list->func)(stack, _op_list)) != 0) {
                    rpn_err_msg_op_error(_op_list->op_name, _retcode);
                    exit(RPN_EXIT_OP_ERROR);
               }

               _stk_result                   = rpn_pop(stack);
			rpn_set_elt_name(_stk_result, DO_LABEL_RESULT);

               sleep(2);
          }
          break;

// }}}
     default:
// {{{
          fprintf(stderr, "%s: %s() : invalid element type !\n",
                  G.progname, __func__);
          RPN_INTERNAL_ERROR;
          break;
// }}}
     }

     rpn_free_elt(&_stk_x);

     rpn_push(stack, _stk_result);

     return _retcode;
}
/* do_op_sys_iperf() }}} */
/* do_init() {{{ */

/******************************************************************************

					DO_INIT

******************************************************************************/
int do_init(dl_module *module)
{
	int					 _retcode = RPN_RET_OK;

	/* Initialize pointers to operators that are independant of the
	 * element types
	 */
	do_ops.list			= dl_search_matching_op(G.stack, "list");
	do_ops.pair			= dl_search_matching_op(G.stack, "pair");
	do_ops.prx			= dl_search_matching_op(G.stack, "prx");
	do_ops.stk			= dl_search_matching_op(G.stack, "stk");
//	_retcode				= RPN_RET_OP_NOT_FOUND;

	if (G.debug_level & RPN_DBG_LEX) {
		do_G.debug_level		|= DO_DEBUG_LEX_IN;
		DO_TRACE_LEX_IN("%s(): trace lex in   : activated\n", __func__);
	}
	if (G.debug_level & RPN_DBG_LEX_2) {
		do_G.debug_level		|= DO_DEBUG_LEX_OUT;
		DO_TRACE_LEX_OUT("%s(): trace lex out  : activated\n", __func__);
	}
	if (G.debug_level & RPN_DBG_YACC) {
		do_G.debug_level		|= DO_DEBUG_YACC_IN;
		DO_TRACE_YACC_IN("%s(): trace yacc in  : activated\n", __func__);
	}
	if (G.debug_level & RPN_DBG_YACC_2) {
		do_G.debug_level		|= DO_DEBUG_YACC_OUT;
		DO_TRACE_YACC_OUT("%s(): trace yacc out : activated\n", __func__);
	}
	if (G.debug_level & RPN_DBG_VERBOSE) {
		do_G.debug_level		|= DO_DEBUG_VERBOSE;
	}
	if (G.debug_level & RPN_DBG_DISP_CMDS) {
		do_G.debug_level		|= DO_DEBUG_DISP_CMDS;
	}

	return _retcode;
}

/* do_init() }}} */
/* do_units() {{{ */

/******************************************************************************

					DO_UNITS

******************************************************************************/
char *do_units(double throughput)
{
	char					 _buf[64];
	int					 _A = 6, _B = 3;
	double				 _t;

	if (throughput >= 1e9) {
		_t					= throughput / 1e9;
		sprintf(_buf, "%*.*f Gbps", _A, _B, _t);
	}
	else if (throughput >= 1e6) {
		_t					= throughput / 1e6;
		sprintf(_buf, "%*.*f Mbps", _A, _B, _t);
	}
	else if (throughput >= 1e3) {
		_t					= throughput / 1e3;
		sprintf(_buf, "%*.*f kbps", _A, _B, _t);
	}
	else {
		_t					= throughput;
		sprintf(_buf, "%*.*f bps", _A, _B, _t);
	}

	return strdup(_buf);
}

/* do_units() }}} */
#if 0
/* do_fprintf() {{{ */

/******************************************************************************

					DO_FPRINTF			XXX

******************************************************************************/
static int do_fprintf(FILE *fp, int delta_before, int delta_after, char *fmt, ...)
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

/* do_fprintf() }}} */
#endif
#if 0
/* do_assert_name() {{{ */

/******************************************************************************

					DO_ASSERT_NAME			XXX

******************************************************************************/
static char *do_assert_name(char *name)
{
	return name ? name : "NO_NAME";
}

/* do_assert_name() }}} */
#endif
/* do_op_sys_better_units() {{{ */

/******************************************************************************

					DO_OP_SYS_BETTER_UNITS

******************************************************************************/
RPN_DEF_OP(do_op_sys_better_units)
{
	/* Transforms bps into kbps, Mbps, Gbps
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     int                       _retcode, _type;
	rpn_elt				*_stk_x, *_stk_result;
	static FILE			*_fp = 0;
	static int			 _calls_level = 0;

	if (_calls_level == 0 && _fp == 0) {
		_fp					= stdout;
	}
	_calls_level++;

     _retcode                 = RPN_RET_OK;
	_stk_x				= rpn_pop(stack);

	switch ((_type = rpn_get_type(_stk_x))) {

	case	RPN_TYPE_MATRIX:
		{
			rpn_stack				*_stack;
			rpn_elt				*_clone_matrix;
			int					 _i, _j, _n, _p;
			struct rpn_matrix		*_mat;

			_clone_matrix			= rpn_clone_elt(_stk_x);

			_mat					= (struct rpn_matrix *) _clone_matrix->value.obj;

			_n					= _mat->n;
			_p					= _mat->p;

			_stack				= rpn_new_stack(__func__);
			for (_i = 1; _i <= _n; _i++) {
				for (_j = 1; _j <= _p; _j++) {
					rpn_elt				*_elt;
					int					 _idx;

					_elt					= dt_mat_extract_elt(_mat, _i, _j);
					rpn_push(_stack, _elt);
					(*op->func)(_stack, op);

					_idx					= RPN_MATRIX_IDX(_i, _j, _n, _p);
					_mat->base[_idx]		= rpn_pop(_stack);
				}
			}

			_stk_result			= _clone_matrix;
		}
		break;

	case	RPN_TYPE_LIST:
		{
			rpn_elt				*_elt, *_clone_list;
			rpn_list				*_list, *_new_list;
			rpn_stack				*_stack;

Z
rpn_dump_elt(_stk_x, 0);
			_list				= _stk_x->value.obj;

			_clone_list			= rpn_new_elt(RPN_TYPE_LIST);
			_new_list				= rpn_new_list((char *) __func__);
			_clone_list->value.obj	= _new_list;

			_stack				= rpn_new_stack(__func__);

			for (_elt = _list->top_elt; _elt != 0; _elt = _elt-> previous_elt) {
				rpn_elt			*_elt_clone, *_elt_result;

				_elt_clone		= rpn_clone_elt(_elt);
				rpn_push(_stack, _elt_clone);
				(*op->func)(_stack, op);

				_elt_result		= rpn_pop(_stack);
				rpn_list_push_tail(_new_list, _elt_result);
			}

			rpn_free_stack(_stack);
Z
rpn_dump_elt(_stk_x, 0);
if (_stk_x->name == 0) {
	RPN_INTERNAL_ERROR;
}
			_clone_list->name		= strdup(_stk_x->name);
			_stk_result			= _clone_list;
Z
rpn_dump_elt(_stk_result, 0);
		}
		break;

	case	RPN_TYPE_OPAIR:
		{
			rpn_pair			*_pair;
			rpn_elt			*_elt_a, *_elt_b, *_elt_clone;
			rpn_stack			*_stack;

			_stack			= rpn_new_stack(__func__);

			_pair			= _stk_x->value.obj;
			_elt_a			= _pair->elt_a;
			_elt_b			= _pair->elt_b;
			_elt_clone		= rpn_clone_elt(_elt_a);
			rpn_push(_stack, _elt_clone);
			(*op->func)(_stack, op);
			_pair->elt_a		= rpn_pop(_stack);

			_elt_clone		= rpn_clone_elt(_elt_b);
			rpn_push(_stack, _elt_clone);
			(*op->func)(_stack, op);
			_pair->elt_b		= rpn_pop(_stack);

			rpn_free_stack(_stack);

			_stk_result		= _stk_x;
		}
		break;

	case	RPN_TYPE_STRING:
		{
			_stk_result		= _stk_x;
		}
		break;

	case	RPN_TYPE_INT:
		{
			_stk_result		= _stk_x;
		}
		break;

	case	RPN_TYPE_DOUBLE:
		{
			_stk_result			= rpn_new_elt(RPN_TYPE_STRING);
			_stk_result->value.s	= do_units(_stk_x->value.d);
	if (_stk_x->name == 0) {
Z
		rpn_dump_elt(_stk_x, 0);
		RPN_INTERNAL_ERROR;
	}
			_stk_result->name		= strdup(_stk_x->name);
		}
		break;

	case	RPN_TYPE_NIL:
		{
			_stk_result		= _stk_x;
		}
		break;

	default:
		fprintf(stderr, "%s: type = %d = %s\n", G.progname, _type,
		        rpn_type_to_string(_type));
		RPN_INTERNAL_ERROR;
		break;
	}

	rpn_push(stack, _stk_result);

	_calls_level--;

	if (_calls_level == 0 && _fp != stdout) {
		/* Final return of the operator : close the file
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		fclose(_fp);
	}

     return _retcode;
}

/* do_op_sys_better_units() }}} */
// GROUP : System }}}
