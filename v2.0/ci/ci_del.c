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
 *	@(#)	[MB] ci_del.c	Version 1.6 du 21/10/19 - 
 */

#include	<stdio.h>

#include	"../cc/cc_types.h"
#include	"../cg/cg_cpub.h"
#include	"../cg/cg_epub.h"
#include	"ci_cpub.h"
#include	"ci_epub.h"

/* ci_del_node() {{{ */

/*******************************************************************************

						CI_DEL_NODE

*******************************************************************************/
ci_ref_node ci_del_node(ci_ref_root tree, ci_ref_node node)
{
	ci_ref_node			 _left, _right, _up, _previous, _next, _base;

	_left		= node->left;
	_right		= node->right;
	_up			= node->up;
	_next		= node->next;
	_previous		= node->previous;
	_base		= 0;

	if (_next) {
		_next->previous	= _previous;
	}
	else {
		tree->last		= _previous;
	}
	if (_previous) {
		_previous->next	= _next;
	}
	else {
		tree->first		= _next;
	}

	if (_left == 0) {
		if (_right == 0) {
			if (_up == 0) {
				ci_check_root(tree, node);

				tree->root			= 0;
			}
			else {
				switch (node->where) {

				case	CI_LEFT:
					_up->left			= 0;
					_up->H_left		= 1;
					break;

				case	CI_RIGHT:
					_up->right		= 0;
					_up->H_right		= 1;
					break;
				}
				_base				= _up;
			}
		}
		else {
			if (_up == 0) {
				ci_check_root(tree, node);

				tree->root			= _right;

				_right->up			= 0;
				_right->where			= 0;
			}
			else {
				_right->up			= _up;
				_right->where			= node->where;

				switch (node->where) {

				case	CI_LEFT:
					_up->left			= _right;
					_up->H_left		= node->H_right;
					break;
				
				case	CI_RIGHT:
					_up->right		= _right;
					_up->H_right		= node->H_right;
					break;
				}
				_base				= _up;
			}
		}
	}
	else {
		if (_right == 0) {
			if (_up == 0) {
				ci_check_root(tree, node);

				tree->root			= _left;

				_left->up				= 0;
				_left->where			= 0;
			}
			else {
				_left->up				= _up;
				_left->where			= node->where;

				switch (node->where) {

				case	CI_LEFT:
					_up->left			= _left;
					_up->H_left		= node->H_left;
					break;

				case	CI_RIGHT:
					_up->right		= _left;
					_up->H_right		= node->H_left;
					break;
				}
				_base				= _left;
			}
		}
		else {
			_previous->H_right			= node->H_right;

			if (_previous == _left) {
				_previous->right		= _right;
				_right->up			= _previous;

				_previous->up			= _up;
				_previous->where		= node->where;
				switch (node->where) {

				case	CI_LEFT:
					_up->left			= _previous;
					break;

				case	CI_RIGHT:
					_up->right		= _previous;
					break;

				default:
					ci_check_root(tree, node);

					tree->root		= _previous;
					break;
				}
				_base		= _previous;
			}
			else {
				_previous->up->right	= _previous->left;
				if (_previous->left) {
					_previous->left->up		= _previous->up;
					_previous->left->where	= _previous->where;
					_base				= _previous->left;
				}
				else {
					_previous->up->H_right	= 1;
					_base				= _previous->up;
				}

				_previous->left		= _left;
				_left->up				= _previous;

				_previous->right		= _right;
				_right->up			= _previous;

				_previous->up			= _up;
				_previous->where		= node->where;
				switch (node->where) {

				case	CI_LEFT:
					_up->left			= _previous;
					break;

				case	CI_RIGHT:
					_up->right		= _previous;
					break;

				default:
					ci_check_root(tree, node);

					tree->root		= _previous;
					break;
				}
			}
		}
	}

	if (_base) {
		tree->root	= ci_rebalance(tree, _base);
	}

	tree->nb--;

	node->up		=
	node->left	=
	node->right	=
	node->next	=
	node->previous	= 0;
	node->where	= 0;

	return node;
}

/* ci_del_node() }}} */
/* ci_check_root() {{{ */

/*******************************************************************************

						CI_CHECK_ROOT

*******************************************************************************/
void ci_check_root(ci_ref_root tree, ci_ref_node node)
{
	if (tree->root != node) {
		CG_FATAL();
	}
	switch (node->where) {

	case	CI_LEFT:
		CG_FATAL();
		break;

	case	CI_RIGHT:
		CG_FATAL();
		break;
	}
}

/* ci_check_root() }}} */
