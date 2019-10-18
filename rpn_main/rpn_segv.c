/*
 * Copyright (C) 2018, Martial Bornet
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
 *   @(#)  [MB] cy_rpn_segv.c Version 1.4 du 19/03/03 - 
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <signal.h>

#include  "rpn_header.h"
#include  "rpn_epri.h"
#include  "rpn_proto.h"

#define   MAX_FUNCTS               (320)

void rpn_print_trace(int);

/******************************************************************************

					RPN_INIT_BACKTRACE

******************************************************************************/
void rpn_init_backtrace(void)
{
	sighandler_t		 _previous_handler;

	if ((_previous_handler = signal(SIGSEGV, rpn_print_trace)) == SIG_ERR) {
		fprintf(stderr, "%s: signal() returned an error !\n", G.progname);
		exit(RPN_EXIT_SIGNAL_ERROR);
	}
}

/******************************************************************************

                              RPN_PRINT_TRACE

******************************************************************************/
void rpn_print_trace(int signum)
{
     void            *_array[MAX_FUNCTS];
     size_t           _num_addr, _max_addr;
     char           **_strings;
     int              _i;

	printf("\n========================================\n");
	printf("%s: received signal %d.\n", G.progname, signum);

     _max_addr      = sizeof(_array)/sizeof(_array[0]);
//   printf("Max addresses number = %d\n", _max_addr);

     _num_addr      = backtrace(_array, _max_addr);
//   printf("size = %d\n", _num_addr);

     _strings       = backtrace_symbols(_array, _num_addr);
     printf("%d currently active functions :\n", _num_addr);

     for (_i = 0; _i < _num_addr; _i++) {
          printf("===> %s\n", _strings[_i]);
     }

     rpn_free(_strings);

	exit(RPN_EXIT_SEGV);
}
