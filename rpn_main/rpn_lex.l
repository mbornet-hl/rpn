%{
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
 *   @(#)  [MB] cy_rpn_lex.l Version 1.48 du 19/06/05 - 
 */

#include "rpn_header.h"
#include "rpn_proto.h"

extern struct rpn_operator      rpn_operators[];

%}

comment             (#.*)
nil                 (nil)
integer             ([-+]?(0|[1-9][0-9]*))
float               ([-+]?(0|[1-9][0-9]*)\.([0-9]*)?)
string              (\"[^"]*\")
litteral            (\'[^']*\')
operator            ([-+*/]|m\*|mat|expl|vec2|vec3|t|^)
str_op              (cat|dupl|length)
cfg_op              (sw_(on|off))
stack_op            (x<>[yztl]|rdn|rup|lastx|enter|clx|del(x|_l)|rcl_x|clst|prx|prstk|stk|push|push_extract|pop|pop_insert|dupx|dump)
lists_op            (get_(head|queue)|get_(elt|ignored|used)|get|list)
tuples_op           (tuple|ntuple|Xtuple)
pairs_op            (opair)
math_op             (1\/x|y^x|sqrt|x^2|ln|exp|log10|10^x|%|delta%|(a)?(sin|cos|tan)|scalar_matrix|seq|dim|chs|[hv](cat|rev)|zmat|vpad|mat_nil_diag)
hyper_op            ((a)?(sinh|cosh|tanh)|sigmoid)
stats_op            (sigma\+|sigma-|mean|sdev|meanx|mmm|(line|exp|log|pow)_set)
neural_op           (create_mlp|learn|analyze|(set|alea)_(mlp|layer)_weights|read_mnist|mnist_pics?|read_elt|mat7seg)
neural_op2          (label|index|orig_index|dispatch|ignore|use)
backup_op           (save|load|restore|autosave|no_autosave|write(_csv)?)
computer            (gen_C)
system              (mem|pause|ping|iperf)
type_op             (filename)
start_mark          (BEGIN)
spaces              ([   ]+)
swap_xy             (x<>y)
swap_xz             (x<>z)
swap_xt             (x<>t)
swap_xl             (x<>l)
roll_down           (rdn)
roll_up             (rup)
lastx               (lastx)
catalog             (catalog)
todo                (todo)
adv                 (adv)
repeat              (\.)
Fkey                (\033O[P-S])
disp_mode           (fix|sci|eng)
test_op             (([lg][te]|ne|eq)z?)
version             (version)

%%

{Fkey}         {
                    int                      _num;

                    _num                     = yytext[2] - 'P' + 1;

                    /* TODO : For future use
                       ~~~~~~~~~~~~~~~~~~~~~ */
                    printf("FUNCT KEY%02d\n", _num);
               }

{comment}      {
                    fprintf(stderr, "========== %s ==========\n", yytext);
               }

{adv}          {
                    // TODO : A TRAITER CORRECTEMENT => creer operateur
                    printf("\n");
               }

{catalog}      {
                    rpn_catalog(rpn_operators);
               }

{todo}         {
                    rpn_todo(rpn_operators);
               }

{nil}          {
                    rpn_elt                  *_elt_nil;

                    /* Allocate element descriptors
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _elt_nil            = rpn_new_elt(RPN_TYPE_NIL);

                    /* Push element on the stack
                       ~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    G.stack->new_value  = TRUE;
                    rpn_push(G.stack, _elt_nil);
               }

{integer}      {
                    rpn_elt                  *_elt_i;

                    /* Allocate element descriptors
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _elt_i              = rpn_new_elt(RPN_TYPE_INT);
                    _elt_i->value.i     = atoi(yytext);

                    /* Push element on the stack
                       ~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    G.stack->new_value  = TRUE;
                    rpn_push(G.stack, _elt_i);
               }

{float}        {
                    rpn_elt                  *_elt_f;

                    /* Allocate element descriptors
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _elt_f              = rpn_new_elt(RPN_TYPE_DOUBLE);
                    _elt_f->value.d     = atof(yytext);

                    /* Push element on the stack
                       ~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    G.stack->new_value  = TRUE;
                    rpn_push(G.stack, _elt_f);
               }

{string}       {
                    rpn_elt                  *_elt_s;

                    /* Allocate element descriptors
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _elt_s              = rpn_new_elt(RPN_TYPE_STRING);
                    _elt_s->value.s     = strdup(yytext);

                    if (rpn_strip_quotes(&_elt_s->value.s) != RPN_RET_OK) {
                         rpn_err_msg_invalid_string(_elt_s->value.s);
//                       exit(RPN_EXIT_INVALID_ELT);
                    }
                    else {
                         /* Push element on the stack
                            ~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         G.stack->new_value  = TRUE;
                         rpn_push(G.stack, _elt_s);
                    }
               }

{litteral}     {
                    rpn_elt                  *_elt_s;

                    /* Allocate element descriptors
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _elt_s              = rpn_new_elt(RPN_TYPE_LITTERAL);
                    _elt_s->value.s     = strdup(yytext);

                    if (rpn_strip_single_quotes(&_elt_s->value.s) != RPN_RET_OK) {
                         rpn_err_msg_invalid_string(_elt_s->value.s);
//                       exit(RPN_EXIT_INVALID_ELT);
                    }
                    else {
                         /* Push element on the stack
                            ~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         G.stack->new_value  = TRUE;
                         rpn_push(G.stack, _elt_s);
                    }
               }

{operator}     |
{str_op}       |
{cfg_op}       |
{stack_op}     |
{lists_op}     |
{tuples_op}    |
{pairs_op}     |
{test_op}      |
{math_op}      |
{hyper_op}     |
{stats_op}     |
{neural_op}    |
{neural_op2}   |
{backup_op}    |
{computer}     |
{system}       |
{type_op}      {
                    char                     *_operator;
                    rpn_operator             *_op;
                    int                       _retcode;

                    /* Initialize operator
                       ~~~~~~~~~~~~~~~~~~~ */
                    _operator      = yytext;

                    if (G.debug_level & RPN_DBG_LEX) {
                         printf("yytext = [%s]\n", yytext);
                    }

                    if ((_op = rpn_search_op(rpn_operators, _operator)) == 0) {
                         rpn_err_msg_op_not_found(_operator);
                         fprintf(stderr, "%s: operator \"%s\" unknown !\n", G.progname, _operator);
//                       exit(RPN_EXIT_OP_UNKNOWN);
                    }
                    else {
                         /* Execute operator
                            ~~~~~~~~~~~~~~~~ */
                         if ((_retcode = rpn_op(G.stack, _op)) != RPN_RET_OK) {
                              rpn_err_msg_op_error(_op->op_name, _retcode);
//                       exit(RPN_EXIT_OP_ERROR);
                         }
                    }

//                  printf("%s", G.prompt);

               }

{repeat}       {
                    int                       _retcode;

                    if (G.stack->last_op) {
                         /* Execute previous operator
                            ~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         if ((_retcode = rpn_op(G.stack, G.stack->last_op)) != RPN_RET_OK) {
                              rpn_err_msg_op_error(G.stack->last_op->op_name, _retcode);
//                            exit(RPN_EXIT_OP_ERROR);
                         }
                    }
               }

{spaces}       ;

.              {
                    printf("[%s] ", yytext);
               }

{start_mark}   {
                    rpn_inc_begin(G.stack);
                    rpn_elt                  *_elt;

                    /* Allocate element descriptors
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _elt                = rpn_new_elt(RPN_TYPE_START_MARKER);

                    /* Push element on the stack
                       ~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    G.stack->new_value  = TRUE;
                    rpn_push(G.stack, _elt);
               }

{version}      {
                    fprintf(stderr, "RPN Version %s\n", G.version);
               }

\n             {
                    printf("%s", G.prompt);
               }

%%

/******************************************************************************

                              MAIN

******************************************************************************/
int main(int argc, char *argv[])
{
     G.progname          = argv[0];
     G.prompt            = "RPN> ";
     G.version           = RPN_VERSION;

     /* Allocate stack descriptor
        ~~~~~~~~~~~~~~~~~~~~~~~~~ */
     G.stack             = rpn_new_stack("MAIN");
     G.sigma             = rpn_new_sigma("default");

     /* Initialize debug level
        ~~~~~~~~~~~~~~~~~~~~~~ */
     G.iperf.debug_level = RPN_DBG_YACC;
     G.iperf.debug_level = RPN_DBG_LEX | RPN_DBG_YACC;
     G.iperf.debug_level = 0;

     /* Initialize the handling of the SIGSEGV signal
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_init_backtrace();

     /* Display stack
        ~~~~~~~~~~~~~ */
     rpn_disp_stack(G.stack);

#define   CHECK
#if defined(CHECK)
     rpn_check_ops();
#endif    // CHECK

#if 0
     {
          /* Tests to compute allocated memory
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          char      *_mem;

          _mem      = rpn_malloc(12345);
          rpn_free(_mem);
          _mem      = rpn_malloc(32768);
          rpn_free(_mem);
          _mem      = rpn_malloc(65536);
          rpn_free(_mem);
     }
#endif

     printf("%s", G.prompt);
     yylex();

     rpn_free_stack(G.stack);

     return 0;
}

/******************************************************************************

                              YYERROR

******************************************************************************/
void yyerror(char *errmsg)
{
     fprintf(stderr, "%s: Parser error: %s\n", G.progname, errmsg);
     exit(RPN_EXIT_SYNTAX);
}

/******************************************************************************

                              YYWRAP

******************************************************************************/
int yywrap()
{
     return 1;
}

