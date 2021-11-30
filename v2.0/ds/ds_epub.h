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
 *	@(#)	[MB] ds_epub.h	Version 1.12 du 21/11/30 - 
 */

static dl_op_desc						 ds_ops_array[];

RPN_DECL_OP(ds_op_core_chs);
RPN_DECL_OP(ds_op_core_clst);
RPN_DECL_OP(ds_op_core_clx);
RPN_DECL_OP(ds_op_core_del_l);
RPN_DECL_OP(ds_op_core_delx);
RPN_DECL_OP(ds_op_core_double);
RPN_DECL_OP(ds_op_core_dump);
RPN_DECL_OP(ds_op_core_dupx);
RPN_DECL_OP(ds_op_core_enter);
RPN_DECL_OP(ds_op_core_filename);
RPN_DECL_OP(ds_op_core_int);
RPN_DECL_OP(ds_op_core_IPv4);
RPN_DECL_OP(ds_op_core_lastx);
RPN_DECL_OP(ds_op_core_litteral);
RPN_DECL_OP(ds_op_core_load);
RPN_DECL_OP(ds_op_core_prstk);
RPN_DECL_OP(ds_op_core_prx);
RPN_DECL_OP(ds_op_core_rcl_x);
RPN_DECL_OP(ds_op_core_roll_down);
RPN_DECL_OP(ds_op_core_roll_up);
RPN_DECL_OP(ds_op_core_stk);
RPN_DECL_OP(ds_op_core_string);
RPN_DECL_OP(ds_op_core_swap_xl);
RPN_DECL_OP(ds_op_core_swap_xt);
RPN_DECL_OP(ds_op_core_swap_xy);
RPN_DECL_OP(ds_op_core_swap_xz);
RPN_DECL_OP(ds_op_core_sw_off);
RPN_DECL_OP(ds_op_core_sw_on);

RPN_DECL_OP(ds_op_core_list);
RPN_DECL_OP(ds_op_core_push);
RPN_DECL_OP(ds_op_core_pop);
RPN_DECL_OP(ds_op_core_pair);
RPN_DECL_OP(ds_op_core_tuple);
RPN_DECL_OP(ds_op_core_coef_a_b);
RPN_DECL_OP(ds_op_core_min_max);
RPN_DECL_OP(ds_op_core_cat);
RPN_DECL_OP(ds_op_core_write);
RPN_DECL_OP(ds_op_core_debug_mem);
#if 0
RPN_DECL_OP(ds_op_core_push_extract);
RPN_DECL_OP(ds_op_core_pop_insert);
RPN_DECL_OP(ds_op_core_get_head);
RPN_DECL_OP(ds_op_core_get_tail);
RPN_DECL_OP(ds_op_core_get);
RPN_DECL_OP(ds_op_core_get_idx);
RPN_DECL_OP(ds_op_core_get_ignored);
RPN_DECL_OP(ds_op_core_get_used);
RPN_DECL_OP(ds_op_core_set_idx);
#endif

RPN_DECL_OP(ds_op_core_set_name);
RPN_DECL_OP(ds_op_core_name);
RPN_DECL_OP(ds_op_core_disp_name);
RPN_DECL_OP(ds_op_core_explode);
RPN_DECL_OP(ds_op_core_clone);
RPN_DECL_OP(ds_op_core_types);
