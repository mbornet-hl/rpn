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
     DO_INT = 258,
     DO_ID = 259,
     DO_DOUBLE = 260,
     DO_SUM = 261,
     DO_FIRST = 262,
     DO_GBYTES = 263,
     DO_MBYTES = 264,
     DO_KBYTES = 265,
     DO_BYTES = 266,
     DO_BITS = 267,
     DO_SEC = 268,
     DO_LOCAL = 269,
     DO_GBPS = 270,
     DO_MBPS = 271,
     DO_KBPS = 272,
     DO_BPS = 273,
     DO_NULL = 274,
     DO_GARBAGE = 275
   };
#endif
/* Tokens.  */
#define DO_INT 258
#define DO_ID 259
#define DO_DOUBLE 260
#define DO_SUM 261
#define DO_FIRST 262
#define DO_GBYTES 263
#define DO_MBYTES 264
#define DO_KBYTES 265
#define DO_BYTES 266
#define DO_BITS 267
#define DO_SEC 268
#define DO_LOCAL 269
#define DO_GBPS 270
#define DO_MBPS 271
#define DO_KBPS 272
#define DO_BPS 273
#define DO_NULL 274
#define DO_GARBAGE 275




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 32 "do_iperf_out_yacc.y"

     int                  i_value;
     double               d_value;
	rpn_elt			*elt;



/* Line 2068 of yacc.c  */
#line 98 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE do_iperf_out_lval;


