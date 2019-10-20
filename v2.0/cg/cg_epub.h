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
 *	@(#)	[MB] cg_epub.h	Version 1.4 du 19/10/01 - 
 */

#if ! defined(CG_EPUB_H)
#	define CG_EPUB_H

/*	Fonctions
 *	~~~~~~~~~
 */
void							 cg_init_backtrace(char *);
void							 cg_fatal(const char *, char *, int);
void							 cg_print_trace(int);

#endif	/* CG_EPUB_H */
