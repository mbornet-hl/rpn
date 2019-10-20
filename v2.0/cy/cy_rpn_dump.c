/*
 * Copyright (C) 2017-2019, Martial Bornet
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
 *   @(#)  [MB] cy_rpn_dump.c Version 1.12 du 19/10/19 - 
 */

#include  "cy_rpn_header.h"
#include  "cy_rpn_proto.h"

#if 0
/******************************************************************************

                         RPN_OP_DUMP

******************************************************************************/
RPN_DEF_OP(rpn_op_dump)
{
     int                  _retcode;
     rpn_elt             *_elt;

     _retcode            = RPN_RET_OK;
     _elt                = stack->top_elt;

     /* Dump element with indentation level = 0
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_dump_elt(_elt, 0);

     return _retcode;
}
#endif

/******************************************************************************

					RPN_DISP_CLONE_LEVEL

******************************************************************************/
void rpn_disp_clone_level(rpn_elt *elt)
{
	printf("[Clone %d] ", elt->clone_level);
}

/******************************************************************************

                         RPN_DUMP_ELT

******************************************************************************/
void rpn_dump_elt(rpn_elt *elt, int level)
{
     const char          *_func    = __func__;
     int                  _type;
     int                  _sz      = 10;
     struct rpn_matrix   *_matrix;
	static int		 _new_call;

	_new_call			= TRUE;

     rpn_indent(level);
	rpn_disp_clone_level(elt);

     if (elt != NULL) {
		if (G.debug_level & RPN_DBG_DEBUG) {
			printf("%s() : name = %p\n", __func__, elt->name);
			if (elt->name != 0) {
				printf("%s() : %s(%d) : name = <%s>\n", __func__, __FILE__, __LINE__, elt->name);
			}
		}
          if (elt->name != 0) {
               printf("<%s> ", elt->name);
          }

          switch (_type = rpn_get_type(elt)) {

          case RPN_TYPE_NIL :
               printf("%-*s %10s", _sz, "NIL", "NIL");
               break;

          case RPN_TYPE_INT :
               printf("%-*s %10d", _sz, "INT", elt->value.i);
               break;

          case RPN_TYPE_DOUBLE :
               printf("%-*s %15.5e", _sz, "DOUBLE", elt->value.d);
               break;

          case RPN_TYPE_STRING :
               printf("%-*s [%s]", _sz, "STRING", elt->value.s);
               break;

          case RPN_TYPE_LITTERAL :
			{
				rpn_litteral				*_litteral;

				_litteral					= elt->value.obj;
				printf("Litteral = %p\n", _litteral);
				printf("Value    = %p\n", _litteral->value);
				printf("%-*s '%s'", _sz, "LITTERAL", _litteral->value);
			}
               break;

          case RPN_TYPE_MATRIX :
               {
                    int                  _n, _p, _i, _j, _idx;
                    struct rpn_elt      *_elt;

                    _matrix        = (struct rpn_matrix *) elt->value.obj;
                    _n             = _matrix->n;
                    _p             = _matrix->p;
                    printf("MATRIX    [%d x %d] :\n", _n, _p);
                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _idx           = ((_i - 1) * _p) + (_j - 1);
                              _elt           = (struct rpn_elt *) _matrix->base[_idx];
						printf("[%3d, %3d] :\n", _i, _j);
                              rpn_dump_elt(_elt, level + 1);
                         }
                    }
//                  printf("]\n");
               }
               break;

          case RPN_TYPE_START_MARKER:
               printf("BEGIN");
               break;

          case RPN_TYPE_NEURAL_MLP:
               {
                    struct rpn_mlp      *_mlp;

                    _mlp                = (struct rpn_mlp *) elt->value.obj;
                    printf("NEURAL MLP (%d layers, %d neurons, %d weights, activation function : %s)",
                           _mlp->nb_layers, _mlp->nb_neurons, _mlp->nb_weights, _mlp->str_af);
               }
               break;

          case RPN_TYPE_FILENAME:
               printf("%-*s '%s'", _sz, "FILENAME", elt->value.s);
               break;

          case RPN_TYPE_TRAINING_ELT:
               {
                    struct rpn_training_elt       *_train;
                    struct rpn_training_data      *_data;
                    struct rpn_training_label     *_label;

                    _train         = elt->value.obj;
                    _data          = _train->data;
                    _label         = _train->label;

                    printf("%-*s [idx = %7d, orig = %7d] '%d' %6d -- [%3d x %3d] %-7s",
                           _sz, "TRAINING_ELT",
                           _data->index, _data->orig_index,
                           _label->value, _data->nb_elts * _data->elt_size,
                           _data->num_rows, _data->num_cols,
                           _data->ignore ? "IGNORED" : "USED");
               }
               break;

          case RPN_TYPE_TRAINING_SET:
               {
                    rpn_training_set         *_train_set;

                    _train_set     = elt->value.obj;

                    printf("%-*s '%s'", _sz, "TRAINING_SET", _train_set->name);
                    printf(" %5d elts, current idx = %d",
                           _train_set->nb_elts, _train_set->current_idx);
               }
               break;

          case RPN_TYPE_TEST_ELT:
               {
                    struct rpn_training_elt       *_train;
                    struct rpn_training_data      *_data;
                    struct rpn_training_label     *_label;

                    _train         = elt->value.obj;
                    _data          = _train->data;
                    _label         = _train->label;

                    printf("%-*s [idx = %7d, orig = %7d] '%d' %6d -- [%3d x %3d]",
                           _sz, "TEST_ELT    ",
                           _data->index, _data->orig_index,
                           _label->value, _data->nb_elts * _data->elt_size,
                           _data->num_rows, _data->num_cols);
               }
               break;

          case RPN_TYPE_TEST_SET:
               {
                    rpn_training_set         *_train_set;

                    _train_set     = elt->value.obj;

                    printf("%-*s '%s'", _sz, "TEST_SET    ", _train_set->name);
                    printf(" %5d elts, current idx = %d",
                           _train_set->nb_elts, _train_set->current_idx);
               }
               break;

          case RPN_TYPE_LIST:
               {
                    rpn_list            *_list;
				rpn_elt			*_elt;

                    _list               = elt->value.obj;

                    printf(_list->num_elts > 1 ? "%-*s [%s] %6d elts (%s)" : "%-*s [%s] %6d elt (%s)",
				       _sz, "LIST",
                           _list->name, _list->num_elts, _list->homogeneous ? "Homogeneous" : "Heterogeneous");
				if (_list->num_elts > 0) {
					printf("\n");
					for (_elt = _list->top_elt; _elt != 0; _elt = _elt->previous_elt) {
						rpn_dump_elt(_elt, level + 1);
					}
				}
               }
               break;

          case RPN_TYPE_TUPLE:
               {
                    rpn_list            *_list;

                    _list               = elt->value.obj;

                    printf("%-*s [%s] %6d elts (%s)\n",
				       _sz, "TUPLE",
                           _list->name, _list->num_elts, _list->homogeneous ? "Homogeneous" : "Heterogeneous");

                    printf(" elt1 ... elt%d\n", _list->num_elts);
                    printf("\n");
                    
               }
               break;

          case RPN_TYPE_OPAIR:
               {
                    rpn_pair            *_pair;
                    rpn_elt             *_elt_a, *_elt_b;

                    _pair               = elt->value.obj;

                    printf("%-*s [%s]\n",
				       _sz, "OPAIR",_pair->name);
                    _elt_a              = _pair->elt_a;
                    _elt_b              = _pair->elt_b;

				rpn_dump_elt(_elt_a, level + 1);
				rpn_dump_elt(_elt_b, level + 1);
               }
               break;

          default :
               fprintf(stderr, "%s: (%s) unknown element type (%d)\n",
                       G.progname, _func, _type);
               exit(RPN_EXIT_INTERNAL_ERR);
          }
     }
     else {
          printf("%-*s ", _sz, "");
     }

	printf(" prev = %p next = %p", elt->previous_elt, elt->next_elt);
	if (_new_call) {
		printf("\n");
	}
	_new_call		= FALSE;
}

/******************************************************************************

                         RPN_INDENT

******************************************************************************/
void rpn_indent(int level)
{
     int                  _i;

     for (_i = 0; _i < level; _i++) {
          putchar('>');
     }

     if (level > 0) {
          putchar(' ');
     }
}
