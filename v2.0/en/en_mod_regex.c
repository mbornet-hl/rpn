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
 *   @(#)  [MB] en_mod_regex.c Version 1.6 du 21/08/30 - 
 */

#include  <stdio.h>
#include  <stdarg.h>

#include  "../cc/cc_types.h"
#include  "../cy/cy_rpn_header.h"
#include  "../cy/cy_rpn_proto.h"
#include  "../dt/dt_epub.h"
#include  "en_cpri.h"
#include  "en_serial.h"

/* REGEX
   ~~~~~ */
RPN_DECL_OP(en_op_regex_regex);
RPN_DECL_OP(en_op_regex_string);
RPN_DECL_OP(en_op_regex_icase);
RPN_DECL_OP(en_op_regex_ere);
RPN_DECL_OP(en_op_regex_nosub);
RPN_DECL_OP(en_op_regex_newline);
RPN_DECL_OP(en_op_regex_notbol);
RPN_DECL_OP(en_op_regex_noteol);
RPN_DECL_OP(en_op_regex_match);

static dl_op_desc                   en_ops_array[];

/* Help messages {{{ */
// regex {{{
char                               *en_help_regex_display[] = {
     "Display global flags",
     0
};

char                               *en_help_regex[] = {
     "Convert string to regular expression",
     0
};

char                               *en_help_regex_regex[] = {
     "Recompile regex using global flags",
     0
};
// regex }}}
// string {{{
char                               *en_help_string[] = {
     "Convert regular expression to string",
     0
};
// string }}}
// icase {{{
char                               *en_help_icase_get_global[] = {
     "Returns icase global flag",
     0
};

char                               *en_help_icase_nil[] = {
     "Initialize icase global flag",
     0
};

char                               *en_help_icase[] = {
     "Recompile regex with modified icase flag",
     0
};

char                               *en_help_icase_global[] = {
     "Recompile regex using icase global flag",
     0
};
// icase }}}
// ere {{{
char                               *en_help_ere_get_global[] = {
     "Returns extended regex global flag",
     0
};

char                               *en_help_ere_nil[] = {
     "Initialize extended regex global flag",
     0
};

char                               *en_help_ere[] = {
     "Recompile regex with modified extended regex flag",
     0
};

char                               *en_help_ere_global[] = {
     "Recompile regex using extended regex global flag",
     0
};
// ere }}}
// nosub {{{
char                               *en_help_nosub_get_global[] = {
     "Returns nosub global flag",
     0
};

char                               *en_help_nosub_nil[] = {
     "Initialize nosub global flag",
     0
};

char                               *en_help_nosub[] = {
     "Recompile regex with modified nosub flag",
     0
};

char                               *en_help_nosub_global[] = {
     "Recompile regex using nosub global flag",
     0
};
// nosub }}}
// newline {{{
char                               *en_help_newline_get_global[] = {
     "Returns newline global flag",
     0
};

char                               *en_help_newline_nil[] = {
     "Initialize newline global flag",
     0
};

char                               *en_help_newline[] = {
     "Recompile regex with modified newline flag",
     0
};

char                               *en_help_newline_global[] = {
     "Recompile regex using newline global flag",
     0
};
// newline }}}
// match {{{
char                               *en_help_match_int[] = {
     "Select int matching regex (NIL when not matching)",
     0
};

char                               *en_help_match_string[] = {
     "Select string matching regex (NIL when not matching)",
     0
};

char                               *en_help_match_list[] = {
     "Select elements from list matching regex",
     0
};

char                               *en_help_match_text[] = {
     "Select elements from text matching regex",
     0
};
// match }}}
// notbol {{{
char                               *en_help_notbol_get_global[] = {
     "Returns notbol global flag",
     0
};

char                               *en_help_notbol_nil[] = {
     "Initialize notbol global flag",
     0
};

// notbol }}}
// noeeol {{{
char                               *en_help_noteol_get_global[] = {
     "Returns noteol global flag",
     0
};

char                               *en_help_noteol_nil[] = {
     "Initialize noteol global flag",
     0
};

// noteol }}}
/* Help messages }}} */
/* Module descriptor {{{ */
struct dl_module         regex_module = {
     "regex",
     "2.0",
     EN_LINK_ID,
     0, //"en_operators",
     en_ops_array,
     0
};

/* Module descriptor }}} */
/* Operator parameters descriptions {{{ */
static dl_op_params                      en_params_regex[] = {
     DL_OP_DEF1H(en_op_regex_regex, 1, NIL,    en_help_regex_display),
     DL_OP_DEF1H(en_op_regex_regex, 1, STRING, en_help_regex),
     DL_OP_DEF1H(en_op_regex_regex, 1, REGEX,  en_help_regex_regex),
     DL_OP_DEF_END
};

static dl_op_params                      en_params_string[] = {
     DL_OP_DEF1H(en_op_regex_string, 1, REGEX, en_help_string),
     DL_OP_DEF_END
};

static dl_op_params                      en_params_icase[] = {
     DL_OP_DEF1H(en_op_regex_icase, 1, NIL,        en_help_icase_get_global),
     DL_OP_DEF2H(en_op_regex_icase, 0, INT, NIL,   en_help_icase_nil),
     DL_OP_DEF2H(en_op_regex_icase, 1, INT, REGEX, en_help_icase),
     DL_OP_DEF1H(en_op_regex_icase, 1, REGEX,      en_help_icase_global),
     DL_OP_DEF_END
};

static dl_op_params                      en_params_extended_re[] = {
     DL_OP_DEF1H(en_op_regex_ere, 1, NIL,          en_help_ere_get_global),
     DL_OP_DEF2H(en_op_regex_ere, 1, INT, NIL,     en_help_ere_nil),
     DL_OP_DEF2H(en_op_regex_ere, 1, INT, REGEX,   en_help_ere),
     DL_OP_DEF1H(en_op_regex_ere, 1, REGEX,        en_help_ere_global),
     DL_OP_DEF_END
};

static dl_op_params                      en_params_nosub[] = {
     DL_OP_DEF1H(en_op_regex_nosub, 1, NIL,        en_help_nosub_get_global),
     DL_OP_DEF2H(en_op_regex_nosub, 1, INT, NIL,   en_help_nosub_nil),
     DL_OP_DEF2H(en_op_regex_nosub, 1, INT, REGEX, en_help_nosub),
     DL_OP_DEF1H(en_op_regex_nosub, 1, REGEX,      en_help_nosub_global),
     DL_OP_DEF_END
};

static dl_op_params                      en_params_newline[] = {
     DL_OP_DEF1H(en_op_regex_newline, 1, NIL,        en_help_newline_get_global),
     DL_OP_DEF2H(en_op_regex_newline, 1, INT, NIL,   en_help_newline_nil),
     DL_OP_DEF2H(en_op_regex_newline, 1, INT, REGEX, en_help_newline),
     DL_OP_DEF1H(en_op_regex_newline, 1, REGEX,      en_help_newline_global),
     DL_OP_DEF_END
};

static dl_op_params                      en_params_notbol[] = {
     DL_OP_DEF1H(en_op_regex_notbol, 1, NIL,        en_help_notbol_get_global),
     DL_OP_DEF1H(en_op_regex_notbol, 0, INT,        en_help_notbol_nil),
     DL_OP_DEF_END
};

static dl_op_params                      en_params_noteol[] = {
     DL_OP_DEF1H(en_op_regex_noteol, 1, NIL,        en_help_noteol_get_global),
     DL_OP_DEF1H(en_op_regex_noteol, 0, INT,        en_help_noteol_nil),
     DL_OP_DEF_END
};

static dl_op_params                      en_params_match[] = {
     DL_OP_DEF2H(en_op_regex_match, 1, REGEX, INT,    en_help_match_int),
     DL_OP_DEF2H(en_op_regex_match, 1, REGEX, STRING, en_help_match_string),
     DL_OP_DEF2H(en_op_regex_match, 1, REGEX, LIST,   en_help_match_list),
     DL_OP_DEF2H(en_op_regex_match, 1, REGEX, TEXT,   en_help_match_text),
     DL_OP_DEF_END
};

/* Operator parameters descriptions }}} */
/* Operators list {{{ */
static dl_op_desc                        en_ops_array[] = {
     {    "regex",                 en_params_regex                         },
     {    "string",                en_params_string                        },
     {    "ere",                   en_params_extended_re                   },
     {    "icase",                 en_params_icase                         },
     {    "nosub",                 en_params_nosub                         },
     {    "newline",               en_params_newline                       },
     {    "match",                 en_params_match                         },
     {    "notbol",                en_params_notbol                        },
     {    "noteol",                en_params_noteol                        },
     {    0,                       0                                       }
};

/* Operators list }}} */

// GROUP : Utils {{{
// en_display_flags() {{{

/******************************************************************************

                         EN_DISPLAY_FLAGS

******************************************************************************/
void en_display_flags()
{
     char                      _str_cflags[64], _str_eflags[64];
     int                       _first_flag;

     _str_cflags[0]           = 0;
     _str_eflags[0]           = 0;

	/* cflags
	   ~~~~~~ */
	_first_flag			= TRUE;
     if ((G.cflags & REG_ICASE) == REG_ICASE) {
          strcpy(_str_cflags, "icase");
          _first_flag              = FALSE;
     }

     if ((G.cflags & REG_EXTENDED) == REG_EXTENDED) {
          if (!_first_flag) {
               strcat(_str_cflags, ", ");
          }
          strcat(_str_cflags, "extended");
          _first_flag              = FALSE;
     }

     if ((G.cflags & REG_NOSUB) == REG_NOSUB) {
          if (!_first_flag) {
               strcat(_str_cflags, ", ");
          }
          strcat(_str_cflags, "nosub");
          _first_flag              = FALSE;
     }

     if ((G.cflags & REG_NEWLINE) == REG_NEWLINE) {
          if (!_first_flag) {
               strcat(_str_cflags, ", ");
          }
          strcat(_str_cflags, "newline");
          _first_flag              = FALSE;
     }

	/* eflags
	   ~~~~~~ */
	_first_flag			= TRUE;
     if ((G.eflags & REG_NOTBOL) == REG_NOTBOL) {
          strcpy(_str_eflags, "notbol");
          _first_flag              = FALSE;
     }

     if ((G.eflags & REG_NOTEOL) == REG_NOTEOL) {
          if (!_first_flag) {
               strcat(_str_eflags, ", ");
          }
          strcat(_str_eflags, "noteol");
          _first_flag              = FALSE;
     }

     printf("===> [%s] [%s]", _str_cflags, _str_eflags);
     if (G.silent) {
          printf("\n");
     }
}

// en_display_flags() }}}
// GROUP : Utils }}}
// GROUP : Regex {{{
/* en_op_regex_regex() {{{ */

/******************************************************************************

                         EN_OP_REGEX_REGEX

******************************************************************************/
RPN_DEF_OP(en_op_regex_regex)
{
     rpn_elt                  *_stk_x, *_stk_result = NULL;
     int                       _X_type, _error;
     int                       _retcode;
     char                      _errbuf[256];
     rpn_regex                *_regex;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_NIL:
// {{{
          en_display_flags();
          break;
// }}}
     case RPN_TYPE_STRING:
// {{{
          {
               _stk_result              = rpn_new_elt(RPN_TYPE_REGEX);
//             _stk_result->name        = strdup(__func__);

               /* Initialize regex descriptor
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _regex                   = rpn_new_regex();
               _regex->expr             = strdup(_stk_x->value.s);
               _regex->cflags           = G.cflags;
               _stk_result->value.obj   = _regex;

               if ((_error = regcomp(&_regex->RE, _regex->expr, _regex->cflags)) != 0) {
                    (void) regerror(_error, &_regex->RE, _errbuf, sizeof(_errbuf));
                    fprintf(stderr, "regcomp error for \"%s\" : %s\n",
                            _regex->expr, _errbuf);
                    exit(RPN_EXIT_REGCOMP_ERROR);
               }
          }
          break;
// }}}
     case RPN_TYPE_REGEX:
// {{{
          {
               rpn_regex                *_new_regex;

               /* Initialize regex descriptor
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _regex                   = _stk_x->value.obj;

               if ((_regex->cflags & EN_REG_FLAGS) != (G.cflags & EN_REG_FLAGS)) {
                    /* Regex must be recompiled with global flags
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _stk_result              = rpn_new_elt(RPN_TYPE_REGEX);
                    _new_regex               = rpn_new_regex();
                    _new_regex->expr         = strdup(_regex->expr);
                    _new_regex->cflags       = G.cflags;
                    _stk_result->value.obj   = _new_regex;

                    if ((_error = regcomp(&_new_regex->RE, _new_regex->expr, _new_regex->cflags)) != 0) {
                         (void) regerror(_error, &_new_regex->RE, _errbuf, sizeof(_errbuf));
                         fprintf(stderr, "regcomp error for \"%s\" : %s\n",
                                 _new_regex->expr, _errbuf);
                         exit(RPN_EXIT_REGCOMP_ERROR);
                    }
               }
               else {
                    /* regex was compiled with the same
                     * settings as global flags
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _stk_result              = rpn_clone_elt(_stk_x);
               }
          }
          break;
// }}}
     default:
// {{{
          rpn_push(stack, _stk_x);
          _retcode                      = RPN_RET_INVALID_X_TYPE;
          goto end;
          break;
// }}}
     }

     rpn_set_lastx(stack, _stk_x);
     if (_stk_result != NULL) {
          rpn_push(stack, _stk_result);
     }

end:
     return _retcode;
}

/* en_op_regex_regex() }}} */
/* en_op_regex_string() {{{ */

/******************************************************************************

                         EN_OP_REGEX_STRING

******************************************************************************/
RPN_DEF_OP(en_op_regex_string)
{
     rpn_elt                  *_stk_x, *_stk_result = NULL;
     int                       _X_type;
     int                       _retcode;
     rpn_regex                *_regex;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_REGEX:
// {{{
          {
               _stk_result              = rpn_new_elt(RPN_TYPE_STRING);

               /* Initialize regex descriptor
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _regex                   = _stk_x->value.obj;
               _stk_result->value.s     = strdup(_regex->expr);
          }
          break;
// }}}
     default:
// {{{
          rpn_push(stack, _stk_x);
          _retcode                      = RPN_RET_INVALID_X_TYPE;
          goto end;
          break;
// }}}
     }

     rpn_set_lastx(stack, _stk_x);
     rpn_push(stack, _stk_result);

end:
     return _retcode;
}

/* en_op_regex_string() }}} */
/* en_op_regex_icase() {{{ */

/******************************************************************************

                         EN_OP_REGEX_ICASE

******************************************************************************/
RPN_DEF_OP(en_op_regex_icase)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result = NULL;
     int                       _X_type, _Y_type, _error, _x, _new_icase = 0;
     rpn_regex                *_regex, *_new_regex;
     int                       _retcode;
     char                      _errbuf[256];

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {
// {{{
     case RPN_TYPE_NIL:
// {{{
          _stk_result              = rpn_new_elt(RPN_TYPE_INT);
          _stk_result->value.i     = (G.cflags & REG_ICASE) == REG_ICASE;
          break;
// }}}
     case RPN_TYPE_INT:
// {{{
          _x                       = _stk_x->value.i;

          /* Control legal values for icase : 0 and 1
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          if (_x != 0 && _x != 1) {
               _retcode                      = RPN_RET_INVALID_X;
               rpn_push(stack, _stk_x);
               goto end;
          }
          else {
               if (_x == 1) {
                    _new_icase                    = REG_ICASE;
               }
          }

          _stk_y                   = rpn_pop(stack);
          _Y_type                  = rpn_get_type(_stk_y);

          switch (_Y_type) {
// {{{
          case RPN_TYPE_NIL:
// {{{
               {
                    G.cflags            &= ~REG_ICASE;
                    G.cflags            |= _new_icase;
               }
               break;
// }}}
          case RPN_TYPE_REGEX:
// {{{
               {
                    /* Initialize regex descriptor
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _regex                   = _stk_y->value.obj;

                    if ((_regex->cflags & REG_ICASE) != _new_icase) {
                         /* Regex must be recompiled with REG_ICASE flag
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _stk_result              = rpn_new_elt(RPN_TYPE_REGEX);
                         _new_regex               = rpn_new_regex();
                         _new_regex->expr         = strdup(_regex->expr);
                         _new_regex->cflags       = (_regex->cflags & ~REG_ICASE) | _new_icase;
                         _stk_result->value.obj   = _new_regex;

                         if ((_error = regcomp(&_new_regex->RE, _new_regex->expr, _new_regex->cflags)) != 0) {
                              (void) regerror(_error, &_new_regex->RE, _errbuf, sizeof(_errbuf));
                              fprintf(stderr, "regcomp error for \"%s\" : %s\n",
                                      _new_regex->expr, _errbuf);
                              exit(RPN_EXIT_REGCOMP_ERROR);
                         }
                    }
                    else {
                         /* REG_ICASE was already present
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _stk_result              = rpn_clone_elt(_stk_y);
                    }
               }
               break;
// }}}
               default:
// {{{
               rpn_push(stack, _stk_x);
               _retcode                      = RPN_RET_INVALID_Y_TYPE;
               goto end;
               break;
// }}}
// }}}
          }

          break;
// }}}
     case RPN_TYPE_REGEX:
// {{{
          {
               /* Initialize regex descriptor
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _regex                   = _stk_x->value.obj;

               if ((_regex->cflags & REG_ICASE) != (G.cflags & REG_ICASE)) {
                    /* Regex must be recompiled with REG_ICASE flag
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _stk_result              = rpn_new_elt(RPN_TYPE_REGEX);
                    _new_regex               = rpn_new_regex();
                    _new_regex->expr         = strdup(_regex->expr);
                    _new_regex->cflags       = (_regex->cflags & ~REG_ICASE) | (G.cflags & REG_ICASE);
                    _stk_result->value.obj   = _new_regex;

                    if ((_error = regcomp(&_new_regex->RE, _new_regex->expr, _new_regex->cflags)) != 0) {
                         (void) regerror(_error, &_new_regex->RE, _errbuf, sizeof(_errbuf));
                         fprintf(stderr, "regcomp error for \"%s\" : %s\n",
                                 _new_regex->expr, _errbuf);
                         exit(RPN_EXIT_REGCOMP_ERROR);
                    }
               }
               else {
                    /* REG_ICASE was already present
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _stk_result              = rpn_clone_elt(_stk_x);
               }
          }
          break;
// }}}
     default:
// {{{
          rpn_push(stack, _stk_x);
          _retcode                      = RPN_RET_INVALID_X_TYPE;
          goto end;
          break;
// }}}
// }}}
     }

     rpn_set_lastx(stack, _stk_x);
     if (_stk_result != NULL) {
          rpn_push(stack, _stk_result);
     }

end:
     return _retcode;
}

/* en_op_regex_icase() }}} */
/* en_op_regex_ere() {{{ */

/******************************************************************************

                         EN_OP_REGEX_ERE

******************************************************************************/
RPN_DEF_OP(en_op_regex_ere)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result = NULL;
     int                       _X_type, _Y_type, _error, _x, _new_ere = 0;
     rpn_regex                *_regex, *_new_regex;
     int                       _retcode;
     char                      _errbuf[256];

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {
// {{{
     case RPN_TYPE_NIL:
// {{{
          _stk_result              = rpn_new_elt(RPN_TYPE_INT);
          _stk_result->value.i     = (G.cflags & REG_EXTENDED) == REG_EXTENDED;
          break;
// }}}
     case RPN_TYPE_INT:
// {{{
          _x                       = _stk_x->value.i;

          /* Control legal values for ERE : 0 and 1
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          if (_x != 0 && _x != 1) {
               _retcode                      = RPN_RET_INVALID_X;
               rpn_push(stack, _stk_x);
               goto end;
          }
          else {
               if (_x == 1) {
                    _new_ere                      = REG_EXTENDED;
               }
          }

          _stk_y                   = rpn_pop(stack);
          _Y_type                  = rpn_get_type(_stk_y);

          switch (_Y_type) {

          case RPN_TYPE_NIL:
// {{{
               {
                    G.cflags            &= ~REG_EXTENDED;
                    G.cflags            |= _new_ere;
               }
               break;
// }}}
          case RPN_TYPE_REGEX:
// {{{
               {
                    /* Initialize regex descriptor
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _regex                   = _stk_y->value.obj;

                    if ((_regex->cflags & REG_EXTENDED) != _new_ere) {
                         /* Regex must be recompiled with REG_EXTENDED flag
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _stk_result              = rpn_new_elt(RPN_TYPE_REGEX);
                         _new_regex               = rpn_new_regex();
                         _new_regex->expr         = strdup(_regex->expr);
                         _new_regex->cflags       = (_regex->cflags & ~REG_EXTENDED) | _new_ere;
                         _stk_result->value.obj   = _new_regex;

                         if ((_error = regcomp(&_new_regex->RE, _new_regex->expr, _new_regex->cflags)) != 0) {
                              (void) regerror(_error, &_new_regex->RE, _errbuf, sizeof(_errbuf));
                              fprintf(stderr, "regcomp error for \"%s\" : %s\n",
                                      _new_regex->expr, _errbuf);
                              exit(RPN_EXIT_REGCOMP_ERROR);
                         }
                    }
                    else {
                         /* REG_EXTENDED was already present
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _stk_result              = rpn_clone_elt(_stk_y);
                    }
               }
               break;
// }}}
          default:
// {{{
               rpn_push(stack, _stk_x);
               _retcode                      = RPN_RET_INVALID_Y_TYPE;
               goto end;
               break;
// }}}
          }

          break;
// }}}
     case RPN_TYPE_REGEX:
// {{{
          {
               /* Initialize regex descriptor
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _regex                   = _stk_x->value.obj;

               if ((_regex->cflags & REG_EXTENDED) != (G.cflags & REG_EXTENDED)) {
                    /* Regex must be recompiled with REG_EXTENDED flag
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _stk_result              = rpn_new_elt(RPN_TYPE_REGEX);
                    _new_regex               = rpn_new_regex();
                    _new_regex->expr         = strdup(_regex->expr);
                    _new_regex->cflags       = (_regex->cflags & ~REG_EXTENDED) | (G.cflags & REG_EXTENDED);
                    _stk_result->value.obj   = _new_regex;

                    if ((_error = regcomp(&_new_regex->RE, _new_regex->expr, _new_regex->cflags)) != 0) {
                         (void) regerror(_error, &_new_regex->RE, _errbuf, sizeof(_errbuf));
                         fprintf(stderr, "regcomp error for \"%s\" : %s\n",
                                 _new_regex->expr, _errbuf);
                         exit(RPN_EXIT_REGCOMP_ERROR);
                    }
               }
               else {
                    /* REG_EXTENDED was already present
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _stk_result              = rpn_clone_elt(_stk_x);
               }
          }
          break;
// }}}
     default:
// {{{
          rpn_push(stack, _stk_x);
          _retcode                      = RPN_RET_INVALID_X_TYPE;
          goto end;
          break;
// }}}
// }}}
     }

     rpn_set_lastx(stack, _stk_x);
     if (_stk_result != NULL) {
          rpn_push(stack, _stk_result);
     }

end:
     return _retcode;
}

/* en_op_regex_icase() }}} */
/* en_op_regex_nosub() {{{ */

/******************************************************************************

                         EN_OP_REGEX_NOSUB

******************************************************************************/
RPN_DEF_OP(en_op_regex_nosub)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result = NULL;
     int                       _X_type, _Y_type, _error, _x, _new_nosub = 0;
     rpn_regex                *_regex, *_new_regex;
     int                       _retcode;
     char                      _errbuf[256];

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {
// {{{
     case RPN_TYPE_NIL:
// {{{
          _stk_result              = rpn_new_elt(RPN_TYPE_INT);
          _stk_result->value.i     = (G.cflags & REG_NOSUB) == REG_NOSUB;
          break;
// }}}
     case RPN_TYPE_INT:
// {{{
          _x                       = _stk_x->value.i;

          /* Control legal values for nosub : 0 and 1
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          if (_x != 0 && _x != 1) {
               _retcode                      = RPN_RET_INVALID_X;
               rpn_push(stack, _stk_x);
               goto end;
          }
          else {
               if (_x == 1) {
                    _new_nosub                    = REG_NOSUB;
               }
          }

          _stk_y                   = rpn_pop(stack);
          _Y_type                  = rpn_get_type(_stk_y);

          switch (_Y_type) {

          case RPN_TYPE_NIL:
// {{{
               {
                    G.cflags            &= ~REG_NOSUB;
                    G.cflags            |= _new_nosub;
               }
               break;
// }}}
          case RPN_TYPE_REGEX:
// {{{
               {
                    /* Initialize regex descriptor
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _regex                   = _stk_y->value.obj;

                    if ((_regex->cflags & REG_NOSUB) != _new_nosub) {
                         /* Regex must be recompiled with REG_NOSUB flag
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _stk_result              = rpn_new_elt(RPN_TYPE_REGEX);
                         _new_regex               = rpn_new_regex();
                         _new_regex->expr         = strdup(_regex->expr);
                         _new_regex->cflags       = (_regex->cflags & ~REG_NOSUB) | _new_nosub;
                         _stk_result->value.obj   = _new_regex;

                         if ((_error = regcomp(&_new_regex->RE, _new_regex->expr, _new_regex->cflags)) != 0) {
                              (void) regerror(_error, &_new_regex->RE, _errbuf, sizeof(_errbuf));
                              fprintf(stderr, "regcomp error for \"%s\" : %s\n",
                                      _new_regex->expr, _errbuf);
                              exit(RPN_EXIT_REGCOMP_ERROR);
                         }
                    }
                    else {
                         /* REG_NOSUB was already present
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _stk_result              = rpn_clone_elt(_stk_y);
                    }
               }
               break;
// }}}
          default:
// {{{
               rpn_push(stack, _stk_x);
               _retcode                      = RPN_RET_INVALID_Y_TYPE;
               goto end;
               break;
// }}}
          }

          break;
// }}}
     case RPN_TYPE_REGEX:
// {{{
          {
               /* Initialize regex descriptor
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _regex                   = _stk_x->value.obj;

               if ((_regex->cflags & REG_NOSUB) != (G.cflags & REG_NOSUB)) {
                    /* Regex must be recompiled with REG_NOSUB flag
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _stk_result              = rpn_new_elt(RPN_TYPE_REGEX);
                    _new_regex               = rpn_new_regex();
                    _new_regex->expr         = strdup(_regex->expr);
                    _new_regex->cflags       = (_regex->cflags & ~REG_NOSUB) | (G.cflags & REG_NOSUB);
                    _stk_result->value.obj   = _new_regex;

                    if ((_error = regcomp(&_new_regex->RE, _new_regex->expr, _new_regex->cflags)) != 0) {
                         (void) regerror(_error, &_new_regex->RE, _errbuf, sizeof(_errbuf));
                         fprintf(stderr, "regcomp error for \"%s\" : %s\n",
                                 _new_regex->expr, _errbuf);
                         exit(RPN_EXIT_REGCOMP_ERROR);
                    }
               }
               else {
                    /* REG_NOSUB was already present
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _stk_result              = rpn_clone_elt(_stk_x);
               }
          }
          break;
// }}}
     default:
// {{{
          rpn_push(stack, _stk_x);
          _retcode                      = RPN_RET_INVALID_X_TYPE;
          goto end;
          break;
// }}}
// }}}
     }

     rpn_set_lastx(stack, _stk_x);
     if (_stk_result != NULL) {
          rpn_push(stack, _stk_result);
     }

end:
     return _retcode;
}

/* en_op_regex_nosub() }}} */
/* en_op_regex_newline() {{{ */

/******************************************************************************

                         EN_OP_REGEX_NEWLINE

******************************************************************************/
RPN_DEF_OP(en_op_regex_newline)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result = NULL;
     int                       _X_type, _Y_type, _error, _x, _new_newline = 0;
     rpn_regex                *_regex, *_new_regex;
     int                       _retcode;
     char                      _errbuf[256];

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {
// {{{
     case RPN_TYPE_NIL:
// {{{
          _stk_result              = rpn_new_elt(RPN_TYPE_INT);
          _stk_result->value.i     = (G.cflags & REG_NEWLINE) == REG_NEWLINE;
          break;
// }}}
     case RPN_TYPE_INT:
// {{{
          _x                       = _stk_x->value.i;

          /* Control legal values for newline : 0 and 1
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          if (_x != 0 && _x != 1) {
               _retcode                      = RPN_RET_INVALID_X;
               rpn_push(stack, _stk_x);
               goto end;
          }
          else {
               if (_x == 1) {
                    _new_newline                  = REG_NEWLINE;
               }
          }

          _stk_y                   = rpn_pop(stack);
          _Y_type                  = rpn_get_type(_stk_y);

          switch (_Y_type) {
// {{{
          case RPN_TYPE_NIL:
// {{{
               {
                    G.cflags            &= ~REG_NEWLINE;
                    G.cflags            |= _new_newline;
               }
               break;
// }}}
          case RPN_TYPE_REGEX:
// {{{
               {
                    /* Initialize regex descriptor
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _regex                   = _stk_y->value.obj;

                    if ((_regex->cflags & REG_NEWLINE) != _new_newline) {
                         /* Regex must be recompiled with REG_NEWLINE flag
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _stk_result              = rpn_new_elt(RPN_TYPE_REGEX);
                         _new_regex               = rpn_new_regex();
                         _new_regex->expr         = strdup(_regex->expr);
                         _new_regex->cflags       = (_regex->cflags & ~REG_NEWLINE) | _new_newline;
                         _stk_result->value.obj   = _new_regex;

                         if ((_error = regcomp(&_new_regex->RE, _new_regex->expr, _new_regex->cflags)) != 0) {
                              (void) regerror(_error, &_new_regex->RE, _errbuf, sizeof(_errbuf));
                              fprintf(stderr, "regcomp error for \"%s\" : %s\n",
                                      _new_regex->expr, _errbuf);
                              exit(RPN_EXIT_REGCOMP_ERROR);
                         }
                    }
                    else {
                         /* REG_NEWLINE was already present
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _stk_result              = rpn_clone_elt(_stk_y);
                    }
               }
               break;
// }}}
          default:
// {{{
               rpn_push(stack, _stk_x);
               _retcode                      = RPN_RET_INVALID_Y_TYPE;
               goto end;
               break;
// }}}
// }}}
          }
          break;
// }}}
     case RPN_TYPE_REGEX:
// {{{
          {
               /* Initialize regex descriptor
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _regex                   = _stk_x->value.obj;

               if ((_regex->cflags & REG_NEWLINE) != (G.cflags & REG_NEWLINE)) {
                    /* Regex must be recompiled with REG_NEWLINE flag
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _stk_result              = rpn_new_elt(RPN_TYPE_REGEX);
                    _new_regex               = rpn_new_regex();
                    _new_regex->expr         = strdup(_regex->expr);
                    _new_regex->cflags       = (_regex->cflags & ~REG_NEWLINE) | (G.cflags & REG_NEWLINE);
                    _stk_result->value.obj   = _new_regex;

                    if ((_error = regcomp(&_new_regex->RE, _new_regex->expr, _new_regex->cflags)) != 0) {
                         (void) regerror(_error, &_new_regex->RE, _errbuf, sizeof(_errbuf));
                         fprintf(stderr, "regcomp error for \"%s\" : %s\n",
                                 _new_regex->expr, _errbuf);
                         exit(RPN_EXIT_REGCOMP_ERROR);
                    }
               }
               else {
                    /* REG_NEWLINE was already present
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _stk_result              = rpn_clone_elt(_stk_x);
               }
          }
          break;
// }}}
     default:
// {{{
          rpn_push(stack, _stk_x);
          _retcode                      = RPN_RET_INVALID_X_TYPE;
          goto end;
          break;
// }}}
// }}}

     }

     rpn_set_lastx(stack, _stk_x);
     if (_stk_result != NULL) {
          rpn_push(stack, _stk_result);
     }

end:
     return _retcode;
}

/* en_op_regex_newline() }}} */
/* en_op_regex_notbol() {{{ */

/******************************************************************************

                         EN_OP_REGEX_NOTBOL

******************************************************************************/
RPN_DEF_OP(en_op_regex_notbol)
{
     rpn_elt                  *_stk_x, *_stk_result = NULL;
     int                       _X_type, _x;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {
// {{{
     case RPN_TYPE_NIL:
// {{{
          _stk_result              = rpn_new_elt(RPN_TYPE_INT);
          _stk_result->value.i     = (G.eflags & REG_NOTBOL) == REG_NOTBOL;
          break;
// }}}
     case RPN_TYPE_INT:
// {{{
          _x                       = _stk_x->value.i;

          /* Control legal values for notbol : 0 and 1
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          if (_x != 0 && _x != 1) {
               _retcode                      = RPN_RET_INVALID_X;
               rpn_push(stack, _stk_x);
               goto end;
          }
          else {
			G.eflags		            &= ~REG_NOTBOL;
               if (_x == 1) {
                    G.eflags            	  |= REG_NOTBOL;
               }
          }

		break;
// }}}
     default:
// {{{
          rpn_push(stack, _stk_x);
          _retcode                      = RPN_RET_INVALID_X_TYPE;
          goto end;
          break;
// }}}
// }}}
     }

     rpn_set_lastx(stack, _stk_x);
     if (_stk_result != NULL) {
          rpn_push(stack, _stk_result);
     }

end:
     return _retcode;
}

/* en_op_regex_notbol() }}} */
/* en_op_regex_noteol() {{{ */

/******************************************************************************

                         EN_OP_REGEX_NOTEOL

******************************************************************************/
RPN_DEF_OP(en_op_regex_noteol)
{
     rpn_elt                  *_stk_x, *_stk_result = NULL;
     int                       _X_type, _x;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {
// {{{
     case RPN_TYPE_NIL:
// {{{
          _stk_result              = rpn_new_elt(RPN_TYPE_INT);
          _stk_result->value.i     = (G.cflags & REG_NOTEOL) == REG_NOTEOL;
          break;
// }}}
     case RPN_TYPE_INT:
// {{{
          _x                       = _stk_x->value.i;

          /* Control legal values for noteol : 0 and 1
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          if (_x != 0 && _x != 1) {
               _retcode                      = RPN_RET_INVALID_X;
               rpn_push(stack, _stk_x);
               goto end;
          }
          else {
			G.eflags		            &= ~REG_NOTEOL;
               if (_x == 1) {
                    G.eflags            	  |= REG_NOTEOL;
               }
          }

          break;
// }}}
     default:
// {{{
          rpn_push(stack, _stk_x);
          _retcode                      = RPN_RET_INVALID_X_TYPE;
          goto end;
          break;
// }}}
// }}}
     }

     rpn_set_lastx(stack, _stk_x);
     if (_stk_result != NULL) {
          rpn_push(stack, _stk_result);
     }

end:
     return _retcode;
}

/* en_op_regex_noteol() }}} */
/* en_op_regex_match() {{{ */

/******************************************************************************

                         EN_OP_REGEX_MATCH

******************************************************************************/
RPN_DEF_OP(en_op_regex_match)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _X_type, _Y_type;
     int                       _retcode;
     rpn_regex                *_regex;
     int                       _error;
#define   NB_MATCH  12
     size_t                    _nmatch  = NB_MATCH;
     regmatch_t                _pmatch[NB_MATCH];
#if 0
     rpn_elt                  *_elt;
     int                       _i, _so, _eo;
#endif

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_REGEX:
// {{{
          _regex				= _stk_x->value.obj;

		_stk_y				= rpn_pop(stack);
		_Y_type				= rpn_get_type(_stk_y);

		switch (_Y_type) {
// {{{
		case RPN_TYPE_INT:
// {{{
	X
	abort();
			break;

// }}}
		case RPN_TYPE_STRING:
// {{{
			{
				int			 _i, _so, _eo;

fprintf(stderr, "String = '%s' expr = '%s'\n", _stk_y->value.s, _regex->expr);
				if ((_error = regexec(&_regex->RE, _stk_y->value.s, _nmatch, _pmatch, _regex->eflags)) == 0) {
fprintf(stderr, "=== Match ===\n");
					_stk_result                   = rpn_clone_elt(_stk_y);
				}
				else {
fprintf(stderr, "=== NO MATCH ===\n");
					_stk_result                   = rpn_new_elt(RPN_TYPE_NIL);
				}

				for (_i = 0; _i < _nmatch; _i++) {
					_so       = _pmatch[_i].rm_so;
					_eo       = _pmatch[_i].rm_eo;
					if (_so != -1) {
						fprintf(stderr, "%2d => %2d : %.*s\n", _so, _eo, _eo - _so, &_stk_y->value.s[_so]);
					}

				}
			}
			break;
// }}}
		case RPN_TYPE_LIST:
// {{{
			{
#if 0
				rpn_stack                *_stack;
				rpn_list                 *_list;
				rpn_elt                  *_sub_elt;

				_stack                   = rpn_new_stack(__func__);

				for (_list = _elt->value.obj; _list->num_elts > 0; ) {
					_sub_elt                 = rpn_list_pop_head(_list);
				}

				RPN_FREE(_list->name);
#endif
			}

			break;

// }}}
		case RPN_TYPE_TEXT:
// {{{
X
abort();
			break;
// }}}
		default:
// {{{
			rpn_push(stack, _stk_y);
			rpn_push(stack, _stk_x);
			_retcode                      = RPN_RET_INVALID_Y_TYPE;
			goto end;
			break;
// }}}
// }}}
     }
          break;
// }}}
     default:
// {{{
          rpn_push(stack, _stk_x);
          _retcode                      = RPN_RET_INVALID_X_TYPE;
          goto end;
          break;
// }}}
     }

     rpn_set_lastx(stack, _stk_x);
     rpn_push(stack, _stk_result);

end:
     return _retcode;
}

/* en_op_regex_match() }}} */
// GROUP : Regex }}}
