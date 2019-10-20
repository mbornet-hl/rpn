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
 *	@(#)	[MB] ci_epub.h	Version 1.6 du 19/10/11 - 
 */

void				 ci_init_root(ci_ref_root);
void				 ci_init_node(ci_ref_node);
ci_ref_node		 ci_add_node(ci_ref_root, ci_ref_node,
				             cc_uint16 (*)(ci_ref_node, ci_ref_node),
						   void (*)(ci_ref_node, ci_ref_node));
ci_ref_node		 ci_del_node(ci_ref_root, ci_ref_node);
ci_ref_node		 ci_rebalance(ci_ref_root, ci_ref_node);

int				 ci_traversal(ci_ref_root, void (*)(ci_ref_node), cc_uint16);
int				 ci_cross(ci_ref_node, void (*)(ci_ref_node), cc_uint16);

int				 ci_fifo(ci_ref_root, void (*)(ci_ref_node), cc_uint16);

ci_ref_node		 ci_search(ci_ref_root, ci_ref_node,
				           cc_uint16 (*)(ci_ref_node, ci_ref_node));
void				 ci_disp(ci_ref_node);
void				 ci_reset(ci_ref_trek, ci_ref_root, cc_uint16);
ci_ref_node		 ci_get_next(ci_ref_trek);

void				 ci_check_root(ci_ref_root, ci_ref_node);

void				 ci_dump_tree(ci_ref_root);
void				 ci_dump_node(ci_ref_node);
void				 ci_raz_node(ci_ref_node);
