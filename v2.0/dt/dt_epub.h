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
 *   @(#)  [MB] dt_epub.h Version 1.8 du 19/10/19 - 
 */

static dl_op_desc                             dt_ops_array[];

RPN_DECL_OP(dt_op_math_reciprocal);
RPN_DECL_OP(dt_op_math_power);
RPN_DECL_OP(dt_op_math_square);
RPN_DECL_OP(dt_op_math_sqrt);
RPN_DECL_OP(dt_op_math_exp);
RPN_DECL_OP(dt_op_math_ln);
RPN_DECL_OP(dt_op_math_10_power_x);
RPN_DECL_OP(dt_op_math_log);
RPN_DECL_OP(dt_op_math_percent);
RPN_DECL_OP(dt_op_math_delta_percent);
RPN_DECL_OP(dt_op_math_sin);
RPN_DECL_OP(dt_op_math_cos);
RPN_DECL_OP(dt_op_math_tan);
RPN_DECL_OP(dt_op_math_asin);
RPN_DECL_OP(dt_op_math_acos);
RPN_DECL_OP(dt_op_math_atan);
RPN_DECL_OP(dt_op_math_sinh);
RPN_DECL_OP(dt_op_math_cosh);
RPN_DECL_OP(dt_op_math_tanh);
RPN_DECL_OP(dt_op_math_asinh);
RPN_DECL_OP(dt_op_math_acosh);
RPN_DECL_OP(dt_op_math_atanh);
RPN_DECL_OP(dt_op_math_sigmoid);
RPN_DECL_OP(dt_op_math_add);
RPN_DECL_OP(dt_op_math_sub);
RPN_DECL_OP(dt_op_math_mul);
RPN_DECL_OP(dt_op_math_div);
RPN_DECL_OP(dt_op_math_div);
RPN_DECL_OP(dt_op_math_math_mul);
RPN_DECL_OP(dt_op_math_seq);
RPN_DECL_OP(dt_op_math_seq2);
RPN_DECL_OP(dt_op_math_dim);
RPN_DECL_OP(dt_op_math_matrix);
RPN_DECL_OP(dt_op_math_explode);
RPN_DECL_OP(dt_op_math_vec2);
RPN_DECL_OP(dt_op_math_vec3);
RPN_DECL_OP(dt_op_math_transpose);
RPN_DECL_OP(dt_op_math_cross_product);
RPN_DECL_OP(dt_op_math_hrev);
RPN_DECL_OP(dt_op_math_vrev);
RPN_DECL_OP(dt_op_math_diagmat);
RPN_DECL_OP(dt_op_math_hcat);
RPN_DECL_OP(dt_op_math_vcat);
RPN_DECL_OP(dt_op_math_zmat);

rpn_elt                                      *dt_mat_get_elt_ref(rpn_matrix *, int, int);
rpn_elt                                      *dt_mat_extract_elt(rpn_matrix *, int, int);
rpn_elt								*dt_mat_clone_elt(rpn_matrix *, int, int);
rpn_elt                                      *dt_op_mat(rpn_elt *, dl_operator *);
rpn_elt                                      *dt_op_mat_X(rpn_elt *, rpn_elt *, dl_operator *);
rpn_elt                                      *dt_op_keep_mat_X(rpn_elt *, rpn_elt *, dl_operator *);
rpn_elt                                      *dt_op_Y_mat(rpn_elt *, rpn_elt *, dl_operator *);
rpn_elt								*dt_op_mat_mat(rpn_elt *, rpn_elt *, dl_operator *);
rpn_elt								*dt_op_keep_mat_mat(rpn_elt *, rpn_elt *, dl_operator *);
int									 dt_mat_check_dim(rpn_elt *, rpn_elt *);
char									*dt_alloc_string(char *, int);
char									*dt_alloc_string2(char *, char *, int);
char									*dt_parentheses(char *);
