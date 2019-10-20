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
 *	@(#)	[MB] ci_recurse.c	Version 1.8 du 19/10/20 - 
 */

#include	"../cc/cc_types.h"
#include	"../cg/cg_cpub.h"
#include	"../cg/cg_epub.h"
#include	"ci_cpub.h"
#include	"ci_epub.h"

/* ci_traversal() {{{ */
/*******************************************************************************

						CI_TRAVERSAL

*******************************************************************************/
int ci_traversal(ci_ref_root tree, void (*fct)(ci_ref_node), cc_uint16 way)
{
	return ci_cross(tree->root, fct, way);
}

/* ci_traversal() }}} */
/* ci_cross() {{{ */
/*******************************************************************************

						CI_CROSS

*******************************************************************************/
int ci_cross(ci_ref_node root, void (*fct)(ci_ref_node), cc_uint16 way)
{
	ci_ref_node		 _root;
	int				 _nb = 0;

	if ((_root = root) == 0) {
		return 0;
	}

	switch (way) {

	case	CI_T_LNR:
		if (_root->left) {
			_nb	+= ci_cross(_root->left, fct, way);
		}

		(*fct)(_root);

		if (_root->right) {
			_nb	+= ci_cross(_root->right, fct, way);
		}
		break;

	case	CI_T_RNL:
		if (_root->right) {
			_nb	+= ci_cross(_root->right, fct, way);
		}

		(*fct)(_root);

		if (_root->left) {
			_nb	+= ci_cross(_root->left, fct, way);
		}
		break;

	case	CI_T_LRN:
		if (_root->left) {
			_nb	+= ci_cross(_root->left, fct, way);
		}

		if (_root->right) {
			_nb	+= ci_cross(_root->right, fct, way);
		}

		(*fct)(_root);
		break;

	case	CI_T_RLN:
		if (_root->right) {
			_nb	+= ci_cross(_root->right, fct, way);
		}

		if (_root->left) {
			_nb	+= ci_cross(_root->left, fct, way);
		}

		(*fct)(_root);
		break;

	default:
		CG_FATAL();
	}
	return _nb + 1;
}

/* ci_cross() }}} */
