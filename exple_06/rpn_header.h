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
 *   rpn_header.h
 *
 *   @(#)  [MB] cy_rpn_header.h Version 1.9 du 17/11/13 - 
 */

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  "rpn_types.h"

#define   X    fprintf(stderr, "%s (%d)\n", __FILE__, __LINE__);

/* Boolean values
   ~~~~~~~~~~~~~~ */
#define   FALSE                      (0)
#define   TRUE                       (1)

/* Exit codes
   ~~~~~~~~~~ */
#define   RPN_EXIT_OK                (0)
#define   RPN_EXIT_USAGE             (1)
#define   RPN_EXIT_NO_MEM            (2)
#define   RPN_EXIT_OP_UNKNOWN        (3)
#define   RPN_EXIT_OP_ERROR          (4)
#define   RPN_EXIT_INVALID_ELT       (5)
#define   RPN_EXIT_SYNTAX            (6)
#define   RPN_EXIT_INTERNAL_ERR    (126)

/* Return codes
   ~~~~~~~~~~~~ */
#define   RPN_RET_INVALID_ELT       (-2)
#define   RPN_RET_ERROR             (-1)
#define   RPN_RET_OK                 (0)
#define   RPN_RET_INVALID_STACK      (1)
#define   RPN_RET_INVALID_STR        (2)
#define   RPN_RET_INVALID_X          (3)
#define   RPN_RET_INVALID_Y          (4)
#define   RPN_RET_TOO_BIG            (5)
#define   RPN_RET_INVALID_TYPES      (6)
#define   RPN_RET_NOT_ENOUGH_ELTS    (7)
#define   RPN_RET_INCOMPATIBLE_ELTS  (8)

/* Arguments types
   ~~~~~~~~~~~~~~~ */
#define   RPN_TYPE_UNDEFINED         (0)
#define   RPN_TYPE_INT               (1)
#define   RPN_TYPE_FLOAT             (2)
#define   RPN_TYPE_STRING            (3)
#define   RPN_TYPE_VECTOR_3          (4)
#define   RPN_TYPE_MATRIX            (5)
#define   RPN_TYPE_LITTERAL          (6)

/* Operators names
   ~~~~~~~~~~~~~~~ */
#define   RPN_OP_NAME_UNDEF          "UNDEFINED OPERATOR NAME"
#define   RPN_OP_NAME_PLUS           "+"
#define   RPN_OP_NAME_MINUS          "-"
#define   RPN_OP_NAME_MULT           "*"
#define   RPN_OP_NAME_DIV            "/"
#define   RPN_OP_NAME_STR_LENGTH     "length"
#define   RPN_OP_NAME_STR_CATENATE   "cat"
#define   RPN_OP_NAME_STR_DUPL       "dupl"
#define   RPN_OP_NAME_MATRIX         "mat"
#define   RPN_OP_RDN                 "rdn"
#define   RPN_OP_RUP                 "rup"
#define   RPN_OP_LASTX               "lastx"
#define   RPN_OP_SWAP_XY             "x<>y"
#define   RPN_OP_SWAP_XZ             "x<>z"
#define   RPN_OP_SWAP_XT             "x<>t"
#define   RPN_OP_SWAP_XL             "x<>l"
#define   RPN_OP_CLX                 "clx"
#define   RPN_OP_CLST                "CLST"
#define   RPN_OP_ENTER               "enter"
#define   RPN_OP_RCL_X               "RCL_X"
#define   RPN_OP_PRe                 "PRX"
#define   RPN_OP_PRSTK               "PRSTK"
#define   RPN_OP_EXPLODE             "expl"
#define	RPN_OP_VEC2			  "vec2"
#define   RPN_OP_VEC3                "vec3"
#define   RPN_OP_TRANSPOSE           "t"
#define   RPN_OP_CROSS_PRODUCT       "^"
#define	RPN_OP_CATALOG			  "catalog"

/* Structures definitions
   ~~~~~~~~~~~~~~~~~~~~~~ */
struct global_struct {
     char                           *progname;
     rpn_stack                      *stack;
};

struct rpn_elt {
     uint32_t                        type;
     union {
          int                        i;
          double                     f;
          bool                       b;
          char                      *s;
          void                      *obj;
     }                               value;
     struct rpn_elt                 *previous_elt,
                                    *next_elt;
};

struct rpn_operator {
     char                           *op_name;
     uint32_t                        nb_args_in;
     uint32_t                        nb_args_out;
     int                            (*func)(rpn_stack *);
//   AJOUTER alias
};

struct rpn_stack {
     const char                     *name;
     uint32_t                        num_elts;
     struct rpn_elt                 *lastx;
	int						  clear_x;
     rpn_elt                        *base_elt,
                                    *top_elt;
//   AJOUTER name, print_mode
};

struct rpn_matrix {
     uint32_t                        n;
     uint32_t                        p;
     void                           *base[1];
};

/* External variables
   ~~~~~~~~~~~~~~~~~~ */
extern struct global_struct         G;
