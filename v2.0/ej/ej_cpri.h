/*
 * Copyright (C) 2019-2020, Martial Bornet
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
 *   @(#)  [MB] ej_cpri.h Version 1.5 du 21/12/19 - 
 */

/* Debug masks
   ~~~~~~~~~~~ */
#define   EJ_DEBUG_LEX             (0x0001)
#define   EJ_DEBUG_YACC            (0x0002)

/* Debug macros
   ~~~~~~~~~~~~ */
#define   EJ_TRACE_LEX(...)      { if (ej_G.debug_level & EJ_DEBUG_LEX) { \
     printf("[%s] ", ej_G.file); printf("HOSTS_LEX  : " __VA_ARGS__); } }
#define   EJ_TRACE_YACC(...)     { if (ej_G.debug_level & EJ_DEBUG_YACC) { \
     printf("[%s] ", ej_G.file); printf("HOSTS_YACC : " __VA_ARGS__); } }

#define   EJ_DUMP_HOST(host)       ej_dump_host(host, __FILE__, __LINE__, __func__)
#define   EJ_DUMP_NAME(name)       ej_dump_name(name, __FILE__, __LINE__, __func__)

#define	EJ_DISP_DIMS			ej_disp_dims(__FILE__, __LINE__)

#define   yylval                   ej_hosts_lval
//#define yyerror                  ej_error

typedef struct ej_host              ej_host;
typedef struct ej_name              ej_name;
typedef struct ej_hosts_tree        ej_hosts_tree;
typedef struct ej_bytes_IP          ej_bytes_IP;

/* Bytes of an IP address
   ~~~~~~~~~~~~~~~~~~~~~~ */
struct ej_bytes_IP {
     byte                           bytes[4];
};

/* Name descriptor
   ~~~~~~~~~~~~~~~ */
struct ej_name {
     struct ci_node                 node;
	cc_uint16					 dim;		// Dimension
     char                          *name;
	bool						*present;
};

/* Hosts descriptor
   ~~~~~~~~~~~~~~~~ */
struct ej_host {
     struct ci_node                 node;
	int						 seq_num;
     char                          *IP;
     unsigned long                  IP_bytes;
     ci_root                        names_alphabetical;
//   struct xx_fifo                 names_by_entry;
};

/* Hosts tree (result of hosts file parsing)
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
struct ej_hosts_tree {
     char                          *filename;
	cc_uint16					 dim;		// Dimension
	cc_uint16					 dim_idx;		//* Current dimension index
     ci_root                        hosts_by_IP;
//   ci_root                        hosts_by_name;
};

/* Global structure
   ~~~~~~~~~~~~~~~~ */
struct ej_global {
     int                            debug_level;
     char                          *file;
     long                           line;
	int						 seq_num;
	cc_uint16					 curr_dim;
	cc_uint16					 dim_idx;
     ej_host                       *tmp_host;
     ej_hosts_tree                 *hosts_tree;
};
