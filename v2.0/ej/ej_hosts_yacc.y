%{

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
 *   @(#)  [MB] ej_hosts_yacc.y Version 1.12 du 23/03/16 - 
 */

#include  "../cy/cy_rpn_header.h"
#include  <stdio.h>
#include  <assert.h>
#include  <sys/types.h>
#include  <sys/types.h>
#include  <time.h>
#include  <string.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <ctype.h>
#include  <arpa/inet.h>
#include  "../ci/ci_cpub.h"
#include  "../ci/ci_epub.h"
#include  "ej_epub.h"
#include  "ej_cpri.h"
#include  "ej_epri.h"

%}

%union {
  char                   *IP;
  char                   *hostname;
}

%token    <IP>            EJ_IPv4 EJ_IPv6
%token    <hostname>      EJ_HOSTNAME
%token                    EJ_NEWLINE
%type     <IP>            hosts_line IP

%start    hostsfile

%%

hostsfile      : hosts_line
               {
                    EJ_TRACE_YACC("hosts_line [%s]\n", $1);

                    if (ej_G.hosts_tree == 0) {
                         ej_G.hosts_tree          = ej_new_hosts_tree(1);
                    }

                    if (ej_G.tmp_host) {
//                       EJ_DUMP_HOST(ej_G.tmp_host);

                         /* Add host to tree
                            ~~~~~~~~~~~~~~~~ */
                         if (ci_add_node(&ej_G.hosts_tree->hosts_by_IP,
                                         &ej_G.tmp_host->node,
                                         ej_host_IP_cmp, 0) != 0) {
                              fprintf(stderr, "%s: %s(%d) ci_add_node_error !\n",
                                      G.progname, __FILE__, __LINE__);
                              exit(1);
                         }
                         ej_G.tmp_host            = 0;
                    }
               }

               | hostsfile hosts_line
               {
                    ci_node                  *_node;

                    EJ_TRACE_YACC("hostsfile hosts_line [%s]\n", $2);

                    if (ej_G.tmp_host) {
//                       EJ_DUMP_HOST(ej_G.tmp_host);

                         /* Add host to tree
                            ~~~~~~~~~~~~~~~~ */
                         if ((_node = ci_add_node(&ej_G.hosts_tree->hosts_by_IP,
                                                  &ej_G.tmp_host->node,
                                                  ej_host_IP_cmp, 0)) != 0) {
                              fprintf(stderr,
                                      "%s: [%s] duplicate entry for IP = %s\n",
                                      G.progname, ej_G.file, ej_G.tmp_host->IP_addr.IP);

                              /* Add names to the existing host
                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                              ej_move_names(_node->data, ej_G.tmp_host);
                         }
                         ej_G.tmp_host            = 0;
                    }
               }
               ;

hosts_line     : IP hosts EJ_NEWLINE
               {
                    EJ_TRACE_YACC("IP hosts EJ_NEWLINE\n");
               }
               | EJ_NEWLINE
               {
                    EJ_TRACE_YACC("EJ_NEWLINE\n");
                    $$                       = "";
               }
               ;

IP             : EJ_IPv4
               {
                    EJ_TRACE_YACC("EJ_IPv4     [%s]\n", $1);

                    if (ej_G.tmp_host == 0) {
                         ej_G.tmp_host                 = ej_new_host();
                    }
                    else {
                         fprintf(stderr,
                                 "%s(%d) : ej_G.tmp_host != 0 ! (Multiple IP addresses)\n",
                                 __FILE__, __LINE__);
                         exit(1);
                    }
                    ej_G.seq_num++;
                    ej_G.tmp_host->seq_num        = ej_G.seq_num;
                    ej_G.tmp_host->IP_addr.IP     = strdup($1);
                    ej_G.tmp_host->IP_addr.type   = AF_INET;
                    if (inet_pton(AF_INET, ej_G.tmp_host->IP_addr.IP, ej_G.tmp_host->IP_addr.v4) == 0) {
                         fprintf(stderr, "%s : invalid IPv4 address !\n", ej_G.tmp_host->IP_addr.IP);
                    }
//                  EJ_DUMP_HOST(ej_G.tmp_host);

                    $$                            = $1;
               }
               | EJ_IPv6
               {
                    EJ_TRACE_YACC("EJ_IPv6     [%s]\n", $1);

                    if (ej_G.tmp_host == 0) {
                         ej_G.tmp_host            = ej_new_host();
                    }
                    else {
                         fprintf(stderr, "%s(%d) : ej_G.tmp_host != 0 ! (Multiple IP addresses)\n",
                                 __FILE__, __LINE__);
                         exit(1);
                    }
                    ej_G.seq_num++;
                    ej_G.tmp_host->seq_num        = ej_G.seq_num;
                    ej_G.tmp_host->IP_addr.IP     = strdup($1);
                    ej_G.tmp_host->IP_addr.type   = AF_INET6;
                    if (inet_pton(AF_INET6, ej_G.tmp_host->IP_addr.IP, ej_G.tmp_host->IP_addr.v6) == 0) {
                         fprintf(stderr, "%s : invalid IPv6 address !\n", ej_G.tmp_host->IP_addr.IP);
                    }
//                  EJ_DUMP_HOST(ej_G.tmp_host);

                    $$                       = $1;
               }
               ;

hosts          : EJ_HOSTNAME
               {
                    struct ej_name           *_name;

                    EJ_TRACE_YACC("EJ_HOSTNAME [%s]\n", $1);

                    _name                         = ej_new_name();
                    _name->name                   = strdup($1);
                    _name->dim                    = ej_G.curr_dim;
                    _name->present[ej_G.dim_idx]  = TRUE;
                    _name->hosts_tree             = ej_G.hosts_tree;

                    if (ci_add_node(&ej_G.tmp_host->names_alphabetical,
                                    &_name->node,
                                    ej_name_cmp, 0) != 0) {
                         fprintf(stderr, "%s: %s(%d) ci_add_node_error !\n",
                                 G.progname, __FILE__, __LINE__);
                         exit(1);
                    }
               }
               | hosts EJ_HOSTNAME
               {
                    struct ej_name           *_name;

                    EJ_TRACE_YACC("hosts EJ_HOSTNAME [%s]\n", $2);

                    _name                         = ej_new_name();
                    _name->name                   = strdup($2);
                    _name->present[ej_G.dim_idx]  = TRUE;
                    _name->hosts_tree             = ej_G.hosts_tree;

//                  EJ_DUMP_HOST(ej_G.tmp_host);

                    if (ci_add_node(&ej_G.tmp_host->names_alphabetical,
                                    &_name->node,
                                    ej_name_cmp, 0) != 0) {
                         fprintf(stderr, "%s: %s(%d) ci_add_node_error !\n",
                                 G.progname, __FILE__, __LINE__);
                         exit(1);
                    }
               }
               ;

%%
