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
 *	@(#)	[MB] dn_epri.h	Version 1.2 du 19/10/12 - 
 */


/* Prototypes
   ~~~~~~~~~~ */
RPN_DECL_OP(dn_op_stats_sum_add);
RPN_DECL_OP(dn_op_stats_sum_sub);
RPN_DECL_OP(dn_op_stats_mean);
RPN_DECL_OP(dn_op_stats_sdev);
RPN_DECL_OP(dn_op_stats_meanx);
RPN_DECL_OP(dn_op_stats_mmm);
RPN_DECL_OP(dn_op_stats_line_data_set);
RPN_DECL_OP(dn_op_stats_exp_data_set);
RPN_DECL_OP(dn_op_stats_log_data_set);
RPN_DECL_OP(dn_op_stats_pow_data_set);

int									dn_init(dl_module *);

