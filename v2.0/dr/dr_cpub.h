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
 *	@(#)	[MB] dr_cpub.h	Version 1.3 du 19/10/01 - 
 */

#if !defined(_DR_CPUB_H)
#define	_DR_CPUB_H

/* Type definitions
   ~~~~~~~~~~~~~~~~ */
typedef	struct dr_fifo					 dr_fifo;
typedef	struct dr_elt					 dr_elt;

/* Description of a FIFO element
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
struct dr_elt {
	dr_elt							*previous,
									*next;
	void								*data;
};

/* FIFO descriptor
   ~~~~~~~~~~~~~~~ */
struct dr_fifo {
	cc_uint32							 num_elts;
	dr_elt							*head;
	dr_elt							*tail;
};

#endif	/* _DR_CPUB_H */
