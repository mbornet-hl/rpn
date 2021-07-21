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
 *   @(#)  [MB] do_cpri.h Version 1.10 du 19/10/25 - 
 */

/* Debug macros
   ~~~~~~~~~~~~ */
#define   DO_TRACE_LEX_IN(...)      { if (do_G.debug_level & DO_DEBUG_LEX_IN)   printf("IPERF_LEX  : " __VA_ARGS__); }
#define   DO_TRACE_YACC_IN(...)     { if (do_G.debug_level & DO_DEBUG_YACC_IN)  printf("IPERF_YACC : " __VA_ARGS__); }
#define   DO_TRACE_LEX_OUT(...)     { if (do_G.debug_level & DO_DEBUG_LEX_OUT)  printf("IPERF_LEX  : " __VA_ARGS__); }
#define   DO_TRACE_YACC_OUT(...)    { if (do_G.debug_level & DO_DEBUG_YACC_OUT) printf("IPERF_YACC : " __VA_ARGS__); }

#if defined(X)
#undef	X
#endif
#define   X                        if (do_G.debug_level & DO_DEBUG_LINE) fprintf(stdout, "%s (%d)\n", __FILE__, __LINE__);

#if defined(Z)
#undef	Z
#endif
#define   Z                        fprintf(stdout, "DEBUG : %s (%d)\n", __FILE__, __LINE__);


/* Debug masks
   ~~~~~~~~~~~ */
#define   DO_DEBUG_LEX_IN               (0x0001)			//   1
#define   DO_DEBUG_YACC_IN              (0x0002)			//   2
#define   DO_DEBUG_LEX_OUT              (0x0004)			//   4
#define   DO_DEBUG_YACC_OUT             (0x0008)			//   8
#define	DO_DEBUG_CMD				(0x0010)			//  16
#define	DO_DEBUG_POINTERS			(0x0020)			//  32
#define	DO_DEBUG_ELEMENTS			(0x0040)			//  64
#define	DO_DEBUG_LINE				(0x0080)			// 128
#define	DO_DEBUG_VERBOSE			(0x0100)			// 256
#define	DO_DEBUG_DISP_CMDS			(0x0200)			// 512

/* Labels (for JSON output)
   ~~~~~~~~~~~~~~~~~~~~~~~~ */
#define	DO_LABEL_ELT_CLIENT			"client"
#define	DO_LABEL_ELT_SERVER			"server"
#define	DO_LABEL_HOSTS				"hosts"
#define	DO_LABEL_GLOBAL_RESULT		"iperf results"
#define	DO_LABEL_MIN				"min"
#define	DO_LABEL_MAX				"max"
#define	DO_LABEL_MEAN				"mean"
#define	DO_LABEL_EXACT				"exact"
#define	DO_LABEL_IPERF_ID			"ID"
#define	DO_LABEL_IPERF_SUM			"SUM"
#define	DO_LABEL_RESULT			"result"
#define	DO_LABEL_ROW_INDEX			"row idx"
#define	DO_LABEL_COL_INDEX			"col idx"
#define	DO_LABEL_NO_DATA			"No data"

/* Default timeout for server (in seconds)
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define	DO_DEFLT_TIMEOUT			"30"

/* Structure definitions
   ~~~~~~~~~~~~~~~~~~~~~ */
struct do_iperf_host_desc {
     rpn_elt                  *elt_server;                  // STRING
     rpn_elt                  *list_client_opts;            // LIST(STRING)
     rpn_elt                  *list_server_opts;            // LIST(STRING)
     rpn_elt                  *list_server_port;            // LIST(STRING)
};
typedef struct do_iperf_host_desc        do_iperf_host_desc;

struct do_global {
     rpn_elt                   *desc;                       // LIST(LIST)
     rpn_elt                   *list_deflt_client_opts;     // LIST(STRING)
     rpn_elt                   *list_deflt_server_opts;     // LIST(STRING)
     rpn_elt                   *list_deflt_server_port;     // LIST(STRING)
	rpn_elt				 *server_timeout;			// STRING
     rpn_elt                   *tmp_opts[3];
     int                        field_no;
	int                       debug_level;
	int                       sum_found;
	rpn_elt                  *ID_list,
						*SUM_list;
};
typedef struct do_global                 do_global;

struct do_host_desc {
     char                      *server;
     char                     **list_client_opts;
     char                     **list_server_opts;
     char                     **list_server_port;
};
typedef struct do_host_desc              do_host_desc;

/* Operators from external modules, needed by this module
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
struct do_operators {
	dl_operator					*list,
								*pair,
								*prx,
								*stk;
};
