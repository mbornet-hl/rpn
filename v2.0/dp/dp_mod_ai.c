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
 *	@(#)	[MB] dp_mod_ai.c	Version 1.12 du 21/10/20 - 
 */

#include	<fcntl.h>
#include	<unistd.h>
#include	<math.h>
#include	<libgen.h>
#include	"../cc/cc_types.h"
#include	"../cy/cy_rpn_header.h"
#include	"../cy/cy_rpn_proto.h"
#include	"../cy/cy_rpn_epub.h"
#include	"../dl/dl_cpub.h"
#include	"../dl/dl_epub.h"
#include	"../df/df_epub.h"
#include	"dp_serial.h"
#include	"dp_cpub.h"
#include	"dp_epri.h"

/* Methods
   ~~~~~~~ */
RPN_DEFN_METHODS(dp);

/* List of types managed by the module
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static int						 dp_managed_types[] = {
	RPN_TYPE_NEURAL_MLP,
	RPN_TYPE_TRAINING_ELT,
	RPN_TYPE_TRAINING_SET,
	RPN_TYPE_TEST_ELT,
	RPN_TYPE_TEST_SET,
	0
};

static dl_op_desc					dp_ops_array[];

/* Help messages {{{ */
char                               *dp_help_ignore[] = {
     "Ignore specified element",
     0
},
                                   *dp_help_use[] = {
     "Use specified element",
     0
};

/* Help messages }}} */
/* Module descriptor {{{ */
struct dl_module		ai_module = {
	"ai",
	"2.0",
	DP_LINK_ID,
	0,
	dp_ops_array,
	dp_init
};

/* Module descriptor }}} */
/* Operator parameters descriptions {{{ */
static dl_op_params					 dp_params_clone[] = {
	DL_OP_DEF1(dp_op_ai_clone, 1, TRAINING_ELT),
	DL_OP_DEF1(dp_op_ai_clone, 1, TEST_ELT),
	DL_OP_DEF1(dp_op_ai_clone, 1, TRAINING_SET),
	DL_OP_DEF1(dp_op_ai_clone, 1, TEST_SET),
	DL_OP_DEF_END
};

static dl_op_params					 dp_params_matrix[] = {
	DL_OP_DEF1(dp_op_ai_matrix, 1, TRAINING_ELT),
	DL_OP_DEF1(dp_op_ai_matrix, 1, TEST_ELT),
	DL_OP_DEF_END
};

#if 0
static dl_op_params					 dp_params_list[] = {
	DL_OP_DEF2(dp_op_ai_list, 1, STRING, STRING),
	DL_OP_DEF_END
};
#endif

static dl_op_params					 dp_params_create_mlp[] = {
	DL_OP_DEF2(dp_op_ai_create_mlp, 1, LITTERAL, INT),
	DL_OP_DEF_END
};

static dl_op_params					 dp_params_read_mnist[] = {
	DL_OP_DEF0(dp_op_ai_read_mnist, 1),
	DL_OP_DEF_END
};

#if 0
static dl_op_params					 dp_params_read_elt[] = {
	DL_OP_DEF1(dp_op_ai_read_elt, 1, STRING),
	DL_OP_DEF_END
};
#endif

static dl_op_params					 dp_params_mnist_pic[] = {
	DL_OP_DEF1(dp_op_ai_mnist_pic, 1, TRAINING_ELT),
	DL_OP_DEF1(dp_op_ai_mnist_pic, 1, TEST_ELT),
	DL_OP_DEF_END
};

static dl_op_params					 dp_params_mnist_pics[] = {
	DL_OP_DEF1(dp_op_ai_mnist_pics, 1, TRAINING_SET),
	DL_OP_DEF1(dp_op_ai_mnist_pics, 1, TEST_SET),
	DL_OP_DEF3(dp_op_ai_mnist_pics, 1, INT, INT, TRAINING_SET),
	DL_OP_DEF3(dp_op_ai_mnist_pics, 1, INT, INT, TEST_SET),
	DL_OP_DEF_END
};

static dl_op_params					 dp_params_get[] = {
	DL_OP_DEF1(dp_op_ai_get, 1, TRAINING_SET),
	DL_OP_DEF1(dp_op_ai_get, 1, TEST_SET),
	DL_OP_DEF2(dp_op_ai_get, 1, INT, TRAINING_SET),
	DL_OP_DEF2(dp_op_ai_get, 1, INT, TEST_SET),
	DL_OP_DEF_END
};

static dl_op_params					 dp_params_mat_7_seg[] = {
	DL_OP_DEF1(dp_op_ai_mat_7_seg, 1, INT),
	DL_OP_DEF_END
};

static dl_op_params					 dp_params_label[] = {
	DL_OP_DEF1(dp_op_ai_label, 1, TRAINING_ELT),
	DL_OP_DEF1(dp_op_ai_label, 1, TEST_ELT),
	DL_OP_DEF_END
};

static dl_op_params					 dp_params_index[] = {
	DL_OP_DEF1(dp_op_ai_index, 1, TRAINING_ELT),
	DL_OP_DEF1(dp_op_ai_index, 1, TEST_ELT),
	DL_OP_DEF1(dp_op_ai_index, 1, TRAINING_SET),
	DL_OP_DEF1(dp_op_ai_index, 1, TEST_SET),
	DL_OP_DEF_END
};

static dl_op_params					 dp_params_orig_index[] = {
	DL_OP_DEF1(dp_op_ai_orig_index, 1, TRAINING_ELT),
	DL_OP_DEF1(dp_op_ai_orig_index, 1, TEST_ELT),
	DL_OP_DEF1(dp_op_ai_orig_index, 1, TRAINING_SET),
	DL_OP_DEF1(dp_op_ai_orig_index, 1, TEST_SET),
	DL_OP_DEF_END
};

static dl_op_params					 dp_params_dispatch[] = {
	DL_OP_DEF1(dp_op_ai_dispatch, 1, TRAINING_SET),
	DL_OP_DEF1(dp_op_ai_dispatch, 1, TEST_SET),
	DL_OP_DEF_END
};

static dl_op_params					 dp_params_ignore[] = {
	DL_OP_DEF1H(dp_op_ai_ignore, 1, TRAINING_ELT, dp_help_ignore),
	DL_OP_DEF2H(dp_op_ai_ignore, 1, INT, TRAINING_SET, dp_help_ignore),
	DL_OP_DEF_END
};

static dl_op_params					 dp_params_use[] = {
	DL_OP_DEF1H(dp_op_ai_use, 1, TRAINING_ELT, dp_help_ignore),
	DL_OP_DEF2H(dp_op_ai_use, 1, INT, TRAINING_SET, dp_help_ignore),
	DL_OP_DEF_END
};

/* Operator parameters descriptions }}} */
/* Operators list {{{ */
static dl_op_desc					 dp_ops_array[] = {
	{	"clone",				dp_params_clone					},
	{	"create_mlp",			dp_params_create_mlp				},
#if 0
	{	"learn",				dp_params_learn					},
	{	"classify",			dp_params_classify					},
	{	"set_layer_weights",	dp_params_set_layer_weights			},
	{	"alea_layer_weights",	dp_params_alea_layer_weights			},
	{	"set_mlp_weights",		dp_params_set_mlp_weights			},
	{	"alea_mlp_weights",		dp_params_alea_mlp_weights			},
#endif
	{	"read_mnist",			dp_params_read_mnist				},
	{	"mnist_pic",			dp_params_mnist_pic					},
	{	"mnist_pics",			dp_params_mnist_pics				},
#if 0
	{	"read_elt",			dp_params_read_elt					},
#endif
	{	"get",				dp_params_get						},
	{	"mat",				dp_params_matrix					},
	{	"mat7seg",			dp_params_mat_7_seg					},
	{	"label",				dp_params_label					},
	{	"index",				dp_params_index					},
	{	"orig_index",			dp_params_orig_index				},
	{	"dispatch",			dp_params_dispatch					},
	{	"ignore",				dp_params_ignore					},
	{	"use",				dp_params_use						},
	{	0,					0								}
};

/* Operators list }}} */

// GROUP : AI {{{
/* dp_init() {{{ */

/******************************************************************************

					DP_INIT

******************************************************************************/
RPN_DEFN_INIT(dp)

/* dp_init() }}} */
/* Methods {{{ */
/* dp_disp_elt() {{{ */

/******************************************************************************

					DP_DISP_ELT

******************************************************************************/
RPN_DEFN_DISP(dp)
{
     int                  _type;

	switch (_type = rpn_get_type(elt)) {

	case RPN_TYPE_NEURAL_MLP:
		{
			struct rpn_mlp      *_mlp;

			_mlp                = (struct rpn_mlp *) elt->value.obj;
			printf("NEURAL MLP (%d layers, %d neurons, %d weights, activation function : %s)",
				  _mlp->nb_layers, _mlp->nb_neurons, _mlp->nb_weights, _mlp->str_af);
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
					  RPN_DISP_SZ, "TRAINING_ELT",
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
				printf("%-*s '%s'", RPN_DISP_SZ, "TRAINING_SET", _train_set->name);
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
					  RPN_DISP_SZ, "TEST_ELT    ",
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
				printf("%-*s '%s'", RPN_DISP_SZ, "TEST_SET    ", _train_set->name);
				if (disp_flags & RPN_DISP_INFOS) {
					printf(" %5d elts, current idx = %d",
						  _train_set->nb_elts, _train_set->current_idx);
				}
			}
		}
		break;

	default :
		RPN_INTERNAL_ERROR;
		break;
	}
}

/* dp_disp_elt() }}} */
/* dp_clone_elt() {{{ */

/******************************************************************************

					DP_CLONE_ELT

******************************************************************************/
RPN_DEFN_CLONE(dp)
{
#if 0
     int                  _type;

     _type               = rpn_get_type(elt);

     switch (type) {

     case RPN_TYPE_HOSTSFILE:
          if (elt->value.s) {
               /* Free string */
               RPN_FREE(_elt->value.s);
          }

          /* Free element */
          RPN_FREE(_elt);
          break;

     default:
          RPN_INTERNAL_ERROR;
		break;
     }
#endif /* 0 */
	RPN_INTERNAL_ERROR;
}

/* dp_clone_elt() }}} */
/* dp_type_to_string() {{{ */

/******************************************************************************

					DP_TYPE_TO_STRING

******************************************************************************/
RPN_DEFN_TYPE_TO_STR(dp)
{
     char                     *_str_type;

     switch (type) {

     case RPN_TYPE_NEURAL_MLP:
          _str_type      = "NEURAL_MLP";
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

     default:
          RPN_INTERNAL_ERROR;
          break;
     }

	return _str_type;
}

/* dp_type_to_string() }}} */
/* dp_free_elt() {{{ */

