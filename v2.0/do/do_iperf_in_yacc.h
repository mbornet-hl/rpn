/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     DO_VAR_C_OPTS = 258,
     DO_VAR_S_OPTS = 259,
     DO_VAR_S_PORT = 260,
     DO_SERVER = 261,
     DO_VAR_S_TIMEOUT = 262,
     DO_OPT = 263,
     DO_INT_PARAM = 264,
     DO_STR_PARAM = 265,
     DO_COMMENT = 266,
     DO_EQ = 267,
     DO_SEP = 268
   };
#endif
/* Tokens.  */
#define DO_VAR_C_OPTS 258
#define DO_VAR_S_OPTS 259
#define DO_VAR_S_PORT 260
#define DO_SERVER 261
#define DO_VAR_S_TIMEOUT 262
#define DO_OPT 263
#define DO_INT_PARAM 264
#define DO_STR_PARAM 265
#define DO_COMMENT 266
#define DO_EQ 267
#define DO_SEP 268




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 35 "do_iperf_in_yacc.y"

     char                *s_value;



/* Line 2068 of yacc.c  */
#line 82 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE do_iperf_in_lval;


