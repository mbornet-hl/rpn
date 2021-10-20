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
 *	@(#)	[MB] ci_next.c	Version 1.7 du 21/10/19 - 
 */

#include	"../cc/cc_types.h"
#include	"../cg/cg_cpub.h"
#include	"../cg/cg_epub.h"
#include	"ci_cpub.h"
#include	"ci_epub.h"

//#include	<stdio.h>
//#undef 	X
//#define	X		printf("%s() : %s(%d) where = %s\n", __func__, __FILE__, __LINE__, ci_where(_where));

/* ci_reset() {{{ */

/*******************************************************************************

						CI_RESET

*******************************************************************************/
void ci_reset(ci_ref_trek p, ci_ref_root tree, cc_uint16 type)
{
	p->type	= type;
	p->where	= CI_DOWN;

	switch (type) {

	case	CI_T_LNR:
		p->node	= tree->first;
		break;

	case	CI_T_RNL:
		p->node	= tree->last;
		break;

	default:
		p->node	= tree->root;
		break;
	}
}

/* ci_reset() }}} */
#if defined(X)
/* ci_where() {{{ */

/******************************************************************************

						CI_WHERE

******************************************************************************/
char *ci_where(cc_uint8 where)
{
	char					*_where;

	switch (where) {
	
	case	CI_LEFT:
		_where				= "LEFT";
		break;

	case	CI_RIGHT:
		_where				= "RIGHT";
		break;

	case	CI_DOWN:
		_where				= "DOWN ";
		break;

	default:
		_where				= " ??? ";
		break;
	}

	return _where;
}

/* ci_where() }}} */
#endif
/* ci_get_next() {{{ */

/*******************************************************************************

						CI_GET_NEXT

*******************************************************************************/
ci_ref_node ci_get_next(ci_ref_trek p)
{
	ci_ref_node			 _node, *_pA, *_pB, _retnode;
	cc_uint16				 _wA, _wB, _where;

	switch (p->type) {

	case	CI_T_LNR:
		if ((_retnode = p->node)) {
			p->node	= _retnode->next;
		}
		return _retnode;

	case	CI_T_RNL:
		if ((_retnode = p->node)) {
			p->node	= _retnode->previous;
		}
		return _retnode;

	default:
		break;
	}

	_retnode	= 0;
	_node	= p->node;
	_where	= p->where;

	if (p->type == CI_T_LNR || p->type == CI_T_RNL) {
		while (_node) {
			if (_retnode) {
				p->node	= _node;
				p->where	= _where;
				return _retnode;
			}

			if (p->type == CI_T_LNR) {
				_pA		= &_node->left;
				_pB		= &_node->right;
				_wA		= CI_LEFT;
				_wB		= CI_RIGHT;
			}
			else {
				_pA		= &_node->right;
				_pB		= &_node->left;
				_wA		= CI_RIGHT;
				_wB		= CI_LEFT;
			}

			if (_where == CI_DOWN && *_pA) {
				_node	= *_pA;
				continue;
			}
			if (_where == CI_DOWN || _where == _wA) {
				_retnode	= _node;
			}
			if ((_where == CI_DOWN || _where == _wA) && *_pB) {
				_node	= *_pB;
				_where	= CI_DOWN;
				continue;
			}

			_where	= _node->where;
			_node	= _node->up;
		}

		p->node	= _node;
		p->where	= _where;
		return _retnode;
	}
	else if ((p->type == CI_T_LRN) || (p->type == CI_T_RLN)) {
		while (_node) {
			if (_retnode) {
				p->node	= _node;
				p->where	= _where;
				return _retnode;
			}

			if (p->type == CI_T_LRN) {
				_pA		= &_node->left;
				_pB		= &_node->right;
				_wA		= CI_LEFT;
				_wB		= CI_RIGHT;
			}
			else {
				_pA		= &_node->right;
				_pB		= &_node->left;
				_wA		= CI_RIGHT;
				_wB		= CI_LEFT;
			}

			if (_where == CI_DOWN && *_pA) {
				_node	= *_pA;
				continue;
			}
			if ((_where == CI_DOWN || _where == _wA) && *_pB) {
				_node	= *_pB;
				_where	= CI_DOWN;
				continue;
			}
			if (_where == CI_DOWN || _where == _wB) {
				_retnode	= _node;
			}

			_where	= _node->where;
			_node	= _node->up;
		}

		p->node	= _node;
		p->where	= _where;
		return _retnode;
	}
	else if ((p->type == CI_T_NLR) || (p->type == CI_T_NRL)) {
		while (_node) {
			if (_retnode) {
				p->node	= _node;
				p->where	= _where;
				return _retnode;
			}

			if (p->type == CI_T_NLR) {
				_pA		= &_node->left;
				_pB		= &_node->right;
				_wA		= CI_LEFT;
				_wB		= CI_RIGHT;
			}
			else {
				_pA		= &_node->right;
				_pB		= &_node->left;
				_wA		= CI_RIGHT;
				_wB		= CI_LEFT;
			}

			if (_where == CI_DOWN) {
				_retnode	= _node;
				if (*_pA) {
					_node	= *_pA;
				}
				else if (*_pB) {
					_node	= *_pB;
				}
				else {
					_where	= _node->where;
					_node	= _node->up;
				}
				continue;
			}
			if ((_where == _wA) && *_pB) {
				_where	= CI_DOWN;
				_node	= *_pB;
				continue;
			}

			_where	= _node->where;
			_node	= _node->up;
		}

		p->node	= _node;
		p->where	= _where;
		return _retnode;
	}
	else {
		CG_FATAL();
	}

	return 0;
}

/* ci_get_next() }}} */
