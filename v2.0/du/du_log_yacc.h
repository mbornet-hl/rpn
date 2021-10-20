/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_DU_LOG_Y_TAB_H_INCLUDED
# define YY_DU_LOG_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int du_log_debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    DU_IP = 258,
    DU_DATE = 259,
    DU = 260,
    DOW = 261,
    DU_TIME = 262,
    DU_ARGS = 263,
    DU_GET = 264,
    DU_PUT = 265,
    DU_POST = 266,
    DU_HEAD = 267,
    DU_OPTIONS = 268,
    DU_PROPFIND = 269,
    DU_YEAR = 270,
    DU_DOW = 271,
    DU_NUM = 272,
    DU_OSB = 273,
    DU_CSB = 274,
    DU_DQUOTE = 275,
    DU_DASH = 276
  };
#endif
/* Tokens.  */
#define DU_IP 258
#define DU_DATE 259
#define DU 260
#define DOW 261
#define DU_TIME 262
#define DU_ARGS 263
#define DU_GET 264
#define DU_PUT 265
#define DU_POST 266
#define DU_HEAD 267
#define DU_OPTIONS 268
#define DU_PROPFIND 269
#define DU_YEAR 270
#define DU_DOW 271
#define DU_NUM 272
#define DU_OSB 273
#define DU_CSB 274
#define DU_DQUOTE 275
#define DU_DASH 276

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 30 "du_log_yacc.y" /* yacc.c:1909  */

	char				*s_value;
	int				 d_value;

#line 101 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE du_log_lval;

int du_log_parse (void);

#endif /* !YY_DU_LOG_Y_TAB_H_INCLUDED  */
