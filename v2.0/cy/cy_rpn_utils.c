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
 *   @(#)  [MB] cy_rpn_utils.c Version 1.99 du 21/11/11 - 
 */

#include  "cy_rpn_header.h"
#include  "cy_epri.h"
#include  "cy_rpn_proto.h"
#include  <time.h>
#include  <sys/time.h>
#include  <sys/stat.h>
#include  <unistd.h>
#include  "../ci/ci_cpub.h"
#include  "../ci/ci_epub.h"
#include  "../dl/dl_cpub.h"
#include  "../dl/dl_epub.h"

struct global_struct     G         = { 0 };

struct rpn_methods		*rpn_methods[RPN_MAX_TYPE + 1];

/* rpn_new_stack() {{{ */
/******************************************************************************

                         RPN_NEW_STACK

******************************************************************************/
rpn_stack *rpn_new_stack(const char *stack_name)
{
     rpn_stack           *_stack;

     if ((_stack = (rpn_stack *) RPN_MALLOC(sizeof(*_stack))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _stack->name        = strdup(stack_name);
     _stack->num_elts    = 0;
     _stack->lastx       = rpn_new_elt(RPN_TYPE_NIL);
     _stack->last_op     = NULL;
     _stack->clear_x     = FALSE;
     _stack->new_value   = FALSE;
     _stack->nb_begin    = 0;
     _stack->base_elt    = 0;
     _stack->top_elt     = 0;

     return _stack;
}

/* rpn_new_stack() }}} */
/* rpn_new_elt() {{{ */
/******************************************************************************

                         RPN_NEW_ELT

******************************************************************************/
rpn_elt *rpn_new_elt(int type)
{
     rpn_elt             *_elt;

//M
     if ((_elt = (rpn_elt *) RPN_MALLOC(sizeof(*_elt))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
//M

     _elt->name          = 0;
     _elt->type          = type;
     _elt->clone_level   = 0;
     _elt->value.ll      = 0;
     _elt->previous_elt  = 0;
     _elt->next_elt      = 0;

     return _elt;
}

/* rpn_new_elt() }}} */
/* rpn_new_named_elt() {{{ */
/******************************************************************************

                         RPN_NEW_NAMED_ELT

******************************************************************************/
rpn_elt *rpn_new_named_elt(int type, char *name)
{
     rpn_elt             *_elt;

//M
     if ((_elt = (rpn_elt *) RPN_MALLOC(sizeof(*_elt))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
//M

     _elt->name          = strdup(name);
     _elt->type          = 0;
     _elt->type          = type;
     _elt->value.i       = 0;
     _elt->previous_elt  = 0;
     _elt->next_elt      = 0;

     return _elt;
}

/* rpn_new_named_elt() }}} */
/* rpn_set_elt_name() {{{ */
/******************************************************************************

                         RPN_SET_ELT_NAME

******************************************************************************/
void rpn_set_elt_name(rpn_elt *elt, char *name)
{
     if (elt == 0) {
          RPN_INTERNAL_ERROR;
     }

     if (elt->name != 0) {
          free(elt->name);
     }

	if (name != 0) {
		elt->name      = strdup(name);
	}
	else {
		elt->name		= 0;
	}
}

/* rpn_new_named_elt() }}} */
/* rpn_new_int() {{{ */
/******************************************************************************

                         RPN_NEW_INT

******************************************************************************/
rpn_elt *rpn_new_int(int i)
{
     rpn_elt             *_elt;

     _elt                = rpn_new_elt(RPN_TYPE_INT);
     _elt->value.i       = i;

     return _elt;
}

/* rpn_new_int() }}} */
/* rpn_new_double() {{{ */
/******************************************************************************

                         RPN_NEW_DOUBLE

******************************************************************************/
rpn_elt *rpn_new_double(double d)
{
     rpn_elt             *_elt;

     _elt                = rpn_new_elt(RPN_TYPE_DOUBLE);
     _elt->value.d       = d;

     return _elt;
}

/* rpn_new_double() }}} */
/* rpn_free_stack() {{{ */
/******************************************************************************

                         RPN_FREE_STACK

******************************************************************************/
void rpn_free_stack(rpn_stack *stack)
{
     rpn_elt             *_elt;
//   int                  _type;

     if (stack == NULL) {
          RPN_INTERNAL_ERROR;
     }

     if (stack->num_elts != 0) {
          _elt                     = rpn_pop(stack);
//        _type                    = rpn_get_type(_elt);
          rpn_push(stack, _elt);
//          printf("%s() [%s (%d]) : stack \"%s\" not empty, %d remaining element(s) -- X type = %s\n",
//                 __func__, __FILE__, __LINE__, stack->name, stack->num_elts, rpn_type_to_string(_type));

          /* Free all elements of the stack
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          for (; stack->num_elts > 0; ) {
               _elt                     = rpn_pop(stack);
//             _type                    = rpn_get_type(_elt);
//             printf("%s() : free(%s)\n", __func__, rpn_type_to_string(_type));
               rpn_free_elt(&_elt);
          }
     }

     /* Free lastx
        ~~~~~~~~~~ */
     _elt                     = stack->lastx;
//   _type                    = rpn_get_type(_elt);
//X
//     printf("%s(%s) : free(%s) (LASTX)\n", __func__, stack->name,
//            rpn_type_to_string(_type));
     rpn_free_elt(&_elt);

     /* Free stack name
        ~~~~~~~~~~~~~~~ */
     RPN_FREE(stack->name);

     /* Free stack descriptor
        ~~~~~~~~~~~~~~~~~~~~~ */
     RPN_FREE(stack);
//printf("END of %s\n", __func__);
}

/* rpn_free_stack() }}} */
/* rpn_undefined_free_elt() {{{ */

/******************************************************************************

					RPN_UNDEFINED_FREE_ELT

******************************************************************************/
void rpn_undefined_free_elt(rpn_elt *elt, int type)
{
	printf("*** Undefined free function for element type %d !\n", type);
	exit(RPN_EXIT_INTERNAL_ERR);
}

/* rpn_undefined_free_elt() }}} */
/* rpn_free_elt() {{{ */
/******************************************************************************

                         RPN_FREE_ELT

******************************************************************************/
void rpn_free_elt(rpn_elt **ref_elt)
{
     rpn_elt             *_elt;
     int                  _type;

     if (ref_elt == NULL) {
          fprintf(stderr, "%s() : ref_elt = %p\n", __func__, ref_elt);
          RPN_INTERNAL_ERROR;
     }
     if (*ref_elt == NULL) {
          fprintf(stderr, "%s() : *ref_elt = %p\n", __func__, *ref_elt);
          RPN_INTERNAL_ERROR;
     }

     _elt                = *ref_elt;
	if (_elt->name) {
		rpn_free(_elt->name);
		_elt->name		= 0;
	}
     _type               = rpn_get_type(_elt);

     /* Make sure the element is not linked to other ones
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if ((_elt->previous_elt != NULL)
     ||  (_elt->next_elt     != NULL)) {
		printf("%s() : elt %p is linked to another one :\n", __func__, _elt);
          printf("%s() (%s: %d) : _elt->previous = %p _elt->next_elt = %p\n",
                 __func__, __FILE__, __LINE__, _elt->previous_elt, _elt->next_elt);
          RPN_INTERNAL_ERROR;
     }

     switch (_type) {

     case RPN_TYPE_NIL:
     case RPN_TYPE_INT:
     case RPN_TYPE_DOUBLE:
	case	RPN_TYPE_IPv4:
          /* Free element */
          RPN_FREE(_elt);
          break;

     case RPN_TYPE_STRING:
     case RPN_TYPE_FILENAME:
          if (_elt->value.s) {
               /* Free string */
               RPN_FREE(_elt->value.s);
          }

          /* Free element */
          RPN_FREE(_elt);
          break;

     case RPN_TYPE_HOSTSFILE:
		(*rpn_methods[_type]->free_elt)(_elt, _type);
		break;

     case RPN_TYPE_LITTERAL:
		{
			rpn_litteral			*_litteral;

			_litteral				= _elt->value.obj;
			if (_litteral) {
				if (_litteral->value) {
					RPN_FREE(_litteral->value);
				}
				RPN_FREE(_litteral);
			}
		}

          /* Free element */
          RPN_FREE(_elt);
		break;

     case RPN_TYPE_MATRIX:
          {
               rpn_elt				*_sub_elt;
               rpn_matrix			*_mat;
               int                       _idx;

               _mat                = (rpn_matrix *) _elt->value.obj;

               for (_idx = 0; _idx < (_mat->n * _mat->p); _idx++) {
                    _sub_elt                 = (rpn_elt *) _mat->base[_idx];

                    if (_sub_elt != NULL) {
                         /* Free sub element
                            ~~~~~~~~~~~~~~~~ */
                         rpn_free_elt(&_sub_elt);
                    }
               }

               /* Free matrix
                  ~~~~~~~~~~~ */
               RPN_FREE(_mat);
          }

          /* Free element */
          RPN_FREE(_elt);
          break;

     case RPN_TYPE_TRAINING_ELT:
     case RPN_TYPE_TEST_ELT:
		(*rpn_methods[_type]->free_elt)(_elt, _type);
#if 0
          {
               struct rpn_training_elt       *_train;
               struct rpn_training_data      *_data;
//             struct rpn_training_label     *_label;

               _train                        = _elt->value.obj;
               _data                         = _train->data;
//             _label                        = _train->label;

               RPN_FREE(_train->label);
               RPN_FREE(_data->vector);
               RPN_FREE(_data);
               RPN_FREE(_train);
          }

          /* Free element */
          RPN_FREE(_elt);
#endif /* 0 */
          break;

     case RPN_TYPE_TRAINING_SET:
     case RPN_TYPE_TEST_SET:
		(*rpn_methods[_type]->free_elt)(_elt, _type);
#if 0
          {
               int                       _nb;
               rpn_training_set         *_train_set;

               _train_set               = _elt->value.obj;

               /* Free name */
               RPN_FREE(_train_set->name);

               /* Free all elements in the array */
               for (_nb = 0; _nb < _train_set->nb_elts; _nb++) {
                    rpn_free_elt(&_train_set->array[_nb]);
               }

               /* Free array */
               RPN_FREE(_train_set);
          }

          /* Free element */
          RPN_FREE(_elt);
#endif /* 0 */
          break;

     case RPN_TYPE_TEXT_FILE:
          {
               rpn_text_file            *_text_file;
               int                       _i;

               _text_file               = _elt->value.obj;
               if (!_text_file->static_data) {
                    /* Free data_structure
                       ~~~~~~~~~~~~~~~~~~~ */
                    RPN_FREE(_text_file->dirname);

                    RPN_FREE(_text_file->filename);
                    for (_i = 0; _text_file->text[_i] != NULL; _i++) {
                         RPN_FREE(_text_file->text[_i]);
                    }

                    RPN_FREE(_text_file);
               }
               else {
                    /* Structure is statically defined : nothing to do
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               }
          }
          break;

     case RPN_TYPE_LIST:
     case RPN_TYPE_TUPLE:
          {
               rpn_list                 *_list;

               for (_list = _elt->value.obj; _list->num_elts > 0; ) {
                    _elt                     = rpn_list_pop_head(_list);
                    rpn_free_elt(&_elt);
               }

               RPN_FREE(_list->name);
          }
          break;

     case RPN_TYPE_OPAIR:
          {
               rpn_pair                 *_pair;

               _pair				= _elt->value.obj;

			if (_pair->elt_a) {
				rpn_free_elt(&_pair->elt_a);
			}
			if (_pair->elt_b) {
				rpn_free_elt(&_pair->elt_b);
			}

               RPN_FREE(_pair->name);
          }
          break;

     case RPN_TYPE_COEF_A_B:
          {
               rpn_coef_a_b			*_coef_a_b;

               _coef_a_b               = _elt->value.obj;

			if (_coef_a_b) {
				RPN_FREE(_coef_a_b);
			}
          }
		break;

     case RPN_TYPE_MIN_MAX:
          {
               rpn_min_max			*_min_max;

               _min_max				= _elt->value.obj;

			if (_min_max) {
				RPN_FREE(_min_max);
			}
          }
		break;

	case	RPN_TYPE_CLONE:
		printf("%s() : temporary type %s should not remain after clone creation !\n",
		       __func__, rpn_type_to_string(_type));
		RPN_INTERNAL_ERROR;
		break;

#if 0
	case	RPN_TYPE_NEURAL_MLP:
		dp_free_mlp(elt);
		break;
#endif

	case	RPN_TYPE_REGEX:
		{
			struct rpn_regex		*_regex;

			_regex				= _elt->value.obj;
			RPN_FREE(_regex->expr);
			RPN_FREE(_regex);
		}
		break;

     default:
		if (_type <= RPN_MAX_TYPE) {
			(*rpn_methods[_type]->free_elt)(_elt, _type);
		}
		else {
			printf("%s() [%s (%d)] element type %d : not managed yet !\n",
				   __func__, __FILE__, __LINE__, _type);
		}
          break;
     }

     /* Reset pointer value
        ~~~~~~~~~~~~~~~~~~~ */
     *ref_elt            = NULL;
}

/* rpn_free_elt() }}} */
/* rpn_undefined_clone_elt() {{{ */

/******************************************************************************

					RPN_UNDEFINED_CLONE_ELT

******************************************************************************/
void rpn_undefined_clone_elt(rpn_elt *elt, rpn_elt *clone)
{
	printf("*** Undefined clone function for element type %d !\n", 
	       rpn_get_type(elt));
	exit(RPN_EXIT_INTERNAL_ERR);
}

/* rpn_undefined_disp_elt() }}} */
/* rpn_clone_elt() {{{ */
/******************************************************************************

                         RPN_CLONE_ELT

******************************************************************************/
rpn_elt *rpn_clone_elt(rpn_elt *elt)
{
     rpn_elt             *_clone;
     int                  _idx, _type, _retcode = RPN_RET_OK;

     /* Create new element with temporary "CLONE" type
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _clone              = rpn_new_elt(RPN_TYPE_CLONE);

     /* Increment clone level
       ~~~~~~~~~~~~~~~~~~~~~~ */
     _clone->clone_level++;
#if 0
if (_clone->clone_level > 10) {		// XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX
	printf("############################## CLONE LEVEL = %d !!! ##############################\n",
	       _clone->clone_level);
	rpn_dump_elt(elt, 0);
	RPN_INTERNAL_ERROR;
}
#endif
     
     /* Clone element name
        ~~~~~~~~~~~~~~~~~~ */
     if (elt->name) {
          _clone->name        = strdup(elt->name);
     }

     switch (_type = elt->type) {

     case RPN_TYPE_MATRIX:
//X
          {
               rpn_elt				*_sub_elt, *_clone_sub;
               rpn_matrix			*_mat, *_clone_mat;
               size_t                    _size;

               _mat                = (rpn_matrix *) elt->value.obj;
               _size               = sizeof(rpn_matrix)
                                   + (((_mat->n *_mat->p) - 1) * sizeof(rpn_elt *));
               _clone->value.obj   = RPN_MALLOC(_size);
               _clone_mat          = _clone->value.obj;
               _clone_mat->n       = _mat->n;
               _clone_mat->p       = _mat->p;

               for (_idx = 0; _idx < (_mat->n * _mat->p); _idx++) {
                    _sub_elt                 = (rpn_elt *) _mat->base[_idx];
//X
                    _clone_sub               = rpn_clone_elt(_sub_elt);
//X
                    _clone_mat->base[_idx]   = _clone_sub;
               }
          }
          break;

     case RPN_TYPE_NIL:
//X
          _clone->value.obj   = NULL;
          break;

     case RPN_TYPE_INT:
          _clone->value.i     = elt->value.i;
          break;

     case RPN_TYPE_DOUBLE:
          _clone->value.d     = elt->value.d;
          break;

     case RPN_TYPE_STRING:
     case RPN_TYPE_FILENAME:
          _clone->value.s     = strdup(elt->value.s);
          break;

     case RPN_TYPE_HOSTSFILE:
		(*rpn_methods[_type]->clone_elt)(elt, _clone);
//          _clone->value.s     = strdup(elt->value.s);
          break;

     case RPN_TYPE_LITTERAL:
		{
			rpn_litteral					*_litteral, *_clone_litteral;

			_litteral						= elt->value.obj;

			_clone_litteral				= rpn_new_litteral();
			_clone_litteral->value			= strdup(_litteral->value);
			_clone_litteral->need_parentheses	= _litteral->need_parentheses;
			_clone->value.obj				= _clone_litteral;
		}
		break;

     case RPN_TYPE_NEURAL_MLP:
          RPN_INTERNAL_ERROR;
          break;

     case RPN_TYPE_TEXT_FILE:
          {
               // TODO
			RPN_INTERNAL_ERROR;
          }
          break;

     case RPN_TYPE_LIST:
     case RPN_TYPE_TUPLE:
          {
               rpn_elt                  *_sub_elt, *_sub_clone;
               rpn_list                 *_list, *_list_clone;

               _list                    = elt->value.obj;

               _list_clone              = rpn_new_list(_list->name);
               for (_sub_elt = _list->base_elt; _sub_elt != NULL;
                                               _sub_elt = _sub_elt->next_elt) {
                    _sub_clone               = rpn_clone_elt(_sub_elt);
                    rpn_list_push_head(_list_clone, _sub_clone);
               }
               _clone->value.obj        = _list_clone;
          }
          break;

     case RPN_TYPE_OPAIR:
          {
               rpn_pair                 *_pair, *_pair_clone;

               _pair                    = elt->value.obj;

               _pair_clone              = rpn_new_pair(strdup(_pair->name));
               _pair_clone->elt_a       = rpn_clone_elt(_pair->elt_a);
               _pair_clone->elt_b       = rpn_clone_elt(_pair->elt_b);

               _clone->value.obj        = _pair_clone;
          }
          break;

	case	RPN_TYPE_MIN_MAX:
		{
			rpn_min_max				*_min_max;
			_min_max					= rpn_new_min_max();
			_min_max->min				= rpn_get_min(elt);
			_min_max->max				= rpn_get_max(elt);

			_clone					= rpn_new_elt(RPN_TYPE_MIN_MAX);
			_clone->value.obj			= _min_max;
			rpn_set_elt_name(_clone, elt->name);
		}
		break;

	case	RPN_TYPE_REGEX:
		{
			rpn_regex					*_regex, *_clone_regex;
			int						 _error;
			char						 _errbuf[256];

			_regex					= elt->value.obj;
			_clone_regex				= rpn_new_regex();
			_clone_regex->expr			= strdup(_regex->expr);
			_clone_regex->cflags		= _regex->cflags;
			_clone_regex->eflags		= _regex->eflags;

			if ((_error = regcomp(&_clone_regex->RE, _clone_regex->expr, _clone_regex->cflags)) != 0) {
				(void) regerror(_error, &_clone_regex->RE, _errbuf, sizeof(_errbuf));
				fprintf(stderr, "regcomp error for \"%s\" : %s\n",
				        _clone_regex->expr, _errbuf);
				exit(RPN_EXIT_REGCOMP_ERROR);
			}
			_clone					= rpn_new_elt(RPN_TYPE_REGEX);
			_clone->value.obj			= _clone_regex;
			rpn_set_elt_name(_clone, elt->name);
		}
		break;

     default:
		{
#if 0
			rpn_stack				*_stack;

			if (G.debug_level & RPN_DBG_DEBUG) {
				printf( "%s() : unknown type (%d)\n", __func__, elt->type);
				RPN_INTERNAL_ERROR;
			}

			_stack				= rpn_new_stack(__func__);
			rpn_push(_stack, _clone);
			rpn_push(_stack, elt);
			if (dl_exec_op(_stack, "clone") == RPN_RET_OK) {
				_clone				= rpn_pop(_stack);
			}
			else {
				_clone				= 0;
				RPN_INTERNAL_ERROR;
			}
			rpn_free_stack(_stack);

			_retcode				= RPN_RET_INVALID_TYPE;
#else
			if (_type <= RPN_MAX_TYPE) {
				_clone->type			= elt->type;
				(*rpn_methods[_type]->clone_elt)(elt, _clone);
			}
			else {
				_retcode				= RPN_RET_INVALID_TYPE;
			}
#endif
		}
          break;
     }

	if (_retcode == RPN_RET_OK) {
		rpn_set_type(_clone, elt->type);
	}
	else {
		printf( "%s() : unknown type (%d)\n", __func__, elt->type);
		RPN_INTERNAL_ERROR;
	}

//printf("%s(): clone type = %s\n", __func__, rpn_type_to_string(_clone->type));
     return _clone;
}

/* rpn_clone_elt() }}} */
/* rpn_push() {{{ */
/******************************************************************************

                         RPN_PUSH

******************************************************************************/
int rpn_push(rpn_stack *stack, rpn_elt *elt)
{
     int             _retcode;
     rpn_elt        *_stk_x;

     /* Consistency check
        ~~~~~~~~~~~~~~~~~ */
     if ((elt->previous_elt != 0)
     ||  (elt->next_elt     != 0)) {
          fprintf(stderr, "%s: element is already used in a stack !\n", G.progname);
          RPN_INTERNAL_ERROR;
     }

     if (stack != 0) {
          if (stack->clear_x) {
               stack->clear_x      = FALSE;
               _stk_x         = rpn_pop(stack);
               rpn_free_elt(&_stk_x);
          }

          if (stack->base_elt == 0) {
               stack->base_elt          = elt;
               stack->top_elt           = elt;

               elt->previous_elt        = 0;
               elt->next_elt            = 0;
          }
          else {
               stack->top_elt->next_elt = elt;

               elt->previous_elt        = stack->top_elt;
               elt->next_elt            = 0;

               stack->top_elt           = elt;
          }
          stack->num_elts++;
          _retcode                 = RPN_RET_OK;
     }
     else {
          _retcode                 = RPN_RET_INVALID_STACK;
     }

     return _retcode;
}

/* rpn_push() }}} */
/* rpn_pop() {{{ */
/******************************************************************************

                         RPN_POP

******************************************************************************/
rpn_elt *rpn_pop(rpn_stack *stack)
{
     rpn_elt        *_elt     = 0;

     if ((stack != 0) && (stack->num_elts != 0)) {
          _elt                     = stack->top_elt;
          stack->top_elt           = _elt->previous_elt;
          if (stack->top_elt != 0) {
               stack->top_elt->next_elt = 0;
          }
          else {
               stack->base_elt     = 0;
          }

          _elt->previous_elt       = 0;
          _elt->next_elt           = 0;

          stack->num_elts--;
     }
     else {
          _elt                     = rpn_new_elt(RPN_TYPE_NIL);
     }

     return _elt;
}

/* rpn_pop() }}} */
/* rpn_base_push() {{{ */
/******************************************************************************

                         RPN_BASE_PUSH

******************************************************************************/
int rpn_base_push(rpn_stack *stack, rpn_elt *elt)
{
     int             _retcode;

     if (stack != 0) {
          if (stack->base_elt == 0) {
               stack->base_elt               = elt;
               stack->top_elt                = elt;

               elt->previous_elt             = 0;
               elt->next_elt                 = 0;
          }
          else {
               stack->base_elt->previous_elt = elt;

               elt->previous_elt             = 0;
               elt->next_elt                 = stack->base_elt;

               stack->base_elt                = elt;
          }
          stack->num_elts++;
          _retcode                 = RPN_RET_OK;
     }
     else {
          _retcode                 = RPN_RET_INVALID_STACK;
     }

     return _retcode;
}

/* rpn_base_push() }}} */
/* rpn_base_pop() {{{ */
/******************************************************************************

                         RPN_BASE_POP

******************************************************************************/
rpn_elt *rpn_base_pop(rpn_stack *stack)
{
     rpn_elt        *_elt     = 0;

     if ((stack != 0) && (stack->num_elts != 0)) {
          _elt                     = stack->base_elt;
          stack->base_elt          = _elt->next_elt;
          if (stack->base_elt != 0) {
               stack->base_elt->previous_elt = 0;
          }
          else {
               stack->top_elt      = 0;
          }

          _elt->previous_elt       = 0;
          _elt->next_elt           = 0;

          stack->num_elts--;
     }
     else {
          _elt                     = rpn_new_elt(RPN_TYPE_NIL);
     }

     return _elt;
}

/* rpn_base_pop() }}} */
/* rpn_list_push_head() {{{ */
/******************************************************************************

                         RPN_LIST_PUSH_HEAD

******************************************************************************/
int rpn_list_push_head(rpn_list *list, rpn_elt *elt)
{
     int                       _retcode, _elt_type;

     _elt_type                = rpn_get_type(elt);

     /* Consistency check
        ~~~~~~~~~~~~~~~~~ */
     if ((elt->previous_elt != 0)
     ||  (elt->next_elt     != 0)) {
          fprintf(stderr, "%s: element is already used in a list !\n", G.progname);
          RPN_INTERNAL_ERROR;
     }

     if (list != 0) {
          if (list->base_elt == 0) {
               list->base_elt           = elt;
               list->top_elt            = elt;

               elt->previous_elt        = 0;
               elt->next_elt            = 0;
          }
          else {
               list->top_elt->next_elt  = elt;

               elt->previous_elt        = list->top_elt;
               elt->next_elt            = 0;

               list->top_elt            = elt;
          }
          list->num_elts++;
          _retcode                 = RPN_RET_OK;
     }
     else {
          _retcode                 = RPN_RET_INVALID_LIST;
     }

     if (list->type == RPN_TYPE_UNDEFINED) {
          list->type               = _elt_type;
     }
     else {
          if (list->type != _elt_type) {
               list->homogeneous        = FALSE;
          }
     }

     return _retcode;
}

/* rpn_list_push_head() }}} */
/* rpn_list_pop_head() {{{ */
/******************************************************************************

                         RPN_LIST_POP_HEAD

******************************************************************************/
rpn_elt *rpn_list_pop_head(rpn_list *list)
{
     rpn_elt        *_elt     = 0;

     if ((list != 0) && (list->num_elts != 0)) {
          _elt                     = list->top_elt;
          list->top_elt            = _elt->previous_elt;
          if (list->top_elt != 0) {
               list->top_elt->next_elt  = 0;
          }
          else {
               list->base_elt      = 0;
          }

          _elt->previous_elt       = 0;
          _elt->next_elt           = 0;

          list->num_elts--;
     }
     else {
          _elt                     = rpn_new_elt(RPN_TYPE_NIL);
     }

     return _elt;
}

/* rpn_list_pop_head() }}} */
/* rpn_list_push_tail() {{{ */
/******************************************************************************

                         RPN_LIST_PUSH_TAIL

******************************************************************************/
int rpn_list_push_tail(rpn_list *list, rpn_elt *elt)
{
     int                       _retcode, _elt_type;

     _elt_type                = rpn_get_type(elt);

     /* Consistency check
        ~~~~~~~~~~~~~~~~~ */
     if ((elt->previous_elt != 0)
     ||  (elt->next_elt     != 0)) {
          fprintf(stderr, "%s: element is already used in a list !\n", G.progname);
          RPN_INTERNAL_ERROR;
     }

     if (list != 0) {
          if (list->base_elt == 0) {
               list->base_elt                = elt;
               list->top_elt                 = elt;

               elt->previous_elt             = 0;
               elt->next_elt                 = 0;
          }
          else {
               list->base_elt->previous_elt  = elt;

               elt->previous_elt             = 0;
               elt->next_elt                 = list->base_elt;

               list->base_elt                = elt;
          }
          list->num_elts++;
          _retcode                 = RPN_RET_OK;
     }
     else {
          _retcode                 = RPN_RET_INVALID_LIST;
     }

     if (list->type == RPN_TYPE_UNDEFINED) {
          list->type               = _elt_type;
     }
     else {
          if (list->type != _elt_type) {
               list->homogeneous        = FALSE;
          }
     }

     return _retcode;
}

/* rpn_list_push_tail() }}} */
/* rpn_list_pop_tail() {{{ */
/******************************************************************************

                         RPN_LIST_POP_TAIL

******************************************************************************/
rpn_elt *rpn_list_pop_tail(rpn_list *list)
{
     rpn_elt        *_elt     = 0;

     if ((list != 0) && (list->num_elts != 0)) {
          _elt                     = list->base_elt;
          list->base_elt           = _elt->next_elt;
          if (list->base_elt != 0) {
               list->base_elt->previous_elt  = 0;
          }
          else {
               list->top_elt       = 0;
          }

          _elt->previous_elt       = 0;
          _elt->next_elt           = 0;

          list->num_elts--;
     }
     else {
          _elt                     = rpn_new_elt(RPN_TYPE_NIL);
     }

     return _elt;
}

/* rpn_list_pop_tail() }}} */
/* rpn_new_operator() {{{ */
/******************************************************************************

                         RPN_NEW_OPERATOR

******************************************************************************/
RPN_NEW(operator)

/* rpn_new_operator() }}} */
/* rpn_retcode_to_exit_code() {{{ */

/******************************************************************************

					RPN_RETCODE_TO_EXIT_CODE

******************************************************************************/
int rpn_retcode_to_exit_code(int retcode)
{
	int					 _exit_code;

	switch (retcode) {

	case	RPN_RET_OK:
		_exit_code			= RPN_EXIT_OK;
		break;

	case	RPN_RET_INVALID_ELT:
		_exit_code			= RPN_EXIT_INVALID_ELT;
		break;

	case	RPN_RET_CANNOT_LINK:
		_exit_code			= RPN_EXIT_LINK_ERROR;
		break;

	case	RPN_RET_OPEN_ERROR:
		_exit_code			= RPN_EXIT_OPEN_ERROR;
		break;

	default:
		_exit_code			= RPN_EXIT_OP_ERROR;
		break;
	}

	return _exit_code;
}

/* rpn_retcode_to_exit_code() }}} */
/* rpn_op() {{{ */
/******************************************************************************

                         RPN_OP

******************************************************************************/
int rpn_op(rpn_stack *stack, dl_operator *op)
{
     int                       _retcode;
     struct timeval            _time1, _time2;
     int                       _sec, _usec, _sw_on;

     stack->clear_x      = FALSE;
     stack->last_op      = op;

     if (!G.silent) {
		if (stack->new_value) {
			rpn_disp_elt(stack->top_elt, RPN_DISP_NO_TYPE|RPN_DISP_INFOS);
//			printf("%15s\n", op->op_name);
			printf(" %s\n", op->op_name);
		}
		else {
			rpn_disp_elt(NULL, RPN_DISP_NO_TYPE|RPN_DISP_INFOS);
		}
	}
     stack->new_value    = FALSE;

     if ((_sw_on = G.sw_on)) {
          /* Get system_time
             ~~~~~~~~~~~~~~~ */
          if (gettimeofday(&_time1, 0) != 0) {
               perror("gettimeofday");
               exit(RPN_EXIT_GET_TIME);
          }
     }

     /* Execute operator
        ~~~~~~~~~~~~~~~~ */
     _retcode            =  (*op->func)(stack, op);
fprintf(stderr, "RETURN CODE = %d\n", _retcode);

     if (_sw_on) {
          /* Get system_time
             ~~~~~~~~~~~~~~~ */
          if (gettimeofday(&_time2, 0) != 0) {
               perror("gettimeofday");
               exit(RPN_EXIT_GET_TIME);
          }

          /* Compute delta-T
             ~~~~~~~~~~~~~~~ */
          _sec           = _time2.tv_sec - _time1.tv_sec;
          if (_time2.tv_usec < _time1.tv_usec) {
               _usec          = 1000000 + _time2.tv_usec - _time1.tv_usec;
               _sec           -= 1;
          }
          else {
               _usec          = _time2.tv_usec - _time1.tv_usec;
          }
		if (!G.silent) {
			printf("%s : %6d.%06d s\n", op->op_name, _sec, _usec);
		}
     }

     if (!G.silent && op->nb_args_out > 0) {
          rpn_disp_elt(stack->top_elt, RPN_DISP_NO_TYPE|RPN_DISP_INFOS);
          printf("%15s\n", "***");
     }

	G.exit_code	= rpn_retcode_to_exit_code(_retcode);

     return _retcode;
}

/* rpn_op() }}} */
/* ron_set_lastx() {{{ */
/******************************************************************************

                         RPN_SET_LASTX

******************************************************************************/
void rpn_set_lastx(rpn_stack *stack, rpn_elt *elt)
{
     if (stack->lastx) {
          /* Free previous last X
             ~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&stack->lastx);
     }

     stack->lastx        = rpn_clone_elt(elt);
}

/* ron_set_lastx() }}} */
/* rpn_get_type() {{{ */
/******************************************************************************

                         RPN_GET_TYPE

******************************************************************************/
int rpn_get_type(rpn_elt *elt)
{
     int        _retcode;

     if (elt) {
          _retcode  = elt->type;
     }
     else {
          _retcode  = RPN_RET_INVALID_ELT;
     }

     return _retcode;
}

/* rpn_get_type() }}} */
/* rpn_set_time() {{{ */
/******************************************************************************

                         RPN_SET_TYPE

******************************************************************************/
int rpn_set_type(rpn_elt *elt, int type)
{
     int             _retcode;

     if (elt) {
          elt->type      = type;
          _retcode       = RPN_RET_OK;
     }
     else {
          _retcode       = RPN_RET_INVALID_ELT;
     }

     return _retcode;
}
/* rpn_set_time() }}} */
/* rpn_get_X_type() {{{ */

/******************************************************************************

                         RPN_GET_X_TYPE

******************************************************************************/
int rpn_get_X_type(struct rpn_stack *stack)
{
     int             _retcode;
     struct rpn_elt *_elt;

     if ((stack != 0) && (stack->num_elts != 0)) {
          _elt             = stack->top_elt;
          _retcode         = _elt->type;
     }
     else {
          _retcode       = RPN_RET_INVALID_ELT;
     }

     return _retcode;
}

/* rpn_get_X_type() }}} */
/* rpn_get_Y_type() {{{ */

/******************************************************************************

                         RPN_GET_Y_TYPE

******************************************************************************/
int rpn_get_Y_type(struct rpn_stack *stack)
{
     int             _retcode;
     struct rpn_elt *_elt;

     if ((stack != 0) && (stack->num_elts >= 2)) {
          _elt             = stack->top_elt->previous_elt;
          _retcode         = _elt->type;
     }
     else {
          _retcode       = RPN_TYPE_NIL;
     }

     return _retcode;
}

/* rpn_get_Y_type() }}} */
/* rpn_get_Z_type() {{{ */

/******************************************************************************

                         RPN_GET_Z_TYPE

******************************************************************************/
int rpn_get_Z_type(struct rpn_stack *stack)
{
     int             _retcode;
     struct rpn_elt *_elt;

     if ((stack != 0) && (stack->num_elts >= 3)) {
          _elt             = stack->top_elt->previous_elt->previous_elt;
          _retcode         = _elt->type;
     }
     else {
          _retcode       = RPN_TYPE_NIL;
     }

     return _retcode;
}

/* rpn_get_Z_type() }}} */
/* rpn_get_T_type() {{{ */

/******************************************************************************

                         RPN_GET_T_TYPE

******************************************************************************/
int rpn_get_T_type(struct rpn_stack *stack)
{
     int             _retcode;
     struct rpn_elt *_elt;

     if ((stack != 0) && (stack->num_elts >= 4)) {
          _elt             = stack->top_elt->previous_elt->previous_elt->previous_elt;
          _retcode         = _elt->type;
     }
     else {
          _retcode       = RPN_TYPE_NIL;
     }

     return _retcode;
}

/* rpn_get_T_type() }}} */
/* rpn_search_op() {{{ */

/******************************************************************************

                         RPN_SEARCH_OP

******************************************************************************/
dl_operator *rpn_search_op(rpn_stack *stack, char *operator)
{
     dl_operator              *_op;

	_op                      = 0;

	G.exit_code			= RPN_EXIT_OP_ERROR;

	/* Search for a matching V2 operator
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if ((_op = dl_search_matching_op(stack, operator)) == 0) {
		printf("%s: operator \"%s\" not found !\n",
			  "ERROR", operator);
	}
	else if (_op == (dl_operator *) 0x1) {
		printf("%s: operator \"%s\" found, but not for the specified element types !\n",
			  "ERROR", operator);
		_op						= 0;
	}
	else {
		/* Matching definition found !
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		if (G.debug_level & RPN_DBG_DEBUG) {
			printf("%s: %s() : operator \"%s\" found : module = \"%s\"\n",
				   G.progname, __func__, operator, _op->module_name);
		}

		G.exit_code			= RPN_EXIT_OK;
	}

     return _op;
}

/* rpn_search_op() }}} */
/* rpn_modules() {{{ */
/******************************************************************************

                         RPN_MODULES

******************************************************************************/
int rpn_modules()
{
     /* List dynamically loaded modules
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_disp_loaded_modules();

     return 0;
}

/* rpn_modules() }}} */
/* rpn_catalog() {{{ */
/******************************************************************************

                         RPN_CATALOG

******************************************************************************/
int rpn_catalog(struct rpn_operator *ops)
{
     /* Search for dynamically loaded modules
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_disp_ops_tree();

     return 0;
}

/* rpn_catalog() }}} */
/* rpn_err_msg_no_mem() {{{ */
/******************************************************************************

                         RPN_ERR_MSG_NO_MEM

******************************************************************************/
void rpn_err_msg_no_mem()
{
     fprintf(stderr, "%s: not enough memory !\n", "ERROR");
}

/* rpn_err_msg_no_mem() }}} */
/* rpn_err_msg_op_not_found() {{{ */
/******************************************************************************

                         RPN_ERR_MSG_OP_NOT_FOUND

******************************************************************************/
void rpn_err_msg_op_not_found(char *operator)
{
     fprintf(stderr, "%s: operator \"%s\" not found !\n",
	        "ERROR", operator);
}

/* rpn_err_msg_op_not_found() }}} */
/* rpn_err_msg_op_error() {{{ */
/******************************************************************************

                         RPN_ERR_MSG_OP_ERROR

******************************************************************************/
void rpn_err_msg_op_error(char *operator, int retcode)
{
     fprintf(stderr, "%s: operator \"%s\" : %s !\n",
             "ERROR", operator, rpn_err_to_string(retcode));
}

/* rpn_err_msg_op_error() }}} */
/* rpn_err_to_string() {{{ */
/******************************************************************************

                         RPN_ERR_TO_STRING

******************************************************************************/
char *rpn_err_to_string(int errcode)
{
     static char          _mesg[32];
     char                *_err_mesg;

     switch (errcode) {

     case RPN_RET_INVALID_ELT:
          _err_mesg      =  "invalid element";
          break;

     case RPN_RET_ERROR:
          _err_mesg      = "error";
          break;

     case RPN_RET_OK:
          _err_mesg      = "OK";
          break;

     case RPN_RET_INVALID_STACK:
          _err_mesg      = "invalid stack";
          break;

     case RPN_RET_INVALID_STR:
          _err_mesg      = "invalid string";
          break;

     case RPN_RET_INVALID_X:
          _err_mesg      = "invalid X";
          break;

     case RPN_RET_INVALID_Y:
          _err_mesg      = "invalid Y";
          break;

     case RPN_RET_TOO_BIG:
          _err_mesg      = "too big";
          break;

     case RPN_RET_INVALID_TYPES:
          _err_mesg      = "invalid types";
          break;

     case RPN_RET_NOT_ENOUGH_ELTS:
          _err_mesg      = "not enough elements";
          break;

     case RPN_RET_INCOMPATIBLE_ELTS:
          _err_mesg      = "incompatible elements";
          break;

     case RPN_RET_INVALID_X_TYPE:
          _err_mesg      = "invalid X type";
          break;

     case RPN_RET_INVALID_Y_TYPE:
          _err_mesg      = "invalid Y type";
          break;

     case RPN_RET_NO_START_MARKER:
          _err_mesg       = "no start marker";
          break;

     case RPN_RET_INCOMPATIBLE_DIM:
          _err_mesg       = "incompatible dimensions";
          break;

     default:
          sprintf(_mesg, "UNKNOWN ERROR CODE (%d)", errcode);
          _err_mesg      = _mesg;
          break;
     }

     return _err_mesg;
}

/* rpn_err_to_string() }}} */
/* rpn_err_msg_invalid_string() {{{ */
/******************************************************************************

                         RPN_ERR_MSG_INVALID_STRING

******************************************************************************/
void rpn_err_msg_invalid_string(char *str)
{
     fprintf(stderr, "%s: invalid string (%s)\n", G.progname, str);
}

/* rpn_err_msg_invalid_string() }}} */
/* rpn_internal_error() {{{ */
/******************************************************************************

                         RPN_INTERNAL_ERROR

******************************************************************************/
void rpn_internal_error(const char *funct, char *file, int line)
{
     fprintf(stderr, "%s: INTERNAL ERROR !!! (%s() : %s(%d))\n",
             G.progname, funct, file, line);
     rpn_print_trace(0);
     exit(RPN_EXIT_INTERNAL_ERR);
}

/* rpn_internal_error() }}} */
/* rpn_unimplemented() {{{ */
/******************************************************************************

                         RPN_UNIMPLEMENTED

******************************************************************************/
void rpn_unimplemented(char *op_name, const char *funct, char *file, int line)
{
     fprintf(stderr, "%s: operator \"%s\" is unimplemented !!! (%s() : %s(%d))\n",
             G.progname, op_name, funct, file, line);
     rpn_print_trace(0);
     exit(RPN_EXIT_UNIMPLEMENTED);
}

/* rpn_unimplemented() }}} */
/* rpn_strip_quotes() {{{ */
/******************************************************************************

                         RPN_STRIP_QUOTES

******************************************************************************/
int rpn_strip_quotes(char **str)
{
	size_t			 _lg;
     int                  _retcode;
     char                *_new_str;

     _lg            = strlen(*str);
     if ((*str)[0] != '"' || (*str)[_lg - 1] != '"') {
          _retcode       = RPN_RET_INVALID_STR;
     }
     else {
#if 0
          _new_str            = strdup(*str);
          _new_str[_lg - 1]   = 0;
          _new_str++;

          *str                = _new_str;
#else
          _new_str            = RPN_MALLOC(_lg);
          strncpy(_new_str, *str + 1, _lg - 2);
          _new_str[_lg - 2]   = 0;

          RPN_FREE(*str);
          *str                = _new_str;
#endif

          _retcode            = RPN_RET_OK;
     }

     return _retcode;
}

/* rpn_strip_quotes() }}} */
/* rpn_strip_singlequotes() {{{ */
/******************************************************************************

                         RPN_STRIP_SINGLEQUOTES

******************************************************************************/
int rpn_strip_single_quotes(char **str)
{
     size_t               _lg;
	int				 _retcode;
     char                *_new_str;

     _lg            = strlen(*str);
     if ((*str)[0] != '\'' || (*str)[_lg - 1] != '\'') {
          _retcode       = RPN_RET_INVALID_STR;
     }
     else {
#if 0
          _new_str            = strdup(*str);
          _new_str[_lg - 1]   = 0;
          _new_str++;

          *str                = _new_str;
#else
          _new_str            = RPN_MALLOC(_lg);
          strncpy(_new_str, *str + 1, _lg - 2);
          _new_str[_lg - 2]   = 0;

          RPN_FREE(*str);
          *str                = _new_str;
#endif


          _retcode            = RPN_RET_OK;
     }

     return _retcode;
}

/* rpn_strip_singlequotes() }}} */
/* rpn_check_types() {{{ */
/******************************************************************************

                         RPN_CHECK_TYPES

******************************************************************************/
int rpn_check_types(rpn_operator *op, struct rpn_stack *stack)
{
     struct rpn_st_types      *_t;
     int                       _X_type = -1, _Y_type   = -1,
                               _retcode;
     bool                      _good_X = FALSE;

     _retcode       = RPN_RET_INVALID_TYPES;

     _t             = op->st_types;
     if (_t->nb_operands != 0) {
          switch (_t->nb_operands) {

          case 1:
               _X_type   = rpn_get_X_type(stack);

               for (_t = op->st_types; _t->nb_operands != 0; _t++) {
                    if ((_X_type == _t->X_type)
                    || (_t->X_type == RPN_TYPE_ANY_TYPE)) {
                         /* X type validated
                            ~~~~~~~~~~~~~~~~ */
                         _retcode       = RPN_RET_OK;
                         goto end;
                    }
               }

               _retcode       = RPN_RET_INVALID_X_TYPE;
               break;

          case 2:
               _X_type   = rpn_get_X_type(stack);
               _Y_type   = rpn_get_Y_type(stack);
               for (_t = op->st_types; _t->nb_operands != 0; _t++) {
                    if ((_X_type == _t->X_type)
                    ||  (_t->X_type == RPN_TYPE_ANY_TYPE)) {
                         /* X type validated
                            ~~~~~~~~~~~~~~~~ */
                         _good_X        = TRUE;

                         if ((_Y_type == _t->Y_type)
                         ||  (_t->Y_type == RPN_TYPE_ANY_TYPE)) {
                              /* Y type validated
                                 ~~~~~~~~~~~~~~~~ */
                              _retcode       = RPN_RET_OK;
                              goto end;
                         }
                    }
               }

               if (_good_X) {
                    /* If X has a good type, then Y has not
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _retcode       = RPN_RET_INVALID_Y_TYPE;
               }
               else {
                    _retcode       = RPN_RET_INVALID_X_TYPE;
               }
               break;

          default:
               RPN_INTERNAL_ERROR;
               break;
          }

     }
     else {
          _retcode       = RPN_RET_OK;
     }

end:
     if (_retcode != RPN_RET_OK) {
          fprintf(stderr, "%s: authorized types for %s :\n",
                  G.progname, op->op_name);

          for (_t = op->st_types; _t->nb_operands != 0; _t++) {
               switch (_t->nb_operands) {

               case 1:
                    fprintf(stderr, "     %s\n", rpn_type_to_string(_t->X_type));
                    break;

               case 2:
                    fprintf(stderr, "     %-15s %-15s\n",
                            rpn_type_to_string(_t->X_type),
                            rpn_type_to_string(_t->Y_type));
                    break;

               default:
                    RPN_INTERNAL_ERROR;
                    break;
               }
          }
     }
     return _retcode;
}

/* rpn_check_types() }}} */
/* rpn_undefined_type_to_string() {{{ */

/******************************************************************************

					RPN_UNDEFINED_TYPE_TO_STRING

******************************************************************************/
char *rpn_undefined_type_to_string(int type)
{
	static char					_msg[32];

	sprintf(_msg, "(???:%d)", type);
#if 0
	printf("*** Undefined type to string function for type %d !\n", 
	       type);
	exit(RPN_EXIT_INTERNAL_ERR);
#endif /* 0 */

	return _msg;
}

/* rpn_undefined_type_to_string() }}} */
/* rpn_type_to_string() {{{ */
/******************************************************************************

                         RPN_TYPE_TO_STRING

******************************************************************************/
char *rpn_type_to_string(int type)
{
     char                     *_str_type;
     static char               _err_mesg[32];

     switch (type) {

     case RPN_TYPE_ANY_TYPE:
          _str_type      = "ANY_TYPE";
          break;

     case RPN_TYPE_UNUSED:
          _str_type      = "UNUSED";
          break;

     case RPN_TYPE_UNDEFINED:
          _str_type      = "UNDEFINED";
          break;

     case RPN_TYPE_NIL:
          _str_type      = "NIL";
          break;

     case RPN_TYPE_INT:
          _str_type      = "INT";
          break;

     case RPN_TYPE_FLOAT:
          _str_type      = "FLOAT";
          break;

     case RPN_TYPE_DOUBLE:
          _str_type      = "DOUBLE";
          break;

     case RPN_TYPE_STRING:
          _str_type      = "STRING";
          break;

     case RPN_TYPE_VECTOR_3:
          _str_type      = "VECTOR_3";
          break;

     case RPN_TYPE_MATRIX:
          _str_type      = "MATRIX";
          break;

     case RPN_TYPE_LITTERAL:
          _str_type      = "LITTERAL";
          break;

     case RPN_TYPE_START_MARKER:
          _str_type      = "START_MARKER";
          break;

#if 0
     case RPN_TYPE_NEURAL_MLP:
          _str_type      = "NEURAL_MLP";
          break;
#endif /* 0 */

     case RPN_TYPE_FILENAME:
          _str_type      = "FILENAME";
          break;

#if 0
     case RPN_TYPE_TRAINING_ELT:
          _str_type      = "TRAINING_ELT";
          break;

     case RPN_TYPE_TRAINING_SET:
          _str_type      = "TRAINING_SET";
          break;

     case RPN_TYPE_TEST_ELT:
          _str_type      = "TEST_ELT";
          break;

     case RPN_TYPE_TEST_SET:
          _str_type      = "TEST_SET";
          break;
#endif /* 0 */

     case RPN_TYPE_TEXT_FILE:
          _str_type      = "TEXT_FILE";
          break;

     case RPN_TYPE_LIST:
          _str_type      = "LIST";
          break;

     case RPN_TYPE_TUPLE:
          _str_type      = "TUPLE";
          break;

     case RPN_TYPE_OPAIR:
          _str_type      = "OPAIR";
          break;

     case RPN_TYPE_TEXT:
          _str_type      = "TEXT";
          break;

     case RPN_TYPE_CLONE:
          _str_type      = "CLONE";
          break;

	case	RPN_TYPE_COEF_A_B:
		_str_type		= "COEF_A_B";
		break;

	case	RPN_TYPE_MIN_MAX:
		_str_type		= "MIN_MAX";
		break;

	case	RPN_TYPE_IPv4:
		_str_type		= "IP v4";
		break;

	case	RPN_TYPE_REGEX:
		_str_type		= "REGEX";
		break;

#if 0
	case	RPN_TYPE_HOSTSFILE:
		_str_type		= "HOSTSFILE";
		break;
#endif /* 0 */

     default:
		if (type <= RPN_MAX_TYPE) {
			_str_type		= (*rpn_methods[type]->type_to_string)(type);
		}
		else {
			sprintf(_err_mesg, "??? (%d)", type);
			_str_type      = _err_mesg;
		}
          break;
     }

     return _str_type;
}

/* rpn_type_to_string() }}} */
/* rpn_new_sigma() {{{ */
/******************************************************************************

                         RPN_NEW_SIGMA

******************************************************************************/
struct rpn_sigma *rpn_new_sigma(char *name)
{
     rpn_sigma		          *_sigma;
     int                       _i;

     if ((_sigma = (rpn_sigma *) RPN_MALLOC(sizeof(*_sigma))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _sigma->sigma_name       = strdup(name);

     for (_i = 0; _i < RPN_NB_SIGMA; _i++) {
          _sigma->reg[_i]          = rpn_new_elt(RPN_TYPE_INT);
     }

     return _sigma;
}

/* rpn_new_sigma() }}} */
/* rpn_inc_begin() {{{ */
/******************************************************************************

                             RPN_INC_BEGIN

******************************************************************************/
void rpn_inc_begin(rpn_stack *stack)
{
     stack->nb_begin++;
}

/* rpn_inc_begin() }}} */
/* rpn_dec_begin() {{{ */
/******************************************************************************

                              RPN_DEC_BEGIN

******************************************************************************/
void rpn_dec_begin(rpn_stack *stack)
{
     stack->nb_begin--;
}

/* rpn_dec_begin() }}} */
/* rpn_x_to_int() {{{ */
/******************************************************************************

                              RPN_X_TO_INT

******************************************************************************/
int rpn_x_to_int(rpn_stack *stack)
{
     /** Returns integer value of X.
      *  Stack is left unchanged.
      */
     rpn_elt                  *_stk_x;
     int                       _X_type, _i;

     _stk_x                   = rpn_pop(stack);
     if ((_X_type = rpn_get_type(_stk_x)) != RPN_TYPE_INT) {
          fprintf(stderr, "%s() [%s (%d)] : X is not an integer ! (type = %d)\n",
                  __func__, __FILE__, __LINE__, _X_type);
          _i                       = -1;
     }
     else {
          _i                       = _stk_x->value.i;
     }

     rpn_push(stack, _stk_x);

     return _i;
}

/* rpn_x_to_int() }}} */
/* rpn_malloc() {{{ */
#define   SHOW_MEM  0
/******************************************************************************

                              RPN_MALLOC

******************************************************************************/
void *rpn_malloc(size_t size)
{
     void                     *_mem;
     int                       _real_size;

     _mem                      = malloc(size);

     _real_size               = *(int *)(_mem - 8);
#if (SHOW_MEM == 1)
     printf("malloc(%6d) : %6d\n", size, _real_size);
#endif
     G.allocated_current      += _real_size;
     G.allocated_total        += _real_size;
     if (G.allocated_current > G.allocated_peak) {
          G.allocated_peak         = G.allocated_current;
     }
     G.allocated_nb++;
#if (SHOW_MEM == 1)
     M
//   printf("Allocated memory : %lld\n", G.allocated_current);
     printf("==> MALLOC : %p %10d\n", _mem, G.allocated_nb);
#endif

     return _mem;
}

/* rpn_malloc() }}} */
/* rpn_free() {{{ */
/******************************************************************************

                              RPN_FREE

******************************************************************************/
void rpn_free(void *mem)
{
     int                       _real_size;

     _real_size               = *(int *)(mem - 8);
#if (SHOW_MEM == 1)
     printf("free           : %6d\n", _real_size);
#endif
     G.allocated_current     -= _real_size;
     G.free_nb++;
#if (SHOW_MEM == 1)
     M
//   printf("Allocated memory : %lld\n", G.allocated_current);
     printf("==> FREE   : %p\n", mem);
#endif

     free(mem);
}
/* rpn_free() }}} */
/* rpn_strdup() {{{ */

/******************************************************************************

						RPN_STRDUP

******************************************************************************/
char *rpn_strdup(char *str)
{
	char					*_str;

	_str					= rpn_malloc(strlen(str) + 1);
	return strcpy(_str, str);
}

/* rpn_strdup() }}} */
/* rpn_mkdir() {{{ */

/******************************************************************************

                              RPN_MKDIR

******************************************************************************/
void rpn_mkdir(char *dirname)
{
     struct stat                    _stat_buf;

     /* Check existence of data directory
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (stat(dirname, &_stat_buf) < 0) {
          /* Directory does not exist : create it
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          if (mkdir(dirname, RPN_MNIST_DIR_MODE) < 0) {
               fprintf(stderr, "%s: cannot create directory \"%s\" !\n",
                       G.progname, dirname);
               perror("mkdir");
               exit(RPN_EXIT_MKDIR_ERROR);
          }
     }
}

/* rpn_mkdir() }}} */
/* rpn_gen_command() {{{ */

/******************************************************************************

                              RPN_GEN_COMMAND

******************************************************************************/
char *rpn_gen_command(rpn_elt *elt)
{
     int                       _type;
	size_t				 _size;
     char                     *_cmd;
     rpn_list                *_list;
     rpn_elt                  *_elt;

     _cmd                     = "";
     _type                    = rpn_get_type(elt);

     switch (_type) {

     case RPN_TYPE_TUPLE:
     case RPN_TYPE_LIST:
          _list                    = elt->value.obj;
          break;

     default:
          RPN_INTERNAL_ERROR;
          break;
     }

     _size                    = rpn_str_size(_list);

     if ((_cmd = RPN_MALLOC(_size)) == NULL) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     _cmd[0]                  = '\0';

     for (_elt = _list->top_elt; _elt != NULL; _elt = _elt->previous_elt) {
          _type                    = rpn_get_type(_elt);

          switch (_type) {

          case RPN_TYPE_STRING:
          case RPN_TYPE_FILENAME:
               strcat(_cmd, _elt->value.s);
               strcat(_cmd, " ");
               break;

          case RPN_TYPE_LITTERAL:
               RPN_INTERNAL_ERROR;
			break;

          default:
               RPN_INTERNAL_ERROR;
               break;
          }
     }

     return _cmd;
}
/* rpn_gen_command() }}} */
/* rpn_str_size() {{{ */

/******************************************************************************

                              RPN_STR_SIZE

******************************************************************************/
int rpn_str_size(rpn_list *list)
{
     int                       _size, _type;
     rpn_elt                  *_elt;

     _size                    = 0;

     for (_elt = list->top_elt; _elt != NULL; _elt = _elt->previous_elt) {
          _type                    = rpn_get_type(_elt);

          switch (_type) {

          case RPN_TYPE_STRING:
          case RPN_TYPE_FILENAME:
               _size               += strlen(_elt->value.s) + 1;
               break;

          case RPN_TYPE_LITTERAL:
			{
				rpn_litteral			*_litteral;

				_litteral				= _elt->value.obj;
				if (_litteral) {
					if (_litteral->value) {
						_size               += strlen(_litteral->value) + 1;
					}
				}
			}
			break;

          default:
               RPN_INTERNAL_ERROR;
               break;
          }
     }

     return _size;
}

/* rpn_str_size() }}} */
/* rpn_exec() {{{ */

/******************************************************************************

                              RPN_EXEC

******************************************************************************/
int rpn_exec(char *cmd_path, char **argp, int *fd_pipe, int *fd_to_close)
{
     int                  _pid, _child_pid = 0, _i;

     switch ((_pid = fork())) {

     case -1:
          fprintf(stderr, "%s: cannot fork !\n", G.progname);
          exit(RPN_EXIT_FORK_ERROR);
          break;

     case 0:
          /* Child process
             ~~~~~~~~~~~~~ */
          if (fd_pipe) {
               close(0);
               dup(fd_pipe[0]);

               close(1);
               dup(fd_pipe[1]);

               close(fd_pipe[1]);
               close(fd_pipe[0]);
          }

		for (_i = 0; fd_to_close[_i] >= 0; _i++) {
			close(fd_to_close[_i]);
		}

          execvp(cmd_path, argp);
          perror("execvp");
          fprintf(stderr, "%s: cannot exec \"%s\" !\n", G.progname, cmd_path);
          exit(RPN_EXIT_EXEC_ERROR);
          break;

     default:
          /* Parent process
             ~~~~~~~~~~~~~~ */
          _child_pid               = _pid;
          break;
     }

     return _child_pid;
}

/* rpn_exec() }}} */
/* rpn_new_module() {{{ */

/******************************************************************************

                              RPN_NEW_MODULE

******************************************************************************/
RPN_NEW(module)

/* rpn_new_module() }}} */
/* rpn_new_dyn_op() {{{ */

/******************************************************************************

                              RPN_NEW_DYN_OP

******************************************************************************/
RPN_NEW(dyn_op)

/* rpn_new_dyn_op() }}} */
/* rpn_module_tree_add() {{{ */

/******************************************************************************

                              RPN_MODULE_TREE_ADD

******************************************************************************/
int rpn_module_tree_add(struct rpn_module *module)
{
     int                  _retcode = RPN_RET_OK;

     if (ci_add_node(&G.modules_tree, &module->node, rpn_cmp_modules, 0) != 0) {
          /* Module already present in the modules tree
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          _retcode       = RPN_RET_DUPLICATE;
     }

     return _retcode;
}

/* rpn_module_tree_add() }}} */
/* rpn_module_tree_search() {{{ */

/******************************************************************************

                              RPN_MODULE_TREE_SEARCH

******************************************************************************/
struct rpn_module *rpn_module_tree_search(char *module_name)
{
     struct rpn_module         *_module, *_dyn_mod;
     struct ci_node            *_node;

     _module                       = rpn_new_module();
     _module->name                 = module_name;
     _module->node.data            = _module;

     _node                         = ci_search(&G.modules_tree, &_module->node, rpn_cmp_modules);
     if (_node != 0) {
          /* Module already present in the modules tree
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          _dyn_mod                      = _node->data;
     }
     else {
          _dyn_mod                      = 0;
     }

     return _dyn_mod;
}

/* rpn_module_tree_search() }}} */
/* rpn_cmp_modules() {{{ */

/******************************************************************************

                              RPN_CMP_MODULES

******************************************************************************/
cc_uint16 rpn_cmp_modules(struct ci_node *n1, struct ci_node *n2)
{
     cc_uint16            _retcode;
     int                  _r;
     struct rpn_module   *_m1, *_m2;

     _m1            = (struct rpn_module *) n1->data;
     _m2            = (struct rpn_module *) n2->data;

     _r             = strcmp(_m1->name, _m2->name);
     if (_r < 0) {
          _retcode       = CI_CMP_GT;
     }
     else if (_r > 0) {
          _retcode       = CI_CMP_LT;
     }
     else {
          _retcode       = CI_CMP_EQ;
     }

     return _retcode;
}

/* rpn_cmp_modules() }}} */
/* rpn_cmp_operators() {{{ */

/******************************************************************************

                              RPN_CMP_OPERATORS

******************************************************************************/
cc_uint16 rpn_cmp_operators(struct ci_node *n1, struct ci_node *n2)
{
     cc_uint16            _retcode;
     int                  _r;
     struct rpn_dyn_op   *_dyn_op1, *_dyn_op2;
     char                *_mod_name1, *_mod_name2, *_unknown = "???";
     struct rpn_module   *_m1, *_m2;

//printf("%s(n1 = %p, n2 = %p)\n", __func__, n1, n2);
     _dyn_op1       = (struct rpn_dyn_op *) n1->data;
     _dyn_op2       = (struct rpn_dyn_op *) n2->data;
//printf("%s() : _dyn_op1 = %p, _dyn_op2 = %p)\n", __func__, _dyn_op1, _dyn_op2);
     _m1            = _dyn_op1->module;
     _m2            = _dyn_op2->module;
     if (_m1 == 0) {
          _mod_name1     = _unknown;
     }
     else {
          _mod_name1     = _m1->name;
     }

     if (_m2 == 0) {
          _mod_name2     = _unknown;
     }
     else {
          _mod_name2     = _m2->name;
     }

     if (G.debug_level & RPN_DBG_DYN_MODULE) {
          printf("Module1 = [%s] Module2 = [%s]\n", _mod_name1, _mod_name2);
     }

     if (G.debug_level & RPN_DBG_DYN_OP) {
          printf("OP1 = [%s:%s]  OP2 = [%s:%s]\n",
                 _mod_name1, _dyn_op1->operator->op_name,
                 _mod_name2, _dyn_op2->operator->op_name);
     }
     _r             = strcmp(_dyn_op1->operator->op_name, _dyn_op2->operator->op_name);
     if (_r < 0) {
          _retcode       = CI_CMP_LT;
     }
     else if (_r > 0) {
          _retcode       = CI_CMP_GT;
     }
     else {
          _retcode       = CI_CMP_EQ;
     }

     return _retcode;
}

/* rpn_cmp_operators() }}} */
/* rpn_set_list_name() {{{ */

/******************************************************************************

                         RPN_SET_LIST_NAME

******************************************************************************/
void rpn_set_list_name(rpn_elt *elt_list, char *new_name)
{
     rpn_list                      *_list;
     int                            _type;

     /* Check element type
        ~~~~~~~~~~~~~~~~~~ */
     if ((_type = rpn_get_type(elt_list)) != RPN_TYPE_LIST) {
          fprintf(stderr, "%s: %s(): element type = %s\n",
                  G.progname, __func__, rpn_type_to_string(_type));
          RPN_INTERNAL_ERROR;
     }

     /* Get list address
        ~~~~~~~~~~~~~~~~ */
     _list                         = elt_list->value.obj;

     if (_list->name) {
          /* Free old list name
             ~~~~~~~~~~~~~~~~~~ */
          free(_list->name);
     }

     /* Set new list name
        ~~~~~~~~~~~~~~~~~ */
     _list->name                   = strdup(new_name);
}

/* rpn_set_list_name() }}} */
/* rpn_get_list_name() {{{ */

/******************************************************************************

                         RPN_GET_LIST_NAME

******************************************************************************/
char *rpn_get_list_name(rpn_elt *elt_list)
{
     int                            _type;
     char                          *_name;
     rpn_list                      *_list;

     if ((_type = rpn_get_type(elt_list)) != RPN_TYPE_LIST) {
          fprintf(stderr, "%s: %s() : bad element type (%s) !\n",
                  G.progname, __func__, rpn_type_to_string(_type));
          _name                    = 0;
     }
     else {
          _list                    = elt_list->value.obj;
          _name                    = strdup(_list->name);
     }

     return _name;
}

/* rpn_get_list_name() }}} */
/* rpn_get_list_num() {{{ */

/******************************************************************************

                         RPN_GET_LIST_NUM

******************************************************************************/
int rpn_get_list_num(rpn_elt *elt_list)
{
     int                            _type;
     int                            _num;
     rpn_list                      *_list;

     if ((_type = rpn_get_type(elt_list)) != RPN_TYPE_LIST) {
          fprintf(stderr, "%s: %s() : bad element type (%s) !\n",
                  G.progname, __func__, rpn_type_to_string(_type));
          _num                     = -1;
     }
     else {
          _list                    = elt_list->value.obj;
          _num                     = _list->num_elts;
     }

     return _num;
}

/* rpn_get_list_num() }}} */
/* rpn_get_next_elt() {{{ */

/******************************************************************************

                         RPN_GET_NEXT_ELT

******************************************************************************/
rpn_elt *rpn_get_next_elt(rpn_elt *elt)
{
     rpn_elt                       *_elt;

     _elt                          = elt->next_elt;
     return _elt;
}

/* rpn_get_next_elt() }}} */
/* rpn_get_list_addr() {{{ */

/******************************************************************************

                         RPN_GET_LIST_ADDR

******************************************************************************/
rpn_list *rpn_get_list_addr(rpn_elt *elt)
{
     int                            _type;

     if ((_type = rpn_get_type(elt)) != RPN_TYPE_LIST) {
          fprintf(stderr, "%s: %s() : element type is %s !\n",
                  G.progname, __func__, rpn_type_to_string(_type));
          RPN_INTERNAL_ERROR;
     }

     return elt->value.obj;
}

/* rpn_get_list_addr() }}} */
/* rpn_coef_get_a() {{{ */

/******************************************************************************

                         RPN_COEF_GET_A

******************************************************************************/
double rpn_coef_get_a(rpn_elt *elt_coef_a_b)
{
     int                            _type;
     rpn_coef_a_b                  *_a_b;

     if ((_type = rpn_get_type(elt_coef_a_b)) != RPN_TYPE_COEF_A_B) {
          fprintf(stderr, "%s: %s() : element type is %s !\n",
                  G.progname, __func__, rpn_type_to_string(_type));
          RPN_INTERNAL_ERROR;
     }

     _a_b                          = elt_coef_a_b->value.obj;

     return _a_b->a;
}

/* rpn_coef_get_a() }}} */
/* rpn_coef_get_b() {{{ */

/******************************************************************************

                         RPN_COEF_GET_B

******************************************************************************/
double rpn_coef_get_b(rpn_elt *elt_coef_a_b)
{
     int                            _type;
     rpn_coef_a_b                  *_a_b;

     if ((_type = rpn_get_type(elt_coef_a_b)) != RPN_TYPE_COEF_A_B) {
          fprintf(stderr, "%s: %s() : element type is %s !\n",
                  G.progname, __func__, rpn_type_to_string(_type));
          RPN_INTERNAL_ERROR;
     }

     _a_b                          = elt_coef_a_b->value.obj;

     return _a_b->b;
}

/* rpn_coef_get_b() }}} */
/* rpn_get_min() {{{ */

/******************************************************************************

                         RPN_GET_MIN

******************************************************************************/
double rpn_get_min(rpn_elt *elt_min_max)
{
     int                            _type;
     rpn_min_max                   *_min_max;

     if ((_type = rpn_get_type(elt_min_max)) != RPN_TYPE_MIN_MAX) {
          fprintf(stderr, "%s: %s() : element type is %s !\n",
                  G.progname, __func__, rpn_type_to_string(_type));
          RPN_INTERNAL_ERROR;
     }

     _min_max                      = elt_min_max->value.obj;

     return _min_max->min;
}

/* rpn_get_min() }}} */
/* rpn_get_max() {{{ */

/******************************************************************************

                         RPN_GET_MAX

******************************************************************************/
double rpn_get_max(rpn_elt *elt_min_max)
{
     int                            _type;
     rpn_min_max                   *_min_max;

     if ((_type = rpn_get_type(elt_min_max)) != RPN_TYPE_MIN_MAX) {
          fprintf(stderr, "%s: %s() : element type is %s !\n",
                  G.progname, __func__, rpn_type_to_string(_type));
          RPN_INTERNAL_ERROR;
     }

     _min_max                      = elt_min_max->value.obj;

     return _min_max->max;
}

/* rpn_get_max() }}} */
/* rpn_new_coef_a_b() {{{ */

/******************************************************************************

					RPN_NEW_COEF_A_B

******************************************************************************/
RPN_NEW(coef_a_b)

/* rpn_new_coef_a_b() }}} */
/* rpn_new_min_max() {{{ */

/******************************************************************************

					RPN_NEW_MIN_MAX

******************************************************************************/
RPN_NEW(min_max)

/* rpn_new_min_max() }}} */
/* rpn_str_printf() {{{ */

/******************************************************************************

					RPN_STR_PRINTF

******************************************************************************/
char *rpn_str_printf(char *fmt, char *str)
{
	size_t					 _lg;
	int						 _n;
	char						*_buf;

	_lg						= strlen(fmt) + strlen(str) + 1;
	_buf						= RPN_MALLOC(_lg);
	if ((_n = sprintf(_buf, fmt, str)) > _lg) {
		printf("%s(): sprintf() returned %d while buffer size is %lu !\n",
		       __func__, _n, _lg);
		RPN_INTERNAL_ERROR;
	}

	return _buf;
}

/* rpn_str_printf() }}} */
/* rpn_new_litteral() {{{ */

/******************************************************************************

					RPN_NEW_LITTERAL

******************************************************************************/
rpn_litteral *rpn_new_litteral()
{
     rpn_litteral	          *_litteral;

     if ((_litteral = (rpn_litteral *) RPN_MALLOC(sizeof(*_litteral))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _litteral->value			= 0;
	_litteral->need_parentheses	= FALSE;

     return _litteral;
}

/* rpn_new_litteral() }}} */
/* rpn_litteral_value() {{{ */

/******************************************************************************

					RPN_LITTERAL_VALUE

******************************************************************************/
char *rpn_litteral_value(rpn_elt *elt)
{
	int					 _type;
	rpn_litteral			*_litteral;
	char					*_value = 0;

	if ((_type = rpn_get_type(elt) != RPN_TYPE_LITTERAL)) {
		printf("%s(): unexpected type (%s)\n",
		       __func__, rpn_type_to_string(_type));
	}
	else {
		_litteral			= elt->value.obj;
		if (_litteral) {
			_value			= _litteral->value;
		}
	}

	return _value;
}

/* rpn_litteral_value() }}} */
/* rpn_litteral_set_value() {{{ */

/******************************************************************************

					RPN_LITTERAL_SET_VALUE

******************************************************************************/
void rpn_litteral_set_value(rpn_elt *elt, char *str)
{
	int					 _type;
	rpn_litteral			*_litteral;

	if ((_type = rpn_get_type(elt) != RPN_TYPE_LITTERAL)) {
		printf("%s(): unexpected type (%s)\n",
		       __func__, rpn_type_to_string(_type));
	}
	else {
		_litteral			= elt->value.obj;
		_litteral->value	= strdup(str);
	}
}

/* rpn_litteral_set_value() }}} */
/* rpn_new_regex() {{{ */

/******************************************************************************

					RPN_NEW_REGEX

******************************************************************************/
RPN_NEW(regex)

/* rpn_new_regex() }}} */
