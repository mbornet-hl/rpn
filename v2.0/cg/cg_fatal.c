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
 *	@(#)	[MB] cg_fatal.c	Version 1.5 du 19/09/29 - 
 */

#include	<stdio.h>
#include	<stdlib.h>

#include	"../cc/cc_types.h"
#include	"../cd/cd_cpub.h"
#include	"cg_cpub.h"
#include	"cg_epub.h"

/* cg_fatal() {{{ */
/*******************************************************************************

						CG_FATAL

*******************************************************************************/
void cg_fatal(const char *funct, char *file, int line)
{
     fprintf(stderr, "INTERNAL ERROR !!! (%s() : %s(%d))\n",
             funct, file, line);
     cg_print_trace(0);
     exit(CD_EXIT_INTERNAL_ERR);
}

/* cg_fatal() }}} */