/******************************************************************************

					DP_FREE_ELT

******************************************************************************/
RPN_DEFN_FREE(dp)
{
     switch (type) {

     case RPN_TYPE_TRAINING_ELT:
     case RPN_TYPE_TEST_ELT:
          {
               struct rpn_training_elt       *_train;
               struct rpn_training_data      *_data;
//             struct rpn_training_label     *_label;

               _train                        = elt->value.obj;
               _data                         = _train->data;
//             _label                        = _train->label;

               RPN_FREE(_train->label);
               RPN_FREE(_data->vector);
               RPN_FREE(_data);
               RPN_FREE(_train);
          }

          /* Free element */
          RPN_FREE(elt);
          break;

     case RPN_TYPE_TRAINING_SET:
     case RPN_TYPE_TEST_SET:
          {
               int                       _nb;
               rpn_training_set         *_train_set;

               _train_set               = elt->value.obj;

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
          RPN_FREE(elt);
          break;

#if 0
	case	RPN_TYPE_NEURAL_MLP:
		dp_free_mlp(elt);
		break;
#endif

     default:
		RPN_INTERNAL_ERROR;
          break;
     }
}

/* dp_free_elt() }}} */
/* Methods }}} */
/* dp_op_ai_clone() {{{ */

/******************************************************************************

					DP_OP_AI_CLONE

******************************************************************************/
RPN_DEF_OP(dp_op_ai_clone)
{
	int					 _retcode = RPN_RET_OK;
	rpn_elt				*_stk_x, *_clone;

	_stk_x				= rpn_pop(stack);
	_clone				= rpn_pop(stack);

     switch (_stk_x->type) {

     case RPN_TYPE_TRAINING_ELT:
     case RPN_TYPE_TEST_ELT:
          {
               dp_training_elt          *_train, *_train_clone;
               dp_training_data         *_data,  *_data_clone;
               dp_training_label        *_label, *_label_clone;
               size_t                    _size;

               _train                   = _stk_x->value.obj;
               _data                    = _train->data;
               _label                   = _train->label;

               _train_clone             = dp_new_training_elt();

               _data_clone              = dp_new_training_data();
               _data_clone->index       = _data->index;
               _data_clone->orig_index  = _data->orig_index;
               _data_clone->ignore      = _data->ignore;
               _data_clone->nb_elts     = _data->nb_elts;
               _data_clone->elt_size    = _data->elt_size;
               _data_clone->num_rows    = _data->num_rows;
               _data_clone->num_cols    = _data->num_cols;
               _size                    = _data->nb_elts * _data->elt_size;

               if ((_data_clone->vector = (unsigned char *) RPN_MALLOC(_size)) == NULL) {
                    fprintf(stderr, "%s : cannot allocate %lu bytes for the images !\n", G.progname, _size);
                    exit(RPN_EXIT_NO_MEM);
               }

               memcpy(_data_clone->vector, _data->vector, _size);

               _train_clone->data       = _data_clone;

               _label_clone             = dp_new_training_label();
               _label_clone->index      = _label->index;
               _label_clone->value      = _label->value;

               _train_clone->label      = _label_clone;

               _clone->value.obj        = _train_clone;
          }
          break;

     case RPN_TYPE_TRAINING_SET:
     case RPN_TYPE_TEST_SET:
          {
               rpn_training_set         *_train_set, *_set_clone;
               size_t                    _size;
			int					 _i;

               _train_set               = _stk_x->value.obj;

               _size                    = sizeof(*_train_set) + ((_train_set->nb_elts - 1) * sizeof(rpn_elt *));
               _set_clone               = (rpn_training_set *) RPN_MALLOC(_size);
               _set_clone->name         = strdup(_train_set->name);
               _set_clone->nb_elts      = _train_set->nb_elts;
               _set_clone->current_idx  = _train_set->current_idx;
               _set_clone->nrows        = _train_set->nrows;
               _set_clone->ncols        = _train_set->ncols;
               _set_clone->width_pix    = _train_set->width_pix;
               _set_clone->height_pix   = _train_set->height_pix;
               _clone->value.obj        = _set_clone;

               for (_i = 0; _i < _train_set->nb_elts; _i++) {
                    _set_clone->array[_i]    = rpn_clone_elt(_train_set->array[_i]);
               }
          }
          break;

     default:
		fprintf(stderr, "%s() : unknown type (%d)\n", __func__, _stk_x->type);
		RPN_INTERNAL_ERROR;
          break;
     }

	rpn_push(stack, _clone);

	return _retcode;
}

/* dp_op_ai_clone() }}} */
#if 0
/* dp_op_ai_list() {{{ */

/******************************************************************************

                         DP_OP_AI_LIST

******************************************************************************/
RPN_DEF_OP(dp_op_ai_list)
{
     rpn_elt                  *_stk_x, *_stk_result;
     rpn_stack				*_stack;
     int                       _nb_layers, _X_type, _prev_dim, _dim,
                               _layer, _i, _nb_neurons, _nb_weights,
                               _nb_litteral = 0;
     int                       _retcode;
     dp_mlp				*_mlp;
     dp_perceptron			*_perceptron;
     dp_neuron				*_neuron;

     _retcode                 = RPN_RET_OK;

     /* Control that there is a BEGIN marker in the stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (stack->nb_begin == 0) {
          _retcode  = RPN_RET_NO_START_MARKER;
          rpn_err_msg_op_error(op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     /* Allocate temporary new stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _stack                   = rpn_new_stack(__func__);
     _nb_layers               = 0;

     /* Transfer elements into the new stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     for (;;) {
          _stk_x                   = rpn_pop(stack);
          _X_type                  =  rpn_get_type(_stk_x);
          if (_X_type == RPN_TYPE_START_MARKER) {
               rpn_dec_begin(stack);
               break;
          }

          switch (op->op_id) {

          case RPN_OP_ID_NN_CREAT_MLP:
               /* Only integers are allowed to specifiy dimensions
                * and only one litteral is allowed to specify which
                * activation function will be used
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               switch (_X_type) {

               case RPN_TYPE_INT:
                    break;

               case RPN_TYPE_LITTERAL:
                    if (++_nb_litteral > 1) {
                         /* Only one litteral is allowed
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _retcode                 = RPN_RET_INVALID_TYPES;
                         goto end;
                    }
                    break;

               default:
                    _retcode                 = RPN_RET_INVALID_TYPES;
                    goto end;
               }

               _nb_layers++;
               break;

          default:
               RPN_INTERNAL_ERROR;
               break;
          }

          rpn_push(_stack, _stk_x);
          rpn_disp_stack(_stack);
     }

     /* Decrease nb_layers by 1 to take the input layer into account
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _nb_layers--;

     switch (op->op_id) {

     case RPN_OP_ID_NN_CREAT_MLP:
          if (_nb_layers < 2) {
               /* Not enough layers
                  ~~~~~~~~~~~~~~~~~ */
               _retcode                 = RPN_RET_NOT_ENOUGH_ELTS;

               /* Restore stack contents
                  ~~~~~~~~~~~~~~~~~~~~~~ */
               for (; _stack->num_elts > 0; ) {
                    _stk_x    = rpn_pop(_stack);
                    rpn_push(stack, _stk_x);
               }

               goto end;
          }

          /* Allocate and create a neural network
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          _stk_result              = rpn_new_elt(RPN_TYPE_NEURAL_MLP);

          _layer                   = 0;
          _prev_dim                = 0;
          _dim                     = 0;
          _mlp                     = dp_new_mlp(_nb_layers);
          _stk_result->value.obj   = _mlp;

          for (; _stack->num_elts > 0; ) {
               _stk_x                   = rpn_pop(_stack);
               _X_type                  =  rpn_get_type(_stk_x);
//printf("X TYPE = %s\n", rpn_type_to_string(_X_type));
               if (_X_type == RPN_TYPE_INT) {
                    _prev_dim                = _dim;
                    _dim                     = _stk_x->value.i;
                    _nb_neurons              = _dim;
                    _nb_weights              = _prev_dim;

                    if (_prev_dim == 0) {
                         _mlp->input              = dp_new_input_layer(_dim);
                         _perceptron              = NULL;
                    }
                    else {
                         _perceptron              = dp_new_perceptron(_nb_neurons);

                         for (_i = 0; _i < _nb_neurons; _i++) {
                              _neuron                  = dp_new_neuron(_nb_weights);
                              _perceptron->neurons[_i] = _neuron;
                              _mlp->nb_weights         += _nb_weights + 1;
                         }
                         _mlp->nb_neurons         += _nb_neurons;
                         _mlp->layers[_layer++]   = _perceptron;
//fprintf(stderr, "mlp->layers[%d] = 0x%08X\n", _layer - 1, _mlp->layers[_layer - 1]);
                    }

               }
               else if (_X_type == RPN_TYPE_LITTERAL) {
                    char                     *_af_function_str;

                    /* Decode activation function specifier
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _af_function_str         = _stk_x->value.s;

                    if (!strcmp(_af_function_str, RPN_NN_AF_SIGMOID)) {
                         _mlp->activation         = dp_nn_af_sigmoid;
                    }
                    else if (!strcmp(_af_function_str, RPN_NN_AF_TANH)) {
                         _mlp->activation         = dp_nn_af_tanh;
                    }
                    else if (!strcmp(_af_function_str, RPN_NN_AF_ARC_TAN)) {
                         _mlp->activation         = dp_nn_af_arc_tan;
                    }
                    else if (!strcmp(_af_function_str, RPN_NN_AF_RELU)) {
                         _mlp->activation         = dp_nn_af_relu;
                    }
                    else {
                         fprintf(stderr, "%s (%d) : unknown activation function (\"%s\')\n",
                                 __func__, __LINE__, _af_function_str);
                    }
                    _mlp->str_af             = strdup(_af_function_str);
               }
//printf("\n");
          }
          break;

     default:
          fprintf(stderr, "%s (%s) (%d) : not implemented for \"%s\"\n",
                  G.progname, __func__, __LINE__, op->op_name);
          RPN_INTERNAL_ERROR;
     }

//printf("MLP : %10d layers\n",  _mlp->nb_layers);
//printf("MLP : %10d neurons\n", _mlp->nb_neurons);
//printf("MLP : %10d weights\n",   _mlp->nb_weights);
//printf("\n");

     rpn_free_stack(_stack);
     rpn_push(stack, _stk_result);

end:
     return _retcode;
}

/* dp_op_ai_list() }}} */
#endif
/* dp_op_ai_create_mlp() {{{ */

/******************************************************************************

                         DP_OP_AI_CREATE_MLP

******************************************************************************/
RPN_DEF_OP(dp_op_ai_create_mlp)
{
     rpn_elt                  *_stk_x, *_stk_result;
     rpn_stack				*_stack;
     int                       _nb_layers, _X_type, _prev_dim, _dim,
                               _layer, _i, _nb_neurons, _nb_weights,
                               _nb_litteral = 0;
     int                       _retcode;
     dp_mlp				*_mlp;
     dp_perceptron			*_perceptron;
     dp_neuron				*_neuron;

     _retcode                 = RPN_RET_OK;

     /* Control that there is a BEGIN marker in the stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (stack->nb_begin == 0) {
          _retcode  = RPN_RET_NO_START_MARKER;
          rpn_err_msg_op_error(op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     /* Allocate temporary new stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _stack                   = rpn_new_stack(__func__);
     _nb_layers               = 0;

     /* Transfer elements into the new stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     for (;;) {
          _stk_x                   = rpn_pop(stack);
          _X_type                  =  rpn_get_type(_stk_x);
          if (_X_type == RPN_TYPE_START_MARKER) {
               rpn_dec_begin(stack);
               break;
          }

		/* Only integers are allowed to specify dimensions
		 * and only one litteral is allowed to specify which
		 * activation function will be used
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		switch (_X_type) {

		case RPN_TYPE_INT:
			break;

		case RPN_TYPE_LITTERAL:
			if (++_nb_litteral > 1) {
				/* Only one litteral is allowed
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				_retcode                 = RPN_RET_INVALID_TYPES;
				goto end;
			}
			break;

		default:
			_retcode                 = RPN_RET_INVALID_TYPES;
			goto end;
		}

		_nb_layers++;

          rpn_push(_stack, _stk_x);
//          rpn_disp_stack(_stack);
     }

     /* Decrease nb_layers by 1 to take the input layer into account
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _nb_layers--;

	if (_nb_layers < 2) {
		/* Not enough layers
		   ~~~~~~~~~~~~~~~~~ */
		_retcode                 = RPN_RET_NOT_ENOUGH_ELTS;

		/* Restore stack contents
		   ~~~~~~~~~~~~~~~~~~~~~~ */
		for (; _stack->num_elts > 0; ) {
			_stk_x    = rpn_pop(_stack);
			rpn_push(stack, _stk_x);
		}

		goto end;
	}

	/* Allocate and create a neural network
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	_stk_result              = rpn_new_elt(RPN_TYPE_NEURAL_MLP);

	_layer                   = 0;
	_prev_dim                = 0;
	_dim                     = 0;
	_mlp                     = dp_new_mlp(_nb_layers);
	_stk_result->value.obj   = _mlp;

	for (; _stack->num_elts > 0; ) {
		_stk_x                   = rpn_pop(_stack);
		_X_type                  =  rpn_get_type(_stk_x);

		switch (_X_type) {

		case	RPN_TYPE_INT:
			_prev_dim                = _dim;
			_dim                     = _stk_x->value.i;
			_nb_neurons              = _dim;
			_nb_weights              = _prev_dim;

			if (_prev_dim == 0) {
				_mlp->input              = dp_new_input_layer(_dim);
				_perceptron              = NULL;
			}
			else {
				_perceptron              = dp_new_perceptron(_nb_neurons);

				for (_i = 0; _i < _nb_neurons; _i++) {
					_neuron                  = dp_new_neuron(_nb_weights);
					_perceptron->neurons[_i] = _neuron;
					_mlp->nb_weights         += _nb_weights + 1;
				}
				_mlp->nb_neurons         += _nb_neurons;
				_mlp->layers[_layer++]   = _perceptron;
			}
			break;

		case	 RPN_TYPE_LITTERAL:
			{
				char                     *_af_function_str;

				/* Decode activation function specifier
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				_af_function_str         = rpn_litteral_value(_stk_x);

				if (!strcmp(_af_function_str, DP_AF_SIGMOID)) {
					_mlp->activation         = dp_af_sigmoid;
				}
				else if (!strcmp(_af_function_str, DP_AF_TANH)) {
					_mlp->activation         = dp_af_tanh;
				}
				else if (!strcmp(_af_function_str, DP_AF_ARC_TAN)) {
					_mlp->activation         = dp_af_arc_tan;
				}
				else if (!strcmp(_af_function_str, DP_AF_RELU)) {
					_mlp->activation         = dp_af_relu;
				}
				else {
					fprintf(stderr, "%s (%d) : unknown activation function (\"%s\")\n",
						   __func__, __LINE__, _af_function_str);
				}
				_mlp->str_af             = strdup(_af_function_str);
			}
			break;

		default:
			RPN_INTERNAL_ERROR;
			break;
		}
     }

//printf("MLP : %10d layers\n",  _mlp->nb_layers);
//printf("MLP : %10d neurons\n", _mlp->nb_neurons);
//printf("MLP : %10d weights\n",   _mlp->nb_weights);
//printf("\n");

     rpn_free_stack(_stack);
     rpn_push(stack, _stk_result);

end:
     return _retcode;
}

/* dp_op_ai_create_mlp() }}} */
/* dp_op_ai_read_mnist() {{{ */

/******************************************************************************

                              DP_OP_AI_READ_MNIST

******************************************************************************/
RPN_DEF_OP(dp_op_ai_read_mnist)
{
     char                          _cwd[RPN_SYS_CMD_SIZE];
     dp_mnist_desc                *_mnist;
     dp_mnist_set                  _train,  _test;

     /* Initalize the module
        ~~~~~~~~~~~~~~~~~~~~ */
     _mnist                        = dp_mnist_init();

     /* Get current working directory
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (getcwd(_cwd, sizeof(_cwd)) == NULL) {
          fprintf(stderr, "%s: cannot get current working directory\n",
                  G.progname);
          perror("getcwd");
          exit(RPN_EXIT_GETCWD_ERROR);
     }

     /* Change current directory to the specified directory
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (chdir(_mnist->data_dir) < 0) {
          fprintf(stderr, "%s: cannot chdir to \"%s\" !\n",
                  G.progname, _mnist->data_dir);
          perror("chdir");
          exit(RPN_EXIT_CHDIR_ERROR);
     }

     /* Download and uncompress files if not present
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _train.name         = "MNIST_TRAIN";
     _train.images       = dp_mnist_install_file(RPN_MNIST_URL_TRAIN_IMG);
     _train.labels       = dp_mnist_install_file(RPN_MNIST_URL_TRAIN_LBL);
     _train.set_type     = RPN_TYPE_TRAINING_SET;
     _train.elt_type     = RPN_TYPE_TRAINING_ELT;

     _test.name          = "MNIST_TESTS";
     _test.images        = dp_mnist_install_file(RPN_MNIST_URL_TESTS_IMG);
     _test.labels        = dp_mnist_install_file(RPN_MNIST_URL_TESTS_LBL);
     _test.set_type      = RPN_TYPE_TEST_SET;
     _test.elt_type      = RPN_TYPE_TEST_ELT;

     dp_create_set(stack, &_test);
     dp_create_set(stack, &_train);

     /* Go back to the previous directory
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (chdir(_cwd) < 0) {
          fprintf(stderr, "%s: cannot chdir to \"%s\" !\n",
                  G.progname, _cwd);
          perror("chdir");
          exit(RPN_EXIT_CHDIR_ERROR);
     }

     return 0;
}
/* dp_op_ai_read_mnist() }}} */
/* dp_op_ai_read_elt() {{{ */

/******************************************************************************

                         DP_OP_AI_READ_ELT

******************************************************************************/
RPN_DEF_OP(dp_op_ai_read_elt)
{
     rpn_elt                  *_stk_x;
     int                       _X_type, _retcode;

     _retcode                 = RPN_RET_OK;

     /* Check_type
        ~~~~~~~~~~ */
     _stk_x                   = rpn_pop(stack);

     _X_type                  = rpn_get_type(_stk_x);

     if ((_X_type != RPN_TYPE_TRAINING_SET)
     &&  (_X_type != RPN_TYPE_TEST_SET)) {
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(rpn_get_X_type(stack)));
          goto end;
     }

end:
     return _retcode;
}
/* dp_op_ai_read_elt() }}} */
/* dp_op_ai_mnist_pic() {{{ */

/******************************************************************************

                         DP_OP_AI_MNIST_PIC

******************************************************************************/
/**
 *   \brief    Generate a PNG image derived from MNIST hand written digit.
 *             If X type is RPN_TYPE_TRAINING_ELT, then the image is the image
 *             of the digit of the MNIST element.
 *             If X type is RPN_TYPE_INT and Y type is RPN_TRAINING_ELT, then
 *             the image is the image of the digit of the mnist element and the
 *             digit in X is displayed by the side of the MNIST digit as a
 *             7 segments digit.
 */
RPN_DEF_OP(dp_op_ai_mnist_pic)
{
     rpn_elt                  *_stk_x, *_stk_result;
     dp_training_elt		*_train_elt;
     int                       _X_type, _retcode;

     _retcode                 = RPN_RET_OK;

     /* Check_type
        ~~~~~~~~~~ */
     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_TRAINING_ELT:
     case RPN_TYPE_TEST_ELT:
          {
               /* Generate image with MNIST digit with a 7 segments digit
                * by its side
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_stack				*_stack;
               char                      _txt[64], _png[64], _base[64],
                                        *_template, *_template_base, *_template_dir,
                                        *_dirname;
//             int                       _index;
               int                       _orig_index, _label;
			size_t				 _size;
               rpn_elt                  *_stk_file, *_stk_label;
               dp_training_data         *_st_data;
               dp_training_label        *_st_label;

               _template_base           = "%02d/mnist_%d_%05d";
               _template                = "%s/%s.%s";
               _template_dir            = "%s/%02d";

               _stack                   = rpn_new_stack(__func__);

               _train_elt               = _stk_x->value.obj;
               _st_data                 = _train_elt->data;
               _st_label                = _train_elt->label;
//             _index                   = _st_data->index;
               _orig_index              = _st_data->orig_index;
               _label                   = _st_label->value;

               rpn_push(_stack, _stk_x);

               /* Convert MNIST digit to a matrix */
			dl_exec_op(_stack, RPN_OP_NAME_MATRIX);

               /* Generate 7 segments digit */
               _stk_label               = rpn_new_elt(RPN_TYPE_INT);
               _stk_label->value.i      = _label;
               rpn_push(_stack, _stk_label);
               dl_exec_op(_stack, RPN_OP_NAME_NN_MAT_7_SEG);

               /* Concatenate 7 segment matrix to the digit matrix */
               dl_exec_op(_stack, RPN_OP_NAME_H_CAT);

               /* Revert lines for correct plotting */
               dl_exec_op(_stack, RPN_OP_NAME_H_REV);

               /* Generate filenames */
               sprintf(_base, _template_base, _orig_index / 1000, _label, _orig_index);

               _size                    = strlen(RPN_MNIST_DIR_TEXTS) + 1
                                        + 2 + 1
                                        + strlen(_base) + 1 + 3 + 1;
               _dirname                 = RPN_MALLOC(_size);
               sprintf(_dirname, _template_dir, RPN_MNIST_DIR_TEXTS, _orig_index / 1000);
// printf("DIRNAME = [%s]\n", _dirname);
               rpn_mkdir(_dirname);
               strcpy(_dirname, RPN_MNIST_DIR_TEXTS);
               sprintf(_txt, _template, _dirname,  _base, "txt");
               rpn_free(_dirname);

               _size                    = strlen(RPN_MNIST_DIR_IMAGES) + 1
                                        + 2 + 1
                                        + strlen(_base) + 1 + 3 + 1;
               _dirname                 = RPN_MALLOC(_size);
               sprintf(_dirname, _template_dir, RPN_MNIST_DIR_IMAGES, _orig_index / 1000);
// printf("DIRNAME = [%s]\n", _dirname);
               rpn_mkdir(_dirname);
               strcpy(_dirname, RPN_MNIST_DIR_IMAGES);
               sprintf(_png, _template, _dirname, _base, "png");

               printf("TXT FILE : %s\n", _txt);
               printf("PNG FILE : %s\n", _png);

               _stk_file                = rpn_new_elt(RPN_TYPE_STRING);    // XXX
               _stk_file->value.s       = strdup(_txt);

               rpn_push(_stack, _stk_file);

               /* Generate output filename */
               dl_exec_op(_stack, RPN_OP_NAME_TYPE_FILENAME);

Z
               /* Write matrix to file */
               dl_exec_op(_stack, RPN_OP_NAME_WRITE);

               /* Call the plot command
                  ~~~~~~~~~~~~~~~~~~~~~ */
               dp_plot_pic(_base, G.mnist);

               _stk_result              = rpn_pop(_stack);
               rpn_free_stack(_stack);
          }
          break;

     case RPN_TYPE_INT:
          {
#if 0
               rpn_stack				*_stack;
               char                      _txt[32], _png[32], _base[32],
                                        *_template, *_template_base;
               int                       _index, _label, _value;
               rpn_elt                  *_file;

               _template_base           = "mnist_%05d_%d_%d";
               _template                = "%s/%s.%s";

               _stack                   = rpn_new_stack(__func__);
               _index                   = _stk_x->value.i;

               /* Generate image with MNIST digit with a 7 segments digit
                * by its side
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_push(_stack, _stk_x);
               _train_elt               = _stk_x->value.obj;
               printf("==========> Label = %d\n", _train_elt->label->value);

               dp_op_mat_7_seg(_stack, RPN_OP_DESC(NN_MAT_7_SEG));
               rpn_op_swap_xy(_stack, RPN_OP_DESC(SWAP_XY));
               rpn_op_matrix(_stack, RPN_OP_DESC(MATRIX));
               rpn_op_swap_xy(_stack, RPN_OP_DESC(SWAP_XY));
               rpn_op_generic2(_stack, RPN_OP_DESC(H_CAT));
               rpn_op_h_rev(_stack, RPN_OP_DESC(H_REV));

               /* Generate filenames */
               sprintf(_base, _template_base, _index, _value, _label);

               sprintf(_txt, _template, RPN_MNIST_DIR_TEXTS,  _base, "txt");
               sprintf(_png, _template, RPN_MNIST_DIR_IMAGES, _base, "png");

               printf("TXT FILE : %s\n", _txt);
               printf("PNG FILE : %s\n", _png);

               _file                    = rpn_new_elt(RPN_TYPE_STRING);
               _file->value.s           = strdup(_txt);

               rpn_push(_stack, _file);

               /* Generate output filename */
               rpn_op_generic1(_stack, RPN_OP_DESC(TYPE_FILENAME));

               /* Write matrix to file */
               rpn_op_write(_stack, RPN_OP_DESC(WRITE));

               /* Call the plot command
                  ~~~~~~~~~~~~~~~~~~~~~ */
               dp_plot_pic(_base, G.mnist);

               _stk_result              = rpn_pop(_stack);
               rpn_free_stack(_stack);
#endif    /* 0 */
          }
          break;

     default:
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(rpn_get_X_type(stack)));
          goto end;
          break;
     }

     rpn_set_lastx(stack, _stk_result);

