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
 *	@(#)	[MB] ci_init.c	Version 1.4 du 19/08/25 - 
 */

#include	"../cc/cc_types.h"
#include	"ci_cpub.h"
#include	"ci_epub.h"

/*******************************************************************************

						CI_INIT_ROOT

*******************************************************************************/
void ci_init_root(ci_ref_root tree)
{
	tree->root		= 0;
	tree->first		= 0;
	tree->last		= 0;
	tree->nb			= 0;
}

/*******************************************************************************

						CI_INIT_NODE

*******************************************************************************/
void ci_init_node(ci_ref_node node)
{
	node->up		=
	node->left	=
	node->right	=
	node->next	=
	node->previous	= 0;
	node->where	= 0;

	node->H_left	=
	node->H_right	= 1;
}
