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
 *	@(#)	[MB] ci_next.c	Version 1.4 du 19/08/25 - 
 */

#include	"../cc/cc_types.h"
#include	"../cg/cg_cpub.h"
#include	"../cg/cg_epub.h"
#include	"ci_cpub.h"
#include	"ci_epub.h"

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

/*******************************************************************************

						CI_GET_NEXT

*******************************************************************************/
ci_ref_node ci_get_next(ci_ref_trek p)
{
	ci_ref_node			 _node, *_pA, *_pB, _retcode;
	cc_uint16				 _wA, _wB, _where;

	switch (p->type) {

	case	CI_T_LNR:
		if ((_retcode = p->node)) {
			p->node	= _retcode->next;
		}
		return _retcode;

	case	CI_T_RNL:
		if ((_retcode = p->node)) {
			p->node	= _retcode->previous;
		}
		return _retcode;

	default:
		break;
	}

	_retcode	= 0;
	_node	= p->node;
	_where	= p->where;

	if (p->type == CI_T_LNR || p->type == CI_T_RNL) {
		while (_node) {
			if (_retcode) {
				p->node	= _node;
				p->where	= _where;
				return _retcode;
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
				_retcode	= _node;
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
		return _retcode;
	}
	else if ((p->type == CI_T_LRN) || (p->type == CI_T_RLN)) {
		while (_node) {
			if (_retcode) {
				p->node	= _node;
				p->where	= _where;
				return _retcode;
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
				_retcode	= _node;
			}

			_where	= _node->where;
			_node	= _node->up;
		}

		p->node	= _node;
		p->where	= _where;
		return _retcode;
	}
	else {
		CG_FATAL();
	}

	return 0;
}