end:
     return _retcode;
}
/* dp_op_ai_mnist_pic() }}} */
/* dp_op_ai_mnist_pics() {{{ */

/******************************************************************************

                         DP_OP_AI_MNIST_PICS

******************************************************************************/
RPN_DEF_OP(dp_op_ai_mnist_pics)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_z, *_stk_t, *_stk_u,
                              *_train_elt;
     dp_training_set		*_train;
     int                       _X_type, _Y_type, _Z_type, _T_type, _U_type, _retcode,
                               _first_idx, _last_idx, _n, _p, _w, _h,
                               _first, _last;

     _retcode                 = RPN_RET_OK;

     /* Check types
      * ~~~~~~~~~~~
      * Admissible stack contents :
      *
      *  1st possibility     2nd possibility                     3rd possibility
      *  ~~~~~~~~~~~~~~~     ~~~~~~~~~~~~~~~                     ~~~~~~~~~~~~~~~
      *                                                          U : TRAINING_SET
      *                                                          T : INT  Index of first element
      *                      Z : TRAINING_SET                    Z : INT  Index of last element
      *                      Y : INT  Index of first element     Y : INT  n (number of rows)
      * X: TRAINING_SET      X : INT  Index of last element      X : INT  p (number of columns
      *
      * Dimensions of the generated image (in pixels) :
      * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      * - width_pix
      * - height_pix
      */

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     /* Try first possibility
        ~~~~~~~~~~~~~~~~~~~~~ */
     if ((_X_type == RPN_TYPE_TRAINING_SET)
     ||  (_X_type == RPN_TYPE_TEST_SET)) {
          _train_elt               = _stk_x;
          _first_idx               = 0;
          _last_idx                = 0;
          _n                       = 0;
          _p                       = 0;
          _w                       = 0;
          _h                       = 0;
     }
     else {
          /* Try second possibility
             ~~~~~~~~~~~~~~~~~~~~~~ */
          _stk_y                   = rpn_pop(stack);
          _stk_z                   = rpn_pop(stack);

          _Y_type                  = rpn_get_type(_stk_y);
          _Z_type                  = rpn_get_type(_stk_z);

          if ((_Z_type == RPN_TYPE_TRAINING_SET)
          ||  (_Z_type == RPN_TYPE_TEST_SET)) {
               if (_Y_type == RPN_TYPE_INT
               &&  _X_type == RPN_TYPE_INT) {
                    _train_elt               = _stk_z;
                    _first_idx               = _stk_y->value.i;
                    _last_idx                = _stk_x->value.i;
                    _n                       = 0;
                    _p                       = 0;
                    _w                       = 0;
                    _h                       = 0;
               }
               else {
                    /* Invalid types
                       ~~~~~~~~~~~~~ */
                    rpn_push(stack, _stk_z);
                    rpn_push(stack, _stk_y);
                    rpn_push(stack, _stk_x);

                    _retcode                 = RPN_RET_INVALID_TYPES;
                    goto end;
               }
          }
          else {
               /* Try third possibility
                  ~~~~~~~~~~~~~~~~~~~~~ */
               _stk_t                   = rpn_pop(stack);
               _stk_u                   = rpn_pop(stack);

               _T_type                  = rpn_get_type(_stk_t);
               _U_type                  = rpn_get_type(_stk_u);

               if ((_U_type == RPN_TYPE_TRAINING_SET || _U_type == RPN_TYPE_TEST_SET)
               &&  _T_type == RPN_TYPE_INT
               &&  _Z_type == RPN_TYPE_INT
               &&  _Y_type == RPN_TYPE_INT
               &&  _X_type == RPN_TYPE_INT) {
                    _train_elt               = _stk_u;
                    _first_idx               = _stk_t->value.i;
                    _last_idx                = _stk_z->value.i;
                    _n                       = _stk_y->value.i;
                    _p                       = _stk_x->value.i;
                    _w                       = 0;
                    _h                       = 0;
               }
               else {
                    /* Invalid types
                       ~~~~~~~~~~~~~ */
                    rpn_push(stack, _stk_u);
                    rpn_push(stack, _stk_t);
                    rpn_push(stack, _stk_z);
                    rpn_push(stack, _stk_y);
                    rpn_push(stack, _stk_x);

                    _retcode                 = RPN_RET_INVALID_TYPES;
                    goto end;
               }
          }
     }

     _train                   = _train_elt->value.obj;
     
     if (_n == 0) {
          _n                       = _train->nrows;
     }
     if (_p == 0) {
          _p                       = _train->ncols;
     }
     if (_w == 0) {
          _w                       = _train->width_pix;
     }
     if (_h == 0) {
          _h                       = _train->height_pix;
     }
     if (_first_idx == 0) {
          _first_idx               = _train->current_idx;
     }
     if (_last_idx == 0) {
          _last_idx                = _train->current_idx
                                   + (RPN_DEFLT_MNIST_NROWS * RPN_DEFLT_MNIST_NCOLS) -1;
     }
     if (_last_idx > _train->nb_elts) {
          _last_idx                = _train->nb_elts;
     }

     /* Keep the training set in the stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_push(stack, _train_elt);

     for (_first = _first_idx; _first <= _last_idx; _first += _n * _p) {
          _last = MIN(_first + (_n * _p) - 1, _last_idx);
          printf("%5d - %5d\n", _first, _last);

#define   MNIST_PICS_VERSION  5
          /* VERSION 1 : HCAT_VERSION = 1
           * ~~~~~~~~~   VCAT_VERSION = 1
           *             VPAD_VERSION = 1
           *
           * VERSION 2 : HCAT_VERSION = 2
           * ~~~~~~~~~   VCAT_VERSION = 2
           *             VPAD_VERSION = 2
           *
           * VERSION 3 : creation of the target matrix, rpn_clone_elt(), rpn_free(matrix)
           * ~~~~~~~~~
           * 
           * VERSION 4 : creation of the target matrix, copy of the element pointers, free(matrix)
           * ~~~~~~~~~
           *
           * VERSION 5 : creation of the target matrix, copy of the integers, free(matrix)
           * ~~~~~~~~~
           */

