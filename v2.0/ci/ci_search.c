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
 *   @(#)  [MB] ci_search.c Version 1.6 du 21/10/19 - 
 */

#include  <stdio.h>

#include  "../cc/cc_types.h"
#include  "../cg/cg_cpub.h"
#include  "../cg/cg_epub.h"
#include  "ci_cpub.h"
#include  "ci_epub.h"

/* ci_search() {{{ */

/*******************************************************************************

                              CI_SEARCH

*******************************************************************************/
ci_ref_node ci_search(ci_ref_root tree, ci_ref_node searched_node,
                      cc_uint16 (*cmp)(ci_ref_node, ci_ref_node))
{
     ci_ref_node               _node;

     if ((_node = tree->root) != 0) {
          while (_node != 0) {
               switch ((*cmp)(_node, searched_node)) {

               case CI_CMP_LT:
                    if ((_node = _node->left) == 0) {
                         goto end;
                    }
                    break;

               case CI_CMP_GT:
                    if ((_node = _node->right) == 0) {
                         goto end;
                    }
                    break;

               case CI_CMP_EQ:
                    goto end;

               default:
                    CG_FATAL();
               }
          }
     }
     else {
          _node     = NULL;
     }

end:
     return _node;
}

/* ci_search() }}} */
