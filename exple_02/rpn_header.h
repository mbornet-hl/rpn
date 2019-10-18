/*
 *	rpn_header.h
 *
 *	@(#)	[MB] cy_rpn_header.h	Version 1.3 du 17/09/28 - 
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	"rpn_types.h"

#define   X    fprintf(stderr, "%s (%d)\n", __FILE__, __LINE__);

/* Exit codes
   ~~~~~~~~~~ */
#define	RPN_EXIT_OK			  (0)
#define	RPN_EXIT_USAGE			  (1)
#define	RPN_EXIT_NO_MEM		  (2)
#define	RPN_EXIT_OP_UNKNOWN		  (3)
#define	RPN_EXIT_OP_ERROR		  (4)
#define	RPN_EXIT_INVALID_ELT	  (5)
#define	RPN_EXIT_INTERNAL_ERR	(126)

/* Return codes
   ~~~~~~~~~~~~ */
#define	RPN_RET_INVALID_ELT		 (-2)
#define	RPN_RET_ERROR			 (-1)
#define	RPN_RET_OK			  (0)
#define	RPN_RET_INVALID_STACK	  (1)
#define	RPN_RET_INVALID_STR		  (2)
#define	RPN_RET_INVALID_X		  (3)
#define	RPN_RET_INVALID_Y		  (4)
#define	RPN_RET_TOO_BIG		  (5)

/* Arguments types
   ~~~~~~~~~~~~~~~ */
#define	RPN_TYPE_UNDEFINED		  (0)
#define	RPN_TYPE_INT			  (1)
#define	RPN_TYPE_FLOAT			  (2)
#define	RPN_TYPE_STRING		  (3)

/* Operators names
   ~~~~~~~~~~~~~~~ */
#define	RPN_OP_NAME_UNDEF		  "UNDEFINED OPERATOR NAME"
#define	RPN_OP_NAME_PLUS		  "+"
#define	RPN_OP_NAME_MINUS		  "-"
#define	RPN_OP_NAME_MULT		  "*"
#define	RPN_OP_NAME_DIV		  "/"
#define	RPN_OP_NAME_STR_LENGTH	  "length"
#define	RPN_OP_NAME_STR_CATENATE	  "cat"
#define	RPN_OP_NAME_STR_DUPL	  "dupl"

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
void							  rpn_err_msg_op_error(char *, int);
void							  rpn_err_msg_invalid_string(char *);


/* External variables
   ~~~~~~~~~~~~~~~~~~ */
extern struct global_struct		 G;
