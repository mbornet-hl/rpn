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
 *   @(#)  [MB] cy_rpn_disp.c Version 1.50 du 19/10/24 - 
 */

#include	<unistd.h>
#include  "cy_rpn_header.h"
#include  "cy_rpn_proto.h"
#include  "../ci/ci_epub.h"
#include	"../dl/dl_cpub.h"
#include	"../dl/dl_epub.h"

/* rpn_disp_elt() {{{ */
/******************************************************************************

                         RPN_DISP_ELT

******************************************************************************/
void rpn_disp_elt(rpn_elt *elt, int disp_flags)
{
     const char          *_func    = __func__;
     int                  _type;
     int                  _sz      = 10;
     struct rpn_matrix   *_matrix;

     if (elt != NULL) {
          if (elt->name != 0) {
               printf("[[%s]] ", elt->name);
          }

          switch (_type = rpn_get_type(elt)) {

          case RPN_TYPE_NIL :
               if (disp_flags & RPN_DISP_VALUE) {
                    printf("%10s", "NIL");
               }
               else {
                    printf("%-*s %10s", _sz, "NIL", "NIL");
               }
               break;

          case RPN_TYPE_INT :
               if (disp_flags & RPN_DISP_VALUE) {
                    printf("%10d", elt->value.i);
               }
               else {
                    printf("%-*s %10d", _sz, "INT", elt->value.i);
               }
               break;

          case RPN_TYPE_DOUBLE :
               if (disp_flags & RPN_DISP_VALUE) {
//                  printf("%15.9e", elt->value.d);
                    printf("%16.9g", elt->value.d);
               }
               else {
//                  printf("%-*s %15.9e", _sz, "DOUBLE", elt->value.d);
                    printf("%-*s %16.9g", _sz, "DOUBLE", elt->value.d);
               }
               break;

          case RPN_TYPE_STRING :
               if (disp_flags & RPN_DISP_VALUE) {
                    printf(" [%s] ", elt->value.s);
               }
               else {
                    printf("%-*s [%s]", _sz, "STRING", elt->value.s);
               }
               break;

          case RPN_TYPE_MATRIX :
               {
                    int                  _n, _p, _i, _j, _idx;
                    struct rpn_elt      *_elt;

                    _matrix        = (struct rpn_matrix *) elt->value.obj;
                    _n             = _matrix->n;
                    _p             = _matrix->p;
                    printf("MATRIX    [%d x %d]", _n, _p);
                    if (disp_flags & RPN_DISP_VALUE) {
                         printf("\n");
                         for (_i = 1; _i <= _n; _i++) {
                              printf("|");
                              for (_j = 1; _j <= _p; _j++) {
                                   _idx           = ((_i - 1) * _p) + (_j - 1);
                                   _elt           = (struct rpn_elt *) _matrix->base[_idx];
                                   printf(" ");
                                   rpn_disp_elt(_elt, disp_flags);
                              }
                              printf(" |\n");
                         }
                    }
               }
               break;

          case RPN_TYPE_LITTERAL :
			{
				if (disp_flags & RPN_DISP_VALUE) {
					printf(" '%s' ", rpn_litteral_value(elt));
				}
				else {
					printf("%-*s '%s'", _sz, "LITTERAL", rpn_litteral_value(elt));
				}
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
               if (disp_flags & RPN_DISP_VALUE) {
                    printf(" '%s' ", elt->value.s);
               }
               else {
                    printf("%-*s '%s'", _sz, "FILENAME", elt->value.s);
               }
               break;

          case RPN_TYPE_TRAINING_ELT:
               {
                    struct rpn_training_elt       *_train;
                    struct rpn_training_data      *_data;
                    struct rpn_training_label     *_label;

                    _train         = elt->value.obj;
                    _data          = _train->data;
                    _label         = _train->label;

                    if (disp_flags & RPN_DISP_VALUE) {
                         printf(" <%d> ", _label->value);
                    }
                    else {
                         printf("%-*s [idx = %7d, orig = %7d] '%d' %6d -- [%3d x %3d] %-7s",
                                _sz, "TRAINING_ELT",
                                _data->index, _data->orig_index,
                                _label->value, _data->nb_elts * _data->elt_size,
                                _data->num_rows, _data->num_cols,
                                _data->ignore ? "IGNORED" : "USED");
                    }
               }
               break;

          case RPN_TYPE_TRAINING_SET:
               {
                    rpn_training_set         *_train_set;

                    _train_set     = elt->value.obj;

                    if (disp_flags & RPN_DISP_VALUE) {
                         printf(" <%s> ", _train_set->name);
                    }
                    else {
                         printf("%-*s '%s'", _sz, "TRAINING_SET", _train_set->name);
                         if (disp_flags & RPN_DISP_INFOS) {
                              printf(" %5d elts, current idx = %d",
                                     _train_set->nb_elts, _train_set->current_idx);
                         }
                    }
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

                    if (disp_flags & RPN_DISP_VALUE) {
                         printf(" <%d> ", _label->value);
                    }
                    else {
                         printf("%-*s [idx = %7d, orig = %7d] '%d' %6d -- [%3d x %3d]",
                                _sz, "TEST_ELT    ",
                                _data->index, _data->orig_index,
                                _label->value, _data->nb_elts * _data->elt_size,
                                _data->num_rows, _data->num_cols);
                    }
               }
               break;

          case RPN_TYPE_TEST_SET:
               {
                    rpn_training_set         *_train_set;

                    _train_set     = elt->value.obj;

                    if (disp_flags & RPN_DISP_VALUE) {
                         printf(" <%s> ", _train_set->name);
                    }
                    else {
                         printf("%-*s '%s'", _sz, "TEST_SET    ", _train_set->name);
                         if (disp_flags & RPN_DISP_INFOS) {
                              printf(" %5d elts, current idx = %d",
                                     _train_set->nb_elts, _train_set->current_idx);
                         }
                    }
               }
               break;

		case	RPN_TYPE_TEXT_FILE:
               if (disp_flags & RPN_DISP_VALUE) {
                    printf(" '%s' ", elt->value.s);
               }
               else {
                    printf("%-*s '%s'", _sz, "TEXT_FILE", elt->value.s);
               }
               break;

          case RPN_TYPE_LIST:
               {
                    rpn_list            *_list;

                    _list               = elt->value.obj;

                    printf("LIST      [%s] %6d elts (%s)",
                           _list->name, _list->num_elts, _list->homogeneous ? "Homogeneous" : "Heterogeneous");

                    if (disp_flags & RPN_DISP_VALUE) {
                         printf(" elt1 ... elt%d\n", _list->num_elts);
                         printf("\n");
                    }
                    
               }
               break;

          case RPN_TYPE_TUPLE:
               {
                    rpn_list            *_list;
				rpn_elt			*_elt;

                    _list               = elt->value.obj;

                    printf("TUPLE     [%s] %6d elts (%s)",
                           _list->name, _list->num_elts, _list->homogeneous ? "Homogeneous" : "Heterogeneous");

                    if (disp_flags & RPN_DISP_VALUE) {
                         printf(" elt1 ... elt%d\n", _list->num_elts);
                         printf("\n");
					for (_elt = _list->top_elt; _elt != 0; _elt = _elt->previous_elt) {
						rpn_disp_elt(_elt, disp_flags);
					}
                    }
               }
               break;

          case RPN_TYPE_OPAIR:
               {
                    rpn_pair            *_pair;
				int				 _a_type, _b_type;
//                    rpn_list            *_list_a, *_list_b;
//                    char                *_str_a, *_str_b;

                    _pair               = elt->value.obj;

                    if (!(disp_flags & RPN_DISP_VALUE)) {
                         printf("OPAIR     [%s] ", _pair->name);
                    }
                    else {
                         rpn_elt             *_elt_a, *_elt_b;
                         char                *_a_str_type, *_b_str_type;

                         _elt_a              = _pair->elt_a;
                         _elt_b              = _pair->elt_b;

                         _a_type             = rpn_get_type(_elt_a);
                         _b_type             = rpn_get_type(_elt_b);

                         _a_str_type         = rpn_type_to_string(_a_type);
                         _b_str_type         = rpn_type_to_string(_b_type);

                         printf("(%s, %s) ", _a_str_type, _b_str_type);


					rpn_disp_elt(_elt_a, disp_flags);
					rpn_disp_elt(_elt_b, disp_flags);
                    }
                    if (disp_flags & RPN_DISP_NEWLINE) {
                         printf("\n");
                    }
               }
               break;

          case RPN_TYPE_TEXT :
               if (disp_flags & RPN_DISP_VALUE) {
                    printf(" '%s' ", elt->value.s);
               }
               else {
                    printf("%-*s '%s'", _sz, "TEXT", elt->value.s);
               }
               break;

          case RPN_TYPE_CLONE :
               if (disp_flags & RPN_DISP_VALUE) {
                    printf(" <%s> ", "CLONE");
               }
               else {
                    printf("%-*s ", _sz, "CLONE");
               }
               break;

		case	RPN_TYPE_COEF_A_B:
			{
				rpn_coef_a_b			*_coef_a_b;

				if (disp_flags & RPN_DISP_VALUE) {
					printf(" %s ", "COEF_A_B");
				}
				else {
					printf("%-*s ", _sz, "COEF_A_B");
				}

				_coef_a_b				= elt->value.obj;
                    printf("%16.9e %16.9e", _coef_a_b->a, _coef_a_b->b);
			}
			break;

		case	RPN_TYPE_MIN_MAX:
			{
				rpn_min_max			*_min_max;

				if (disp_flags & RPN_DISP_VALUE) {
					printf(" %s ", "MIN_MAX");
				}
				else {
					printf("%-*s ", _sz, "MIN_MAX");
				}

				_min_max				= elt->value.obj;
                    printf("%16.9e %16.9e", _min_max->min, _min_max->max);
			}
			break;

          default :
               fprintf(stderr, "%s: (%s) unknown element type (%d)\n",
                       G.progname, _func, _type);
               RPN_INTERNAL_ERROR;
          }
     }
     else {
//          printf("%-*s ", _sz, "");
     }

     if (disp_flags & RPN_DISP_NEWLINE) {
          printf("\n");
     }
}

/* rpn_disp_elt() }}} */
/* rpn_disp_stack() {{{ */
/******************************************************************************

                         RPN_DISP_STACK

******************************************************************************/
void rpn_disp_stack(rpn_stack *stack)
{
     rpn_elt             *_elt;
     int                  _num     = 0;
     char                *_reg;
     int                  _name_sz = 20;

     if (stack->base_elt == 0) {
          printf("%-*s STACK EMPTY\n", _name_sz, stack->name);
     }
     else {
          _num      = stack->num_elts;
          for (_elt = stack->base_elt; _elt != 0; _elt = _elt->next_elt, _num--) {
               switch (_num) {

               case 4:
                    _reg      = "T";
                    break;

               case 3:
                    _reg      = "Z";
                    break;

               case 2:
                    _reg      = "Y";
                    break;

               case 1:
                    _reg      = "X";
                    break;

               default:
                    _reg      = "";
                    break;
               }

               printf("%-*s STACK %3d %3s : ", _name_sz, stack->name, _num, _reg);
               rpn_disp_elt(_elt, RPN_DISP_VALUE);

               printf("\n");
          }
     }

     printf("\n");
}

/* rpn_disp_stack() }}} */
/* rpn_disp_stk() {{{ */
/******************************************************************************

                         RPN_DISP_STK

******************************************************************************/
void rpn_disp_stk(rpn_stack *stack)
{
     rpn_elt             *_elt;
     int                  _num     = 0;
     char                *_reg;
     int                  _name_sz = 20;

     if (stack->base_elt == 0) {
          printf("%-*s STACK EMPTY\n", _name_sz, stack->name);
     }
     else {
		printf("\n");
          _num      = stack->num_elts;
          for (_elt = stack->base_elt; _elt != 0; _elt = _elt->next_elt, _num--) {
               switch (_num) {

               case 4:
                    _reg      = "T";
                    break;

               case 3:
                    _reg      = "Z";
                    break;

               case 2:
                    _reg      = "Y";
                    break;

               case 1:
                    _reg      = "X";
                    break;

               default:
                    _reg      = "";
                    break;
               }

               printf("%-*s STACK %3d %3s : ", _name_sz, stack->name, _num, _reg);
               rpn_disp_elt(_elt, RPN_DISP_INFOS);

               printf("\n");
          }
     }

     printf("\n");
}

/* rpn_disp_stk() }}} */
/* rpn_disp_sigma() {{{ */
/******************************************************************************

                         RPN_DISP_SIGMA

******************************************************************************/
void rpn_disp_sigma(struct rpn_sigma *sigma)
{
//     int                  _i;

//printf("%s(%p):\n", __func__, sigma);
//printf("sigma(%p) : &sigma->reg[0] = %p\n", sigma, &sigma->reg[0]);

#if 0
     for (_i = 0; _i < RPN_NB_SIGMA; _i++) {
//printf("SIGMA : reg[%d] : %p\n", _i, sigma->reg[_i]);
          rpn_disp_elt(sigma->reg[_i], RPN_DISP_VALUE|RPN_DISP_NEWLINE);
     }
#endif

     printf("SIGMA : SUM(x)   : ");
     rpn_disp_elt(sigma->reg[RPN_SIGMA_IDX_SUM_X], RPN_DISP_VALUE|RPN_DISP_NEWLINE);

     printf("SIGMA : SUM(x^2) : ");
     rpn_disp_elt(sigma->reg[RPN_SIGMA_IDX_SUM_X2], RPN_DISP_VALUE|RPN_DISP_NEWLINE);

     printf("SIGMA : SUM(y)   : ");
     rpn_disp_elt(sigma->reg[RPN_SIGMA_IDX_SUM_Y], RPN_DISP_VALUE|RPN_DISP_NEWLINE);

     printf("SIGMA : SUM(y^2) : ");
     rpn_disp_elt(sigma->reg[RPN_SIGMA_IDX_SUM_Y2], RPN_DISP_VALUE|RPN_DISP_NEWLINE);

     printf("SIGMA : SUM(xy)  : ");
     rpn_disp_elt(sigma->reg[RPN_SIGMA_IDX_SUM_XY], RPN_DISP_VALUE|RPN_DISP_NEWLINE);

     printf("SIGMA : N        : ");
     rpn_disp_elt(sigma->reg[RPN_SIGMA_IDX_N], RPN_DISP_VALUE|RPN_DISP_NEWLINE);

}
/* rpn_disp_sigma() }}} */
/* rpn_disp_dyn_module_node() {{{ */
/******************************************************************************

                         RPN_DISP_DYN_MODULE_NODE

******************************************************************************/
void rpn_disp_dyn_module_node(ci_ref_node node)
{
#if 0
     dl_op_dyn_node			*_dyn_op;
     struct rpn_module        *_module;
     struct rpn_operator      *_op;

     _dyn_op                  = node->data;
     _module                  = _dyn_op->module;
     _op                      = _dyn_op->operator;
     printf("[%s] : %s\n", _module->name, _op->op_name);
#endif
}

/* rpn_disp_dyn_module_node() }}} */
/* rpn_disp_ops_tree() {{{ */
/******************************************************************************

                         RPN_DISP_OPS_TREE

******************************************************************************/
void rpn_disp_ops_tree()
{
	int					 _nb;

	_nb					= G.modules_tree.nb;
	printf("===== %d imported module%s : =====\n", _nb, _nb > 1 ? "s" : "");
     (void) ci_traversal(&G.modules_tree, dl_disp_module_node, CI_T_LNR);
	printf("\n");

	dl_reset_count_op_defs();
     (void) ci_traversal(&G.ops_tree_v2, dl_count_op_defs, CI_T_LNR);
     printf("===== %4llu imported operators (%u definitions) : =====\n",
	       G.ops_tree_v2.nb, dl_get_count_op_defs());
//     (void) ci_traversal(&G.ops_tree_v2, rpn_disp_dyn_module_node, CI_T_LNR);
     (void) ci_traversal(&G.ops_tree_v2, dl_disp_op_dyn_node, CI_T_LNR);
}

/* rpn_disp_ops_tree() }}} */
/* rpn_disp_argp() {{{ */

/******************************************************************************

                         RPN_DISP_ARGP

******************************************************************************/
void rpn_disp_argp(char *mesg, char **argp)
{
     char                **_p;
     bool                  _first = TRUE;

     if (mesg) {
          printf("%s : ", mesg);
     }
     putchar('[');

     for (_p = argp; *_p != 0; _p++) {
          if (_first) {
               _first    = FALSE;
          }
          else {
               putchar(' ');
          }
          printf("'%s'", *_p);
     }

     putchar(' ');
     putchar(']');
     putchar('\n');
}

/* rpn_disp_argp() }}} */
/* rpn_disp_prompt() {{{ */

/******************************************************************************

					RPN_DISP_PROMPT

******************************************************************************/
void rpn_disp_prompt()
{
	if (!G.silent) {
		if (G.show_prompt || isatty(0)) {
			printf("%s", G.prompt);
		}
		G.show_prompt			= FALSE;
	}
}
/* rpn_disp_prompt() }}} */
