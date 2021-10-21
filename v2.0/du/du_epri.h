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
 *	@(#)	[MB] du_epri.h	Version 1.2 du 21/07/23 - 
 */

extern struct du_global			 du_G;

/* Function declarations
   ~~~~~~~~~~~~~~~~~~~~~ */
RPN_DECL_PREF_NEW(du, log_entry);
void							 du_add_to_list(struct du_list *, struct du_log_entry *);
unsigned int					 du_IP_to_bytes(char *);
void							 yyerror(char *);