#if (MNIST_PICS_VERSION == 1)
          {    /* VERSION 1 and 2 */
               rpn_stack					*_stack, *_stack2;
               char                           _txt[64], _png[64], _base[64],
                                             *_template, *_template_base;
               int                            _index, _label, _value, _row, _col;
               rpn_elt                       *_file, *_elt_i, *_elt_set, *_mat,
                                             *_stk_result;

               _template_base           = "mnist_%s_%05d-%05d_%dx%d";
               _template                = "%s/GROUPS/%s.%s";

               _stack                   = rpn_new_stack(__func__);
               _stack2                  = rpn_new_stack("MNIST_MATRIX");
               _elt_set                 = rpn_clone_elt(_train_elt);
               rpn_push(_stack, _elt_set);

               _index                   = _first;

               for (_row = 1; _row <= _n; _row++) {
                    for (_col = 1; _col <= _p; _col++) {

                         _elt_i              = rpn_new_elt(RPN_TYPE_INT);
                         _elt_i->value.i     = _index;

                         /* Push index on stack
                            ~~~~~~~~~~~~~~~~~~~ */
                         rpn_push(_stack, _elt_i);

                         /* Get element
                            ~~~~~~~~~~~ */
					dl_exec_op(_stack, RPN_OP_NAME_GET);

                         /* Convert element into a matrix
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
					dl_exec_op(_stack, RPN_OP_NAME_MATRIX);

                         /* Reverse lines
                            ~~~~~~~~~~~~~ */
					dl_exec_op(_stack, RPN_OP_NAME_H_REV);

                         /* Get the matrix
                            ~~~~~~~~~~~~~~ */
                         _mat                = rpn_pop(_stack);

                         /* Push the matrix on the second stack
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         rpn_push(_stack2, _mat);

                         if (_col > 1) {
                              /* Concatenate the current matrix with the previous one (column)
                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
						dl_exec_op(_stack2, RPN_OP_NAME_H_CAT);
                         }

                         _index++;
                         if (_index > _last) {
                              break;
                         }
                    }

                    if (_row > 1) {
                         /* Eventually pad the matrix
                            ~~~~~~~~~~~~~~~~~~~~~~~~~ */
					dl_exec_op(_stack2, RPN_OP_NAME_V_PAD);

                         /* Swap lines
                            ~~~~~~~~~~ */
                         dl_exec_op(_stack2, RPN_OP_NAME_SWAP_XY);

                         /* Concatenate the current matrix with the previous one (row)
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
					dl_exec_op(_stack2, RPN_OP_NAME_V_CAT);
                    }


                    if (_index > _last) {
                         break;
                    }
               }

               _stk_result              = rpn_pop(_stack2);
               rpn_push(_stack, _stk_result);

               /* Generate filenames */
               sprintf(_base, _template_base, _train->name, _first, _last, _n, _p);

               sprintf(_txt, _template, RPN_MNIST_DIR_TEXTS,  _base, "txt");
               sprintf(_png, _template, RPN_MNIST_DIR_IMAGES, _base, "png");

               printf("TXT FILE : %s\n", _txt);
               printf("PNG FILE : %s\n", _png);

               _file                    = rpn_new_elt(RPN_TYPE_STRING);
               _file->value.s           = strdup(_txt);

               rpn_push(_stack, _file);

               /* Generate output filename */
               dl_exec_op(_stack, RPN_OP_NAME_TYPE_FILENAME);

Z
               /* Write matrix to file */
               dl_exec_op(_stack, RPN_OP_NAME_WRITE);

               /* Call the plot command
                  ~~~~~~~~~~~~~~~~~~~~~ */
               dp_plot_pics(_base, G.mnist);

               /* Free temporary stacks
                  ~~~~~~~~~~~~~~~~~~~~~ */
               rpn_free_stack(_stack);
               rpn_free_stack(_stack2);
          }

#elif (MNIST_PICS_VERSION == 5)

          {    /* VERSION 5 */
               char                           _txt[64], _png[64], _base[64],
                                             *_template, *_template_base;
               unsigned char                 *_vector;
               rpn_elt                       *_elt;
               dp_training_elt               *_elt_train;
               dp_training_data              *_elt_data;
               FILE                          *_fp;
               char                          *_fmt = "%3d";
               char                          *_tab = "     ";
               int                            _idx, _val;
               int                            _R, _C;

               /* Line / column of the matrix containing all digits,
                * in digit units
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               int                            _row, _col;

               /* Dimensions of the matrix, in cells
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               int                            _N0, _P0;

               /* Size of the matrix, in bytes
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               size_t                         _size;

               /* Index of the element to copy
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               int                            _index;

               int                            _i0, _j0, _i, _j, _dst_idx, _pixel_idx;

               int                            _value;
               unsigned char                 *_matrix;

               _template_base           = "mnist_v5_%s_%05d-%05d_%dx%d";
               _template                = "%s/GROUPS/%s.%s";

               _N0                      = (_n + 3) * _train->elt_height;
               _P0                      = (_p + 3) * _train->elt_width;

// printf("N0 = %4d P0 = %4d  (%3d x %3d)\n", _N0, _P0, _n + 3, _p + 3);

//             _N                       = _n * _train->elt_height;
//             _P                       = _p * _train->elt_width;
// printf("N  = %4d P  = %4d  (%3d x %3d)\n", _N, _P, _n, _p);

               _size                    = _N0 * _P0 * sizeof(_matrix[0]);
//               printf("N = %5d  P = %5d  size = %10d\n", _N0, _P0, _size);

               /* Allocate memory for a simplified matrix (only bytes values)
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _matrix                  = RPN_MALLOC(_size);

               /* Fill the matrix with 0
                  ~~~~~~~~~~~~~~~~~~~~~~ */
               (void) memset(_matrix, 0, _size);

               /* Initialize the reference borders :
                * 1. Initialize columns numbering (first two lines)
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _row                     = 1;
               for (_col = 1 + 3; _col <= _p + 1 + 3; _col += 10) {
                    _value              = _col / 10;
                    _R                  = _row;
                    _C                  = _col - ((_col / 10) > 0);

                    /* Compute the position of the upper left corner of the destination matrix
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _i0                 = (_R - 1) * _train->elt_height;
                    _j0                 = (_C - 1) * _train->elt_width;

                    for (_i = 0; _i < _train->elt_width; _i++) {
                         for (_j = 0; _j < _train->elt_width; _j++) {
                              _dst_idx            = RPN_MATRIX_IDX(_i0 + _i + 1, _j0 + _j + 1,  N0, _P0);
                              _pixel_idx          = RPN_MATRIX_IDX(_i + 1, _j + 1,
                                                                   _train->elt_height, _train->elt_width);

                              _matrix[_dst_idx]   = dp_7_segments[_value].digit[_pixel_idx];
                              if (_i == 0 && _j == 0) {
//                                 printf("DIGIT 7 segments %d : pos = (%2d, %2d) coord :(%3d, %3d) [%3d] => (%6d, %6d) [%6d]\n",
//                                        _value, _R, _C, _i, _j, _pixel_idx, _i0 + _i, _j0 + _j, _dst_idx);
                              }
                              if (_i == _train->elt_height - 1 && _j == _train->elt_height - 1) {
//                                 printf("DIGIT 7 segments %d : pos = (%2d, %2d) coord :(%3d, %3d) [%3d] => (%6d, %6d) [%6d]\n",
//                                        _value, _R, _C, _i, _j, _pixel_idx, _i0 + _i, _j0 + _j, _dst_idx);
                              }
                         }
                    }
               }

               _row                     = 2;
               for (_col = 1 + 3; _col < _p + 1 + 3; _col++) {

                    _value              = (_col - 3) % 10;
                    _R                  = _row;
                    _C                  = _col;

                    /* Compute the position of the upper left corner of the destination matrix
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _i0                 = (_R - 1) * _train->elt_height;
                    _j0                 = (_C - 1) * _train->elt_width;

                    for (_i = 0; _i < _train->elt_width; _i++) {
                         for (_j = 0; _j < _train->elt_width; _j++) {
                              _dst_idx            = RPN_MATRIX_IDX(_i0 + _i + 1, _j0 + _j + 1,  N0, _P0);
                              _pixel_idx          = RPN_MATRIX_IDX(_i + 1, _j + 1,
                                                                   _train->elt_height, _train->elt_width);

                              _matrix[_dst_idx]   = dp_7_segments[_value].digit[_pixel_idx];
                         }
                    }
               }

               /* 2. Initialize lines numbering (first tow columns)
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _col                     = 1;
               for (_row = 1 + 3; _row <= _n + 1 + 3; _row += 10) {
                    _value              = _row / 10;
                    _R                  = _row - ((_row / 10) > 0);
                    _C                  = _col;

                    /* Compute the position of the upper left corner of the destination matrix
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _i0                 = (_R - 1) * _train->elt_height;
                    _j0                 = (_C - 1) * _train->elt_width;

                    for (_i = 0; _i < _train->elt_width; _i++) {
                         for (_j = 0; _j < _train->elt_width; _j++) {
                              _dst_idx            = RPN_MATRIX_IDX(_i0 + _i + 1, _j0 + _j + 1,  N0, _P0);
                              _pixel_idx          = RPN_MATRIX_IDX(_i + 1, _j + 1,
                                                                   _train->elt_height, _train->elt_width);

                              _matrix[_dst_idx]   = dp_7_segments[_value].digit[_pixel_idx];
                         }
                    }
               }

               _col                     = 2;
               for (_row = 1 + 3; _row < _n + 1 + 3; _row++) {
//                  printf(" ROW %2d COL %2d : %d\n", _row, _col, (_row - 3 - 1) % 10);
                    _value              = (_row - 3) % 10;
                    _R                  = _row;
                    _C                  = _col;

                    /* Compute the position of the upper left corner of the destination matrix
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _i0                 = (_R - 1) * _train->elt_height;
                    _j0                 = (_C - 1) * _train->elt_width;

                    for (_i = 0; _i < _train->elt_width; _i++) {
                         for (_j = 0; _j < _train->elt_width; _j++) {
                              _dst_idx            = RPN_MATRIX_IDX(_i0 + _i + 1, _j0 + _j + 1,  N0, _P0);
                              _pixel_idx          = RPN_MATRIX_IDX(_i + 1, _j + 1,
                                                                   _train->elt_height, _train->elt_width);

                              _matrix[_dst_idx]   = dp_7_segments[_value].digit[_pixel_idx];
                         }
                    }
               }

               /* Copy loop
                  ~~~~~~~~~ */
               _index                   = _first - 1;
               for (_row = 1; _row <= _n; _row++) {
                    for (_col = 1; _col <= _p; _col++) {
                         /* Get element reference
                            ~~~~~~~~~~~~~~~~~~~~~ */
                         _elt                = _train->array[_index];
                         _elt_train          = _elt->value.obj;
                         _elt_data           = _elt_train->data;
                         _vector             = _elt_data->vector;

                         /* Compute the position of the upper left corner of the destination matrix
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _i0                 = (_row - 1 + 3) * _train->elt_height;
                         _j0                 = (_col - 1 + 3) * _train->elt_width;

                         /* Copy the handwritten digit into the matrix
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _pixel_idx          = 0;
                         for (_i = 0; _i < _train->elt_width; _i++) {
                              for (_j = 0; _j < _train->elt_width; _j++) {
                                   _dst_idx            = RPN_MATRIX_IDX(_i0 + _i + 1, _j0 + _j + 1,  N0, _P0);
                                   _pixel_idx          = RPN_MATRIX_IDX(_i + 1, _j + 1,
                                                                        _train->elt_height, _train->elt_width);

                                   if (!_elt_data->ignore) {
                                        _matrix[_dst_idx]   = _vector[_pixel_idx];
                                   }
                                   else {
                                        /* Hand written digit is not used
                                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                        _matrix[_dst_idx]   = 0;
                                   }
                              }
                         }

                         _index++;
                         if (_index > _last - 1) {
                              break;
                         }
                    }

                    if (_index > _last - 1) {
                         break;
                    }
               }

               /* Generate filenames */
               sprintf(_base, _template_base, _train->name, _first, _last, _n, _p);

               sprintf(_txt, _template, RPN_MNIST_DIR_TEXTS,  _base, "txt");
               sprintf(_png, _template, RPN_MNIST_DIR_IMAGES, _base, "png");

               printf("TXT FILE : %s\n", _txt);
               printf("PNG FILE : %s\n", _png);

               if ((_fp = fopen(_txt, "w")) == NULL) {
                    fprintf(stderr, "%s (%d) : cannot open \"%s\" !\n",
                            __func__, __LINE__, _txt);
                    RPN_INTERNAL_ERROR;
               }

               for (_i = _N0; _i >= 1; _i--) {
                    for (_j = 1; _j <= _P0; _j++) {
                         if (_j == 1 && _i < _N0) {
                              fprintf(_fp, "\n");
                         }
                         if (_j == 1) {
                              fprintf(_fp, "%s", _tab);
                         }

                         _idx                     = RPN_MATRIX_IDX(_i, _j, _N0, _P0);
                         _val                     = _matrix[_idx];

                         if (_j > 1) {
                              fprintf(_fp, " ");
                         }
                         fprintf(_fp, _fmt, _val);
                    }
               }
               fprintf(_fp, "\n");
               fclose(_fp);

               /* Increment current_index
                  ~~~~~~~~~~~~~~~~~~~~~~~ */
               if (_last >= _train->nb_elts) {
                    _train->current_idx      = 1;
               }
               else {
                    _train->current_idx      = _first + (_n * _p);
               }

               /* Call the plot command
                  ~~~~~~~~~~~~~~~~~~~~~ */
               dp_plot_pics(_base, G.mnist);

               RPN_FREE(_matrix);
#endif
          }
     }

// rpn_disp_stk(stack);

//     rpn_set_lastx(stack, _stk_result);

end:
     return _retcode;
}

/* dp_op_a_mnist_pics() }}} */
/* dp_op_ai_get() {{{ */

/******************************************************************************

                         DP_OP_AI_GET

******************************************************************************/
RPN_DEF_OP(dp_op_ai_get)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result, *_elt;
     int                       _X_type, _Y_type;
     int                       _idx;
     int                       _retcode;
     dp_training_set          *_train_set;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
          _stk_y                   = rpn_pop(stack);
          _Y_type                  = rpn_get_type(_stk_y);

          if ((_Y_type == RPN_TYPE_TRAINING_SET)
          ||  (_Y_type == RPN_TYPE_TEST_SET)) {
               _train_set               = _stk_y->value.obj;
               _idx                     = _stk_x->value.i - 1;

               if (_idx < 0 || _idx >= _train_set->nb_elts) {
                    /* Invalid X
                       ~~~~~~~~~ */
                    _retcode                 = RPN_RET_INVALID_X;

                    /* Restore the stack
                       ~~~~~~~~~~~~~~~~~ */
                    rpn_push(stack, _stk_y);
                    rpn_push(stack, _stk_x);
                    goto end;
               }

               rpn_set_lastx(stack, _stk_x);

               _elt                     = _train_set->array[_idx];
               _stk_result              = rpn_clone_elt(_elt);        // XXX
               _train_set->current_idx  = _idx + 1 + 1;

               /* Keep set element in the stack
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, _stk_y);

               /* Push the extracted element into the stack
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, _stk_result);
          }
          else {
               /* Invalid Y
                  ~~~~~~~~~ */
               _retcode                 = RPN_RET_INVALID_Y;

               /* Restore the stack
                  ~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, _stk_y);
               rpn_push(stack, _stk_x);
               goto end;
          }

          rpn_free_elt(&_stk_x);

          break;

     case RPN_TYPE_TRAINING_SET:
     case RPN_TYPE_TEST_SET:
          _train_set               = _stk_x->value.obj;
          _idx                     = _train_set->current_idx - 1;
          if (_idx >= _train_set->nb_elts) {
               /* No more element
                  ~~~~~~~~~~~~~~~ */
               _retcode                 = RPN_RET_NO_MORE_ELT;

               /* Restore the stack
                  ~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, _stk_x);
               goto end;
          }
          if (_idx < 0) {
               /* Invalid X
                  ~~~~~~~~~ */
               _retcode                 = RPN_RET_INVALID_X;

               /* Restore the stack
                  ~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, _stk_x);
               goto end;
          }

//        _clone                   = rpn_clone_elt(_stk_x);
//        rpn_set_lastx(stack, _clone);
//        rpn_set_lastx(stack, _stk_x);

          _elt                     = _train_set->array[_idx];
          _stk_result              = rpn_clone_elt(_elt);
          _train_set->current_idx++;

          /* Keep set element in the stack
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_x);

          /* Push the extracted element into the stack
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
          break;

     default:
          _retcode                 = RPN_RET_INVALID_TYPES;

          /* Restore the stack
             ~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_x);
          break;
     }

end:
     return _retcode;
}

/* dp_op_ai_get() }}} */
/* dp_op_ai_matrix() {{{ */

/******************************************************************************

					DP_OP_AI_MATRIX

******************************************************************************/
RPN_DEF_OP(dp_op_ai_matrix)
{
     rpn_elt                  *_stk_x, *_stk_matrix,
                              *_elt;
     int                       _n, _p, _i, _j, _idx;
	size_t				 _size;
     rpn_matrix			*_matrix;
     rpn_training_elt		*_train;
     rpn_training_data		*_data;
     int                       _X_type, _Y_type = RPN_TYPE_UNDEFINED;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_TRAINING_ELT:
     case RPN_TYPE_TEST_ELT:
          _train                   = _stk_x->value.obj;
          _data                    = _train->data;
          _n                       = _data->num_rows;
          _p                       = _data->num_cols;

          _stk_matrix              = rpn_new_elt(RPN_TYPE_MATRIX);

          _size                    = sizeof(*_matrix) + (((_n * _p) - 1) * sizeof (void *));
          _matrix                  = (rpn_matrix *) RPN_MALLOC(_size);
          _matrix->n               = _n;
          _matrix->p               = _p;
          _stk_matrix->value.obj   = _matrix;

          for (_i = 1; _i <= _n; _i++) {
               for (_j = 1; _j <= _p; _j++) {
                    _elt                = rpn_new_elt(RPN_TYPE_INT);
                    _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
                    _elt->value.i       = (int) ((uint8_t *)_data->vector)[_idx];
                    _matrix->base[_idx] = (void *) _elt;
               }
          }
          break;

     default:
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
          break;
     }

     rpn_push(stack, _stk_matrix);

     return _retcode;
}

/* dp_op_ai_matrix() }}} */
/* dp_op_ai_mat_7seg() {{{ */

/******************************************************************************

                         DP_OP_AI_MAT_7_SEG

******************************************************************************/
RPN_DEF_OP(dp_op_ai_mat_7_seg)
{
     rpn_elt                  *_stk_x, *_stk_result, *_elt_i;
     int                       _X_type, _row, _col, _idx;
     int                       _retcode;
     int                      *_digit_w;
     rpn_stack				*_stack;
	char					 _name[4];

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     /* Check types
        ~~~~~~~~~~~ */
     if (_X_type != RPN_TYPE_INT) {
          _retcode                 = RPN_RET_INVALID_X;
          rpn_push(stack, _stk_x);
          goto end;
     }

     _digit_w                 = dp_7_segments[_stk_x->value.i].digit;
     _stack                   = rpn_new_stack(__func__);
     for (_row = 0, _idx = 0; _row < RPN_MNIST_ROWS; _row++) {
          for (_col = 0; _col < RPN_MNIST_COLS; _col++) {
               _elt_i              = rpn_new_elt(RPN_TYPE_INT);
               _elt_i->value.i     = _digit_w[_idx++];

               rpn_push(_stack, _elt_i);
          }
     }

#define PRINT_7_SEG      0
#if (PRINT_7_SEG == 1)
     for (_row = 1; _row <= RPN_MNIST_ROWS; _row++) {
          for (_col = 1; _col <= RPN_MNIST_COLS; _col++) {
               _idx                = RPN_MATRIX_IDX(_row, _col, RPN_MNIST_ROWS, RPN_MNIST_COLS);
               if (_col == 0) {
                    printf("%3d", _digit_w[_idx]);
               }
               else {
                    printf(" %3d", _digit_w[_idx]);
               }
          }
          printf("\n");
     }
#endif

     _elt_i              = rpn_new_elt(RPN_TYPE_INT);
     _elt_i->value.i     = RPN_MNIST_ROWS;
     rpn_push(_stack, _elt_i);

     _elt_i              = rpn_new_elt(RPN_TYPE_INT);
     _elt_i->value.i     = RPN_MNIST_COLS;
     rpn_push(_stack, _elt_i);

	dl_exec_op(_stack, RPN_OP_NAME_MATRIX);

	sprintf(_name, "%d", _stk_x->value.i);
     rpn_set_lastx(stack, _stk_x);

     _stk_result         = rpn_pop(_stack);
     rpn_free_stack(_stack);

	_stk_result->name	= strdup(_name);

     rpn_push(stack, _stk_result);

end:
     return _retcode;
}

/* dp_op_ai_mat_7seg() }}} */
/* dp_op_ai_label() {{{ */

/******************************************************************************

                         DP_OP_AI_LABEL

******************************************************************************/
RPN_DEF_OP(dp_op_ai_label)
{
     rpn_elt              *_stk_x, *_stk_result;
     int                  _X_type;
     int                  _retcode;
     dp_training_elt     *_train;
     dp_training_label   *_label;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_TRAINING_ELT:
     case RPN_TYPE_TEST_ELT:
          _train                   = _stk_x->value.obj;
          _label                   = _train->label;

          _stk_result              = rpn_new_elt(RPN_TYPE_INT);
          _stk_result->value.i     = _label->value;
          break;

     default:
          _retcode                 = RPN_RET_INVALID_X;

          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          RPN_INTERNAL_ERROR;
          break;
     }

     rpn_set_lastx(stack, _stk_x);
     rpn_free_elt(&_stk_x);

     rpn_push(stack, _stk_result);

     return _retcode;
}

/* dp_op_ai_label() }}} */
/* dp_op_ai_index() {{{ */

/******************************************************************************

                         DP_OP_AI_INDEX

******************************************************************************/
RPN_DEF_OP(dp_op_ai_index)
{
     rpn_elt             *_stk_x, *_stk_result;
     int                  _X_type;
     int                  _retcode;
     dp_training_elt     *_train;
     dp_training_data    *_data;
	dp_training_set	*_train_set;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_TRAINING_ELT:
     case RPN_TYPE_TEST_ELT:
          _train                   = _stk_x->value.obj;
          _data                    = _train->data;

          _stk_result              = rpn_new_elt(RPN_TYPE_INT);
		_stk_result->value.i     = _data->index;
          break;

	case	RPN_TYPE_TRAINING_SET:
	case	RPN_TYPE_TEST_SET:
          _train_set               = _stk_x->value.obj;

          _stk_result              = rpn_new_elt(RPN_TYPE_INT);
		_stk_result->value.i     = _train_set->current_idx;
		break;

     default:
          _retcode                 = RPN_RET_INVALID_X;

          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          RPN_INTERNAL_ERROR;
          break;
     }

     rpn_set_lastx(stack, _stk_x);

     rpn_push(stack, _stk_result);

     return _retcode;
}

