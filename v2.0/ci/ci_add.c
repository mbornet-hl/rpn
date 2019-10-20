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
 *   @(#)  [MB] ci_add.c Version 1.7 du 19/08/25 - 
 */

#include  "../cc/cc_types.h"
#include  "../cg/cg_cpub.h"
#include  "../cg/cg_epub.h"
#include  "ci_cpub.h"
#include  "ci_epub.h"

/*******************************************************************************

                              CI_ADD_NODE

*******************************************************************************/
ci_ref_node ci_add_node(ci_ref_root tree, ci_ref_node node,
                        cc_uint16  (*cmp)(ci_ref_node, ci_ref_node),
                        void (*drop)(ci_ref_node, ci_ref_node))
{
     ci_ref_node               _root, _p, _retval, _min, _max;
     cc_uint16                 _result;

     ci_init_node(node);

     if (tree->root == 0) {
          tree->root     = node;
          tree->first    = node;
          tree->last     = node;
          _retval        = 0;
     }
     else {
          _root          = tree->root;
          _min           = 0;
          _max           = 0;

          while ((_p = _root) != 0) {
               switch (_result = (*cmp)(_root, node)) {

               case CI_CMP_LT:
                    _max      = _root;
                    if ((_root = _root->left) == 0) {
                         _retval        = 0;
                         _p->left       = node;
                         node->up       = _p;
                         node->where    = CI_LEFT;
                    }
                    break;

               case CI_CMP_GT:
                    _min      = _root;
                    if ((_root = _root->right) == 0) {
                         _retval        = 0;
                         _p->right      = node;
                         node->up       = _p;
                         node->where    = CI_RIGHT;
                    }
                    break;

               case CI_CMP_EQ:
                    _retval        = _root;
                    if (drop) {
                         (*drop)(_root, node);
                    }
                    goto end;

               default:
                    CG_FATAL();
               }
          }

          if (_min) {
               node->next     = _min->next;
               _min->next     = node;
          }
          else {
               node->next     = tree->first;
               tree->first    = node;
          }
          if (_max) {
               node->previous = _max->previous;
               _max->previous = node;
          }
          else {
               node->previous = tree->last;
               tree->last     = node;
          }

          tree->root     = ci_rebalance(tree, node);
     }

     tree->nb++;

end:
     return _retval;
}
