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
 *   @(#)  [MB] cy_rpn_lex.l Version 1.73 du 19/10/25 - 
 */

#include	"../ci/ci_cpub.h"
#include  "../ci/ci_epub.h"
#include  "cy_rpn_header.h"
#include  "cy_rpn_proto.h"
#include	"cy_serial.h"

extern struct rpn_operator      rpn_operators[];

%}

comment             (#.*\n)
nil                 (nil)
integer             ([-+]?(0|[1-9][0-9]*))
float               ([-+]?(0|[1-9][0-9]*)\.([0-9]*)?)
string              (\"[^"]*\")
litteral            (\'[^']*\')
operator            ([-+*/]|m\*|mat|expl|vec2|vec3|^)
str_op              (cat|dupl|length)
cfg_op              (sw_(on|off))
stack_op            (x<>[yztl]|rdn|rup|lastx|enter|clx|del(x|_l)|rcl_x|clst|prx|prstk|stk|push|push_extract|pop|pop_insert|dupx|dump)
lists_op            (get_(head|tail)|get_(elt|ignored|used)|get|list)
tuples_op           (tuple|ntuple|Xtuple)
pairs_op            (opair)
math_op             (1\/x|y^x|sqrt|x^2|ln|exp|log10|10^x|%|delta%|(a)?(sin|cos|tan)|scalar_matrix|dim|chs|[hv](cat|rev)|zmat|vpad|mat_nil_diag)
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
adv                 (adv)
repeat              (\.)
exit				(exit|quit|bye)
Fkey_1_4            (\033O[P-S])
Fkey_5_8            (\033\[1[5789]~)
disp_mode           (fix|sci|eng)
test_op             (([lg][te]|ne|eq)z?)
version             (version)
import              (import[ \t]+([^ \t\n]+))
unknown_str		([a-z][a-zA-Z_0-9]*)

%%

{Fkey_1_4}     {
                    int                      _num;

                    _num                     = yytext[2] - 'P' + 1;

                    /* TODO : For future use
                       ~~~~~~~~~~~~~~~~~~~~~ */
                    RPN_TRACE_LEX("FUNCT KEY%02d\n", _num);
               }

{Fkey_5_8}      {
                    int                      _c, _num;

                    _c                       = yytext[3] - '0';

                    switch (_c) {
                         case 5 :
                              _num                = 5;
                              break;

                         case 7 :
                              _num                = 6;
                              break;

                         case 8 :
                              _num                = 7;
                              break;

                         case 9 :
                              _num                = 8;
                              break;

                         default:
                              _num                = 0;
                              break;
                    }

                    /* TODO : For future use
                       ~~~~~~~~~~~~~~~~~~~~~ */
                    RPN_TRACE_LEX("FUNCT KEY%02d\n", _num);
               }

{comment}      {
                    RPN_TRACE_LEX("========== %s ==========\n", yytext);
               }

{version}      {
                    fprintf(stderr, "RPN Version %s (link ID %d)\n", G.version, CY_LINK_ID);
               }

{adv}          {
                    // TODO : A TRAITER CORRECTEMENT => creer operateur
                    printf("\n");
               }

{catalog}      {
                    rpn_catalog(rpn_operators);
               }

{import}       {
                    char                *_p;

                    RPN_TRACE_LEX("yytext = [%s]\n", yytext);

                    /* Search for space or tab (skip "import" keyword)
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    for (_p = yytext; *_p != ' ' && *_p != '\t'; _p++);

                    /* Skip spaces and tabs (search for module name)
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    for ( ; *_p == ' ' || *_p == '\t'; _p++);

                    /* Import module
                       ~~~~~~~~~~~~~ */
                    G.err_no			= rpn_import(strdup(_p));
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
				G.show_prompt		= TRUE;
               }

{exit}		{
				yyterminate();
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
				G.show_prompt		= TRUE;
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
				G.show_prompt		= TRUE;
               }

{string}       {
                    rpn_elt                  *_elt_s;

                    /* Allocate element descriptors
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _elt_s              = rpn_new_elt(RPN_TYPE_STRING);
                    _elt_s->value.s     = strdup(yytext);

                    if (rpn_strip_quotes(&_elt_s->value.s) != RPN_RET_OK) {
                         rpn_err_msg_invalid_string(_elt_s->value.s);
                    }
                    else {
                         /* Push element on the stack
                            ~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         G.stack->new_value  = TRUE;
                         rpn_push(G.stack, _elt_s);
                    }
				G.show_prompt		= TRUE;
               }

{litteral}     {
                    rpn_elt                  *_elt_s;
				rpn_litteral			*_litteral;

                    /* Allocate element descriptors
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				_litteral				= rpn_new_litteral();
				_litteral->value		= strdup(yytext);

                    _elt_s				= rpn_new_elt(RPN_TYPE_LITTERAL);
				_elt_s->value.obj		= _litteral;

                    if (rpn_strip_single_quotes(&_litteral->value) != RPN_RET_OK) {
                         rpn_err_msg_invalid_string(_litteral->value);
                    }
                    else {
                         /* Push element on the stack
                            ~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         G.stack->new_value  = TRUE;
                         rpn_push(G.stack, _elt_s);
                    }
				G.show_prompt		= TRUE;
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
{type_op}      |
{unknown_str}	{
                    char                     *_op_name;
                    dl_operator              *_op;
                    int                       _retcode;

				RPN_TRACE_LEX("Searching for operator [%s]\n", strdup(yytext));

                    /* Initialize operator name
                       ~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _op_name				= strdup(yytext);

                    RPN_TRACE_LEX("operator = [%s]\n", _op_name);

                    if ((_op = rpn_search_op(G.stack, _op_name)) != 0) {
                         /* Execute operator
                            ~~~~~~~~~~~~~~~~ */
                         if ((_retcode = rpn_op(G.stack, _op)) != RPN_RET_OK) {
                              rpn_err_msg_op_error(_op->op_name, _retcode);
                         }
                    }
               }

{repeat}       {
                    char                     *_op_name;
                    int                       _retcode;

                    /* Initialize operator name
                       ~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _op_name				= strdup(yytext);

                    RPN_TRACE_LEX("operator = [%s]\n", _op_name);

                    if (G.stack->last_op) {
                         /* Execute previous operator
                            ~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         if ((_retcode = rpn_op(G.stack, G.stack->last_op)) != RPN_RET_OK) {
                              rpn_err_msg_op_error(G.stack->last_op->op_name, _retcode);
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
				G.show_prompt		= TRUE;
               }

\n             {
				rpn_disp_prompt();
               }

<<EOF>>		{
				yyterminate();
			}

%%

/******************************************************************************

                              MAIN

******************************************************************************/
int main(int argc, char *argv[])
{
	char				*_optstrg = "CdhlnsvyLYx";
	int				 _opt, _no_rc = FALSE, _show_exit_mesg = FALSE;
	char				*_rc_file = ".rpn.rc";
	FILE				*_fp_rc;
	char				*_var_libpath = RPN_ENV_LIBPATH,
					*_deflt_libpath = RPN_DEFLT_LIBPATH;

     G.progname          = argv[0];
     G.prompt            = "RPN> ";
     G.version           = RPN_VERSION;
	G.show_prompt		= TRUE;

	while ((_opt = getopt(argc, argv, _optstrg)) != -1) {
		switch (_opt) {

		case	'C':
			G.debug_level		|= RPN_DBG_DISP_CMDS;
			break;

		case	'd':
			G.debug_level		|= RPN_DBG_DEBUG;
			break;

		case	'h':
			cy_usage(_optstrg);
			break;

		case	'l':
			G.debug_level		|= RPN_DBG_LEX;
			break;

		case	'n':
			_no_rc			= TRUE;
			break;

		case 's':
			G.silent			= TRUE;
			break;

		case	'v':
			G.debug_level		|= RPN_DBG_VERBOSE;
			break;

		case	'y':
			G.debug_level		|= RPN_DBG_YACC;
			break;

		case	'L':
			G.debug_level		|= RPN_DBG_LEX_2;
			break;

		case	'Y':
			G.debug_level		|= RPN_DBG_YACC_2;
			break;

		case	'x':
			_show_exit_mesg	= TRUE;
			break;

		default:
			fprintf(stderr, "Usage: %s [-%s]\n", G.progname, _optstrg);
			exit(RPN_EXIT_USAGE);
		}
	}

     /* Allocate stack descriptor
        ~~~~~~~~~~~~~~~~~~~~~~~~~ */
     G.stack             = rpn_new_stack("MAIN");
     G.sigma             = rpn_new_sigma("default");

	/* Search for the modules location
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if ((G.libpath = getenv(_var_libpath)) == 0) {
		RPN_TRACE_VERBOSE("Environment variable \"%s\" is not initialized : using \"%s\".\n",
				  _var_libpath, _deflt_libpath);
		G.libpath			= _deflt_libpath;
	}
	else {
		RPN_TRACE_VERBOSE("Environment variable \"%s\" =  \"%s\".\n",
			  _var_libpath, G.libpath);
	}

     /* Initialize the root of the modules tree
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     ci_init_root(&G.modules_tree);

     /* Initialize the handling of the SIGSEGV signal
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_init_backtrace();

	rpn_disp_prompt();

	if (!_no_rc) {
		/* Search for a readable commands file (.rpn.rc)
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		if (access(_rc_file, 0) == 0) {
			if ((_fp_rc = fopen(_rc_file, "r")) == 0) {
				fprintf(stderr, "%s: cannot open \"%s\" !\n", G.progname, _rc_file);
			}
			else {
				yyrestart(_fp_rc);
				yylex();
			}
		}
	}

	if (G.silent) {
		/* Reset debug level
		   ~~~~~~~~~~~~~~~~~ */
		G.debug_level = 0;
	}

	yyrestart(stdin);
     yylex();

     rpn_free_stack(G.stack);

	if (_show_exit_mesg) {
		printf("RPN : exiting.\n");
	}

     return 0;
}

/******************************************************************************

                              REMOVE_WARN_UNUSED

******************************************************************************/
static void remove_warn_unused(int always_false)
{
     if (always_false == TRUE) {
          yyunput(0, 0);
          input();
     }
}

/******************************************************************************

                              YYERROR

******************************************************************************/
void yyerror(char *errmsg)
{
     fprintf(stderr, "%s: Parser error: %s\n", G.progname, errmsg);
     remove_warn_unused(FALSE);
     exit(RPN_EXIT_SYNTAX);
}

/******************************************************************************

                              YYWRAP

******************************************************************************/
int yywrap()
{
     return 1;
}

/******************************************************************************

						CY_USAGE

******************************************************************************/
void cy_usage(char *optstrg)
{
	char				*_var_libpath = RPN_ENV_LIBPATH,
					*_deflt_libpath = RPN_DEFLT_LIBPATH;

	fprintf(stderr, "Usage: %s [-%s]\n", G.progname, optstrg);
	fprintf(stderr, "  -C : display commands\n");
	fprintf(stderr, "  -d : activate debug\n");
	fprintf(stderr, "  -h : help\n");
	fprintf(stderr, "  -l : display lex debug (parser 1)\n");
	fprintf(stderr, "  -n : do not read the rc file\n");
	fprintf(stderr, "  -s : silent\n");
	fprintf(stderr, "  -v : verbose\n");
	fprintf(stderr, "  -y : display yacc debug (parser 1)\n");
	fprintf(stderr, "  -L : display lex debug (parser 2)\n");
	fprintf(stderr, "  -Y : display yacc debug (parser 2)\n");

	/* Search for the modules location
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if ((G.libpath = getenv(_var_libpath)) == 0) {
		fprintf(stderr, "Environment variable \"%s\" is not initialized : using \"%s\".\n",
				  _var_libpath, _deflt_libpath);
		G.libpath			= _deflt_libpath;
	}
	else {
		fprintf(stderr, "Environment variable \"%s\" =  \"%s\".\n",
			  _var_libpath, G.libpath);
	}

	exit(RPN_EXIT_USAGE);
}
