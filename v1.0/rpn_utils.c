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
 *   rpn_utils.c
 *
 *	@(#)	[MB] cy_rpn_utils.c	Version 1.58 du 19/06/05 - 
 */

#include  "rpn_header.h"
#include  "rpn_epri.h"
#include  "rpn_proto.h"
#include  <time.h>
#include  <sys/time.h>
#include  <sys/stat.h>

struct global_struct     G         = { 0 };

/* OP TYPES {{{ */
struct rpn_st_types       op_types_gen_math1[] = {
     T(1, 1, INT,        UNUSED),
     T(1, 1, DOUBLE,     UNUSED),
     T(1, 1, MATRIX,     UNUSED),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

#if 0
struct rpn_st_types       op_types_sqrt[] = {
     T(1, 1, INT,        UNUSED),
     T(1, 1, DOUBLE,     UNUSED),
     T(1, 1, MATRIX,     UNUSED),

     T(0, 0, UNDEFINED,  UNDEFINED)
};
#endif    /* 0 */

struct rpn_st_types       op_types_add[] = {
     T(2, 1, INT,        INT),
     T(2, 1, INT,        DOUBLE),
     T(2, 1, INT,        VECTOR_3),
     T(2, 1, INT,        MATRIX),
     T(2, 1, INT,        LITTERAL),

     T(2, 1, DOUBLE,     INT),
     T(2, 1, DOUBLE,     DOUBLE),
     T(2, 1, DOUBLE,     VECTOR_3),
     T(2, 1, DOUBLE,     MATRIX),

     T(2, 1, STRING,     STRING),

     T(2, 1, VECTOR_3,   INT),
     T(2, 1, VECTOR_3,   DOUBLE),
     T(2, 1, VECTOR_3,   VECTOR_3),
     T(2, 1, VECTOR_3,   MATRIX),

     T(2, 1, MATRIX,     INT),
     T(2, 1, MATRIX,     DOUBLE),
     T(2, 1, MATRIX,     VECTOR_3),
     T(2, 1, MATRIX,     MATRIX),

     T(2, 1, LITTERAL,   INT),
     T(2, 1, LITTERAL,   LITTERAL),

     T(2, 1, NIL,        ANY_TYPE),
     T(2, 1, ANY_TYPE,   NIL),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_sub[] = {
     T(2, 1, INT,        INT),
     T(2, 1, INT,        DOUBLE),
     T(2, 1, INT,        STRING),
     T(2, 1, INT,        VECTOR_3),
     T(2, 1, INT,        MATRIX),
     T(2, 1, INT,        LITTERAL),

     T(2, 1, DOUBLE,     INT),
     T(2, 1, DOUBLE,     DOUBLE),
     T(2, 1, DOUBLE,     VECTOR_3),
     T(2, 1, DOUBLE,     MATRIX),

     T(2, 1, VECTOR_3,   INT),
     T(2, 1, VECTOR_3,   DOUBLE),
     T(2, 1, VECTOR_3,   VECTOR_3),
     T(2, 1, VECTOR_3,   MATRIX),

     T(2, 1, MATRIX,     INT),
     T(2, 1, MATRIX,     DOUBLE),
     T(2, 1, MATRIX,     VECTOR_3),
     T(2, 1, MATRIX,     MATRIX),

     T(2, 1, LITTERAL,   INT),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_mul[] = {
     T(2, 1, INT,        INT),
     T(2, 1, INT,        DOUBLE),
     T(2, 1, INT,        STRING),
     T(2, 1, INT,        VECTOR_3),
     T(2, 1, INT,        MATRIX),
     T(2, 1, INT,        LITTERAL),

     T(2, 1, DOUBLE,     INT),
     T(2, 1, DOUBLE,     DOUBLE),
     T(2, 1, DOUBLE,     VECTOR_3),
     T(2, 1, DOUBLE,     MATRIX),

     T(2, 1, VECTOR_3,   INT),
     T(2, 1, VECTOR_3,   DOUBLE),
     T(2, 1, VECTOR_3,   VECTOR_3),
     T(2, 1, VECTOR_3,   MATRIX),

     T(2, 1, MATRIX,     INT),
     T(2, 1, MATRIX,     DOUBLE),
     T(2, 1, MATRIX,     VECTOR_3),
     T(2, 1, MATRIX,     MATRIX),

     T(2, 1, LITTERAL,   INT),
     T(2, 1, LITTERAL,   LITTERAL),

     T(2, 1, LIST,       LIST),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_div[] = {
     T(2, 1, INT,        INT),
     T(2, 1, INT,        DOUBLE),
     T(2, 1, INT,        STRING),
     T(2, 1, INT,        VECTOR_3),
     T(2, 1, INT,        MATRIX),
     T(2, 1, INT,        LITTERAL),

     T(2, 1, DOUBLE,     INT),
     T(2, 1, DOUBLE,     DOUBLE),
     T(2, 1, DOUBLE,     VECTOR_3),
     T(2, 1, DOUBLE,     MATRIX),

     T(2, 1, VECTOR_3,   INT),
     T(2, 1, VECTOR_3,   DOUBLE),
     T(2, 1, VECTOR_3,   VECTOR_3),
     T(2, 1, VECTOR_3,   MATRIX),

     T(2, 1, MATRIX,     INT),
     T(2, 1, MATRIX,     DOUBLE),
     T(2, 1, MATRIX,     VECTOR_3),
     T(2, 1, MATRIX,     MATRIX),

     T(2, 1, LITTERAL,   INT),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_delta_percent[] = {
     T(2, 1, INT,        INT),
     T(2, 1, INT,        DOUBLE),
     T(2, 1, INT,        VECTOR_3),
     T(2, 1, INT,        MATRIX),
     T(2, 1, INT,        LITTERAL),

     T(2, 1, DOUBLE,     INT),
     T(2, 1, DOUBLE,     DOUBLE),
     T(2, 1, DOUBLE,     VECTOR_3),
     T(2, 1, DOUBLE,     MATRIX),

     T(2, 1, STRING,     STRING),

     T(2, 1, VECTOR_3,   INT),
     T(2, 1, VECTOR_3,   DOUBLE),
     T(2, 1, VECTOR_3,   VECTOR_3),
     T(2, 1, VECTOR_3,   MATRIX),

     T(2, 1, MATRIX,     INT),
     T(2, 1, MATRIX,     DOUBLE),
     T(2, 1, MATRIX,     VECTOR_3),
     T(2, 1, MATRIX,     MATRIX),

     T(2, 1, LITTERAL,   INT),
     T(2, 1, LITTERAL,   LITTERAL),

     T(2, 1, NIL,        ANY_TYPE),
     T(2, 1, ANY_TYPE,   NIL),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_power[] = {
     T(2, 1, INT,        INT),
     T(2, 1, INT,        DOUBLE),
     T(2, 1, INT,        VECTOR_3),
     T(2, 1, INT,        MATRIX),
//   T(2, 1, INT,        LITTERAL),

     T(2, 1, DOUBLE,     INT),
     T(2, 1, DOUBLE,     DOUBLE),
     T(2, 1, DOUBLE,     VECTOR_3),
     T(2, 1, DOUBLE,     MATRIX),

//   T(2, 1, STRING,     STRING),

     T(2, 1, VECTOR_3,   INT),
     T(2, 1, VECTOR_3,   DOUBLE),
     T(2, 1, VECTOR_3,   VECTOR_3),
     T(2, 1, VECTOR_3,   MATRIX),

     T(2, 1, MATRIX,     INT),
     T(2, 1, MATRIX,     DOUBLE),
     T(2, 1, MATRIX,     VECTOR_3),
     T(2, 1, MATRIX,     MATRIX),

//   T(2, 1, LITTERAL,   INT),
//   T(2, 1, LITTERAL,   LITTERAL),

     T(2, 1, NIL,        ANY_TYPE),
     T(2, 1, ANY_TYPE,   NIL),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_h_cat[] = {
     T(2, 1, MATRIX,     MATRIX),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_v_cat[] = {
     T(2, 1, MATRIX,     MATRIX),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_z_mat[] = {
     T(2, 1, INT,        INT),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_backup[] = {
     T(2, 1, NEURAL_MLP, FILENAME),
     T(2, 1, FILENAME,   NEURAL_MLP),
     T(2, 1, MATRIX,     FILENAME),
     T(2, 1, FILENAME,   MATRIX),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_type[] = {
     T(1, 1, STRING,      UNUSED),
     T(1, 1, LITTERAL,    UNUSED),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_gen_nn[] = {
     T(0, 0, UNDEFINED,  UNDEFINED),
     T(2, 1, NEURAL_MLP, INT),
     T(2, 1, INT,        NEURAL_MLP)
};

/* OP TYPES }}} */

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

     _elt->type          = type;
     _elt->value.i       = 0;
     _elt->previous_elt  = NULL;
     _elt->next_elt      = NULL;

     return _elt;
}

/* rpn_new_elt() }}} */
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
/* rpn_free_stack() {{{ */
/******************************************************************************

                         RPN_FREE_STACK

******************************************************************************/
void rpn_free_stack(rpn_stack *stack)
{
     rpn_elt             *_elt;
     int                  _type;

     if (stack == NULL) {
          RPN_INTERNAL_ERROR;
     }

     if (stack->num_elts != 0) {
          _elt                     = rpn_pop(stack);
          _type                    = rpn_get_type(_elt);
          rpn_push(stack, _elt);
          printf("%s() [%s (%d]) : stack \"%s\" not empty, %d remaining element(s) -- X type = %s\n",
                 __func__, __FILE__, __LINE__, stack->name, stack->num_elts, rpn_type_to_string(_type));

          /* Free all elements of the stack
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          for (; stack->num_elts > 0; ) {
               _elt                     = rpn_pop(stack);
               _type                    = rpn_get_type(_elt);
//             printf("%s() : free(%s)\n", __func__, rpn_type_to_string(_type));
               rpn_free_elt(&_elt);
          }
     }

     /* Free lastx
        ~~~~~~~~~~ */
     _elt                     = stack->lastx;
     _type                    = rpn_get_type(_elt);
     printf("%s(%s) : free(%s) (LASTX)\n", __func__, stack->name,
	       rpn_type_to_string(_type));
//X
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
/* rpn_free_elt() {{{ */
/******************************************************************************

                         RPN_FREE_ELT

******************************************************************************/
void rpn_free_elt(rpn_elt **ref_elt)
{
     rpn_elt             *_elt;
     int                  _type;

//   printf("RPN_FREE_ELT(%p) ...\n", ref_elt);
     if (ref_elt == NULL) {
          RPN_INTERNAL_ERROR;
     }
     if (*ref_elt == NULL) {
          RPN_INTERNAL_ERROR;
     }

     _elt                = *ref_elt;
     _type               = rpn_get_type(_elt);

     /* Make sure the element is not linked to other ones
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if ((_elt->previous_elt != NULL)
     ||  (_elt->next_elt     != NULL)) {
          printf("%s() (%s: %d) : _elt->previous = %p _elt->next_elt = %p\n",
                 __func__, __FILE__, __LINE__, _elt->previous_elt, _elt->next_elt);
          RPN_INTERNAL_ERROR;
     }

// printf("%s() [%s (%d)] : %s(%s) ...\n", __func__, __FILE__, __LINE__, __func__, rpn_type_to_string(_type));

     switch (_type) {

     case RPN_TYPE_NIL:
     case RPN_TYPE_INT:
     case RPN_TYPE_DOUBLE:
          /* Free element */
//X
          RPN_FREE(_elt);
          break;

     case RPN_TYPE_STRING:
     case RPN_TYPE_LITTERAL:
     case RPN_TYPE_FILENAME:
          if (_elt->value.s) {
               /* Free string */
               RPN_FREE(_elt->value.s);
          }

          /* Free element */
          RPN_FREE(_elt);
          break;

     case RPN_TYPE_MATRIX:
          {
               struct rpn_elt           *_sub_elt;
               struct rpn_matrix        *_mat;
               int                       _idx;

               _mat                = (struct rpn_matrix *) _elt->value.obj;

               for (_idx = 0; _idx < (_mat->n * _mat->p); _idx++) {
                    _sub_elt                 = (struct rpn_elt *) _mat->base[_idx];

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
          {
               struct rpn_training_elt       *_train;
               struct rpn_training_data      *_data;
               struct rpn_training_label     *_label;

               _train                        = _elt->value.obj;
               _data                         = _train->data;
               _label                        = _train->label;

               RPN_FREE(_train->label);
               RPN_FREE(_data->vector);
               RPN_FREE(_data);
               RPN_FREE(_train);
          }

          /* Free element */
          RPN_FREE(_elt);
          break;

     case RPN_TYPE_TRAINING_SET:
     case RPN_TYPE_TEST_SET:
          {
               int                       _nb;
               rpn_training_set         *_train_set;

               _train_set               = _elt->value.obj;

               /* Free name */
               RPN_FREE(_train_set->name);

               /* Free all elements in the array */
               for (_nb = 0; _nb < _train_set->nb_elts; _nb++) {
//printf("%s() : RPN_FREE(array[%5d])\n", __func__, _nb);
                    rpn_free_elt(&_train_set->array[_nb]);
               }

               /* Free array */
               RPN_FREE(_train_set);
          }

          /* Free element */
          RPN_FREE(_elt);
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
          {
               rpn_list                 *_list;

               for (_list = _elt->value.obj; _list->num_elts > 0; ) {
                    _elt                     = rpn_list_pop_head(_list);
                    rpn_free_elt(&_elt);
               }

               RPN_FREE(_list->name);
          }
          break;

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

  			_pair			= _elt->value.obj;

               rpn_free_elt(&_pair->elt_a);
               rpn_free_elt(&_pair->elt_b);

               RPN_FREE(_pair->name);
          }
          break;

     default:
//        fprintf(stderr, "%s : (%s) element type %d : not managed yet !\n",
          printf("%s() [%s (%d)] element type %d : not managed yet !\n",
                  __func__, __FILE__, __LINE__, _type);
          break;
     }
// printf("FREE(%s) : done\n", rpn_type_to_string(_type));

     /* Reset pointer value
        ~~~~~~~~~~~~~~~~~~~ */
     *ref_elt            = NULL;
}

/* rpn_free_elt() }}} */
/* rpn_clone_elt() {{{ */
/******************************************************************************

                         RPN_CLONE_ELT

******************************************************************************/
rpn_elt *rpn_clone_elt(rpn_elt *elt)
{
     rpn_elt             *_clone;
     int                  _idx;

     _clone              = rpn_new_elt(elt->type);

//rpn_disp_elt(elt, RPN_DISP_VALUE|RPN_DISP_NEWLINE);
//X

     switch (elt->type) {

     case RPN_TYPE_MATRIX:
//X
          {
               struct rpn_elt           *_sub_elt, *_clone_sub;
               struct rpn_matrix        *_mat, *_clone_mat;
               int                       _size;

               _mat                = (struct rpn_matrix *) elt->value.obj;
               _size               = sizeof(struct rpn_matrix)
                                   + (((_mat->n *_mat->p) - 1) * sizeof(struct rpn_elt *));
               _clone->value.obj   = RPN_MALLOC(_size);
               _clone_mat          = _clone->value.obj;
               _clone_mat->n       = _mat->n;
               _clone_mat->p       = _mat->p;

               for (_idx = 0; _idx < (_mat->n * _mat->p); _idx++) {
                    _sub_elt                 = (struct rpn_elt *) _mat->base[_idx];
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
     case RPN_TYPE_LITTERAL:
     case RPN_TYPE_FILENAME:
          _clone->value.s     = strdup(elt->value.s);
          break;

     case RPN_TYPE_NEURAL_MLP:
          RPN_INTERNAL_ERROR;
          break;

     case RPN_TYPE_TRAINING_ELT:
     case RPN_TYPE_TEST_ELT:
          {
               rpn_training_elt         *_train, *_train_clone;
               rpn_training_data        *_data,  *_data_clone;
               rpn_training_label       *_label, *_label_clone;
               int                       _size;

               _train                   = elt->value.obj;
               _data                    = _train->data;
               _label                   = _train->label;

               _train_clone             = rpn_new_training_elt();

               _data_clone              = rpn_new_training_data();
               _data_clone->index       = _data->index;
               _data_clone->orig_index  = _data->orig_index;
               _data_clone->ignore      = _data->ignore;
               _data_clone->nb_elts     = _data->nb_elts;
               _data_clone->elt_size    = _data->elt_size;
               _data_clone->num_rows    = _data->num_rows;
               _data_clone->num_cols    = _data->num_cols;
               _size                    = _data->nb_elts * _data->elt_size;

               if ((_data_clone->vector = (unsigned char *) RPN_MALLOC(_size)) == NULL) {
                    fprintf(stderr, "%s : cannot allocate %d bytes for the images !\n", G.progname, _size);
                    exit(RPN_EXIT_NO_MEM);
               }

               memcpy(_data_clone->vector, _data->vector, _size);

               _train_clone->data       = _data_clone;

               _label_clone             = rpn_new_training_label();
               _label_clone->index      = _label->index;
               _label_clone->value      = _label->value;

               _train_clone->label      = _label_clone;

               _clone->value.obj        = _train_clone;
          }
          break;

     case RPN_TYPE_TRAINING_SET:
     case RPN_TYPE_TEST_SET:
          {
               rpn_elt                  *_elt_clone;
               rpn_training_set         *_train_set, *_set_clone;
               int                       _size, _i;

               _train_set               = elt->value.obj;

               _elt_clone               = rpn_new_elt(elt->type);
               _size                    = sizeof(*_train_set) + ((_train_set->nb_elts - 1) * sizeof(struct rpn_elt *));
               _set_clone               = (struct rpn_training_set *) RPN_MALLOC(_size);
               _set_clone->name         = strdup(_train_set->name);
               _set_clone->nb_elts      = _train_set->nb_elts;
               _set_clone->current_idx  = _train_set->current_idx;
               _set_clone->nrows        = _train_set->nrows;
               _set_clone->ncols        = _train_set->ncols;
               _set_clone->width_pix    = _train_set->width_pix;
               _set_clone->height_pix   = _train_set->height_pix;
               _elt_clone->value.obj    = _set_clone;

               for (_i = 0; _i < _train_set->nb_elts; _i++) {
                    _set_clone->array[_i]    = rpn_clone_elt(_train_set->array[_i]);
               }
               _clone         = _elt_clone;
          }
          break;

     case RPN_TYPE_TEXT_FILE:
          {
               rpn_elt                  *_elt_clone;
               int                       _size, _i;

               _elt_clone               = rpn_new_elt(RPN_TYPE_TEXT_FILE);

               _clone                   = _elt_clone;
          }
          break;

     case RPN_TYPE_LIST:
//X
     case RPN_TYPE_TUPLE:
//X
          {
               rpn_elt                  *_elt_clone, *_sub_elt, *_sub_clone;
               rpn_list                 *_list, *_list_clone;

// X
// rpn_disp_elt(elt, RPN_DISP_VALUE|RPN_DISP_INFOS);
               _list                    = elt->value.obj;

               _elt_clone               = rpn_new_elt(rpn_get_type(elt));
               _list_clone              = rpn_new_list(_list->name);
               for (_sub_elt = _list->base_elt; _sub_elt != NULL;
                                               _sub_elt = _sub_elt->next_elt) {
// X
// rpn_disp_elt(_sub_elt, RPN_DISP_VALUE|RPN_DISP_INFOS);
                    _sub_clone               = rpn_clone_elt(_sub_elt);
                    rpn_list_push_head(_list_clone, _sub_clone);
               }
               _elt_clone->value.obj    = _list_clone;

               _clone                   = _elt_clone;
          }
// X
          break;

     case RPN_TYPE_OPAIR:
//X
          {
               rpn_elt                  *_elt_clone, *_clone_a, *_clone_b;
               rpn_pair                 *_pair, *_pair_clone;

               _pair                    = elt->value.obj;
//X

               _elt_clone               = rpn_new_elt(RPN_TYPE_OPAIR);
               _pair_clone              = rpn_new_pair((char *) __func__);
//X
//printf("pair->elt_a = %p\n", _pair->elt_a);

               _pair_clone->elt_a       = rpn_clone_elt(_pair->elt_a);
//X
               _pair_clone->elt_b       = rpn_clone_elt(_pair->elt_b);
//X

               _elt_clone->value.obj    = _pair_clone;
               _clone                   = _elt_clone;
//X
          }
          break;

     default:
          fprintf(stderr, "%s() : unknown type (%d)\n", __func__, elt->type);
          break;
     }

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

//X
//fprintf(stderr, "List type = %s\n", rpn_type_to_string(list->type));
//fprintf(stderr, "elt type  = %s\n", rpn_type_to_string(_elt_type));
     if (list->type == RPN_TYPE_UNDEFINED) {
          list->type               = _elt_type;
     }
     else {
//X
//fprintf(stderr, "List type = %s\n", rpn_type_to_string(list->type));
//fprintf(stderr, "elt type  = %s\n", rpn_type_to_string(_elt_type));

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

#if 0
X
fprintf(stderr, "List type = %s\n", rpn_type_to_string(list->type));
fprintf(stderr, "elt type  = %s\n", rpn_type_to_string(_elt_type));
#endif
     if (list->type == RPN_TYPE_UNDEFINED) {
          list->type               = _elt_type;
     }
     else {
#if 0
X
fprintf(stderr, "List type = %s\n", rpn_type_to_string(list->type));
fprintf(stderr, "elt type  = %s\n", rpn_type_to_string(_elt_type));
#endif

          if (list->type != _elt_type) {
               list->homogeneous        = FALSE;
          }
     }

     return _retcode;
}

/* rpn_list_push_tail() }}} */
/* rpn_list_pop_queue() {{{ */
/******************************************************************************

                         RPN_LIST_POP_QUEUE

******************************************************************************/
rpn_elt *rpn_list_pop_queue(rpn_list *list)
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

/* rpn_list_pop_queue() }}} */
/* rpn_roll_down() {{{ */
/******************************************************************************

                         RPN_ROLL_DOWN

******************************************************************************/
RPN_DEF_OP(rpn_op_roll_down)
{
     int                       _retcode;
     struct rpn_elt           *_elt;

     _elt                     = rpn_pop(stack);
     rpn_base_push(stack, _elt);

     rpn_op_prx(stack, RPN_OP_DESC(PRX));

     _retcode                 = RPN_RET_OK;

     return _retcode;
}

/* rpn_roll_down() }}} */
/* rpn_roll_up() {{{ */
/******************************************************************************

                         RPN_ROLL_UP

******************************************************************************/
RPN_DEF_OP(rpn_op_roll_up)
{
     int                       _retcode;
     struct rpn_elt           *_elt;

     _elt                     = rpn_base_pop(stack);
     rpn_push(stack, _elt);

     rpn_op_prx(stack, RPN_OP_DESC(PRX));

     _retcode                 = RPN_RET_OK;

     return _retcode;
}

/* rpn_roll_up() }}} */
/* rpn_op_swap_xy() {{{ */
/******************************************************************************

                         RPN_OP_SWAP_XY

******************************************************************************/
RPN_DEF_OP(rpn_op_swap_xy)
{
     struct rpn_elt           *_stk_x, *_stk_y;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 2) {
          _stk_x        = rpn_pop(stack);
          _stk_y        = rpn_pop(stack);

          rpn_push(stack, _stk_x);
          rpn_push(stack, _stk_y);
     }

     return _retcode;
}

/* rpn_op_swap_xy() }}} */
/* rpn_op_swap_xz() {{{ */
/******************************************************************************

                         RPN_OP_SWAP_XZ

******************************************************************************/
RPN_DEF_OP(rpn_op_swap_xz)
{
     struct rpn_elt           *_stk_x, *_stk_y,
                              *_stk_z;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 3) {
          _stk_x    = rpn_pop(stack);
          _stk_y    = rpn_pop(stack);
          _stk_z    = rpn_pop(stack);

          rpn_push(stack, _stk_x);
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_z);
     }

     return _retcode;
}

/* rpn_op_swap_xz() }}} */
/* rpn_op_swap_xt() {{{ */
/******************************************************************************

                         RPN_OP_SWAP_XT

******************************************************************************/
RPN_DEF_OP(rpn_op_swap_xt)
{
     struct rpn_elt           *_stk_x, *_stk_y,
                              *_stk_z, *_stk_t;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 4) {
          _stk_x    = rpn_pop(stack);
          _stk_y    = rpn_pop(stack);
          _stk_z    = rpn_pop(stack);
          _stk_t    = rpn_pop(stack);

          rpn_push(stack, _stk_x);
          rpn_push(stack, _stk_z);
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_t);
     }

     return _retcode;
}

/* rpn_op_swap_xt() }}} */
/* rpn_op_swap_xl() {{{ */
/******************************************************************************

                         RPN_OP_SWAP_XL

******************************************************************************/
RPN_DEF_OP(rpn_op_swap_xl)
{
     struct rpn_elt           *_stk_x;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 1) {
          _stk_x    = rpn_pop(stack);

          rpn_push(stack, stack->lastx);
          stack->lastx   = _stk_x;
     }

     return _retcode;
}

/* rpn_op_swap_xl() }}} */
/* rpn_op() {{{ */
/******************************************************************************

                         RPN_OP

******************************************************************************/
int rpn_op(rpn_stack *stack, rpn_operator *op)
{
     int                       _retcode, _lg_start;
     struct timeval            _time1, _time2;
     struct tm                *_tm;
     int                       _sec, _usec, _sw_on;

     stack->clear_x      = FALSE;
     stack->last_op      = op;

     if (stack->new_value) {
          rpn_disp_elt(stack->top_elt, RPN_DISP_VALUE|RPN_DISP_INFOS);
          printf("%15s\n", op->op_name);
     }
     else {
          rpn_disp_elt(NULL, RPN_DISP_VALUE|RPN_DISP_INFOS);
          printf("%15s\n", op->op_name);
     }
     stack->new_value    = FALSE;

     if ((_sw_on = G.sw_on)) {
          /* Get system_time
             ~~~~~~~~~~~~~~~ */
          if (gettimeofday(&_time1, 0) != 0) {
               perror("gettimeofday");
               exit(RPN_EXIT_GET_TIME);
          }
//        _tm       = localtime(&_time1.tv_sec);
     }

     /* Execute operator
        ~~~~~~~~~~~~~~~~ */
     _retcode            =  (*op->func)(stack, op);

     if (_sw_on) {
          /* Get system_time
             ~~~~~~~~~~~~~~~ */
          if (gettimeofday(&_time2, 0) != 0) {
               perror("gettimeofday");
               exit(RPN_EXIT_GET_TIME);
          }
//        _tm       = localtime(&_time2.tv_sec);

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
          printf("%s : %6d.%06d s\n", op->op_name, _sec, _usec);
     }

     if (op->nb_args_out > 0) {
          rpn_disp_elt(stack->top_elt, RPN_DISP_VALUE|RPN_DISP_INFOS);
          printf("%15s\n", "***");
     }

     return _retcode;
}

/* rpn_op() }}} */
/* rpn_op_enter() {{{ */
/******************************************************************************

                         RPN_OP_ENTER

******************************************************************************/
RPN_DEF_OP(rpn_op_enter)
{
     struct rpn_elt           *_stk_x, *_elt;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 1) {
          _stk_x    = rpn_pop(stack);
          _elt      = rpn_clone_elt(_stk_x);
          rpn_push(stack, _stk_x);
          rpn_push(stack, _elt);
     }

     stack->clear_x = TRUE;

     return _retcode;
}

/* rpn_op_enter() }}} */
/* rpn_op_clx() {{{ */
/******************************************************************************

                         RPN_OP_CLX

******************************************************************************/
RPN_DEF_OP(rpn_op_clx)
{
     struct rpn_elt           *_stk_x, *_nil;

     _stk_x         = rpn_pop(stack);
     rpn_free_elt(&_stk_x);

     _nil           = rpn_new_elt(RPN_TYPE_NIL);
     rpn_push(stack, _nil);

     stack->clear_x = TRUE;

     return RPN_RET_OK;
}

/* rpn_op_clx() }}} */
/* rpn_op_delx() {{{ */
/******************************************************************************

                         RPN_OP_DELX

******************************************************************************/
RPN_DEF_OP(rpn_op_delx)
{
     struct rpn_elt           *_stk_x, *_nil;

//M
     _stk_x         = rpn_pop(stack);
//M
     rpn_free_elt(&_stk_x);
//M

     return RPN_RET_OK;
}

/* rpn_op_delx() }}} */
/* rpn_op_del_l() {{{ */
/******************************************************************************

                         RPN_OP_DEL_L

******************************************************************************/
RPN_DEF_OP(rpn_op_del_l)
{
     struct rpn_elt           *_stk_x, *_nil;

     rpn_free_elt(&stack->lastx);
     stack->lastx             = NULL;

     return RPN_RET_OK;
}

/* rpn_op_del_l() }}} */
/* rpn_op_clst() {{{ */
/******************************************************************************

                         RPN_OP_CLST

******************************************************************************/
RPN_DEF_OP(rpn_op_clst)
{
     struct rpn_elt           *_stk_x, *_nil;

     for (_stk_x = rpn_pop(stack); stack->num_elts != 0;_stk_x = rpn_pop(stack)) {
          rpn_free_elt(&_stk_x);
     }

     _nil           = rpn_new_elt(RPN_TYPE_NIL);
     rpn_push(stack, _nil);

     stack->clear_x = TRUE;

     return RPN_RET_OK;
}

/* rpn_op_clst() }}} */
/* rpn_op_prx() {{{ */
/******************************************************************************

                         RPN_OP_PRX

******************************************************************************/
RPN_DEF_OP(rpn_op_prx)
{
     rpn_disp_elt(stack->top_elt, RPN_DISP_VALUE|RPN_DISP_INFOS);
     printf("%15s\n", op->op_name);

     return RPN_RET_OK;
}

/* rpn_op_prx() }}} */
/* rpn_op_prstk() {{{ */
/******************************************************************************

                         RPN_OP_PRSTK

******************************************************************************/
RPN_DEF_OP(rpn_op_prstk)
{
     /* Display stack
        ~~~~~~~~~~~~~ */
     rpn_disp_stack(stack);

     return RPN_RET_OK;
}

/* rpn_op_prstk() }}} */
/* rpn_op_stk() {{{ */
/******************************************************************************

                         RPN_OP_STK

******************************************************************************/
RPN_DEF_OP(rpn_op_stk)
{
     /* Display stack
        ~~~~~~~~~~~~~ */
     rpn_disp_stk(stack);

     return RPN_RET_OK;
}

/* rpn_op_stk() }}} */
/* rpn_op_dupx() {{{ */

/******************************************************************************

                         RPN_OP_DUPX

******************************************************************************/
RPN_DEF_OP(rpn_op_dupx)
{
     int                       _retcode, _X_type, _max, _i;
     rpn_elt                  *_stk_x, *_stk_y, *_clone;

     _retcode                 = RPN_RET_OK;

     /* Check minimum number of elements in the stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (stack->num_elts < 2) {
          _retcode                 = RPN_RET_NOT_ENOUGH_ELTS;
          goto end;
     }

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     if (_X_type != RPN_TYPE_INT) {
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          goto end;
     }

     _stk_y                   = rpn_pop(stack);

     /* Push Y back on the stack
        ~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_push(stack, _stk_y);

     _max                     = _stk_x->value.i;
     for (_i = 0; _i < _max - 1; _i++) {
          /* Clone Y
             ~~~~~~~ */
          _clone                   = rpn_clone_elt(_stk_y);

          /* Push clone on the stack
             ~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _clone);
     }

     /* Initialize lastx
        ~~~~~~~~~~~~~~~~ */
     rpn_set_lastx(stack, _stk_x);

     /* Free X
        ~~~~~~ */
     rpn_free_elt(&_stk_x);

end:
     return _retcode;
}

/* rpn_op_dupx() }}} */
/* ron_set_lastx() {{{ */
/******************************************************************************

                         RPN_SET_LASTX

******************************************************************************/
void rpn_set_lastx(rpn_stack *stack, rpn_elt *elt)
{
//X
     if (stack->lastx) {
          /* Free previous last X */
//X
          rpn_free_elt(&stack->lastx);
//X
     }

//X
     stack->lastx        = rpn_clone_elt(elt);
//X
}

/* ron_set_lastx() }}} */
/* rpn_op_lastx() {{{ */
/******************************************************************************

                         RPN_OP_LASTX

******************************************************************************/
RPN_DEF_OP(rpn_op_lastx)
{
     struct rpn_elt           *_stk_x;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 1) {
          rpn_push(stack, rpn_clone_elt(stack->lastx));
     }

     return _retcode;
}

/* rpn_op_lastx() }}} */
/* rpn_op_rcl_x() {{{ */
/******************************************************************************

                         RPN_OP_RCL_X

******************************************************************************/
RPN_DEF_OP(rpn_op_rcl_x)
{
     struct rpn_elt           *_stk_x, *_elt;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 1) {
          _stk_x    = rpn_pop(stack);
          _elt      = rpn_clone_elt(_stk_x);
          rpn_push(stack, _stk_x);
          rpn_push(stack, _elt);
     }

     return _retcode;
}

/* rpn_op_rcl_x() }}} */
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
/* rpn_op_sw_on() {{{ */
/******************************************************************************

                         RPN_OP_SW_ON

******************************************************************************/
RPN_DEF_OP(rpn_op_sw_on)
{
     int             _retcode;

     _retcode            = RPN_RET_OK;

     G.sw_on             = TRUE;

     return _retcode;
}

/* rpn_op_sw_on() }}} */
/* rpn_op_sw_off() {{{ */
/******************************************************************************

                         RPN_OP_SW_OFF

******************************************************************************/
RPN_DEF_OP(rpn_op_sw_off)
{
     int             _retcode;

     _retcode            = RPN_RET_OK;

     G.sw_on             = FALSE;

     return _retcode;
}

/* rpn_op_sw_off() }}} */
/* rpn_op_list() {{{ */

/******************************************************************************

                         RPN_OP_LIST

******************************************************************************/
RPN_DEF_OP(rpn_op_list)
{
     int                  _retcode;
     rpn_elt             *_elt_list, *_elt;
     rpn_list            *_list;

     _retcode            = RPN_RET_OK;

     if (stack->nb_begin > 0) {
          _elt_list           = rpn_new_elt(RPN_TYPE_LIST);
          _list               = rpn_new_list((char *)__func__);

          for ( ; ; ) {
               _elt                = rpn_pop(stack);
               if (rpn_get_type(_elt) == RPN_TYPE_START_MARKER) {
                    rpn_dec_begin(stack);
                    break;
               }
               rpn_list_push_head(_list, _elt);
          }

          _elt_list->value.obj = _list;

          rpn_push(stack, _elt_list);
     }
     else {
          _retcode            = RPN_RET_NO_START_MARKER;
     }

     return _retcode;
}

/* rpn_op_list() }}} */
/* rpn_op_tuples() {{{ */

/******************************************************************************

                         RPN_OP_TUPLES

******************************************************************************/
RPN_DEF_OP(rpn_op_tuples)
{
     int                  _retcode;
     rpn_elt             *_elt_list, *_elt;
     rpn_list            *_list;

     _retcode            = RPN_RET_OK;

     if (stack->nb_begin > 0) {
          _elt_list           = rpn_new_elt(RPN_TYPE_TUPLE);
          _list               = rpn_new_list((char *)__func__);

          for ( ; ; ) {
               _elt                = rpn_pop(stack);
               if (rpn_get_type(_elt) == RPN_TYPE_START_MARKER) {
                    rpn_dec_begin(stack);
                    break;
               }
               rpn_list_push_head(_list, _elt);
          }

          _elt_list->value.obj = _list;

          rpn_push(stack, _elt_list);
     }
     else {
          _retcode            = RPN_RET_NO_START_MARKER;
     }

     return _retcode;
}

/* rpn_op_tuples() }}} */
/* rpn_op_pairs() {{{ */

/******************************************************************************

                         RPN_OP_PAIRS

******************************************************************************/
RPN_DEF_OP(rpn_op_pairs)
{
     int                  _retcode;
     rpn_elt             *_elt_pair, *_elt;
     rpn_pair            *_pair;

     _retcode            = RPN_RET_OK;

     if (stack->num_elts >= 2) {
          _elt_pair                = rpn_new_elt(RPN_TYPE_OPAIR);
          _pair                    = rpn_new_pair((char *)__func__);

          _pair->elt_b             = rpn_pop(stack);
          _pair->elt_a             = rpn_pop(stack);

          _elt_pair->value.obj     = _pair;

          rpn_push(stack, _elt_pair);
     }
     else {
          _retcode            = RPN_RET_NOT_ENOUGH_ELTS;
     }

     return _retcode;
}

/* rpn_op_pairs() }}} */
/* rpn_search_op() {{{ */

/******************************************************************************

                         RPN_SEARCH_OP

******************************************************************************/
rpn_operator *rpn_search_op(rpn_operator *operators, char *operator)
{
     rpn_operator        *_op;

     for (_op = operators; _op->func != 0; _op++) {
          if (!strcmp(_op->op_name, operator)) {
               /* Operator found
                  ~~~~~~~~~~~~~~ */
               break;
          }
     }

     if (_op->func == 0) {
          _op       = 0;
     }

     return _op;
}

/* rpn_search_op() }}} */
/* rpn_catalog() {{{ */
/******************************************************************************

                         RPN_CATALOG

******************************************************************************/
int rpn_catalog(struct rpn_operator *ops)
{
     struct rpn_operator      *_op;
     char                     *_fmt_s, *_fmt_d;
     struct rpn_op_groups     *_group;
     int                       _i, _lang;
     char                     *_env_lang;

     _fmt_s                   = "     %-25s %10s %10s\n";
     _fmt_d                   = "     %-25s %10d %10d\n";

     _env_lang                = getenv("LANG");
     if (_env_lang != NULL && (!strncmp(_env_lang, "fr", 2))) {
          _lang                    = 1;
     }
     else {
          _lang                    = 0;
     }

     for (_group = rpn_op_groups, _op = ops; _op == ops || _group->end != 0; _group++) {
//        printf("=== [%s] === MSG_IDX = %3d\n", rpn_mesg_groups[_group->desc_idx].mesg[0], _group->end);
          printf("[%s]\n", rpn_mesg_groups[_group->desc_idx].mesg[_lang]);
          printf(_fmt_s, "--- OPERATOR ---", "IN PARAMS", "OUT PARAMS");

//        for (_i = 0; _i < 2; _i++) {
//             printf("     ===> %s\n", rpn_mesg_groups[_group->desc_idx].mesg[_i]);
//        }

          for (; _op->op_name != NULL && ((_op->op_id <= _group->end) || (_op == ops)); _op++) {
               if (_op->activated) {
                    printf(_fmt_d, _op->op_name, _op->nb_args_in, _op->nb_args_out);
               }
          }

          printf("\n");
     }
}

/* rpn_catalog() }}} */
/* rpn_todo() {{{ */
/******************************************************************************

                         RPN_TODO

******************************************************************************/
int rpn_todo(struct rpn_operator *ops)
{
     struct rpn_operator      *_op;
     char                     *_fmt_s, *_fmt_d;
     struct rpn_op_groups     *_group;
     int                       _i;

     _fmt_s    = "     %-25s %10s %10s\n";
     _fmt_d    = "     %-25s %10d %10d\n";

     for (_group = rpn_op_groups, _op = ops; _op == ops || _group->end != 0; _group++) {
          printf("[%s]\n", rpn_mesg_groups[_group->desc_idx].mesg[0]);
          printf(_fmt_s, "===   TODO   ===", "IN PARAMS", "OUT PARAMS");
          for (_i = 0; _i < 2; _i++) {
//             printf("     ===> %s\n", rpn_mesg_groups[_group->desc_idx].mesg[_i]);
          }
          for (; _op->op_name != NULL && ((_op->op_id <= _group->end) || (_op == ops)); _op++) {
               if (!_op->activated) {
                    printf(_fmt_d, _op->op_name, _op->nb_args_in, _op->nb_args_out);
               }
          }

          printf("\n");
     }
}

/* rpn_todo() }}} */
/* rpn_err_msg_no_mem() {{{ */
/******************************************************************************

                         RPN_ERR_MSG_NO_MEM

******************************************************************************/
void rpn_err_msg_no_mem()
{
     fprintf(stderr, "%s: not enough memory !\n", G.progname);
}

/* rpn_err_msg_no_mem() }}} */
/* rpn_err_msg_op_not_found() {{{ */
/******************************************************************************

                         RPN_ERR_MSG_OP_NOT_FOUND

******************************************************************************/
void rpn_err_msg_op_not_found(char *operator)
{
     fprintf(stderr, "%s: operator \"%s\" not found !\n", G.progname, operator);
}

/* rpn_err_msg_op_not_found() }}} */
/* rpn_err_msg_op_error() {{{ */
/******************************************************************************

                         RPN_ERR_MSG_OP_ERROR

******************************************************************************/
void rpn_err_msg_op_error(char *operator, int retcode)
{
     fprintf(stderr, "%s: operator \"%s\" : %s !\n",
             G.progname, operator, rpn_err_to_string(retcode));
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
/* rpn_strip_quotes() {{{ */
/******************************************************************************

                         RPN_STRIP_QUOTES

******************************************************************************/
int rpn_strip_quotes(char **str)
{
     int                 _lg, _retcode;
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
          _new_str            = malloc(_lg);
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
     int                 _lg, _retcode;
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
          _new_str            = malloc(_lg);
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
/* rpn_cehck_types() {{{ */
/******************************************************************************

                         RPN_CHECK_TYPES

******************************************************************************/
int rpn_check_types(rpn_operator *op, struct rpn_stack *stack)
{
     struct rpn_st_types      *_t;
     int                       _X_type = -1, _Y_type   = -1,
                               _retcode;
     bool                      _good_X = FALSE, _good_Y = FALSE;

     _retcode       = RPN_RET_INVALID_TYPES;

     _t             = op->st_types;
     if (_t->nb_operands != 0) {
          switch (_t->nb_operands) {

          case 1:
               _X_type   = rpn_get_X_type(stack);

               for (_t = op->st_types; _t->nb_operands != 0; _t++) {
//fprintf(stderr, "_X_type = %d  _t->X_type = %d\n", _X_type, _t->X_type);
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
// fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
// fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));

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

/* rpn_cehck_types() }}} */
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

     case RPN_TYPE_NEURAL_MLP:
          _str_type      = "NEURAL_MLP";
          break;

     case RPN_TYPE_FILENAME:
          _str_type      = "FILENAME";
          break;

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

     default:
          sprintf(_err_mesg, "??? (%d)", type);
          _str_type      = _err_mesg;
          break;
     }

     return _str_type;
}

/* rpn_type_to_string() }}} */
/* rpn_check_ops() {{{ */
/******************************************************************************

                         RPN_CHECK_OPS

******************************************************************************/
void rpn_check_ops()
{
     int                       _i, _error;
     rpn_operator             *_op;

     _error         = FALSE;

     for (_i = 0; rpn_operators[_i].func != 0; _i++) {
          _op            = &rpn_operators[_i];

          if (_op->op_id != _i) {
               _error         = TRUE;
               fprintf(stderr, "*** Inconsistency : %-25s : ID = %3d, should be %3d !\n",
                       _op->op_name, _op->op_id, _i);
          }
     }

     if (_error) {
          for (_i = 0; rpn_operators[_i].func != 0; _i++) {
               _op            = &rpn_operators[_i];

               fprintf(stderr, "*** %5d %-25s : ID = %3d\n",
                       _i, _op->op_name, _op->op_id);
          }
          RPN_INTERNAL_ERROR;
     }
}

/* rpn_check_ops() }}} */
/* rpn_new_sigma() {{{ */
/******************************************************************************

                         RPN_NEW_SIGMA

******************************************************************************/
struct rpn_sigma *rpn_new_sigma(char *name)
{
     struct rpn_sigma         *_sigma;
     int                       _i;

     if ((_sigma = (struct rpn_sigma *) RPN_MALLOC(sizeof(*_sigma))) == 0) {
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
     int                       _type, _size;
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

     if ((_cmd = malloc(_size)) == NULL) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     _cmd[0]                  = '\0';

     for (_elt = _list->top_elt; _elt != NULL; _elt = _elt->previous_elt) {
          _type                    = rpn_get_type(_elt);

          switch (_type) {

          case RPN_TYPE_STRING:
          case RPN_TYPE_LITTERAL:
          case RPN_TYPE_FILENAME:
               strcat(_cmd, _elt->value.s);
               strcat(_cmd, " ");
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
          case RPN_TYPE_LITTERAL:
          case RPN_TYPE_FILENAME:
               _size               += strlen(_elt->value.s) + 1;
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
int rpn_exec(char *cmd_path, char **argp, int *fd_pipe)
{
	int				 _pid, _child_pid = 0;

     switch ((_pid = fork())) {

     case -1:
          fprintf(stderr, "%s: cannot fork !\n", G.progname);
          exit(RPN_EXIT_FORK_ERROR);
          break;

     case 0:
          /* Child process
             ~~~~~~~~~~~~~ */
          if (fd_pipe) {
               close(1);
               dup(fd_pipe[1]);
               close(fd_pipe[1]);
               close(fd_pipe[0]);
			close(0);
          }

          execvp(cmd_path, argp);
          perror("execvp");
          fprintf(stderr, "%s: cannot exec \"%s\" !\n", G.progname, cmd_path);
          exit(RPN_EXIT_EXEC_ERROR);
          break;

     default:
          /* Parent process
             ~~~~~~~~~~~~~~ */
		_child_pid			= _pid;
          if (fd_pipe) {
               close(fd_pipe[1]);
//             close(0);
//             dup(fd_pipe[0]);
//             close(fd_pipe[0]);
          }
          break;
     }

	return _child_pid;
}

/* rpn_exec() }}} */
