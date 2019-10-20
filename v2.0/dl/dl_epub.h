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
 *   @(#)  [MB] dl_epub.h Version 1.9 du 19/10/11 - 
 */

void							 dl_set_lang(cc_uint16);

cc_uint64						 dl_type0(void);
cc_uint64						 dl_type1(cc_uint16);
cc_uint64						 dl_type2(cc_uint16, cc_uint16);
cc_uint64						 dl_type3(cc_uint16, cc_uint16, cc_uint16);
cc_uint64						 dl_type4(cc_uint16, cc_uint16, cc_uint16, cc_uint16);
void                                dl_op_tree_add(ci_root *, dl_module *);
dl_op_dyn_node                     *dl_op_tree_search(ci_root *, char *);
dl_op_dyn_node                     *dl_new_op_dyn_node(void);
void                                dl_free_op_dyn_node(dl_op_dyn_node *);
cc_uint16                           dl_op_dyn_node_cmp(ci_node *, ci_node *);
dl_params_dyn_node                 *dl_new_params_dyn_node();
dl_params_def_node                 *dl_new_params_def_node();
cc_uint16                           dl_params_dyn_node_cmp(ci_node *, ci_node *);
dl_operator                        *dl_search_matching_op(rpn_stack *, char *);
void							 dl_node_op_disp(ci_node *);
cc_uint64						 dl_global_types(dl_op_params *);
//dl_params_def_node				*dl_get_params_def(dl_op_dyn_node *, dl_op_params *);
dl_params_def_node				*dl_get_params_def(ci_root *, dl_op_types *);
dl_params_def_node				*dl_add_params_def(ci_root *, dl_params_def_node *);
cc_uint16						 dl_params_def_node_cmp(ci_node *, ci_node *);
dl_operator					*dl_new_operator(void);
dl_op_types					*dl_new_op_types(void);
void							 dl_dump_params_def_node(dl_params_def_node *);
void							 dl_dump_params_dyn_node(dl_params_dyn_node *);
void							 dl_dump_ops_tree(ci_root *);
void							 dl_dump_op_dyn_node(ci_node *);
void							 dl_dump_operator(dl_operator *);
void							 dl_disp_op_dyn_node(ci_node *);
void							 dl_disp_op_def(struct ci_node *);
void							 dl_disp_params_dyn_node(dl_params_dyn_node *);
void							 dl_disp_module_node(ci_node *);
void							 dl_reset_count_op_defs(void);
cc_uint32						 dl_get_count_op_defs(void);
void							 dl_count_op_defs(ci_node *);
void							 dl_disp_types(dl_op_types *);
int							 dl_exec_op(rpn_stack *, char *);
