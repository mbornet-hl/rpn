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
 *	@(#)	[MB] do_epri.h	Version 1.8 du 19/10/16 - 
 */

/* Variable declarations
   ~~~~~~~~~~~~~~~~~~~~~ */
extern struct do_global			 do_G;
extern struct do_operators		 do_ops;

/* Prototypes
   ~~~~~~~~~~ */
struct do_iperf_host_desc		 *do_new_iperf_host_desc(void);
rpn_elt						**do_get_curr_list(int, int);
void							  do_add_host_desc(do_iperf_host_desc *);
void							  do_dump_desc(rpn_elt *);
void							  do_dump_host_desc(do_iperf_host_desc *);
rpn_elt						 *do_empty_list(void);
char                                *do_list_label(int);
char							**do_list_to_argp(rpn_list *);
rpn_elt						 *do_gen_matrix(do_host_desc *, do_host_desc *, int);
char							**do_get_client_argp(do_host_desc *, do_host_desc *);
char							**do_get_server_argp(do_host_desc *, do_host_desc *);
char							**do_get_port_argp(do_host_desc *, do_host_desc *);
do_host_desc					 *do_new_host_desc(void);
void							  do_disp_argp(char **);
char							 *do_argp_to_string(char **);

int							 do_init(dl_module *);
char							*do_get_timeout(void);
