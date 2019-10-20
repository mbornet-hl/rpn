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
 *	@(#)	[MB] cg_segv.c	Version 1.2 du 19/10/01 - 
 */

#define _GNU_SOURCE

#include	<stdio.h>
#include	<stdlib.h>
#include	<execinfo.h>
#include	<signal.h>
#include	"../cd/cd_cpub.h"
#include	"cg_epub.h"

#define   MAX_FUNCTS               (320)

char							*cd_progname;

/* cg_init_backtrace() {{{ */

/******************************************************************************

					CG_INIT_BACKTRACE

******************************************************************************/
void cg_init_backtrace(char *progname)
{
	sighandler_t		 _previous_handler;

	cd_progname		= progname;

	if ((_previous_handler = signal(SIGSEGV, cg_print_trace)) == SIG_ERR) {
		fprintf(stderr, "%s: signal() returned an error !\n", cd_progname);
		exit(CD_EXIT_SIGNAL_ERROR);
	}
}

/* cg_init_backtrace() }}} */
/* cg_print_trace() {{{ */

/******************************************************************************

                              CG_PRINT_TRACE

******************************************************************************/
void cg_print_trace(int signum)
{
     void				 *_array[MAX_FUNCTS];
     size_t			  _num_addr, _max_addr;
     char				**_strings;
     int				  _i;

	printf("\n========================================\n");
	printf("%s: received signal %d.\n", cd_progname, signum);

     _max_addr      = sizeof(_array)/sizeof(_array[0]);
//   printf("Max addresses number = %d\n", _max_addr);

     _num_addr      = backtrace(_array, _max_addr);
//   printf("size = %d\n", _num_addr);

     _strings       = backtrace_symbols(_array, _num_addr);
     printf("%lu currently active functions :\n", _num_addr);

     for (_i = 0; _i < _num_addr; _i++) {
          printf("===> %s\n", _strings[_i]);
     }

     free(_strings);

	exit(CD_EXIT_SEGV);
}

/* cg_print_trace() }}} */
