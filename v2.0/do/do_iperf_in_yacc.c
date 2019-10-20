/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse         do_iperf_in_parse
#define yylex           do_iperf_in_lex
#define yyerror         do_iperf_in_error
#define yylval          do_iperf_in_lval
#define yychar          do_iperf_in_char
#define yydebug         do_iperf_in_debug
#define yynerrs         do_iperf_in_nerrs


/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "do_iperf_in_yacc.y"

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
 *   @(#)  [MB] do_iperf_in_yacc.y Version 1.15 du 19/10/16 - 
 */

#include "../cy/cy_rpn_header.h"
#include "../cy/cy_rpn_proto.h"
#include  "do_cpri.h"
#include  "do_epri.h"
#include	"do_epub.h"

extern struct rpn_operator          rpn_operators[];   // XXX : a supprimer

int                       do_iperf_in_lex();
void                      do_iperf_in_error(char *);
int                       do_iperf_in_input(void);



/* Line 268 of yacc.c  */
#line 114 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 293 of yacc.c  */
#line 35 "do_iperf_in_yacc.y"

     char                *s_value;



/* Line 293 of yacc.c  */
#line 182 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 194 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  17
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   43

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  14
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  11
/* YYNRULES -- Number of rules.  */
#define YYNRULES  24
/* YYNRULES -- Number of states.  */
#define YYNSTATES  38

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   268

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    18,    22,
      26,    30,    32,    40,    47,    52,    55,    57,    58,    60,
      62,    65,    68,    69,    71
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      15,     0,    -1,    16,    -1,    15,    16,    -1,    17,    -1,
      19,    -1,    18,    -1,     3,    12,    22,    -1,     4,    12,
      22,    -1,     5,    12,    22,    -1,     7,    12,     9,    -1,
      11,    -1,    21,    13,    22,    13,    22,    13,    22,    -1,
      21,    13,    22,    13,    22,    20,    -1,    21,    13,    22,
      20,    -1,    21,    20,    -1,    13,    -1,    -1,     6,    -1,
      23,    -1,    22,    23,    -1,     8,    24,    -1,    -1,    10,
      -1,     9,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    50,    50,    54,    60,    64,    68,    74,    92,   110,
     128,   142,   148,   177,   208,   241,   276,   277,   280,   300,
     304,   310,   328,   343,   348
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "DO_VAR_C_OPTS", "DO_VAR_S_OPTS",
  "DO_VAR_S_PORT", "DO_SERVER", "DO_VAR_S_TIMEOUT", "DO_OPT",
  "DO_INT_PARAM", "DO_STR_PARAM", "DO_COMMENT", "DO_EQ", "DO_SEP",
  "$accept", "desc", "line", "var_assign", "comment", "server_desc", "sep",
  "server", "opts", "opt", "param", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    14,    15,    15,    16,    16,    16,    17,    17,    17,
      17,    18,    19,    19,    19,    19,    20,    20,    21,    22,
      22,    23,    23,    24,    24
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     3,     3,     3,
       3,     1,     7,     6,     4,     2,     1,     0,     1,     1,
       2,     2,     0,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,    18,     0,    11,     0,     2,     4,
       6,     5,    17,    22,    22,    22,     0,     1,     3,    16,
      15,     0,     7,    19,     8,     9,    10,    17,    24,    23,
      21,    20,    16,    14,    17,    16,    13,    12
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     7,     8,     9,    10,    11,    20,    12,    22,    23,
      30
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -25
static const yytype_int8 yypact[] =
{
      29,   -10,    -1,    10,   -25,    16,   -25,    20,   -25,   -25,
     -25,   -25,    17,    21,    21,    21,    28,   -25,   -25,     0,
     -25,     5,    21,   -25,    21,    21,   -25,     4,   -25,   -25,
     -25,   -25,     0,   -25,    30,    21,   -25,    21
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -25,   -25,    32,   -25,   -25,   -25,   -24,   -25,   -14,   -18,
     -25
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -23
static const yytype_int8 yytable[] =
{
      24,    25,    13,    33,    31,    27,    31,    31,    21,    31,
      36,    14,    21,   -22,    28,    29,    31,    32,    34,    31,
      17,    37,    15,     1,     2,     3,     4,     5,    16,    21,
      19,     6,     1,     2,     3,     4,     5,    26,    21,    18,
       6,     0,     0,    35
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-25))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
      14,    15,    12,    27,    22,    19,    24,    25,     8,    27,
      34,    12,     8,    13,     9,    10,    34,    13,    32,    37,
       0,    35,    12,     3,     4,     5,     6,     7,    12,     8,
      13,    11,     3,     4,     5,     6,     7,     9,     8,     7,
      11,    -1,    -1,    13
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,    11,    15,    16,    17,
      18,    19,    21,    12,    12,    12,    12,     0,    16,    13,
      20,     8,    22,    23,    22,    22,     9,    22,     9,    10,
      24,    23,    13,    20,    22,    13,    20,    22
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 51 "do_iperf_in_yacc.y"
    {
                    DO_TRACE_YACC_IN("line\n");
               }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 55 "do_iperf_in_yacc.y"
    {
                    DO_TRACE_YACC_IN("desc line\n");
               }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 61 "do_iperf_in_yacc.y"
    {
                    DO_TRACE_YACC_IN("var_assign\n");
               }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 65 "do_iperf_in_yacc.y"
    {
                    DO_TRACE_YACC_IN("server_desc\n");
               }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 69 "do_iperf_in_yacc.y"
    {
                    DO_TRACE_YACC_IN("comment\n");
               }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 75 "do_iperf_in_yacc.y"
    {
                    rpn_elt             **_elt_list;

                    DO_TRACE_YACC_IN("VAR_C_OPTS EQ opts\n");
                    if (do_G.list_deflt_client_opts != 0) {
                         fprintf(stderr, "%s: duplicate variable for client options !\n",
                                 G.progname);
                         exit(RPN_EXIT_DUPLICATE);
                    }

                    _elt_list                     = do_get_curr_list(FALSE, 0);
				if (do_G.debug_level & DO_DEBUG_YACC_IN) {
					rpn_dump_elt(*_elt_list, 0);
				}
                    do_G.list_deflt_client_opts   = *_elt_list;
                    *_elt_list                    = 0;
               }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 93 "do_iperf_in_yacc.y"
    {
                    rpn_elt             **_elt_list;

                    DO_TRACE_YACC_IN("VAR_S_OPTS EQ opts\n");
                    if (do_G.list_deflt_server_opts != 0) {
                         fprintf(stderr, "%s: duplicate variable for server options !\n",
                                 G.progname);
                         exit(RPN_EXIT_DUPLICATE);
                    }

                    _elt_list                     = do_get_curr_list(FALSE, 1);
				if (do_G.debug_level & DO_DEBUG_YACC_IN) {
					rpn_dump_elt(*_elt_list, 0);
				}
                    do_G.list_deflt_server_opts   = *_elt_list;
                    *_elt_list                    = 0;
               }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 111 "do_iperf_in_yacc.y"
    {
                    rpn_elt             **_elt_list;

                    DO_TRACE_YACC_IN("VAR_S_PORT EQ opts\n");
                    if (do_G.list_deflt_server_port != 0) {
                         fprintf(stderr, "%s: duplicate variable for server port !\n",
                                 G.progname);
                         exit(RPN_EXIT_DUPLICATE);
                    }

                    _elt_list                     = do_get_curr_list(FALSE, 2);
				if (do_G.debug_level & DO_DEBUG_YACC_IN) {
					rpn_dump_elt(*_elt_list, 0);
				}
                    do_G.list_deflt_server_port   = *_elt_list;
                    *_elt_list                    = 0;
               }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 129 "do_iperf_in_yacc.y"
    {
                    DO_TRACE_YACC_IN("VAR_S_TIMEOUT EQ opts\n");
                    if (do_G.server_timeout != 0) {
                         fprintf(stderr, "%s: duplicate variable for server timeout !\n",
                                 G.progname);
                         exit(RPN_EXIT_DUPLICATE);
                    }

                    do_G.server_timeout				= rpn_new_elt(RPN_TYPE_STRING);
                    do_G.server_timeout->value.s		= strdup((yyvsp[(3) - (3)].s_value));
               }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 143 "do_iperf_in_yacc.y"
    {
                    DO_TRACE_YACC_IN("COMMENT\n");
               }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 149 "do_iperf_in_yacc.y"
    {
                    do_iperf_host_desc            *_host_desc;
                    int                            _i;

                    DO_TRACE_YACC_IN("server DO_SEP opts DO_SEP opts DO_SEP opts\n");

                    _host_desc                         = do_new_iperf_host_desc();
                    _host_desc->elt_server             = rpn_new_elt(RPN_TYPE_STRING);
                    _host_desc->elt_server->value.s    = strdup((yyvsp[(1) - (7)].s_value));

                    for (_i = 0; _i <= 2; _i++) {
                         rpn_set_list_name(do_G.tmp_opts[_i], do_list_label(_i));
                    }

                    _host_desc->list_client_opts       = do_G.tmp_opts[0];
                    _host_desc->list_server_opts       = do_G.tmp_opts[1];
                    _host_desc->list_server_port       = do_G.tmp_opts[2];

				if (do_G.debug_level & DO_DEBUG_YACC_IN) {
					do_dump_host_desc(_host_desc);
				}

                    for (_i = 0; _i <= 2; _i++) {
                         do_G.tmp_opts[_i]                  = 0;
                    }

                    do_add_host_desc(_host_desc);
               }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 178 "do_iperf_in_yacc.y"
    {
                    do_iperf_host_desc            *_host_desc;
                    int                            _i;

                    DO_TRACE_YACC_IN("server DO_SEP opts DO_SEP opts\n");

                    _host_desc                         = do_new_iperf_host_desc();
                    _host_desc->elt_server             = rpn_new_elt(RPN_TYPE_STRING);
                    _host_desc->elt_server->value.s    = strdup((yyvsp[(1) - (6)].s_value));

                    do_G.tmp_opts[2]                   = do_empty_list();

                    for (_i = 0; _i <= 2; _i++) {
                         rpn_set_list_name(do_G.tmp_opts[_i], do_list_label(_i));
                    }

                    _host_desc->list_client_opts       = do_G.tmp_opts[0];
                    _host_desc->list_server_opts       = do_G.tmp_opts[1];
                    _host_desc->list_server_port       = do_G.tmp_opts[2];

				if (do_G.debug_level & DO_DEBUG_YACC_IN) {
					do_dump_host_desc(_host_desc);
				}

                    for (_i = 0; _i <= 2; _i++) {
                         do_G.tmp_opts[_i]                  = 0;
                    }

                    do_add_host_desc(_host_desc);
               }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 209 "do_iperf_in_yacc.y"
    {
                    do_iperf_host_desc            *_host_desc;
                    int                            _i;

                    DO_TRACE_YACC_IN("server DO_SEP opts\n");

                    _host_desc                         = do_new_iperf_host_desc();
                    _host_desc->elt_server             = rpn_new_elt(RPN_TYPE_STRING);
                    _host_desc->elt_server->value.s    = strdup((yyvsp[(1) - (4)].s_value));

                    for (_i = 1; _i <= 2; _i++) {
                         do_G.tmp_opts[_i]                  = do_empty_list();
                    }

                    for (_i = 0; _i <= 2; _i++) {
                         rpn_set_list_name(do_G.tmp_opts[_i], do_list_label(_i));
                    }

                    _host_desc->list_client_opts       = do_G.tmp_opts[0];
                    _host_desc->list_server_opts       = do_G.tmp_opts[1];
                    _host_desc->list_server_port       = do_G.tmp_opts[2];

				if (do_G.debug_level & DO_DEBUG_YACC_IN) {
					do_dump_host_desc(_host_desc);
				}

                    for (_i = 0; _i <= 2; _i++) {
                         do_G.tmp_opts[_i]                  = 0;
                    }

                    do_add_host_desc(_host_desc);
               }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 242 "do_iperf_in_yacc.y"
    {
                    do_iperf_host_desc            *_host_desc;
                    int                            _i;

                    DO_TRACE_YACC_IN("server\n");

                    _host_desc                         = do_new_iperf_host_desc();
                    _host_desc->elt_server             = rpn_new_elt(RPN_TYPE_STRING);
                    _host_desc->elt_server->value.s    = strdup((yyvsp[(1) - (2)].s_value));

                    for (_i = 0; _i <= 2; _i++) {
                         do_G.tmp_opts[_i]                  = do_empty_list();
                    }

                    for (_i = 0; _i <= 2; _i++) {
                         rpn_set_list_name(do_G.tmp_opts[_i], do_list_label(_i));
                    }

                    _host_desc->list_client_opts       = do_G.tmp_opts[0];
                    _host_desc->list_server_opts       = do_G.tmp_opts[1];
                    _host_desc->list_server_port       = do_G.tmp_opts[2];

				if (do_G.debug_level & DO_DEBUG_YACC_IN) {
					do_dump_host_desc(_host_desc);
				}

                    for (_i = 0; _i <= 2; _i++) {
                         do_G.tmp_opts[_i]                  = 0;
                    }

                    do_add_host_desc(_host_desc);
               }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 281 "do_iperf_in_yacc.y"
    {
                    /*
                    rpn_elt                  *_elt;

                    if (do_G.tmp_host_desc == 0) {
                         do_G.tmp_host_desc                 = do_new_iperf_host_desc();
                         _elt                               = rpn_new_elt(RPN_TYPE_STRING);
                         _elt->value.s                      = strdup($1);
                         do_G.tmp_host_desc->elt_server     = _elt;
                    }
                    else {
                         RPN_INTERNAL_ERROR;
                    }
                    */

                    DO_TRACE_YACC_IN("DO_SERVER : [%s]\n", (yyvsp[(1) - (1)].s_value));
                    (yyval.s_value)                  = (yyvsp[(1) - (1)].s_value);
               }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 301 "do_iperf_in_yacc.y"
    {
                    DO_TRACE_YACC_IN("opt\n");
               }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 305 "do_iperf_in_yacc.y"
    {
                    DO_TRACE_YACC_IN("opts opt\n");
               }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 311 "do_iperf_in_yacc.y"
    {
                    struct rpn_elt           *_elt, **_elt_list;
                    struct rpn_list          *_list;

                    DO_TRACE_YACC_IN("DO_OPT param : [%s] [%s]\n", (yyvsp[(1) - (2)].s_value), (yyvsp[(2) - (2)].s_value));
                    _elt_list           = do_get_curr_list(FALSE, 3);
                    _list               = (*_elt_list)->value.obj;

                    _elt                = rpn_new_elt(RPN_TYPE_STRING);
                    _elt->value.s       = (yyvsp[(1) - (2)].s_value);
                    rpn_list_push_tail(_list, _elt);

                    _elt                = rpn_new_elt(RPN_TYPE_STRING);
                    _elt->value.s       = (yyvsp[(2) - (2)].s_value);
                    rpn_list_push_tail(_list, _elt);
               }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 328 "do_iperf_in_yacc.y"
    {
                    struct rpn_elt           *_elt, **_elt_list;
                    struct rpn_list          *_list;

                    DO_TRACE_YACC_IN("empty opt => \"\"\n");

                    _elt_list           = do_get_curr_list(TRUE, 3);
                    _list               = (*_elt_list)->value.obj;

                    _elt                = rpn_new_elt(RPN_TYPE_STRING);
                    _elt->value.s       = strdup("");
                    rpn_list_push_tail(_list, _elt);
               }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 344 "do_iperf_in_yacc.y"
    {
                    DO_TRACE_YACC_IN("DO_STR_PARAM : [%s]\n", (yyvsp[(1) - (1)].s_value));
                    (yyval.s_value)        = (yyvsp[(1) - (1)].s_value);
               }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 349 "do_iperf_in_yacc.y"
    {
                    DO_TRACE_YACC_IN("DO_INT_PARAM : [%s]\n", (yyvsp[(1) - (1)].s_value));
                    (yyval.s_value)        = (yyvsp[(1) - (1)].s_value);
               }
    break;



/* Line 1806 of yacc.c  */
#line 1831 "y.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 355 "do_iperf_in_yacc.y"


/* do_get_curr_list() {{{ */
/******************************************************************************

                         DO_GET_CURR_LIST

******************************************************************************/
rpn_elt **do_get_curr_list(int empty_field_detected, int label_idx)
{
     int                   _idx, _label_idx;
     rpn_elt             **_elt_list;
     char                  _list_name[32];

     DO_TRACE_YACC_IN("global field_no = %d %s\n",
                          do_G.field_no,
                          empty_field_detected ? "(empty field detected)" : "");

     /* Initialize list pointer according to the current field number
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (empty_field_detected) {
          if (do_G.field_no == 1) {
               _idx           = 2;      // 4th field is empty
          }
          else {
               _idx           = do_G.field_no - 3;
          }
     }
     else {
          if (do_G.field_no == 0) {
               _idx           = 0;
          }
          else if (do_G.field_no >= 2 && do_G.field_no <= 4) {
               _idx           = do_G.field_no - 2;
          }
          else {
               /* Syntax error
                  ~~~~~~~~~~~~ */
               DO_TRACE_YACC_IN("Empty 4th field detected\n");
               _idx           = 2; // Due to empty 4th field
               yyerror("invalid field number !");
               RPN_INTERNAL_ERROR;
          }
     }

     DO_TRACE_YACC_IN("_idx = %d\n", _idx);
     if (label_idx != 0) {
          _label_idx     = label_idx;
      }
      else {
          _label_idx     = _idx;
      }
     strcpy(_list_name, do_list_label(_label_idx));

     _elt_list      = &do_G.tmp_opts[_idx];
     if (*_elt_list == 0) {
          /* Allocate a new list
             ~~~~~~~~~~~~~~~~~~~ */
          *_elt_list               = rpn_new_elt(RPN_TYPE_LIST);
          (*_elt_list)->value.obj  = rpn_new_list(strdup(_list_name));
     }
     else {
          /* Reset list name
             ~~~~~~~~~~~~~~~ */
          rpn_list                 *_list;

          _list                    = (*_elt_list)->value.obj;
          free(_list->name);
          _list->name              = strdup(_list_name);
     }

     return _elt_list;
}

