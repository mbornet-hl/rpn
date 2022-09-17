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
 *   @(#)  [MB] ej_epri.h Version 1.7 du 22/09/17 - 
 */

#define   yyerror                  ej_hosts_error
#define   yyin                     ej_hosts_in

extern struct ej_global             ej_G;
extern FILE                        *ej_hosts_in;

/* Prototypes
   ~~~~~~~~~~ */
RPN_DECL_METHODS(ej);

RPN_DECL_INIT(ej);

RPN_DECL_OP(ej_op_disp_all_hosts);
RPN_DECL_OP(ej_op_disp_all_aliases);
RPN_DECL_OP(ej_op_hostsfile);
RPN_DECL_OP(ej_op_diff);
RPN_DECL_PREF_NEW(ej, host);
RPN_DECL_PREF_NEW(ej, name);

//RPN_DECL_PREF_NEW(ej, addr);
//RPN_DECL_PREF_NEW(ej, alias);
void                                yyerror(char *);

int                                 ej_hosts_lex();
int                                 ej_hosts_parse();
void                                ej_hosts_error(char *);
int                                 ej_hosts_input(void);
cc_uint16                           ej_name_cmp(ci_node *, ci_node *);
cc_uint16                           ej_host_IP_cmp(ci_node *, ci_node *);
void							 ej_dump_hosts_tree(ej_hosts_tree *, char *, int, const char *);
void							 ej_dump_host(ej_host *, char *, int, const char *);
void                                ej_dump_name(ej_name *, char *, int, const char *);
ej_hosts_tree                      *ej_new_hosts_tree(cc_uint16);
void							 ej_move_names(ej_host *, ej_host *);
