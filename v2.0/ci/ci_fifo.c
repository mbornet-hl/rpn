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
 *	@(#)	[MB] ci_fifo.c	Version 1.8 du 19/10/20 - 
 */

#include	<stdio.h>

#include	"../cc/cc_types.h"
#include	"../cg/cg_cpub.h"
#include	"../cg/cg_epub.h"
#include	"ci_cpub.h"
#include	"ci_epub.h"

/*******************************************************************************

						CI_FIFO

*******************************************************************************/
int ci_fifo(ci_ref_root tree, void (*fct)(ci_ref_node), cc_uint16 type)
{
	ci_ref_node			 _node = 0;
	int					 _nb;

	if (type == CI_T_LNR) {
		_node	= tree->first;
	}
	else
	if (type == CI_T_RNL) {
		_node	= tree->last;
	}
	else {
		CG_FATAL();
	}

	_nb		= 0;
	while (_node) {
		(*fct)(_node);
		_nb++;

		if (type == CI_T_LNR) {
			_node	= _node->next;
		}
		if (type == CI_T_RNL) {
			_node	= _node->previous;
		}
	}

	return _nb;
}
