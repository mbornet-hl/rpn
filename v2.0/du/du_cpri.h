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
 *	@(#)	[MB] du_cpri.h	Version 1.2 du 21/07/21 - 
 */

#define	DU_DEBUG_LEX			(0x0001)
#define	DU_DEBUG_YACC			(0x0002)

/* Debug macros
   ~~~~~~~~~~~~ */
#define   DU_TRACE_LEX(...)      { if (du_G.debug_level & DU_DEBUG_LEX)   printf("WEBSTATS_LEX  : " __VA_ARGS__); }
#define   DU_TRACE_YACC(...)     { if (du_G.debug_level & DU_DEBUG_YACC)  printf("WEBSTATS_YACC : " __VA_ARGS__); }

#define	yylval				du_log_lval
#define	yyerror				du_log_error

struct du_global {
	int			 debug_level;
	long			 num_line;
};
