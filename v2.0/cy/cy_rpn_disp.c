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
 *   @(#)  [MB] cy_rpn_disp.c Version 1.58 du 21/11/30 - 
 */

#include  <unistd.h>
#include  "cy_rpn_header.h"
#include  "cy_rpn_proto.h"
#include	"cy_rpn_epub.h"
#include  "../ci/ci_epub.h"
#include  "../dl/dl_cpub.h"
#include  "../dl/dl_epub.h"

/* rpn_undefined_disp_elt() {{{ */

/******************************************************************************

					RPN_UNDEFINED_DISP_ELT

******************************************************************************/
void rpn_undefined_disp_elt(rpn_elt *elt, int disp_flags)
{
	printf("*** Undefined display function for element type %d !\n", 
	       rpn_get_type(elt));
	exit(RPN_EXIT_INTERNAL_ERR);
}

/* rpn_undefined_disp_elt() }}} */
/* rpn_disp_elt() {{{ */
/******************************************************************************

                         RPN_DISP_ELT

******************************************************************************/
void rpn_disp_elt(rpn_elt *elt, int disp_flags)
{
     const char          *_func    = __func__;
     int                  _type, _retcode = RPN_RET_OK;
     int                  _sz      = 10;
#if 0
     struct rpn_matrix   *_matrix;
#endif

     if (elt != NULL) {
          if (elt->name != 0) {
               printf("[[%s]] ", elt->name);
          }

          switch (_type = rpn_get_type(elt)) {

          case RPN_TYPE_NIL :
               if (!(disp_flags & RPN_DISP_NO_TYPE)) {
                    printf("%-*s ", _sz, "NIL");
               }
               printf("%10s", "NIL");
               break;

          case RPN_TYPE_INT :
               if (!(disp_flags & RPN_DISP_NO_TYPE)) {
                    printf("%-*s ", _sz, "INT");
               }
               printf("%10d", elt->value.i);
               break;

          case RPN_TYPE_DOUBLE :
               if (!(disp_flags & RPN_DISP_NO_TYPE)) {
                    printf("%-*s ", _sz, "DOUBLE");
               }
//             printf("%15.9e", elt->value.d);
               printf("%16.9g", elt->value.d);
               break;

          case RPN_TYPE_STRING :
               if (!(disp_flags & RPN_DISP_NO_TYPE)) {
                    printf("%-*s ", _sz, "STRING");
               }
			if (elt->value.s) {
				printf("\"%s\"", elt->value.s);
			}
			else {
				printf("<NIL>");
			}
               break;

          case RPN_TYPE_LITTERAL :
			{
				char				*_s;

				if (!(disp_flags & RPN_DISP_NO_TYPE)) {
					printf("%-*s ", _sz, "LITTERAL");
				}
				_s				= rpn_litteral_value(elt);
				if (_s) {
					printf("'%s'", _s);
				}
				else {
					printf("<NIL>");
				}
			}
               break;

#if 0
          case RPN_TYPE_HOSTSFILE :
			(*rpn_methods[_type]->disp_elt)(elt, disp_flags);
               break;

          case RPN_TYPE_HOSTS :
			(*rpn_methods[_type]->disp_elt)(elt, disp_flags);
               break;

          case RPN_TYPE_MATRIX :
               {
                    int                  _n, _p, _i, _j, _idx;
                    struct rpn_elt      *_elt;

                    _matrix        = (struct rpn_matrix *) elt->value.obj;
                    _n             = _matrix->n;
                    _p             = _matrix->p;
                    printf("MATRIX    [%d x %d]", _n, _p);
                    if (disp_flags & RPN_DISP_NO_TYPE) {
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
#endif

          case RPN_TYPE_START_MARKER:
               printf("BEGIN");
               break;

#if 0
          case RPN_TYPE_NEURAL_MLP:
			(*rpn_methods[_type]->disp_elt)(elt, disp_flags);
#if 0
               {
                    struct rpn_mlp      *_mlp;

                    _mlp                = (struct rpn_mlp *) elt->value.obj;
                    printf("NEURAL MLP (%d layers, %d neurons, %d weights, activation function : %s)",
                           _mlp->nb_layers, _mlp->nb_neurons, _mlp->nb_weights, _mlp->str_af);
               }
#endif
               break;
#endif /* 0 */

          case RPN_TYPE_FILENAME:
               if (!(disp_flags & RPN_DISP_NO_TYPE)) {
                    printf("%-*s ", _sz, "FILENAME");
               }
			if (elt->value.s) {
				printf("'%s'", elt->value.s);
			}
               break;

#if 0
          case RPN_TYPE_TRAINING_ELT:
			(*rpn_methods[_type]->disp_elt)(elt, disp_flags);
#if 0
               {
                    struct rpn_training_elt       *_train;
                    struct rpn_training_data      *_data;
                    struct rpn_training_label     *_label;

                    _train         = elt->value.obj;
                    _data          = _train->data;
                    _label         = _train->label;

                    if (disp_flags & RPN_DISP_NO_TYPE) {
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
#endif /* 0 */
               break;
#endif /* 0 */

#if 0
          case RPN_TYPE_TRAINING_SET:
			(*rpn_methods[_type]->disp_elt)(elt, disp_flags);
#if 0
               {
                    rpn_training_set         *_train_set;

                    _train_set     = elt->value.obj;

                    if (disp_flags & RPN_DISP_NO_TYPE) {
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
#endif /* 0 */
               break;

          case RPN_TYPE_TEST_ELT:
			(*rpn_methods[_type]->disp_elt)(elt, disp_flags);
#if 0
               {
                    struct rpn_training_elt       *_train;
                    struct rpn_training_data      *_data;
                    struct rpn_training_label     *_label;

                    _train         = elt->value.obj;
                    _data          = _train->data;
                    _label         = _train->label;

                    if (disp_flags & RPN_DISP_NO_TYPE) {
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
#endif /* 0 */
               break;

          case RPN_TYPE_TEST_SET:
			(*rpn_methods[_type]->disp_elt)(elt, disp_flags);
#if 0
               {
                    rpn_training_set         *_train_set;

                    _train_set     = elt->value.obj;

                    if (disp_flags & RPN_DISP_NO_TYPE) {
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
#endif /* 0 */
               break;
#endif /* 0 */

          case RPN_TYPE_TEXT_FILE:
               if (!(disp_flags & RPN_DISP_NO_TYPE)) {
                    printf("%-*s ", _sz, "TEXT_FILE");
               }
               printf(" '%s' ", elt->value.s);
               break;

          case RPN_TYPE_LIST:
               {
                    rpn_list            *_list;

                    _list               = elt->value.obj;

                    printf("LIST      [%s] %6d elts (%s)",
                           _list->name, _list->num_elts, _list->homogeneous ? "Homogeneous" : "Heterogeneous");

                    if (disp_flags & RPN_DISP_NO_TYPE) {
                         printf(" elt1 ... elt%d\n", _list->num_elts);
                         printf("\n");
                    }
                    
               }
               break;

          case RPN_TYPE_TUPLE:
               {
                    rpn_list            *_list;
                    rpn_elt             *_elt;

                    _list               = elt->value.obj;

                    printf("TUPLE     [%s] %6d elts (%s)",
                           _list->name, _list->num_elts, _list->homogeneous ? "Homogeneous" : "Heterogeneous");

                    if (disp_flags & RPN_DISP_NO_TYPE) {
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
                    int                  _a_type, _b_type;
//                    rpn_list            *_list_a, *_list_b;
//                    char                *_str_a, *_str_b;

                    _pair               = elt->value.obj;

                    if (!(disp_flags & RPN_DISP_NO_TYPE)) {
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
               if (!(disp_flags & RPN_DISP_NO_TYPE)) {
                    printf("%-*s ", _sz, "TEXT");
               }
               printf(" '%s' ", elt->value.s);
               break;

          case RPN_TYPE_CLONE :
               if (!(disp_flags & RPN_DISP_NO_TYPE)) {
                    printf("%-*s ", _sz, "CLONE");
               }
               else {
                    printf(" <%s> ", "CLONE");
               }
               break;

          case RPN_TYPE_COEF_A_B:
               {
                    rpn_coef_a_b             *_coef_a_b;

                    if (!(disp_flags & RPN_DISP_NO_TYPE)) {
                         printf("%-*s ", _sz, "COEF_A_B");
                    }
                    else {
                         printf(" %s ", "COEF_A_B");
                    }

                    _coef_a_b                = elt->value.obj;
                    printf("%16.9e %16.9e", _coef_a_b->a, _coef_a_b->b);
               }
               break;

          case RPN_TYPE_MIN_MAX:
               {
                    rpn_min_max              *_min_max;

                    if (!(disp_flags & RPN_DISP_NO_TYPE)) {
                         printf("%-*s ", _sz, "MIN_MAX");
                    }
                    else {
                         printf(" %s ", "MIN_MAX");
                    }
                    _min_max                 = elt->value.obj;
                    printf("%16.9e %16.9e", _min_max->min, _min_max->max);
               }
               break;

          case RPN_TYPE_IPv4:
               {
                    union rpn_IP              _IP;

                    _IP.i                    = elt->value.i;

                    if (!(disp_flags & RPN_DISP_NO_TYPE)) {
                         printf("%-*s ", _sz, "IPv4");
                    }

                    // Little endian !
                    printf("%u.%u.%u.%u",
                           _IP.b[3],
                           _IP.b[2],
                           _IP.b[1],
                           _IP.b[0]);
               }
               break;

          case RPN_TYPE_REGEX:
               {
                    rpn_regex                *_regex;
				char					 _str_cflags[128];
				int					 _first_flag = TRUE;

                    _regex                   = elt->value.obj;
				_str_cflags[0]			= 0;

				if ((_regex->cflags & REG_ICASE) == REG_ICASE) {
					strcpy(_str_cflags, "icase");
					_first_flag			= FALSE;
				}

				if ((_regex->cflags & REG_EXTENDED) == REG_EXTENDED) {
					if (!_first_flag) {
						strcat(_str_cflags, ", ");
					}
					strcat(_str_cflags, "extended");
					_first_flag			= FALSE;
				}

				if ((_regex->cflags & REG_NOSUB) == REG_NOSUB) {
					if (!_first_flag) {
						strcat(_str_cflags, ", ");
					}
					strcat(_str_cflags, "nosub");
					_first_flag			= FALSE;
				}

				if ((_regex->cflags & REG_NEWLINE) == REG_NEWLINE) {
					if (!_first_flag) {
						strcat(_str_cflags, ", ");
					}
					strcat(_str_cflags, "newline");
					_first_flag			= FALSE;
				}

                    if (!(disp_flags & RPN_DISP_NO_TYPE)) {
                         printf("%-*s ", _sz, "REGEX");
                    }

                    printf("'%s' [%s]", _regex->expr, _str_cflags);
               }
               break;

          default :
			if (_type <= RPN_MAX_TYPE) {
				(*rpn_methods[_type]->disp_elt)(elt, disp_flags);
			}
			else {
				_retcode			= RPN_RET_INVALID_TYPE;
			}
			break;
          }
     }
     else {
//          printf("%-*s ", _sz, "");
     }

	if (_retcode != RPN_RET_OK) {
		fprintf(stderr, "%s: (%s) unknown element type (%d)\n",
			   G.progname, _func, _type);
		RPN_INTERNAL_ERROR;
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
               rpn_disp_elt(_elt, RPN_DISP_NO_TYPE);

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
          rpn_disp_elt(sigma->reg[_i], RPN_DISP_NO_TYPE|RPN_DISP_NEWLINE);
     }
#endif

     printf("SIGMA : SUM(x)   : ");
     rpn_disp_elt(sigma->reg[RPN_SIGMA_IDX_SUM_X], RPN_DISP_NO_TYPE|RPN_DISP_NEWLINE);

     printf("SIGMA : SUM(x^2) : ");
     rpn_disp_elt(sigma->reg[RPN_SIGMA_IDX_SUM_X2], RPN_DISP_NO_TYPE|RPN_DISP_NEWLINE);

     printf("SIGMA : SUM(y)   : ");
     rpn_disp_elt(sigma->reg[RPN_SIGMA_IDX_SUM_Y], RPN_DISP_NO_TYPE|RPN_DISP_NEWLINE);

     printf("SIGMA : SUM(y^2) : ");
     rpn_disp_elt(sigma->reg[RPN_SIGMA_IDX_SUM_Y2], RPN_DISP_NO_TYPE|RPN_DISP_NEWLINE);

     printf("SIGMA : SUM(xy)  : ");
     rpn_disp_elt(sigma->reg[RPN_SIGMA_IDX_SUM_XY], RPN_DISP_NO_TYPE|RPN_DISP_NEWLINE);

     printf("SIGMA : N        : ");
     rpn_disp_elt(sigma->reg[RPN_SIGMA_IDX_N], RPN_DISP_NO_TYPE|RPN_DISP_NEWLINE);

}
/* rpn_disp_sigma() }}} */
/* rpn_disp_dyn_module_node() {{{ */
/******************************************************************************

                         RPN_DISP_DYN_MODULE_NODE

******************************************************************************/
void rpn_disp_dyn_module_node(ci_ref_node node)
{
#if 0
     dl_op_dyn_node           *_dyn_op;
     struct rpn_module        *_module;
     struct rpn_operator      *_op;

     _dyn_op                  = node->data;
     _module                  = _dyn_op->module;
     _op                      = _dyn_op->operator;
     printf("[%s] : %s\n", _module->name, _op->op_name);
#endif
}

/* rpn_disp_dyn_module_node() }}} */
/* rpn_disp_loaded_modules() {{{ */
/******************************************************************************

                         RPN_DISP_LOADED_MODULES

******************************************************************************/
void rpn_disp_loaded_modules()
{
     int                       _nb;

     _nb                      = G.modules_tree.nb;
     printf("===== %d imported module%s : =====\n", _nb, _nb > 1 ? "s" : "");
     (void) ci_traversal(&G.modules_tree, dl_disp_module_node, CI_T_LNR);
     printf("\n");
}

/* rpn_disp_loaded_modules() }}} */
/* rpn_disp_ops_tree() {{{ */
/******************************************************************************

                         RPN_DISP_OPS_TREE

******************************************************************************/
void rpn_disp_ops_tree()
{
     int                       _nb;

     _nb                      = G.modules_tree.nb;
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
			fflush(stdout);
///* XXX */		printf("\n");
          }
          G.show_prompt            = FALSE;
     }
}
/* rpn_disp_prompt() }}} */
