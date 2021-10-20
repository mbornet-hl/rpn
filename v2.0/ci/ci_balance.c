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
 *	@(#)	[MB] ci_balance.c	Version 1.2 du 21/10/19 - 
 */

#include	<stdio.h>

#include	"../cc/cc_types.h"
#include	"ci_cpub.h"
#include	"ci_epub.h"

/* ci_rebalance() {{{ */

/*******************************************************************************

						CI_REBALANCE

*******************************************************************************/
ci_ref_node ci_rebalance(ci_ref_root tree, ci_ref_node node)
{
	ci_ref_node			 _root, _left1, _right1, _left2, _right2, _p;

	_root	= node;
	while (_root) {
		if (_root->H_left > (cc_uint16) (_root->H_right + 1)) {
			if (_root->left->H_left > _root->left->H_right) {
				_left1				= _root->left;

				_root->left			= _left1->right;
				if (_left1->right != 0) {
					_left1->right->up		= _root;
					_left1->right->where	= CI_LEFT;
				}

				_left1->up			= _root->up;
				_left1->where			= _root->where;
				switch (_left1->where) {

				case	CI_LEFT:
					_left1->up->left	= _left1;
					break;

				case	CI_RIGHT:
					_left1->up->right	= _left1;
					break;
				}

				_left1->right			= _root;
				_root->up				= _left1;
				_root->where			= CI_RIGHT;

				_root->H_left			= _left1->H_right;
				_left1->H_right		= MAX(_root->H_left,
										_root->H_right) + 1;

				_root				= _left1;
			}
			else {
				_left1				= _root->left;
				_right2				= _left1->right;

				_left1->right			= _right2->left;
				if (_right2->left != 0) {
					_right2->left->up		= _left1;
					_right2->left->where	= CI_RIGHT;
				}

				_root->left			= _right2->right;
				if (_right2->right != 0) {
					_right2->right->up		= _root;
					_right2->right->where	= CI_LEFT;
				}

				_right2->left			= _left1;
				_left1->up			= _right2;
				_left1->where			= CI_LEFT;

				_right2->up			= _root->up;
				_right2->where			= _root->where;
				switch (_right2->where) {

				case	CI_LEFT:
					_right2->up->left	= _right2;
					break;

				case	CI_RIGHT:
					_right2->up->right	= _right2;
					break;
				}

				_right2->right			= _root;
				_root->up				= _right2;
				_root->where			= CI_RIGHT;

				_root->H_left			= _right2->H_right;
				_left1->H_right		= _right2->H_left;
				_right2->H_left		= MAX(_left1->H_left,
										_left1->H_right) + 1;
				_right2->H_right		= MAX(_root->H_left,
										_root->H_right) + 1;

				_root				= _right2;
			}
		}
		else if (_root->H_right > (cc_uint16) (_root->H_left + 1)) {
			if (_root->right->H_right > _root->right->H_left) {
				_right1				= _root->right;

				_root->right			= _right1->left;
				if (_right1->left != 0) {
					_right1->left->up		= _root;
					_right1->left->where	= CI_RIGHT;
				}

				_right1->up			= _root->up;
				_right1->where			= _root->where;
				switch (_right1->where) {

				case	CI_LEFT:
					_right1->up->left	= _right1;
					break;

				case	CI_RIGHT:
					_right1->up->right	= _right1;
					break;
				}

				_right1->left			= _root;
				_root->up				= _right1;
				_root->where			= CI_LEFT;

				_root->H_right			= _right1->H_left;
				_right1->H_left		= MAX(_root->H_left,
										_root->H_right) + 1;

				_root				= _right1;
			}
			else {
				_right1				= _root->right;
				_left2				= _right1->left;

				_right1->left			= _left2->right;
				if (_left2->right != 0) {
					_left2->right->up		= _right1;
					_left2->right->where	= CI_LEFT;
				}

				_root->right			= _left2->left;
				if (_left2->left) {
					_left2->left->up		= _root;
					_left2->left->where		= CI_RIGHT;
				}

				_left2->right			= _right1;
				_right1->up			= _left2;
				_right1->where			= CI_RIGHT;

				_left2->up			= _root->up;
				_left2->where			= _root->where;
				if (_root->up != 0) {
					switch (_left2->where) {

					case	CI_LEFT:
						_left2->up->left	= _left2;
						break;

					case	CI_RIGHT:
						_left2->up->right	= _left2;
						break;
					}
				}

				_left2->left			= _root;
				_root->up				= _left2;
				_root->where			= CI_LEFT;

				_root->H_right			= _left2->H_left;
				_right1->H_left		= _left2->H_right;
				_left2->H_right		= MAX(_right1->H_left,
										_right1->H_right) + 1;
				_left2->H_left			= MAX(_root->H_left,
										_root->H_right) + 1;

				_root				= _left2;
			}
		}

		_p		= _root;
		_root	= _root->up;

		switch (_p->where) {

		case	CI_LEFT:
			_root->H_left	= MAX(_p->H_left, _p->H_right) + 1;
			break;

		case	CI_RIGHT:
			_root->H_right	= MAX(_p->H_left, _p->H_right) + 1;
			break;

		default:
			break;
		}
	}

	return _p;
}

/* ci_rebalance() }}} */
