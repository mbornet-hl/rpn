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
 *	rpn_header.h
 *
 *	@(#)	[MB] cy_rpn_header.h	Version 1.2.1.1 du 17/11/03 - 
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	"rpn_types.h"

/* Exit codes
   ~~~~~~~~~~ */
#define	RPN_EXIT_OK			  (0)
#define	RPN_EXIT_USAGE			  (1)
#define	RPN_EXIT_NO_MEM		  (2)
#define	RPN_EXIT_OP_UNKNOWN		  (3)
#define	RPN_EXIT_OP_ERROR		  (4)
#define	RPN_EXIT_INTERNAL_ERR	(126)

/* Return codes
   ~~~~~~~~~~~~ */
#define	RPN_RET_INVALID_ELT		 (-2)
#define	RPN_RET_ERROR			 (-1)
#define	RPN_RET_OK			  (0)
#define	RPN_RET_INVALID_STACK	  (1)

/* Arguments types
   ~~~~~~~~~~~~~~~ */
#define	RPN_TYPE_UNDEFINED		  (0)
#define	RPN_TYPE_INT			  (1)
#define	RPN_TYPE_FLOAT			  (2)

/* Operators names
   ~~~~~~~~~~~~~~~ */
#define	RPN_OP_NAME_UNDEF		  "UNDEFINED OPERATOR NAME"
#define	RPN_OP_NAME_PLUS		  "+"
#define	RPN_OP_NAME_MINUS		  "-"
#define	RPN_OP_NAME_MULT		  "*"
#define	RPN_OP_NAME_DIV		  "/"

/* Structures definitions
   ~~~~~~~~~~~~~~~~~~~~~~ */
struct global_struct {
	char						 *progname;
};

struct rpn_elt {
	uint32_t					  type;
	union {
		int					  i;
		float				  f;
		bool					  b;
		char					 *s;
	}						  value;
	struct rpn_elt				 *previous_elt,
							 *next_elt;
};

struct rpn_operator {
	char						 *op_name;
	uint32_t					  nb_args_in;
	uint32_t					  nb_args_out;
	int						 (*func)(rpn_stack *);
};

struct rpn_stack {
	uint32_t					  num_elts;
	rpn_elt					 *base_elt,
							 *top_elt;
};


/* Functions prototypes
   ~~~~~~~~~~~~~~~~~~~~ */
rpn_stack						 *rpn_new_stack(void);
rpn_elt						 *rpn_new_elt(void);

rpn_elt						 *rpn_pop(rpn_stack *);
int							  rpn_push(rpn_stack *, rpn_elt *);

int							  rpn_get_type(rpn_elt *);
int							  rpn_set_type(rpn_elt *, int);

rpn_operator					 *rpn_search_op(rpn_operator *, char *);

void							  rpn_err_msg_no_mem(void);
void							  rpn_err_msg_op_not_found(char *);

/* External variables
   ~~~~~~~~~~~~~~~~~~ */
extern struct global_struct		 G;
