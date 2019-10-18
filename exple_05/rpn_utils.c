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
 *   rpn_utils.c
 *
 *   @(#)  [MB] cy_rpn_utils.c Version 1.8 du 17/11/05 - 
 */

#include  "rpn_header.h"

struct global_struct     G         = { 0 };

/******************************************************************************

                         RPN_NEW_STACK

******************************************************************************/
rpn_stack *rpn_new_stack()
{
     rpn_stack           *_stack;

     if ((_stack = (rpn_stack *) malloc(sizeof(*_stack))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _stack->num_elts    = 0;
     _stack->base_elt    = 0;
     _stack->top_elt     = 0;

     return _stack;
}

/******************************************************************************

                         RPN_NEW_ELT

******************************************************************************/
rpn_elt *rpn_new_elt()
{
     rpn_elt             *_elt;

     if ((_elt = (rpn_elt *) malloc(sizeof(*_elt))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _elt->type          = RPN_TYPE_UNDEFINED;
     _elt->value.i       = 0;
     _elt->previous_elt  = NULL;
     _elt->next_elt      = NULL;

     return _elt;
}

/******************************************************************************

                         RPN_POP

******************************************************************************/
rpn_elt *rpn_pop(rpn_stack *stack)
{
     rpn_elt        *_elt     = 0;

     if ((stack != 0) && (stack->num_elts != 0)) {
          _elt                     = stack->top_elt;
          stack->top_elt           = _elt->previous_elt;
          if (stack->top_elt != 0) {
               stack->top_elt->next_elt = 0;
          }
          else {
               stack->base_elt     = 0;
          }

          _elt->previous_elt       = 0;
          _elt->next_elt           = 0;

          stack->num_elts--;
     }
     else {
          _elt                     = rpn_new_elt();
          rpn_set_type(_elt, RPN_TYPE_INT);
     }

     return _elt;
}

/******************************************************************************

                         RPN_PUSH

******************************************************************************/
int rpn_push(rpn_stack *stack, rpn_elt *elt)
{
     int             _retcode;

     if (stack != 0) {
          if (stack->base_elt == 0) {
               stack->base_elt          = elt;
               stack->top_elt           = elt;

               elt->previous_elt        = 0;
               elt->next_elt            = 0;
          }
          else {
               stack->top_elt->next_elt = elt;

               elt->previous_elt        = stack->top_elt;
               elt->next_elt            = 0;

               stack->top_elt           = elt;
          }
          stack->num_elts++;
          _retcode                 = RPN_RET_OK;
     }
     else {
          _retcode                 = RPN_RET_INVALID_STACK;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_GET_TYPE

******************************************************************************/
int rpn_get_type(rpn_elt *elt)
{
     int        _retcode;

     if (elt) {
          _retcode  = elt->type;
     }
     else {
          _retcode  = RPN_RET_INVALID_ELT;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_SET_TYPE

******************************************************************************/
int rpn_set_type(rpn_elt *elt, int type)
{
     int             _retcode;

     if (elt) {
          elt->type      = type;
          _retcode       = RPN_RET_OK;
     }
     else {
          _retcode       = RPN_RET_INVALID_ELT;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_SEARCH_OP

******************************************************************************/
rpn_operator *rpn_search_op(rpn_operator *operators, char *operator)
{
     rpn_operator        *_op;

     for (_op = operators; _op->op_name != 0; _op++) {
          if (!strcmp(_op->op_name, operator)) {
               /* Operator found
                  ~~~~~~~~~~~~~~ */
               break;
          }
     }

     if (_op->op_name == 0) {
          _op       = 0;
     }

     return _op;
}

/******************************************************************************

                         RPN_ERR_MSG_NO_MEM

******************************************************************************/
void rpn_err_msg_no_mem()
{
     fprintf(stderr, "%s: not enough memory !\n", G.progname);
}

/******************************************************************************

                         RPN_ERR_MSG_OP_NOT_FOUND

******************************************************************************/
void rpn_err_msg_op_not_found(char *operator)
{
     fprintf(stderr, "%s: operator \"%s\" not found !\n", G.progname, operator);
}

/******************************************************************************

                         RPN_ERR_MSG_OP_ERROR

******************************************************************************/
void rpn_err_msg_op_error(char *operator, int retcode)
{
     fprintf(stderr, "%s: operator \"%s\" returned %d !\n",
             G.progname, operator, retcode);
}


/******************************************************************************

                         RPN_ERR_MSG_INVALID_STRING

******************************************************************************/
void rpn_err_msg_invalid_string(char *str)
{
     fprintf(stderr, "%s: invalid string (%s)\n", G.progname, str);
}

/******************************************************************************

                         RPN_STRIP_QUOTES

******************************************************************************/
int rpn_strip_quotes(char **str)
{
     int                 _lg, _retcode;
     char                *_new_str;

     _lg            = strlen(*str);
     if ((*str)[0] != '"' || (*str)[_lg - 1] != '"') {
          _retcode       = RPN_RET_INVALID_STR;
     }
     else {
          _new_str            = strdup(*str);
          _new_str[_lg - 1]   = 0;
          _new_str++;

          *str                = _new_str;

          _retcode            = RPN_RET_OK;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_STRIP_SINGLEQUOTES

******************************************************************************/
int rpn_strip_single_quotes(char **str)
{
     int                 _lg, _retcode;
     char                *_new_str;

     _lg            = strlen(*str);
     if ((*str)[0] != '\'' || (*str)[_lg - 1] != '\'') {
          _retcode       = RPN_RET_INVALID_STR;
     }
     else {
          _new_str            = strdup(*str);
          _new_str[_lg - 1]   = 0;
          _new_str++;

          *str                = _new_str;

          _retcode            = RPN_RET_OK;
     }

     return _retcode;
}