/* do_get_curr_list() }}} */
/* do_add_host_desc() {{{ */

/******************************************************************************

                         DO_ADD_HOST_DESC

******************************************************************************/
void do_add_host_desc(do_iperf_host_desc *host_desc)
{
     rpn_elt                       *_elt_list;
     rpn_list                      *_list;

     if (do_G.desc == 0) {
          do_G.desc                = rpn_new_elt(RPN_TYPE_LIST);
          do_G.desc->value.obj     = rpn_new_list(strdup("iperf test description"));
     }

     /* Allocate a list for the description of server parameters
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _elt_list                = rpn_new_elt(RPN_TYPE_LIST);
     _list                    = rpn_new_list(strdup("Host parameters"));
     _elt_list->value.obj     = _list;

     rpn_list_push_tail(_list, host_desc->elt_server);
     rpn_list_push_tail(_list, host_desc->list_client_opts);
     rpn_list_push_tail(_list, host_desc->list_server_opts);
     rpn_list_push_tail(_list, host_desc->list_server_port);

     rpn_list_push_tail(do_G.desc->value.obj, _elt_list);

	if (do_G.debug_level & DO_DEBUG_ELEMENTS) {
		do_dump_desc(do_G.desc);
	}
}

/* do_add_host_desc() }}} */
/* do_dump_desc() {{{ */

/******************************************************************************

                         DO_DUMP_DESC

******************************************************************************/
void do_dump_desc(rpn_elt *elt)
{
     rpn_dump_elt(elt, 1);
}

/* do_dump_desc() }}} */
/* do_dump_host_desc() {{{ */

/******************************************************************************

                         DO_DUMP_HOST_DESC

******************************************************************************/
void do_dump_host_desc(do_iperf_host_desc *host_desc)
{
	if (do_G.debug_level & DO_DEBUG_ELEMENTS) {
		rpn_dump_elt(host_desc->elt_server,       0);
		rpn_dump_elt(host_desc->list_client_opts, 0);
		rpn_dump_elt(host_desc->list_server_opts, 0);
		rpn_dump_elt(host_desc->list_server_port, 0);
	}
}

/* do_dump_host_desc() }}} */
/* do_empty_list() {{{ */

/******************************************************************************

                         DO_EMPTY_LIST

******************************************************************************/
rpn_elt *do_empty_list()
{
     rpn_elt                       *_elt_list, *_elt_str;

     /* Allocate a new list
        ~~~~~~~~~~~~~~~~~~~ */
     _elt_list                = rpn_new_elt(RPN_TYPE_LIST);
     _elt_list->value.obj     = rpn_new_list(strdup("empty list"));

     _elt_str                 = rpn_new_elt(RPN_TYPE_STRING);
     _elt_str->value.s        = strdup("");

     rpn_list_push_tail(_elt_list->value.obj, _elt_str);

     return _elt_list;
}

/* do_empty_list() }}} */
/* do_list_label() {{{ */

/******************************************************************************

                         DO_LIST_LABEL

******************************************************************************/
char *do_list_label(int label_idx)
{
     static char          *_labels[] = {
                              "Client options",   /* 0 */
                              "Server options",   /* 1 */
                              "Server port",      /* 2 */
                              "* partial *",      /* 3 */
                              0 };                /* 4 */
     if (label_idx > (sizeof(_labels) / sizeof(_labels[0]))) {
          RPN_INTERNAL_ERROR;
     }

     return _labels[label_idx];
}
/* do_list_label() }}} */
/* do_list_to_argp() {{{ */

/******************************************************************************

					DO_LIST_TO_ARGP

******************************************************************************/
char **do_list_to_argp(rpn_list *list)
{
	char				**_argp;
	rpn_elt			 *_elt;
	cc_uint32			  _i, _size;
	int				  _type;

	/* Verify that the list is homogeneous
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (!list->homogeneous) {
		fprintf(stderr, "%s: %s() : list is not homogeneous !\n",
		        G.progname, __func__);
		RPN_INTERNAL_ERROR;
	}

	/* Verify that the type of all element is STRING
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	_elt				= list->top_elt;
	if ((_type = rpn_get_type(_elt)) != RPN_TYPE_STRING) {
		fprintf(stderr, "%s: %s() : bad element type (%s) !\n",
		        G.progname, __func__, rpn_type_to_string(_type));
		RPN_INTERNAL_ERROR;
	}

	/* Allocate memory for _argp[]
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	_size			= (list->num_elts + 1) * sizeof(char *);
	if ((_argp = (char **) RPN_MALLOC(_size)) == 0) {
		rpn_err_msg_no_mem();
		exit(RPN_EXIT_NO_MEM);
	}

	_i				= 0;
	for (_elt = list->top_elt; _elt != 0; _elt = _elt->previous_elt) {
		_argp[_i++]		= strdup(_elt->value.s);
	}
	_argp[_i]			= 0;

	return _argp;

}

/* do_list_to_argp() }}} */
/* do_gen_matrix() {{{ */

/******************************************************************************

					DO_GEN_MATRIX

******************************************************************************/
rpn_elt *do_gen_matrix(do_host_desc *hosts_desc_list,
                       do_host_desc *deflt_params, int N)
{
	int					 _n, _p, _retcode;
	do_host_desc			*_h_client, *_h_server;
	rpn_stack				*_stack_main;
	rpn_elt				*_elt_mat, *_elt_N;
	char					**_argp_client, **_argp_server, **_argp_port;
	dl_operator			*_op;

	/* Allocate temporary stack
	   ~~~~~~~~~~~~~~~~~~~~~~~~ */
	_stack_main			= rpn_new_stack("iperf_matrix_main");

	/* Generate the matrix components
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	for (_n = 0; _n < N; _n++) {
		for (_p = 0; _p < N; _p++) {
			rpn_elt				*_elt_matrix;
			char					*_opts_server, *_opts_client, *_opts_port,
								*_opts_timeout;;
			rpn_stack				*_stack;

			_stack				= rpn_new_stack("iperf_matrix_loop");

			if (_n == _p) {
				/* Element on the diagonal
				   ~~~~~~~~~~~~~~~~~~~~~~~ */
				_elt_matrix		= rpn_new_elt(RPN_TYPE_NIL);
			}
			else {
				int				 _lg_opts_client, _lg_opts_server, _lg_opts_port,
								 _lg_server, _size_server, _size_client;
				rpn_elt			*_elt_string_server, *_elt_string_client,
								*_elt_cmd_server, *_elt_cmd_client;
				char				*_cmd_server, *_cmd_client;

				_h_client			= &hosts_desc_list[_n];
				_h_server			= &hosts_desc_list[_p];

//				printf("(%-10s, %-10s)\n", _h_client->server, _h_server->server);

				_argp_client		= do_get_client_argp(_h_client, deflt_params);
				_argp_server		= do_get_server_argp(_h_server, deflt_params);
				_argp_port		= do_get_port_argp(_h_server,   deflt_params);

				_opts_client		= do_argp_to_string(_argp_client);
				_opts_server		= do_argp_to_string(_argp_server);
				_opts_port		= do_argp_to_string(_argp_port);
				_opts_timeout		= do_get_timeout();

				_lg_opts_client	= strlen(_opts_client);
				_lg_opts_server	= strlen(_opts_server);
				_lg_opts_port		= strlen(_opts_port);

				_lg_server		= strlen(_h_server->server);

				_size_client		= 128 + _lg_server + _lg_opts_client + _lg_opts_port;
				_size_server		= 128              + _lg_opts_server + _lg_opts_port;

				if ((_cmd_server = (char *) RPN_MALLOC(_size_server)) == 0) {
					rpn_err_msg_no_mem();
					exit(RPN_EXIT_NO_MEM);
				}

				if ((_cmd_client = (char *) RPN_MALLOC(_size_client)) == 0) {
					rpn_err_msg_no_mem();
					exit(RPN_EXIT_NO_MEM);
				}

				/* Generate STRING element for client hostname
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				_elt_string_client			= rpn_new_elt(RPN_TYPE_STRING);
				_elt_string_client->value.s	= strdup(_h_client->server);

				/* Generate command for the client
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//				sprintf(_cmd_client, "iperf -c %s %s %s | tee -a /tmp/rpn_iperf.out",
//				        _h_server->server, _opts_client, _opts_port);        // XXX : tee !!! XXX
				sprintf(_cmd_client, "iperf -c %s %s %s",
				        _h_server->server, _opts_client, _opts_port);
				_elt_cmd_client			= rpn_new_elt(RPN_TYPE_STRING);
				_elt_cmd_client->value.s		= _cmd_client;

				/* Generate pair element for the client
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				rpn_push(_stack, _elt_string_client);
				rpn_push(_stack, _elt_cmd_client);

				(*do_ops.pair->func)(_stack, do_ops.pair);

				/* Generate STRING element for server hostname
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				_elt_string_server			= rpn_new_elt(RPN_TYPE_STRING);
				_elt_string_server->value.s	= strdup(_h_server->server);

				/* Generate command for the server
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//				sprintf(_cmd_server, "iperf -s %s %s > /dev/null 2>&1 & echo $!",
				sprintf(_cmd_server, "iperf -s %s %s & sleep %s; kill $!",
				        _opts_server, _opts_port, _opts_timeout);
				_elt_cmd_server			= rpn_new_elt(RPN_TYPE_STRING);
				_elt_cmd_server->value.s		= _cmd_server;


				/* Generate pair element for the server
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				rpn_push(_stack, _elt_string_server);
				rpn_push(_stack, _elt_cmd_server);

				(*do_ops.pair->func)(_stack, do_ops.pair);

				/* Generate pair for the matrix element
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				(*do_ops.pair->func)(_stack, do_ops.pair);

				_elt_matrix				= rpn_pop(_stack);
			}

			rpn_push(_stack_main, _elt_matrix);

			rpn_free_stack(_stack);
		}
	}

	_elt_N					= rpn_new_elt(RPN_TYPE_INT);
	_elt_N->value.i			= N;

	rpn_push(_stack_main, rpn_clone_elt(_elt_N));
	rpn_push(_stack_main, _elt_N);

#if 0
	/* DEBUG : stk
	   ~~~~~~~~~~~ */
     if ((_retcode = (*do_ops.stk->func)(_stack_main, do_ops.stk)) != 0) {
          rpn_err_msg_op_error(do_ops.stk->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }
#endif

	/* Generate matrix
	   ~~~~~~~~~~~~~~~ */
//printf("_stack_main->num_elts = %d\n", _stack_main->num_elts);
     if ((_op = rpn_search_op(_stack_main, RPN_OP_NAME_MATRIX)) == 0) {
		exit(RPN_EXIT_OP_NOT_FOUND);
//		_retcode							= RPN_RET_OP_NOT_FOUND;
//		goto end;
     }
     if ((_retcode = (*_op->func)(_stack_main, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

	_elt_mat					= rpn_pop(_stack_main);
	rpn_set_elt_name(_elt_mat, DO_LABEL_GLOBAL_RESULT);

	if (do_G.debug_level & DO_DEBUG_ELEMENTS) {
		rpn_dump_elt(_elt_mat, 0);
	}

	rpn_free_stack(_stack_main);

	return _elt_mat;
}


/* do_gen_matrix() }}} */
/* do_get_client_argp() {{{ */

/******************************************************************************

					DO_GET_CLIENT_ARGP

******************************************************************************/
char **do_get_client_argp(do_host_desc *host, do_host_desc *deflt_params)
{

	char					**_client_argp;

	/* Are there specific options for the host ?
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (host->list_client_opts    != 0
	&&  host->list_client_opts[0] != 0
	&& *host->list_client_opts[0] != 0) {
		_client_argp			= host->list_client_opts;
	}
	else {
		/* Are there default options ?
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		if (deflt_params->list_client_opts    != 0
		&&  deflt_params->list_client_opts[0] != 0
		&& *deflt_params->list_client_opts[0] != 0) {
			_client_argp			= deflt_params->list_client_opts;
		}
		else {
			/* No options specified for this host
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			fprintf(stderr, "%s: no options specified for client \"%s\"!\n",
			       G.progname, host->server);
			exit(RPN_EXIT_MISSING_PARAM);
		}
	}

	return _client_argp;
}

/* do_get_client_argp() }}} */
/* do_get_server_argp() {{{ */

/******************************************************************************

					DO_GET_SERVER_ARGP

******************************************************************************/
char **do_get_server_argp(do_host_desc *host, do_host_desc *deflt_params)
{

	char					**_server_argp;

	/* Are there specific options for the host ?
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (host->list_server_opts    != 0
	&&  host->list_server_opts[0] != 0
	&& *host->list_server_opts[0] != 0) {
		_server_argp			= host->list_server_opts;
	}
	else {
		/* Are there default options ?
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		if (deflt_params->list_server_opts    != 0
		&&  deflt_params->list_server_opts[0] != 0
		&& *deflt_params->list_server_opts[0] != 0) {
			_server_argp			= deflt_params->list_server_opts;
		}
		else {
			/* No options specified for this host
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			fprintf(stderr, "%s: no options specified for server \"%s\"!\n",
			       G.progname, host->server);
			exit(RPN_EXIT_MISSING_PARAM);
		}
	}

	return _server_argp;
}

/* do_get_server_argp() }}} */
/* do_get_port_argp() {{{ */

/******************************************************************************

					DO_GET_PORT_ARGP

******************************************************************************/
char **do_get_port_argp(do_host_desc *host, do_host_desc *deflt_params)
{
	char					**_port_argp;

	/* Is there a specific port for the host ?
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (host->list_server_port    != 0
	&&  host->list_server_port[0] != 0
	&& *host->list_server_port[0] != 0) {
		_port_argp			= host->list_server_port;
	}
	else {
		/* Is there a default port ?
		   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
		if (deflt_params->list_server_port    != 0
		&&  deflt_params->list_server_port[0] != 0
		&& *deflt_params->list_server_port[0] != 0) {
			_port_argp			= deflt_params->list_server_port;
		}
		else {
			/* No port specified for this server
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			fprintf(stderr, "%s: no port specified for \"%s\"!\n",
			       G.progname, host->server);
			exit(RPN_EXIT_MISSING_PARAM);
		}
	}

	return _port_argp;
}

/* do_get_port_argp() }}} */
/* do_new_host_desc() {{{ */

/******************************************************************************

					DO_NEW_HOST_DESC

******************************************************************************/
do_host_desc *do_new_host_desc()
{
	do_host_desc			*_host_desc;

	if ((_host_desc = (do_host_desc *) RPN_MALLOC(sizeof(*_host_desc)))== 0) {
		rpn_err_msg_no_mem();
		exit(RPN_EXIT_NO_MEM);
	}

	_host_desc->server				= 0;
	_host_desc->list_client_opts		= 0;
	_host_desc->list_server_opts		= 0;
	_host_desc->list_server_port		= 0;

	return _host_desc;
}

/* do_new_host_desc() }}} */
/* do_disp_argp() {{{ */

/******************************************************************************

					DO_DISP_ARGP

******************************************************************************/
void do_disp_argp(char **argp)
{
	char					*_p, **_argp, *_str;
	int					 _size = 0;

	/* Compute the size of the resulting string
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	_argp				= argp;
	for (_p = *_argp; _p != 0; _p = *(++_argp)) {
		_size				+= strlen(_p) + 1;
	}

	/* Allocate memory for the string
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if ((_str = (char *) RPN_MALLOC(_size + 1)) == 0) {
		rpn_err_msg_no_mem();
		exit(RPN_EXIT_NO_MEM);
	}
	_str[0]				= 0;

	/* Generate the string
	   ~~~~~~~~~~~~~~~~~~~ */
	_argp				= argp;
	for (_p = *_argp; _p != 0; _p = *(++_argp)) {
		strcat(_str, _p);
		strcat(_str, " ");
	}

	_str[strlen(_str) - 1]	= 0;

	printf("          STR : [%s]\n", _str);
}

/* do_disp_argp() }}} */
/* do_argp_to_string() {{{ */

/******************************************************************************

					DO_ARGP_TO_STRING

******************************************************************************/
char *do_argp_to_string(char **argp)
{
	char					*_p, **_argp, *_str;
	int					 _size = 0;

	/* Compute the size of the resulting string
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	_argp				= argp;
	for (_p = *_argp; _p != 0; _p = *(++_argp)) {
		_size				+= strlen(_p) + 1;
	}

	/* Allocate memory for the string
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if ((_str = (char *) RPN_MALLOC(_size + 1)) == 0) {
		rpn_err_msg_no_mem();
		exit(RPN_EXIT_NO_MEM);
	}
	_str[0]				= 0;

	/* Generate the string
	   ~~~~~~~~~~~~~~~~~~~ */
	_argp				= argp;
	for (_p = *_argp; _p != 0; _p = *(++_argp)) {
		strcat(_str, _p);
		strcat(_str, " ");
	}

	/* Remove trailing space
	   ~~~~~~~~~~~~~~~~~~~~~ */
	_str[strlen(_str) - 1]	= 0;

	return _str;
}

/* do_argp_to_string() }}} */
/* do_get_timeout() {{{ */

/******************************************************************************

					DO_GET_TIMEOUT

******************************************************************************/
char *do_get_timeout()
{
	char					*_timeout;

	if (do_G.server_timeout == 0) {
		_timeout			= DO_DEFLT_TIMEOUT;
	}
	else {
		_timeout			= do_G.server_timeout->value.s;
	}

	return _timeout;
}

/* do_get_timeout() }}} */