/* dp_op_ai_index() }}} */
/* dp_op_ai_orig_index() {{{ */

/******************************************************************************

                         DP_OP_AI_ORIG_INDEX

******************************************************************************/
RPN_DEF_OP(dp_op_ai_orig_index)
{
     rpn_elt             *_stk_x, *_stk_result;
     int                  _X_type;
     int                  _retcode;
     dp_training_elt     *_train;
     dp_training_data    *_data;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_TRAINING_ELT:
     case RPN_TYPE_TEST_ELT:
          _train                   = _stk_x->value.obj;
          _data                    = _train->data;

          _stk_result              = rpn_new_elt(RPN_TYPE_INT);
		_stk_result->value.i     = _data->orig_index;
          break;

     default:
          _retcode                 = RPN_RET_INVALID_X;

          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          RPN_INTERNAL_ERROR;
          break;
     }

     rpn_set_lastx(stack, _stk_x);

     rpn_push(stack, _stk_result);

     return _retcode;
}

/* dp_op_ai_orig_index() }}} */
/* dp_op_ai_dispatch() {{{ */

/******************************************************************************

                         DP_OP_AI_DISPATCH

******************************************************************************/
RPN_DEF_OP(dp_op_ai_dispatch)
{
     rpn_elt                  *_stk_x;
     int                       _X_type, _i, _index, _label, _num_elts;
	size_t				 _size;
     rpn_stack				*_lists[10];
     int                       _retcode;
     dp_training_set		*_train;
     dp_training_elt          *_elt_train;
     dp_training_data         *_elt_data;
     dp_training_label        *_elt_label;
     dp_training_set          *_train_set;
     rpn_elt                  *_elt, *_clone, *_new_set;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_TRAINING_SET:
     case RPN_TYPE_TEST_SET:
          /* Push the element back on the stack
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_x);

          _train                   = _stk_x->value.obj;

          /* Allocate the stacks / FIFO descriptors
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          for (_i = 0; _i < sizeof(_lists) / sizeof(_lists[0]); _i++) {
               _lists[_i]               = rpn_new_stack(__func__);
          }

          /* Dispatch the clones of the elements into the stacks / FIFO
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          for (_index = 0; _index < _train->nb_elts; _index++) {
               _elt                = _train->array[_index];
               _elt_train          = _elt->value.obj;
               _elt_data           = _elt_train->data;
               _elt_label          = _elt_train->label;
               _label              = _elt_label->value;

               _clone              = rpn_clone_elt(_elt);
               rpn_push(_lists[_label], _clone);
          }

#if 0
          /* Display counters for each label
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          for (_label = 0; _label < sizeof(_lists) / sizeof(_lists[0]); _label++) {
               printf("LABEL %d : %5d elements\n", _label, _lists[_label]->num_elts);
          }
#endif

          /* Create the training sets for each label
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          for (_label = (sizeof(_lists) / sizeof(_lists[0])) - 1; _label >= 0; _label--) {
               char                      _name[64];

               /* Create the _SET element
                  ~~~~~~~~~~~~~~~~~~~~~~~ */
               _new_set                 = rpn_new_elt(_X_type);
               _size                    = sizeof(*_train_set)
                                        + ((_lists[_label]->num_elts - 1) * sizeof(rpn_elt *));
               _train_set               = (dp_training_set *) RPN_MALLOC(_size);
               sprintf(_name, "%s-%d", _train->name, _label);
               _train_set->name         = strdup(_name);
               _train_set->nb_elts      = _lists[_label]->num_elts;
               _train_set->current_idx  = 1;
               _train_set->nrows        = _train->nrows;
               _train_set->ncols        = _train->ncols;
               _train_set->width_pix    = _train->width_pix;
               _train_set->height_pix   = _train->height_pix;
               _train_set->elt_width    = _train->elt_width;
               _train_set->elt_height   = _train->elt_height;

#if 0
printf("Name                     = %s\n", _name);
printf("sizeof(rpn_training_set) = %6d\n", sizeof(rpn_training_set));
printf("sizeof(train_set)        = %6d\n", _size);
printf("sizeof(rpn_elt *)        = %6d\n", sizeof(rpn_elt *));
#endif

               _new_set->value.obj      = _train_set;


               /* Copy the number of the elements in the stack so that
                * it stays constant during the complete loop.
                * rpn_base_pop(_lists[_label]) decrements the member num_elts,
                * so it cannot be use directly to control the loop
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _num_elts                = _lists[_label]->num_elts;
               for (_i = 0; _i < _num_elts; _i++) {
                    _elt                     = rpn_base_pop(_lists[_label]);
                    _elt_train               = _elt->value.obj;
                    _elt_data                = _elt_train->data;
                    _elt_data->index         = _i + 1;
                    _train_set->array[_i]    = _elt;
               }

               /* Push the new training set on the stack
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, _new_set);
          }

          for (_i = 0; _i < sizeof(_lists) / sizeof(_lists[0]); _i++) {
               /* Free the temporary stacks / FIFO
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//             rpn_free_stack(_lists[_i]);
          }

          break;

     default:
          _retcode                 = RPN_RET_INVALID_X_TYPE;

          /* Restore the stack
             ~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_x);
          goto end;
          break;
     }

end:
     return _retcode;
}

/* dp_op_ai_dispatch() }}} */
/* dp_op_ai_ignore() {{{ */

/******************************************************************************

                         DP_OP_AI_IGNORE

******************************************************************************/
RPN_DEF_OP(dp_op_ai_ignore)
{
     int                       _retcode, _X_type, _Y_type, _idx;
     rpn_elt                  *_stk_x, *_stk_y, *_elt;
     dp_training_set          *_train_set;
     dp_training_elt          *_train_elt;
     dp_training_data         *_data;

     _retcode                 = RPN_RET_OK;

     _X_type                  = rpn_get_X_type(stack);
     _Y_type                  = rpn_get_Y_type(stack);

     switch (_X_type) {

     case RPN_TYPE_INT:
          switch (_Y_type) {

          case RPN_TYPE_TRAINING_SET:
               /* Retrieve index
                  ~~~~~~~~~~~~~~ */
               _stk_x                   = rpn_pop(stack);
               rpn_set_lastx(stack, _stk_x);
               _idx                     = _stk_x->value.i;

               _stk_y                   = rpn_pop(stack);

               /* TRAINING_SET is persistent, so push it back on the stack
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, _stk_y);

               _train_set               = _stk_y->value.obj;

               /* Check validity of the specified index
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if (_idx <= 0 || _idx > _train_set->nb_elts) {
                    rpn_push(stack, _stk_x);
                    _retcode                 = RPN_RET_INVALID_X;
                    goto end;
               }

               _elt                     = _train_set->array[_idx - 1];
               _train_elt               = _elt->value.obj;
               _data                    = _train_elt->data;
			_data->ignore            = TRUE;
               break;

          default:
               printf("*** Invalid Y type (%s) for operator \"%s\" !\n", rpn_type_to_string(_Y_type), op->op_name);
               break;
          }
          break;

     case RPN_TYPE_TRAINING_ELT:
          _stk_x                   = rpn_pop(stack);
          rpn_set_lastx(stack, _stk_x);

          _train_elt               = _stk_x->value.obj;
          _data                    = _train_elt->data;
		_data->ignore            = TRUE;
          rpn_push(stack, _stk_x);
          break;

     default:
          printf("*** Invalid X type (%s) for operator \"%s\" !\n", rpn_type_to_string(_X_type), op->op_name);
          break;
     }

end:
     return _retcode;
}
/* dp_op_ai_ignore() }}} */
/* dp_op_ai_use() {{{ */

/******************************************************************************

                         DP_OP_AI_USE

******************************************************************************/
RPN_DEF_OP(dp_op_ai_use)
{
     int                       _retcode, _X_type, _Y_type, _idx;
     rpn_elt                  *_stk_x, *_stk_y, *_elt;
     dp_training_set          *_train_set;
     dp_training_elt          *_train_elt;
     dp_training_data         *_data;

     _retcode                 = RPN_RET_OK;

     _X_type                  = rpn_get_X_type(stack);
     _Y_type                  = rpn_get_Y_type(stack);

     switch (_X_type) {

     case RPN_TYPE_INT:
          switch (_Y_type) {

          case RPN_TYPE_TRAINING_SET:
               /* Retrieve index
                  ~~~~~~~~~~~~~~ */
               _stk_x                   = rpn_pop(stack);
               rpn_set_lastx(stack, _stk_x);
               _idx                     = _stk_x->value.i;

               _stk_y                   = rpn_pop(stack);

               /* TRAINING_SET is persistent, so push it back on the stack
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, _stk_y);

               _train_set               = _stk_y->value.obj;

               /* Check validity of the specified index
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if (_idx <= 0 || _idx > _train_set->nb_elts) {
                    rpn_push(stack, _stk_x);
                    _retcode                 = RPN_RET_INVALID_X;
                    goto end;
               }

               _elt                     = _train_set->array[_idx - 1];
               _train_elt               = _elt->value.obj;
               _data                    = _train_elt->data;
			_data->ignore            = FALSE;
			break;

          default:
               printf("*** Invalid Y type (%s) for operator \"%s\" !\n", rpn_type_to_string(_Y_type), op->op_name);
               break;
          }
          break;

     case RPN_TYPE_TRAINING_ELT:
          _stk_x                   = rpn_pop(stack);
          rpn_set_lastx(stack, _stk_x);

          _train_elt               = _stk_x->value.obj;
          _data                    = _train_elt->data;
		_data->ignore            = FALSE;
          rpn_push(stack, _stk_x);
          break;

     default:
          printf("*** Invalid X type (%s) for operator \"%s\" !\n", rpn_type_to_string(_X_type), op->op_name);
          break;
     }

end:
     return _retcode;
}
/* dp_op_ai_use() }}} */
/* dp_new_mlp() {{{ */

/******************************************************************************

                              DP_NEW_MLP

******************************************************************************/
dp_mlp *dp_new_mlp(int nb_layers)
{
     /* The input layer is counted in the number of layers
      * of the multilayer perceptron, but isn't associated
      * to a real layer of neurons
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     size_t                    _size;
     dp_mlp				*_mlp;

     _size                    = sizeof(dp_mlp)
                              + ((nb_layers - 1) * sizeof(dp_perceptron *));

     if ((_mlp = (dp_mlp *) RPN_MALLOC(_size)) == NULL) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _mlp->nb_layers               = nb_layers;
     _mlp->nb_neurons              = 0;
     _mlp->nb_weights              = 0;

     return _mlp;
}
/* dp_new_mlp() }}} */
/* dp_new_input_layer() {{{ */

/******************************************************************************

                              DP_NEW_INPUT_LAYER

******************************************************************************/
dp_input_layer *dp_new_input_layer(int nb_inputs)
{
     size_t                    _size;
	int					 _i;
     dp_input_layer			*_input_layer;

     _size                    = sizeof(dp_input_layer)
                              + (nb_inputs * sizeof(dp_input_neuron));

     if ((_input_layer = (dp_input_layer *) RPN_MALLOC(_size)) == NULL) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _input_layer->nb_inputs = nb_inputs;
     for (_i = 0; _i < nb_inputs; _i++) {
          _input_layer->inputs[_i]      = dp_new_input_neuron();
     }

     return _input_layer;
}
/* dp_new_input_layer() }}} */
/* dp_new_perceptron() {{{ */

/******************************************************************************

                              DP_NEW_PERCEPTRON

******************************************************************************/
dp_perceptron *dp_new_perceptron(int nb_neurons)
{
     size_t                    _size;
	int					 _i;
     dp_perceptron			*_perceptron;

     _size                    = sizeof(dp_perceptron)
                              + (nb_neurons * sizeof(dp_neuron));

     if ((_perceptron = (dp_perceptron *) RPN_MALLOC(_size)) == NULL) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _perceptron->nb_neurons  = nb_neurons;
     for (_i = 0; _i < nb_neurons; _i++) {
          /* Neurons will be allocated later, so reset their pointers
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          _perceptron->neurons[_i]      = NULL;
     }

     return _perceptron;
}
/* dp_new_perceptron() }}} */
/* dp_new_neuron() {{{ */

/******************************************************************************

                              DP_NEW_NEURON

******************************************************************************/
dp_neuron *dp_new_neuron(int nb_weights)
{
     size_t                    _size;
	int					 _i;
     dp_neuron				*_neuron;

     _size                    = sizeof(dp_neuron)
                              + (nb_weights * sizeof(dp_weight));

     if ((_neuron = (dp_neuron *) RPN_MALLOC(_size)) == NULL) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _neuron->nb_weights      = nb_weights;
     _neuron->bias            = 0;

     for (_i = 0; _i < nb_weights; _i++) {
          _neuron->weights[_i]          = 0;
     }

     return _neuron;
}
/* dp_new_neuron() }}} */
/* dp_af_sigmoid() {{{ */

/******************************************************************************

                              DP_AF_SIGMOID

******************************************************************************/
dp_weight dp_af_sigmoid(dp_weight x)
{
     dp_weight                 _result;

     _result                  = (dp_weight) 1.0 / (1.0 + exp((double) -x));

     return _result;
}
/* dp_af_sigmoid() }}} */
/* dp_af_tanh() {{{ */

/******************************************************************************

                              DP_AF_TANH

******************************************************************************/
dp_weight dp_af_tanh(dp_weight x)
{
     dp_weight                _result;

     _result                  = (exp((double) x) - exp(-(double) x)) / (exp((double) x) + exp(-(double) x));

     return _result;
}
/* dp_af_tanh() }}} */
/* dp_af_arc_tan() {{{ */

/******************************************************************************

                              DP_AF_ARC_TAN

******************************************************************************/
dp_weight dp_af_arc_tan(dp_weight x)
{
     dp_weight                _result;

     _result                  = atan((double) x);

     return _result;
}
/* dp_af_arc_tan() }}} */
/* dp_af_relu() {{{ */

/******************************************************************************

                              DP_AF_RELU

******************************************************************************/
dp_weight dp_af_relu(dp_weight x)
{
     dp_weight                _result;

     if (x >= 0) {
          _result                  = x;
     }
     else {
          _result                  = 0;
     }

     return _result;
}
/* dp_af_relu() }}} */
/* dp_nn_rand() {{{ */

/******************************************************************************

                              DP_NN_RAND

******************************************************************************/
dp_weight dp_nn_rand(void)
{
     return (dp_weight) rand();
}
/* dp_nn_rand() }}} */
/* dp_set_layer_weights() {{{ */

/******************************************************************************

                              DP_SET_LAYER_WEIGHTS

******************************************************************************/
RPN_DEF_OP( dp_set_layer_weights)
{
     RPN_INTERNAL_ERROR;
	return 0;
}
/* dp_set_layer_weights() }}} */
/* dp_set_mlp_weights() {{{ */

/******************************************************************************

                              DP_SET_MLP_WEIGHTS

******************************************************************************/
RPN_DEF_OP(dp_set_mlp_weights)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _X_type, _Y_type;
     int                       _layer, _n_num, _w_num;
     double                    _fixed_weight;
     int                       _retcode;
     dp_mlp				*_mlp;
     dp_perceptron			*_perceptron;
     dp_neuron				*_neuron;

     _retcode                 = RPN_RET_OK;

#if 0
     /* Check types
        ~~~~~~~~~~~ */
     if ((_retcode = rpn_check_types(op, stack)) != RPN_RET_OK) {
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(rpn_get_X_type(stack)));
          fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(rpn_get_Y_type(stack)));
          goto end;
     }
#endif

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     _X_type                  = rpn_get_type(_stk_x);
     _Y_type                  = rpn_get_type(_stk_y);

     switch (_X_type) {

     case RPN_TYPE_INT:
          _fixed_weight            = (double) _stk_x->value.i;
          break;

     case RPN_TYPE_DOUBLE:
          _fixed_weight            = _stk_x->value.d;
          break;

     case RPN_TYPE_NEURAL_MLP:
          _mlp                     = _stk_x->value.obj;
          _stk_result              = _stk_x;
          break;

     default:
          RPN_INTERNAL_ERROR;
          break;
     }

     switch (_Y_type) {

     case RPN_TYPE_INT:
          _fixed_weight            = (double) _stk_y->value.i;
          break;

     case RPN_TYPE_DOUBLE:
          _fixed_weight            = _stk_y->value.d;
          break;

     case RPN_TYPE_NEURAL_MLP:
          _mlp                     = _stk_y->value.obj;
          _stk_result              = _stk_y;
          break;

     default:
          RPN_INTERNAL_ERROR;
          break;
     }

     for (_layer = 0; _layer < _mlp->nb_layers - 1; _layer++) {
//fprintf(stderr, "mlp->layers[%d] = 0x%08X\n", _layer, _mlp->layers[_layer]);
          _perceptron                   = _mlp->layers[_layer];

          for (_n_num = 0; _n_num < _perceptron->nb_neurons; _n_num++) {
               _neuron                  = _perceptron->neurons[_n_num];
               for (_w_num = 0; _w_num < _neuron->nb_weights; _w_num++) {
//printf("VALUE = %f\n", _fixed_weight);
                    _neuron->weights[_w_num]     = _fixed_weight;
               }
               _neuron->bias            = _fixed_weight;
//printf("BIAS = %f\n", _neuron->bias);
          }
     }
     rpn_push(stack, _stk_result);

     return _retcode;
}
/* dp_set_mlp_weights() }}} */
/* dp_create_set() {{{ */

/******************************************************************************

                         DP_CREATE_SET

******************************************************************************/
void dp_create_set(rpn_stack *stack, dp_mnist_set *set)
{
     uint32_t                       _magic, _nb_images, _dim1, _dim2,
                                    _nb_labels;
	size_t					 _size;
     int                            _fd_images, _fd_labels, _type, _nb;
     rpn_elt                       *_elt, *_set, *_marker;
     dp_training_elt               *_train;
     dp_training_set               *_train_set;

     if ((_fd_images = open(set->images, O_RDONLY)) < 0) {
          fprintf(stderr, "%s : cannot open \"%s\" !\n", G.progname, set->images);
          exit(RPN_EXIT_OPEN_ERROR);
     }

     if (read(_fd_images, &_magic, sizeof(_magic)) < 0) {
          fprintf(stderr, "%s : error while reading magic number !\n", G.progname);
          exit(RPN_EXIT_READ_ERROR);
     }

     printf("Magic number  = 0x%08x\n", df_convert_uint32(_magic));

     if (read(_fd_images, &_nb_images, sizeof(_nb_images)) < 0) {
          fprintf(stderr, "%s : error while reading images number !\n", G.progname);
          exit(RPN_EXIT_READ_ERROR);
     }

     printf("Images number = %10d  (0x%08x)\n", df_convert_uint32(_nb_images), df_convert_uint32(_nb_images));
     _nb_images          = df_convert_uint32(_nb_images);

     if (read(_fd_images, &_dim1, sizeof(_dim1)) < 0) {
          fprintf(stderr, "%s : error while reading dimension 1 !\n", G.progname);
          exit(RPN_EXIT_READ_ERROR);
     }

     if (read(_fd_images, &_dim2, sizeof(_dim2)) < 0) {
          fprintf(stderr, "%s : error while reading dimension 2 !\n", G.progname);
          exit(RPN_EXIT_READ_ERROR);
     }

     _dim1               = df_convert_uint32(_dim1);
     _dim2               = df_convert_uint32(_dim2);

     printf("Dimension 1   = %10d  (0x%08x)\n", _dim1, _dim1);
     printf("Dimension 2   = %10d  (0x%08x)\n", _dim2, _dim2);

     _size               = _dim1 * _dim2;
     if ((_fd_labels = open(set->labels, O_RDONLY)) < 0) {
          fprintf(stderr, "%s : cannot open \"%s\" !\n", G.progname, set->labels);
          exit(RPN_EXIT_OPEN_ERROR);
     }

     if (read(_fd_labels, &_magic, sizeof(_magic)) < 0) {
          fprintf(stderr, "%s : error while reading magic number !\n", G.progname);
          exit(RPN_EXIT_READ_ERROR);
     }

     printf("Magic number  = 0x%08x\n", df_convert_uint32(_magic));

     if (read(_fd_labels, &_nb_labels, sizeof(_nb_labels)) < 0) {
          fprintf(stderr, "%s : error while reading labels number !\n", G.progname);
          exit(RPN_EXIT_READ_ERROR);
     }

     printf("Labels number = %10d  (0x%08x)\n", df_convert_uint32(_nb_labels), df_convert_uint32(_nb_labels));

     /* Allocate element descriptors for start marker
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_inc_begin(stack);
     _elt                = rpn_new_elt(RPN_TYPE_START_MARKER);

     /* Push element on the stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~ */
     stack->new_value  = TRUE;
     rpn_push(stack, _elt);
     for (_nb = 0; _nb < _nb_images; _nb++) {
          dp_training_data			*_data;
          dp_training_label			*_label;
          uint8_t                        _mnist_label;

          _data               = dp_new_training_data();
          _data->orig_index   = 
          _data->index        = _nb + 1;
          _data->ignore       = FALSE;
          _data->nb_elts      = _dim1 * _dim2;
          _data->elt_size     = sizeof(uint8_t);
          _data->num_rows     = _dim1;
          _data->num_cols     = _dim2;
          _size               = _data->nb_elts * _data->elt_size;

          if ((_data->vector = (unsigned char *) RPN_MALLOC(_size)) == NULL) {
               fprintf(stderr, "%s : cannot allocate %lu bytes for the images !\n", G.progname, _size);
               exit(RPN_EXIT_NO_MEM);
          }

          if (read(_fd_images, _data->vector, _size) <= 0) {
               fprintf(stderr, "%s : error while reading image !\n", G.progname);
               exit(RPN_EXIT_READ_ERROR);
          }

          _label              = dp_new_training_label();
          _label->index       = _nb + 1;

          if (read(_fd_labels, (char *) &_mnist_label, sizeof(_mnist_label)) <= 0) {
               fprintf(stderr, "%s : error while reading label !\n", G.progname);
               exit(RPN_EXIT_READ_ERROR);
          }
          _label->value       = _mnist_label;

          _train              = dp_new_training_elt();
          _train->data        = _data;
          _train->label       = _label;

          _elt                = rpn_new_elt(set->elt_type);
          _elt->value.obj     = _train;

          /* Push element on the stack
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          stack->new_value  = TRUE;
          rpn_push(stack, _elt);

//          printf("Image %10d read (size = %10d) ...\n", _nb, _size);
     }

     close(_fd_labels);
     close(_fd_images);

     /* Create the SET element
        ~~~~~~~~~~~~~~~~~~~~~~ */
     _set                     = rpn_new_elt(set->set_type);
     _size                    = sizeof(*_train_set) + ((_nb_images - 1) * sizeof(rpn_elt *));
     _train_set               = (dp_training_set *) RPN_MALLOC(_size);
#if 0
printf("sizeof(dp_training_set)  = %6d\n", sizeof(dp_training_set));
printf("sizeof(train_set)        = %6d\n", _size);
printf("sizeof(rpn_elt *)        = %6d\n", sizeof(rpn_elt *));
#endif
     _train_set->name         = strdup(set->name);
     _train_set->nb_elts      = _nb_images;
     _train_set->current_idx  = 1;
     _train_set->nrows        = RPN_DEFLT_MNIST_NROWS;
     _train_set->ncols        = RPN_DEFLT_MNIST_NCOLS;
     _train_set->width_pix    = RPN_DEFLT_MNIST_WIDTH_PIX;
     _train_set->height_pix   = RPN_DEFLT_MNIST_HEIGHT_PIX;
     _train_set->elt_width    = _dim1;
     _train_set->elt_height   = _dim2;

     _set->value.obj          = _train_set;

     for (_nb = _nb_images - 1; _nb >= 0; _nb--) {
          _elt                     = rpn_pop(stack);
          _train_set->array[_nb]   = _elt;
     }

     _marker             = rpn_pop(stack);
     if ((_type = rpn_get_type(_marker)) != RPN_TYPE_START_MARKER) {
          RPN_INTERNAL_ERROR;
     }

     rpn_push(stack, _set);
}

/* dp_create_set() }}} */
/* dp_new_training_elt() {{{ */

/******************************************************************************

                         DP_NEW_TRAINING_ELT

******************************************************************************/
RPN_PREF_NEW(dp, training_elt)
/* dp_new_training_elt() }}} */
/* dp_new_training_data() {{{ */

/******************************************************************************

                         DP_NEW_TRAINING_DATA

******************************************************************************/
RPN_PREF_NEW(dp, training_data)
/* dp_new_training_data() }}} */
/* dp_new_training_label() {{{ */

/******************************************************************************

                         DP_NEW_TRAINING_LABEL

******************************************************************************/
RPN_PREF_NEW(dp, training_label)
/* dp_new_training_label() }}} */
/* dp_new_input_neuron() {{{ */

/******************************************************************************

                         DP_NEW_INPUT_NEURON

******************************************************************************/
RPN_PREF_NEW(dp, input_neuron)
/* dp_new_input_neuron() }}} */
/* dp_nn_learn() {{{ */

/******************************************************************************

                         DP_NN_LEARN

******************************************************************************/
void dp_nn_learn()
{
}
/* dp_nn_learn() }}} */
/* dp_nn_analyse() {{{ */

/******************************************************************************

                         DP_NN_ANALYZE

******************************************************************************/
void dp_nn_analyze()
{
}
/* dp_nn_analyse() }}} */
/* dp_save_mlp() {{{ */

/******************************************************************************

                         DP_SAVE_MLP

******************************************************************************/
void dp_save_mlp(rpn_elt *mlp)
{
     dp_mlp				*_mlp;
     dp_perceptron			*_perceptron;
     dp_neuron				*_neuron;
     int                       _l_num, _n_num, _w_num;

     _mlp           = (dp_mlp *) mlp->value.obj;

     printf("Nb layers : %d nb_neurons : %d nb_weights : %d\n",
            _mlp->nb_layers, _mlp->nb_neurons, _mlp->nb_weights);
     for (_l_num = 0; _l_num < _mlp->nb_layers - 1; _l_num++) {
          _perceptron         = _mlp->layers[_l_num];

          for (_n_num = 0; _n_num < _perceptron->nb_neurons; _n_num++) {
               _neuron        = _perceptron->neurons[_n_num];

               for (_w_num = 0; _w_num < _neuron->nb_weights; _w_num++) {
                    printf("Layer %3d, neuron %5d : weight %5d = %f\n",
                           _l_num, _n_num, _w_num, _neuron->weights[_w_num]);
               }

               printf("Layer %3d, neuron %5d : bias         = %f\n",
                      _l_num, _n_num, _neuron->bias);
          }
     }
}
/* dp_save_mlp() }}} */
/* dp_mnist_install_file() {{{ */

/******************************************************************************

                         DP_MNIST_INSTALL_FILE

******************************************************************************/
char *dp_mnist_install_file(char *url)
{
     char                     *_gz_file, *_basename;

     _gz_file            = basename(strdup(url));
     _basename           = strdup(_gz_file);
     dp_remove_ext(_basename);

     /* Check for the existence of the file
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (access(_basename, F_OK) < 0) {
          /* File does not exist : uncompress the file
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
fprintf(stderr, "%s (%d) : file \"%s\" does not exist => uncompress ...\n",
        __func__, __LINE__, _basename);
          dp_mnist_uncompress_file(_gz_file, url);
     }

     return _basename;
}

/* dp_mnist_install_file() }}} */
/* dp_mnist_uncompress_file() {{{ */

/******************************************************************************

                         DP_MNIST_UNCOMPRESS_FILE

******************************************************************************/
void dp_mnist_uncompress_file(char *gz_file, char *url)
{
     char                      _cmd[RPN_SYS_CMD_SIZE];
     int                       _ret;

     /* Check for the existence of the compressed file
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (access(gz_file, F_OK) < 0) {
          /* File does not exist : download the file
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
fprintf(stderr, "%s (%d) : file \"%s\" does not exist => download ...\n",
        __func__, __LINE__, gz_file);
          dp_mnist_download_file(url);
     }

     /* Uncompress the downloaded file
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     sprintf(_cmd, "gunzip %s", gz_file);
fprintf(stderr, "%s (%d) : uncompress (cmd = \"%s\" ...\n",
        __func__, __LINE__, gz_file);

     if ((_ret = system(_cmd) < 0)) {
          fprintf(stderr, "%s: uncompress error, command (%s) returned an error (%d)!\n",
                  G.progname, _cmd, _ret);
          perror("system");
          exit(RPN_EXIT_SYSTEM_ERROR);
     }
}

/* dp_mnist_uncompress_file() }}} */
/* dp_mnist_download_file() {{{ */

/******************************************************************************

                         DP_MNIST_DOWNLOAD_FILE

******************************************************************************/
void dp_mnist_download_file(char *url)
{
     char                      _cmd[RPN_SYS_CMD_SIZE];
     int                       _ret;

     /* Prepare download command
        ~~~~~~~~~~~~~~~~~~~~~~~~ */
     sprintf(_cmd, "wget %s", url);
     
     /* Execute the download command
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if ((_ret = system(_cmd) < 0)) {
          fprintf(stderr, "%s: download command (%s) returned an error (%d)!\n",
                  G.progname, _cmd, _ret);
          perror("system");
          exit(RPN_EXIT_SYSTEM_ERROR);
     }
}

/* dp_mnist_download_file() }}} */
/* dp_remove_ext() {{{ */

/*******************************************************************************

                         DP_REMOVE_EXT

*******************************************************************************/
void dp_remove_ext(char *s)
{
     int                 _i, _j;

     _i = _j = 0;

     /* Search for the last '.' character
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     while (s[_i] != '\0') {
          if (s[_i] == '.') {
               _j = _i;
          }
          _i++;
     }

     /* Reset the last '.' character
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (_j != 0) {
          s[_j] = '\0';
     }
}

/* dp_remove_ext() }}} */
/* dp_plot_pic() {{{ */

/******************************************************************************

                         DP_PLOT_PIC

******************************************************************************/
void dp_plot_pic(char *basename, dp_mnist_desc *mnist)
{
     char                 _cmd[RPN_SYS_CMD_SIZE];
     int                  _ret;

     sprintf(_cmd, "gnuplot -e RADIX='\"%s\"' %s/%s",
              basename, RPN_MNIST_DIR_TOOLS, mnist->plot_pic);
printf("PLOT COMMAND [%s]\n", _cmd);

     /* Execute the plot command
        ~~~~~~~~~~~~~~~~~~~~~~~~ */
     if ((_ret = system(_cmd) < 0)) {
          fprintf(stderr, "%s: plot command (%s) returned an error (%d)!\n",
                  G.progname, _cmd, _ret);
          perror("system");
          exit(RPN_EXIT_SYSTEM_ERROR);
     }
}

/* dp_plot_pic() }}} */
/* dp_plot_pics() {{{ */

/******************************************************************************

                         DP_PLOT_PICS

******************************************************************************/
void dp_plot_pics(char *basename, dp_mnist_desc *mnist)
{
     char                 _cmd[RPN_SYS_CMD_SIZE];
     int                  _ret;

     sprintf(_cmd, "gnuplot -e RADIX='\"%s\"' %s/%s",
              basename, RPN_MNIST_DIR_TOOLS, mnist->plot_pics);
printf("PLOT COMMAND [%s]\n", _cmd);

     /* Execute the plot command
        ~~~~~~~~~~~~~~~~~~~~~~~~ */
     if ((_ret = system(_cmd) < 0)) {
          fprintf(stderr, "%s: plot command (%s) returned an error (%d)!\n",
                  G.progname, _cmd, _ret);
          perror("system");
          exit(RPN_EXIT_SYSTEM_ERROR);
     }
}


/* dp_plot_pics() }}} */

/* Plot commands {{{ */
rpn_text_file           dp_text_plot_pic = {
     TRUE,
     0444,
     NULL,               /* Dirname  : dynamically initialized */
     NULL,               /* Filename : dynamically initialized */
     {
          "#!/usr/bin/gnuplot",
          "#",
          "#  @(#) [MB] cy_plot_heatmap_gray_640 Version 1.2 du 19/01/01 - ",
          "#",
          "",
          "reset",
          "",
          "set terminal pngcairo size 640,480 enhanced font 'Verdana,10'",
          "INPUT    = sprintf(\"TEXTS/%s.%s\",  RADIX, \"txt\")",
          "OUTPUT   = sprintf(\"IMAGES/%s.%s\", RADIX, \"png\")",
          "print \"RADIX  = \", RADIX",
          "print \"INPUT  = \", INPUT",
          "print \"OUTPUT = \", OUTPUT",
          "set output OUTPUT",
          "",
          "set border linewidth 0",
          "unset key",
          "unset colorbox",
          "unset tics",
          "set lmargin screen 0.1",
          "set rmargin screen 0.9",
          "set tmargin screen 0.9",
          "set bmargin screen 0.1",
          "set xlabel noenhanced  OUTPUT",
          "# set palette grey",
          "set palette grey negative",
          "",
          "set pm3d map",
          "splot INPUT matrix",
          NULL
     }
};

rpn_text_file           dp_text_plot_pics = {
     TRUE,
     0444,
     NULL,               /* Dirname  : dynamically initialized */
     NULL,               /* Filename : dynamically initialized */
     {
          "#!/usr/bin/gnuplot",
          "#",
          "#    @(#) [MB] cy_plot_heatmap_gray_1920     Version 1.6 du 18/12/31 -",
          "#",
          "",
          "reset",
          "",
          "set terminal pngcairo size 1920,1080 enhanced font 'Verdana,10'",
          "INPUT    = sprintf(\"TEXTS/GROUPS/%s.%s\",  RADIX, \"txt\")",
          "OUTPUT   = sprintf(\"IMAGES/GROUPS/%s.%s\", RADIX, \"png\")",
          "print \"RADIX  = \", RADIX",
          "print \"INPUT  = \", INPUT",
          "print \"OUTPUT = \", OUTPUT",
          "set output OUTPUT",
          "set border linewidth 0",
          "unset key",
          "unset colorbox",
          "unset tics",
          "set lmargin screen 0.1",
          "set rmargin screen 0.9",
          "set tmargin screen 0.9",
          "set bmargin screen 0.1",
          "set xlabel noenhanced  OUTPUT",
          "set palette grey negative",
          "set pm3d map",
          "splot INPUT matrix",
          NULL
     }
};

/* Plot commands }}} */
/* dp_tools_plot() {{{ */

/******************************************************************************

                         DP_TOOLS_PLOT

******************************************************************************/
void dp_tools_plot(rpn_text_file *text_file)
{

     
}
/* dp_tools_plot() }}} */
/* dp_new_mnist_desc() {{{ */
/******************************************************************************

                         DP_NEW_MNIST_DESC

******************************************************************************/
RPN_PREF_NEW(dp, mnist_desc)

/* dp_new_mnist_desc() }}} */
/* dp_mnist_init() {{{ */
/******************************************************************************

                         DP_MNIST_INIT

******************************************************************************/
dp_mnist_desc *dp_mnist_init()
{
     dp_mnist_desc				*_desc;
     size_t                         _size;
     char                          *_subdir_fmt   = "%s/%s";
     rpn_stack                     *_stack;
     rpn_elt                       *_elt;

     _desc                         = dp_new_mnist_desc();
     G.mnist                       = _desc;

     /* Make sure that necessary directories are created
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _desc->data_dir               = RPN_MNIST_DIR_DATA;
     _desc->texts_dir              = RPN_MNIST_DIR_TEXTS;
     _desc->images_dir             = RPN_MNIST_DIR_IMAGES;
     _desc->tools_dir              = RPN_MNIST_DIR_TOOLS;
     _desc->groups_dir             = RPN_MNIST_SUBDIR_GROUPS;

     _desc->plot_pic               = RPN_MNIST_PLOT_PIC;
     _desc->plot_pics              = RPN_MNIST_PLOT_PICS;

     _size                         = strlen(_desc->texts_dir) + 1
                                   + strlen(_desc->groups_dir) + 1;

     if ((_desc->subdir_texts = RPN_MALLOC(_size)) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     sprintf(_desc->subdir_texts, _subdir_fmt, _desc->texts_dir,  _desc->groups_dir);

     _size                         = strlen(_desc->images_dir) + 1
                                   + strlen(_desc->groups_dir) + 1;

     if ((_desc->subdir_images = RPN_MALLOC(_size)) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     sprintf(_desc->subdir_images, _subdir_fmt, _desc->images_dir, _desc->groups_dir);

     /* Check existence of directories
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_mkdir(_desc->data_dir);
     rpn_mkdir(_desc->texts_dir);
     rpn_mkdir(_desc->images_dir);
     rpn_mkdir(_desc->tools_dir);

     rpn_mkdir(_desc->subdir_texts);
     rpn_mkdir(_desc->subdir_images);

     _stack                        = rpn_new_stack(__func__);

     /* Create plot scripts
        ~~~~~~~~~~~~~~~~~~~ */
     dp_text_plot_pic.dirname      = _desc->tools_dir;
     dp_text_plot_pic.filename     = _desc->plot_pic;
     _elt                          = rpn_new_elt(RPN_TYPE_TEXT_FILE);
     _elt->value.obj               = &dp_text_plot_pic;

     rpn_push(_stack, _elt);

     /* Write plot script to file
        ~~~~~~~~~~~~~~~~~~~~~~~~~ */
Z
     dl_exec_op(_stack, RPN_OP_NAME_WRITE);

     dp_text_plot_pics.dirname     = _desc->tools_dir;
     dp_text_plot_pics.filename    = _desc->plot_pics;
     _elt                          = rpn_new_elt(RPN_TYPE_TEXT_FILE);
     _elt->value.obj               = &dp_text_plot_pics;

     rpn_push(_stack, _elt);

     /* Write plot script to file
        ~~~~~~~~~~~~~~~~~~~~~~~~~ */
Z
     dl_exec_op(_stack, RPN_OP_NAME_WRITE);

     /* Free temporary stack
        ~~~~~~~~~~~~~~~~~~~~ */
     rpn_free_stack(_stack);

     return _desc;
}

/* dp_mnist_init() }}} */
#if 0
/* dp_free_mlp() {{{ */

/******************************************************************************

					DP_FREE_MLP

******************************************************************************/
void dp_free_mlp(rpn_elt *elt_mlp)
{
	dp_mlp				*_mlp;
	cc_uint32				 _l;
	int					 _type;

	_type				= rpn_get_type(elt_mlp);
	if (_type != RPN_TYPE_NEURAL_MLP) {
		RPN_INTERNAL_ERROR;
	}

	_mlp					= elt_mlp->value.obj;
	for (_l = 0; _l < _mlp->nb_layers; _l++)  {
		dp_perceptron			*_perceptron;
		int					 _n;

		_perceptron			= _mlp->layers[_l];
		for (_n = 0; _n < _perceptron->nb_neurons; _n++) {
			RPN_FREE(&_perceptron->neurons[_n]);
		}
		RPN_FREE(&_perceptron);
	}
	RPN_FREE(&_mlp);
}

/* dp_free_mlp() }}} */
#endif

/* Definitions of 7 segments digits in MNIST digits dimensions
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
dp_7_seg_digit              dp_7_segments[10] = {
     {
/* 7 segments digits {{{ */
/* 7 segments : 0 {{{ */
		{
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
		}
     },
/* 7 segments : 0 }}} */
/* 7 segments : 1 {{{ */
     {
		{
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
		}
     },
/* 7 segments : 1 }}} */
/* 7 segments : 2 {{{ */
     {
		{
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
		}
     },
/* 7 segments : 2 }}} */
/* 7 segments : 3 {{{ */
     {
		{
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
		}
     },
/* 7 segments : 3 }}} */
/* 7 segments : 4 {{{ */
     {
		{
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
		}
     },
/* 7 segments : 4 }}} */
/* 7 segments : 5 {{{ */
	{
		{
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
		}
     },
/* 7 segments : 5 }}} */
/* 7 segments : 6 {{{ */
     {
		{
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
		}
     },
/* 7 segments : 6 }}} */
/* 7 segments : 7 {{{ */
     {
		{
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
		}
     },
/* 7 segments : 7 }}} */
/* 7 segments : 8 {{{ */
     {
		{
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
		}
     },
/* 7 segments : 8 }}} */
/* 7 segments : 9 {{{ */
     {
		{
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
		}
     }
/* 7 segments : 9 }}} */
/* 7 segments digits }}} */
};
// GROUP : AI }}}
