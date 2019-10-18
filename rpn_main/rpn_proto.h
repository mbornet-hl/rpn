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
 *   rpn_proto.h
 *
 *   @(#)  [MB] cy_rpn_proto.h Version 1.41 du 19/05/05 - 
 */

double                               exp10(double);

/* Functions prototypes
   ~~~~~~~~~~~~~~~~~~~~ */
rpn_stack                           *rpn_new_stack(const char *);
rpn_elt                             *rpn_new_elt(int);
rpn_elt                             *rpn_new_int(int);
void                                 rpn_free_elt(rpn_elt **);
rpn_elt                             *rpn_clone_elt(rpn_elt *);
struct rpn_sigma                    *rpn_new_sigma(char *);
void                                 rpn_sto(rpn_stack *, struct rpn_elt **);

int                                  rpn_push(rpn_stack *, rpn_elt *);
rpn_elt                             *rpn_pop(rpn_stack *);
rpn_elt                             *rpn_base_pop(rpn_stack *);

int                                  rpn_get_type(rpn_elt *);
int                                  rpn_set_type(rpn_elt *, int);

void                                 rpn_set_lastx(rpn_stack *, rpn_elt *);
void                                 rpn_free_stack(rpn_stack *);
int                                  rpn_get_X_type(rpn_stack *);
int                                  rpn_get_Y_type(rpn_stack *);
int                                  rpn_backup_elt(rpn_elt *, struct rpn_backup_ctx *);
void                                 rpn_inc_begin(rpn_stack *);
void                                 rpn_dec_begin(rpn_stack *);
void                                 rpn_nn_learn();
void                                 rpn_nn_analyze();
int                                  rpn_get_type(rpn_elt *);
void                                 rpn_disp_stk(rpn_stack *);
int                                  rpn_catalog(struct rpn_operator *);
int                                  rpn_todo(struct rpn_operator *);
void                                 rpn_init_backtrace(void);
void                                 rpn_check_ops();



RPN_DECL_OP(rpn_op_sw_on);
RPN_DECL_OP(rpn_op_sw_off);

rpn_operator                        *rpn_search_op(rpn_operator *, char *);

int                                  rpn_op(rpn_stack *, rpn_operator *);

void                                 rpn_err_msg_no_mem(void);
void                                 rpn_err_msg_op_not_found(char *);
void                                 rpn_err_msg_op_error(char *, int);
char                                *rpn_err_to_string(int errcode);
void                                 rpn_err_msg_invalid_string(char *);
void                                 rpn_internal_error(const char *, char *, int);

int                                  rpn_strip_quotes(char **);
int                                  rpn_strip_single_quotes(char **);
int                                  rpn_check_types(rpn_operator *, struct rpn_stack *);
char                                *rpn_type_to_string(int);
int                                  rpn_x_to_int(rpn_stack *);

void                                 rpn_print_trace(int);

void                                *rpn_malloc(size_t);
void                                 rpn_free(void *);
void                                 rpn_mkdir(char *);
char                                *rpn_gen_command(rpn_elt *);
int                                  rpn_str_size(rpn_list *);
int							  rpn_exec(char *, char **, int *);

void                                 rpn_tools_plot(rpn_text_file *);
RPN_DECL_NEW(mnist_desc);
rpn_mnist_desc                      *rpn_mnist_init(void);

RPN_DECL_OP(rpn_op_undef);

/* Tuples
   ~~~~~~ */
RPN_DECL_OP(rpn_op_tuples);

/* Pairs
   ~~~~~ */
RPN_DECL_OP(rpn_op_pairs);

/* Tests
   ~~~~~ */
RPN_DECL_OP(rpn_op_test);
void							  rpn_set_test_result(int);

/* Strings
   ~~~~~~~ */
RPN_DECL_OP(rpn_op_str_length);
RPN_DECL_OP(rpn_op_str_catenate);
RPN_DECL_OP(rpn_op_str_dupl);


RPN_DECL_OP(rpn_op_roll_down);
RPN_DECL_OP(rpn_op_roll_up);
RPN_DECL_OP(rpn_op_lastx);
RPN_DECL_OP(rpn_op_rcl_x);

RPN_DECL_OP(rpn_op_swap_xy);
RPN_DECL_OP(rpn_op_swap_xz);
RPN_DECL_OP(rpn_op_swap_xt);
RPN_DECL_OP(rpn_op_swap_xl);
RPN_DECL_OP(rpn_op_enter);
RPN_DECL_OP(rpn_op_clx);
RPN_DECL_OP(rpn_op_delx);
RPN_DECL_OP(rpn_op_del_l);
RPN_DECL_OP(rpn_op_clst);
RPN_DECL_OP(rpn_op_prx);
RPN_DECL_OP(rpn_op_prstk);
RPN_DECL_OP(rpn_op_stk);
RPN_DECL_OP(rpn_op_dupx);
RPN_DECL_OP(rpn_op_dump);


/* Generic functions
   ~~~~~~~~~~~~~~~~~ */
RPN_DECL_OP(rpn_op_generic1);
RPN_DECL_OP(rpn_op_generic2);
RPN_DECL_OP(rpn_op_generic_list);

RPN_DECL_OP(rpn_op_get);

/* Lists
   ~~~~~ */
//RPN_DECL_NEW(list);
rpn_list                            *rpn_new_list(char *);
RPN_DECL_OP(rpn_op_push);
RPN_DECL_OP(rpn_op_pop);
RPN_DECL_OP(rpn_op_list);
int                                  rpn_list_push_head(rpn_list *, rpn_elt *);
rpn_elt                             *rpn_list_pop_head(rpn_list *);
int                                  rpn_list_push_tail(rpn_list *, rpn_elt *);
rpn_elt                             *rpn_list_pop_tail(rpn_list *);

rpn_pair                            *rpn_new_pair(char *);

/* Math
   ~~~~ */
RPN_DECL_OP(rpn_op_math_mul);
RPN_DECL_OP(rpn_op_div);

RPN_DECL_OP(rpn_op_reciprocal);
RPN_DECL_OP(rpn_op_power);
RPN_DECL_OP(rpn_op_sqrt);
RPN_DECL_OP(rpn_op_square);
RPN_DECL_OP(rpn_op_ln);
RPN_DECL_OP(rpn_op_exp);
RPN_DECL_OP(rpn_op_log10);
RPN_DECL_OP(rpn_op_10_power_x);
RPN_DECL_OP(rpn_op_percent);
RPN_DECL_OP(rpn_op_delta_percent);
RPN_DECL_OP(rpn_op_sine);
RPN_DECL_OP(rpn_op_cosine);
RPN_DECL_OP(rpn_op_tan);
RPN_DECL_OP(rpn_op_matrix);
RPN_DECL_OP(rpn_op_explode);
RPN_DECL_OP(rpn_op_vec2);
RPN_DECL_OP(rpn_op_vec3);
RPN_DECL_OP(rpn_op_transpose);
RPN_DECL_OP(rpn_op_cross_prdct);
RPN_DECL_OP(rpn_op_scalar_matrix);
RPN_DECL_OP(rpn_op_seq);
RPN_DECL_OP(rpn_op_dim);
RPN_DECL_OP(rpn_op_h_rev);
RPN_DECL_OP(rpn_op_diag_mat);
RPN_DECL_OP(rpn_op_z_mat);
RPN_DECL_OP(rpn_op_mat_nil_diag);

/* Statistics
   ~~~~~~~~~~ */
RPN_DECL_OP(rpn_op_sum_add);
RPN_DECL_OP(rpn_op_sum_sub);
RPN_DECL_OP(rpn_op_mean);
RPN_DECL_OP(rpn_op_sdev);
RPN_DECL_OP(rpn_op_meanx);
RPN_DECL_OP(rpn_op_mmm);
RPN_DECL_OP(rpn_op_gen_data_set);

/* Neural networks
   ~~~~~~~~~~~~~~~ */
RPN_DECL_OP(rpn_op_neural_gen_nn);
RPN_DECL_OP(rpn_op_read_elt);
RPN_DECL_OP(rpn_op_mnist_pic);
RPN_DECL_OP(rpn_op_mnist_pics);
RPN_DECL_OP(rpn_op_mat_7_seg);
struct rpn_mlp                      *rpn_new_mlp(int);
struct rpn_input_layer              *rpn_new_input_layer(int);
struct rpn_perceptron               *rpn_new_perceptron(int);
struct rpn_neuron                   *rpn_new_neuron(int);
RPN_DECL_OP(rpn_set_mlp_weights);
RPN_DECL_OP(rpn_op_read_mnist);
RPN_DECL_OP(rpn_op_nn_label);
RPN_DECL_OP(rpn_op_nn_train_index);
char                                *rpn_mnist_install_file(char *);
void                                 rpn_mnist_uncompress_file(char *, char *);
void                                 rpn_mnist_download_file(char *);
void                                 rpn_remove_ext(char *);
void                                 rpn_plot_pic(char *, rpn_mnist_desc *);
void                                 rpn_plot_pics(char *, rpn_mnist_desc *);
RPN_DECL_OP(rpn_op_nn_dispatch);
RPN_DECL_OP(rpn_op_nn_ignore);
void                                 rpn_create_set(rpn_stack *, rpn_mnist_set *);

rpn_weight                           rpn_nn_rand(void);

rpn_weight                           rpn_nn_af_sigmoid(rpn_weight),
                                     rpn_nn_af_tanh(rpn_weight),
                                     rpn_nn_af_arc_tan(rpn_weight),
                                     rpn_nn_af_relu(rpn_weight);

int                                  rpn_mat_get_i(struct rpn_matrix *, int, int);
struct rpn_elt                      *rpn_mat_get_elt_ref(struct rpn_matrix *, int, int);
struct rpn_elt                      *rpn_mat_extract_elt(struct rpn_matrix *, int, int);

void                                 rpn_disp_elt(rpn_elt *, int);
// void                                 rpn_disp_stack(rpn_stack *, char *);
void                                 rpn_disp_stack(rpn_stack *);
void                                 rpn_disp_sigma(void);

void							  rpn_dump_elt(rpn_elt *, int);
void							  rpn_indent(int);

RPN_DECL_NEW(training_elt);
RPN_DECL_NEW(training_data);
RPN_DECL_NEW(training_label);
RPN_DECL_NEW(input_neuron);

void                                 rpn_save_mlp(struct rpn_elt *);

RPN_DECL_OP(rpn_op_write);
RPN_DECL_OP(rpn_op_write_csv);
void							  rpn_write_csv(rpn_elt *, FILE *);

/* Computer science
   ~~~~~~~~~~~~~~~~ */
RPN_DECL_OP(rpn_op_cs_gen_C);

/* System
   ~~~~~~ */
RPN_DECL_OP(rpn_op_sys_memory);
RPN_DECL_OP(rpn_op_sys_pause);
RPN_DECL_OP(rpn_op_sys_ping);
RPN_DECL_OP(rpn_op_sys_iperf);
