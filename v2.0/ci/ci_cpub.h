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
 *   @(#)  [MB] ci_cpub.h Version 1.10 du 19/11/29 - 
 */

#if ! defined(CI_CPUB_H)
# define CI_CPUB_H

#include  "../cc/cc_types.h"

#ifndef MAX
#define   MAX(a, b)      ( (a) > (b) ? (a) : (b) )
#endif

/* Return codes of comparison functions
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   CI_CMP_LT                0x01
#define   CI_CMP_GT                0x02
#define   CI_CMP_EQ                0x03

#define   CI_LEFT                  0x11
#define   CI_RIGHT                 0x22
#define   CI_DOWN                  0x33

#define   CI_T_LNR                 0xAA
#define   CI_T_RNL                 0xAB
#define   CI_T_LRN                 0xAC
#define   CI_T_RLN                 0xAD
#define   CI_T_NLR                 0xAE
#define   CI_T_NRL                 0xAF

#define	CI_PHASE_UNUSED		0x00
#define	CI_PHASE_BEGIN			0xBB
#define	CI_PHASE_END			0xEE

/* Type definitions
   ~~~~~~~~~~~~~~~~ */
typedef	struct ci_root			 ci_root;
typedef   struct ci_root           *ci_ref_root;
typedef	struct ci_node			 ci_node;
typedef   struct ci_node           *ci_ref_node;
typedef	struct ci_trek			 ci_trek;
typedef   struct ci_trek           *ci_ref_trek;

/* Structures
   ~~~~~~~~~~ */
struct ci_root {
     ci_ref_node                    root;
     ci_ref_node                    first;
     ci_ref_node                    last;
     cc_uint64                      nb;
};

struct ci_node {
     ci_ref_node                    left;
     ci_ref_node                    right;
     ci_ref_node                    up;
     ci_ref_node                    next;
     ci_ref_node                    previous;
     cc_uint8                       where;
     cc_uint8                       H_left;
     cc_uint8                       H_right;
     void                          *data;
};

struct ci_trek {
     ci_ref_node                    node;
     cc_uint16                      type;
     cc_uint16                      where;
};

#endif    /* CI_CPUB_H */
