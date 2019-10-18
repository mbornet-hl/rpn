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
 *   rpn_epri.h
 *
 *   @(#)  [MB] cy_rpn_epri.h Version 1.11 du 19/02/10 - 
 */

#if ! defined(RPNA_EPRI_H)
#define   _RPN_EPRI_H

/* External variables
   ~~~~~~~~~~~~~~~~~~ */
extern struct global_struct         G;

extern struct rpn_st_types          op_types_add[],
                                    op_types_sub[],
                                    op_types_mul[],
                                    op_types_div[],
                                    op_types_math_mul[],
                                    op_types_delta_percent[],
                                    op_types_power[],
                                    op_types_h_cat[],
                                    op_types_v_cat[],
                                    op_types_z_mat[],
                                    op_types_backup[],
                                    op_types_gen_nn[],
                                    op_types_type[],
                                    
                                    op_types_gen_math1[];

extern struct rpn_operator          rpn_operators[];

extern struct rpn_7_seg_digit       rpn_7_segments[10];

extern struct rpn_op_groups         rpn_op_groups[];
extern struct rpn_mesg_groups		 rpn_mesg_groups[];

extern FILE					*iperf_out_in;

#endif    // _RPN_EPRI_H
