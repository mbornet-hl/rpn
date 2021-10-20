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
 *	@(#)	[MB] dp_epri.h	Version 1.5 du 21/10/20 - 
 */

/* Variables
   ~~~~~~~~~ */
extern struct dp_7_seg_digit              dp_7_segments[10];

/* Prototypes
   ~~~~~~~~~~ */
RPN_DECL_METHODS(dp);

RPN_DECL_INIT(dp);

RPN_DECL_OP(dp_op_ai_clone);
RPN_DECL_OP(dp_op_ai_list);
RPN_DECL_OP(dp_op_ai_create_mlp);
RPN_DECL_OP(dp_op_ai_read_mnist);
RPN_DECL_OP(dp_op_ai_read_elt);
RPN_DECL_OP(dp_op_ai_mnist_pic);
RPN_DECL_OP(dp_op_ai_mnist_pics);
RPN_DECL_OP(dp_op_ai_get);
RPN_DECL_OP(dp_op_ai_matrix);
RPN_DECL_OP(dp_op_ai_mat_7_seg);
RPN_DECL_OP(dp_op_ai_label);
RPN_DECL_OP(dp_op_ai_index);
RPN_DECL_OP(dp_op_ai_orig_index);
RPN_DECL_OP(dp_op_ai_dispatch);
RPN_DECL_OP(dp_op_ai_ignore);
RPN_DECL_OP(dp_op_ai_use);

RPN_DECL_PREF_NEW(dp, training_elt);
RPN_DECL_PREF_NEW(dp, training_data);
RPN_DECL_PREF_NEW(dp, training_label);
RPN_DECL_PREF_NEW(dp, input_neuron);
RPN_DECL_PREF_NEW(dp, mnist_desc);
RPN_DECL_PREF_NEW(dp, mnist_desc);

dp_mnist_desc                       *dp_mnist_init(void);
#if 0
void							  dp_free_mlp(rpn_elt *);
#endif
char                                *dp_mnist_install_file(char *);
void                                 dp_create_set(rpn_stack *, dp_mnist_set *);
void                                 dp_plot_pic(char *, dp_mnist_desc *);
void                                 dp_plot_pics(char *, dp_mnist_desc *);
void                                 dp_remove_ext(char *);
void                                 dp_mnist_uncompress_file(char *, char *);
void                                 dp_mnist_download_file(char *);

dp_mlp						 *dp_new_mlp(int);
dp_input_layer					 *dp_new_input_layer(int);
dp_perceptron					 *dp_new_perceptron(int);
dp_neuron						 *dp_new_neuron(int);
dp_weight						  dp_af_sigmoid(dp_weight);
dp_weight						  dp_af_tanh(dp_weight);
dp_weight						  dp_af_arc_tan(dp_weight);
dp_weight						  dp_af_relu(dp_weight);
