/*
 * Copyright (C) 2017, Martial Bornet
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
 *   @(#)  [MB] cy_rpn_disp.c Version 1.9 du 17/11/13 - 
 */

#include  "rpn_header.h"


/******************************************************************************

                         RPN_DISP_ELT

******************************************************************************/
void rpn_disp_elt(rpn_elt *elt, int value_only)
{
     const char          *_func    = __func__;
     int                  _type;
     int                  _sz      = 10;
     struct rpn_matrix   *_matrix;

     switch (_type = rpn_get_type(elt)) {

     case RPN_TYPE_INT :
          if (value_only) {
               printf("%10d", elt->value.i);
          }
          else {
               printf("%-*s %10d\n", _sz, "INT", elt->value.i);
          }
          break;

     case RPN_TYPE_FLOAT :
          if (value_only) {
               printf("%15.5f", elt->value.f);
          }
          else {
               printf("%-*s %15.5f\n", _sz, "FLOAT", elt->value.f);
          }
          break;

     case RPN_TYPE_STRING :
          if (value_only) {
               printf(" [%s] ", elt->value.s);
          }
          else {
               printf("%-*s [%s]\n", _sz, "STRING", elt->value.s);
          }
          break;

     case RPN_TYPE_LITTERAL :
          if (value_only) {
               printf(" '%s' ", elt->value.s);
          }
          else {
               printf("%-*s '%s'\n", _sz, "LITTERAL", elt->value.s);
          }
          break;

     case RPN_TYPE_MATRIX :
          {
               int                  _n, _p, _i, _j, _idx;
               struct rpn_elt      *_elt;

               _matrix        = (struct rpn_matrix *) elt->value.obj;
               _n             = _matrix->n;
               _p             = _matrix->p;
               printf("MATRIX    [%d x %d]\n", _n, _p);
               for (_i = 1; _i <= _n; _i++) {
                    printf("|");
                    for (_j = 1; _j <= _p; _j++) {
                         _idx           = ((_j - 1) * _n) + (_i - 1);
                         _elt           = (struct rpn_elt *) _matrix->base[_idx];
                         printf(" ");
                         rpn_disp_elt(_elt, TRUE);
                    }
                    printf(" |\n");
               }
          }
          break;

     default :
          fprintf(stderr, "%s: (%s) unknown element type (%d)\n",
                  G.progname, _func, _type);
          exit(RPN_EXIT_INTERNAL_ERR);
     }
}

/******************************************************************************

                         RPN_DISP_STACK

******************************************************************************/
void rpn_disp_stack(rpn_stack *stack)
{
     rpn_elt             *_elt;
     int                  _num     = 0;
	char				*_reg;
	int				 _name_sz	= 20;

     if (stack->base_elt == 0) {
          printf("%-*s STACK EMPTY\n", _name_sz, stack->name);
     }
     else {
		_num		= stack->num_elts;
          for (_elt = stack->base_elt; _elt != 0; _elt = _elt->next_elt, _num--) {
			switch (_num) {

			case	4:
				_reg		= "T";
				break;

			case	3:
				_reg		= "Z";
				break;

			case	2:
				_reg		= "Y";
				break;

			case	1:
				_reg		= "X";
				break;

			default:
				_reg		= "";
				break;
			}

               printf("%-*s STACK %3d %3s : ", _name_sz, stack->name, _num, _reg);
               rpn_disp_elt(_elt, FALSE);
          }
     }

     printf("\n");
}
