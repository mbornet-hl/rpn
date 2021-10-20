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
 *   @(#)  [MB] cy_rpn_proto.h Version 1.70 du 21/10/19 - 
 */

#if ! defined(_RPN_PROTO_H)
#define   _RPN_PROTO_H

double                               exp10(double);

/* Functions prototypes
   ~~~~~~~~~~~~~~~~~~~~ */
void							  rpn_undefined_disp_elt(rpn_elt *, int);
void							  rpn_undefined_clone_elt(rpn_elt *, rpn_elt *);
char							 *rpn_undefined_type_to_string(int);
void							  rpn_undefined_free_elt(rpn_elt *, int);

rpn_stack                           *rpn_new_stack(const char *);
rpn_elt                             *rpn_new_elt(int);
rpn_elt                             *rpn_new_named_elt(int, char *);
void                                 rpn_set_elt_name(rpn_elt *, char *);
rpn_elt                             *rpn_new_int(int);
rpn_elt                             *rpn_new_double(double);
void                                 rpn_free_elt(rpn_elt **);
rpn_elt                             *rpn_clone_elt(rpn_elt *);
struct rpn_sigma                    *rpn_new_sigma(char *);
void                                 rpn_sto(rpn_stack *, struct rpn_elt **);

int                                  rpn_push(rpn_stack *, rpn_elt *);
rpn_elt                             *rpn_pop(rpn_stack *);
int                                  rpn_base_push(rpn_stack *, rpn_elt *);
rpn_elt                             *rpn_base_pop(rpn_stack *);

int                                  rpn_get_type(rpn_elt *);
int                                  rpn_set_type(rpn_elt *, int);

void                                 rpn_set_lastx(rpn_stack *, rpn_elt *);
void                                 rpn_free_stack(rpn_stack *);
int                                  rpn_get_X_type(rpn_stack *);
int                                  rpn_get_Y_type(rpn_stack *);
int                                  rpn_get_Z_type(rpn_stack *);
int                                  rpn_get_T_type(rpn_stack *);
int                                  rpn_backup_elt(rpn_elt *, struct rpn_backup_ctx *);
void                                 rpn_inc_begin(rpn_stack *);
void                                 rpn_dec_begin(rpn_stack *);
void                                 rpn_nn_learn(void);
void                                 rpn_nn_analyze(void);
int                                  rpn_get_type(rpn_elt *);
void                                 rpn_disp_stk(rpn_stack *);
int                                  rpn_modules(void);
int                                  rpn_catalog(struct rpn_operator *);
int                                  rpn_todo(struct rpn_operator *);
void                                 rpn_init_backtrace(void);
void                                 rpn_check_ops(void);


dl_operator                         *rpn_search_op(rpn_stack *, char *);

int                                  rpn_op(rpn_stack *, dl_operator *);

void                                 rpn_err_msg_no_mem(void);
void                                 rpn_err_msg_op_not_found(char *);
void                                 rpn_err_msg_op_error(char *, int);
char                                *rpn_err_to_string(int errcode);
void                                 rpn_err_msg_invalid_string(char *);
void                                 rpn_internal_error(const char *, char *, int);
void                                 rpn_unimplemented(char *, const char *, char *, int);

int                                  rpn_strip_quotes(char **);
int                                  rpn_strip_single_quotes(char **);
int                                  rpn_check_types(rpn_operator *, struct rpn_stack *);
char                                *rpn_type_to_string(int);
int                                  rpn_x_to_int(rpn_stack *);

void                                 rpn_print_trace(int);

void                                *rpn_malloc(size_t);
void                                 rpn_free(void *);
char							 *rpn_strdup(char *);
void                                 rpn_mkdir(char *);
char                                *rpn_gen_command(rpn_elt *);
int                                  rpn_str_size(rpn_list *);
int                                  rpn_exec(char *, char **, int *, int *);
void							  cy_usage(char *);

RPN_DECL_NEW(operator);
RPN_DECL_NEW(module);
RPN_DECL_NEW(dyn_op);
int                                  rpn_module_tree_add(struct rpn_module *);
struct rpn_module                   *rpn_module_tree_search(char *);

void                                 rpn_tools_plot(rpn_text_file *);
int                                  rpn_import(char *);
cc_uint16                            rpn_cmp_modules(struct ci_node *, struct ci_node *);
cc_uint16                            rpn_cmp_operators(struct ci_node *, struct ci_node *);
void                                 rpn_set_list_name(rpn_elt *, char *);
char                                *rpn_get_list_name(rpn_elt *);
int                                  rpn_get_list_num(rpn_elt *);
rpn_elt                             *rpn_get_next_elt(rpn_elt *);
rpn_list                            *rpn_get_list_addr(rpn_elt *);
double                               rpn_coef_get_a(rpn_elt *);
double                               rpn_coef_get_b(rpn_elt *);
double                               rpn_get_min(rpn_elt *);
double                               rpn_get_max(rpn_elt *);
RPN_DECL_NEW(coef_a_b);
RPN_DECL_NEW(min_max);
char							 *rpn_str_printf(char *, char *);
rpn_litteral					 *rpn_new_litteral(void);
char							 *rpn_litteral_value(rpn_elt *);
void							 rpn_litteral_set_value(rpn_elt *, char *);


RPN_DECL_NEW(regex);
RPN_DECL_OP(en_op_regex_regex);

#if 0
RPN_DECL_OP(rpn_op_undef);
#endif

#if 0
/* Tuples
   ~~~~~~ */
RPN_DECL_OP(rpn_op_tuples);

/* Pairs
   ~~~~~ */
RPN_DECL_OP(rpn_op_pairs);

/* Tests
   ~~~~~ */
RPN_DECL_OP(rpn_op_test);
#endif
void                                 rpn_set_test_result(rpn_stack *, int);

#if 0
/* Strings
   ~~~~~~~ */
RPN_DECL_OP(rpn_op_str_length);
RPN_DECL_OP(rpn_op_str_catenate);
RPN_DECL_OP(rpn_op_str_dupl);
#endif


#if 0
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
#endif


#if 0
/* Generic functions
   ~~~~~~~~~~~~~~~~~ */
RPN_DECL_OP(rpn_op_generic1);
RPN_DECL_OP(rpn_op_generic2);
RPN_DECL_OP(rpn_op_generic_list);

RPN_DECL_OP(rpn_op_get);
#endif

/* Lists
   ~~~~~ */
//RPN_DECL_NEW(list);
rpn_list                            *rpn_new_list(char *);
#if 0
RPN_DECL_OP(rpn_op_push);
RPN_DECL_OP(rpn_op_pop);
RPN_DECL_OP(rpn_op_list);
#endif
int                                  rpn_list_push_head(rpn_list *, rpn_elt *);
rpn_elt                             *rpn_list_pop_head(rpn_list *);
int                                  rpn_list_push_tail(rpn_list *, rpn_elt *);
rpn_elt                             *rpn_list_pop_tail(rpn_list *);

rpn_pair                            *rpn_new_pair(char *);

#if 0
/* Neural networks
   ~~~~~~~~~~~~~~~ */
RPN_DECL_OP(rpn_op_neural_gen_nn);
RPN_DECL_OP(rpn_op_read_elt);
RPN_DECL_OP(rpn_op_mnist_pic);
RPN_DECL_OP(rpn_op_mnist_pics);
RPN_DECL_OP(rpn_op_mat_7_seg);
RPN_DECL_OP(rpn_set_mlp_weights);
RPN_DECL_OP(rpn_op_read_mnist);
RPN_DECL_OP(rpn_op_nn_label);
RPN_DECL_OP(rpn_op_nn_train_index);
RPN_DECL_OP(rpn_op_nn_dispatch);
RPN_DECL_OP(rpn_op_nn_ignore);
#endif
struct rpn_mlp                      *rpn_new_mlp(int);
struct rpn_input_layer              *rpn_new_input_layer(int);
struct rpn_perceptron               *rpn_new_perceptron(int);
struct rpn_neuron                   *rpn_new_neuron(int);
char                                *rpn_mnist_install_file(char *);
void                                 rpn_mnist_uncompress_file(char *, char *);
void                                 rpn_mnist_download_file(char *);
void                                 rpn_remove_ext(char *);
void                                 rpn_plot_pic(char *, rpn_mnist_desc *);
void                                 rpn_plot_pics(char *, rpn_mnist_desc *);
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
void                                 rpn_disp_sigma(struct rpn_sigma *);
void                                 rpn_disp_loaded_modules(void);
void                                 rpn_disp_ops_tree(void);
void                                 rpn_disp_argp(char *, char **);
void                                 rpn_disp_prompt(void);

void                                 rpn_dump_elt(rpn_elt *, int);
void                                 rpn_indent(int);

RPN_DECL_NEW(training_elt);
RPN_DECL_NEW(training_data);
RPN_DECL_NEW(training_label);
RPN_DECL_NEW(input_neuron);

void                                 rpn_save_mlp(struct rpn_elt *);

#if 0
RPN_DECL_OP(rpn_op_write);
RPN_DECL_OP(rpn_op_write_csv);
#endif
void                                 rpn_write_csv(rpn_elt *, FILE *);

#if 0
/* Computer science
   ~~~~~~~~~~~~~~~~ */
RPN_DECL_OP(rpn_op_cs_gen_C);
#endif

#if 0
/* System
   ~~~~~~ */
RPN_DECL_OP(rpn_op_sys_memory);
RPN_DECL_OP(rpn_op_sys_pause);
RPN_DECL_OP(rpn_op_sys_ping);
RPN_DECL_OP(rpn_op_sys_iperf);
#endif

#endif    // _RPN_PROTO_H
