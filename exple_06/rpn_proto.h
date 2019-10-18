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
 *   rpn_proto.h
 *
 *   @(#)  [MB] rpn_proto.h Version 1.1 du 17/11/13 - 
 */


/* Functions prototypes
   ~~~~~~~~~~~~~~~~~~~~ */
rpn_stack                           *rpn_new_stack(const char *);
rpn_elt                             *rpn_new_elt(void);
void                                 rpn_free_elt(rpn_elt *);
rpn_elt                             *rpn_clone_elt(rpn_elt *);

int                                  rpn_push(rpn_stack *, rpn_elt *);
rpn_elt                             *rpn_pop(rpn_stack *);

int                                  rpn_op_roll_down(rpn_stack *);
int                                  rpn_op_roll_up(rpn_stack *);
int                                  rpn_op_lastx(rpn_stack *);

int                                  rpn_op_swap_xy(rpn_stack *);
int                                  rpn_op_swap_xz(rpn_stack *);
int                                  rpn_op_swap_xt(rpn_stack *);
int                                  rpn_op_swap_xl(rpn_stack *);
int                                  rpn_op_enter(rpn_stack *);
int							  rpn_op_clx(rpn_stack *);

int                                  rpn_get_type(rpn_elt *);
int                                  rpn_set_type(rpn_elt *, int);

rpn_operator                        *rpn_search_op(rpn_operator *, char *);

void                                 rpn_err_msg_no_mem(void);
void                                 rpn_err_msg_op_not_found(char *);
void                                 rpn_err_msg_op_error(char *, int);
void                                 rpn_err_msg_invalid_string(char *);

int                                  my_op_plus(rpn_stack *);
int                                  my_op_minus(rpn_stack *);
int                                  my_op_mult(rpn_stack *);
int                                  my_op_div(rpn_stack *);

int                                  my_op_str_length(rpn_stack *);
int                                  my_op_str_catenate(rpn_stack *);
int                                  my_op_str_dupl(rpn_stack *);

int                                  my_op_matrix(rpn_stack *);
int                                  my_op_explode(rpn_stack *);
int                                  my_op_vec2(rpn_stack *);
int                                  my_op_vec3(rpn_stack *);
int                                  my_op_transpose(rpn_stack *);
int                                  my_op_cross_prdct(rpn_stack *);

int                                  rpn_mat_get_i(struct rpn_matrix *, int, int);
struct rpn_elt                      *rpn_mat_get_elt(struct rpn_matrix *, int, int);

void							  rpn_disp_elt(rpn_elt *, int);
void							  rpn_disp_stack(rpn_stack *);
