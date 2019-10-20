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
 *	@(#)	[MB] dr_epub.h	Version 1.3 du 19/10/03 - 
 */

void									 dr_init_fifo(dr_fifo *);
void									 dr_init_elt(dr_elt *);
void									 dr_reset_links(dr_elt *);
void									 dr_fifo_insert(dr_fifo *, dr_elt *);
void									 dr_fifo_add(dr_fifo *, dr_elt *);
dr_elt								*dr_fifo_read_elt(dr_fifo *, cc_uint32);
