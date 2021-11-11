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
 *   @(#)  [MB] cy_rpn_header.h Version 1.97 du 21/11/11 - 
 */

#if ! defined(_RPN_HEADER_H)
# define  _RPN_HEADER_H

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <regex.h>
#include  "cy_rpn_types.h"
#include  "../dl/dl_cpub.h"
#include  "../ci/ci_cpub.h"

#define   RPN_VERSION    "2.0"     // Version with dynamic modules

/* Generic parameters {{{ */
#define	RPN_ENV_LIBPATH		"RPN_LIBPATH"
#define	RPN_DEFLT_LIBPATH		"/usr/local/rpn/modules"

#define	RPN_DISP_SZ			(10)
/* Generic parameters }}} */
/* Debug macros {{{
   ~~~~~~~~~~~~~~~~ */
#define   X                        fprintf(stdout, "%s (%d)\n", __FILE__, __LINE__);
#define   Y                        { if (G.debug_level & RPN_DBG_DEBUG) fprintf(stdout, "DEBUG : %s() : %s (%d)\n", __func__, __FILE__, __LINE__); }
#define   Z                        fprintf(stderr, "DEBUG : %s() : %s (%d)\n", __func__, __FILE__, __LINE__);

#define   M                        fprintf(stdout, "%s (%d) CURRENT : %10lld TOTAL : %10lld PEAK : %10lld NB ALLOC : %10lld NB FREE : %10lld\n",\
                                           __FILE__, __LINE__,   \
                                           G.allocated_current,  \
                                           G.allocated_total,    \
                                           G.allocated_peak,     \
                                           G.allocated_nb,       \
                                           G.free_nb);           \

#define   RPN_INTERNAL_ERROR       rpn_internal_error(__func__, __FILE__, __LINE__)
#define	RPN_UNIMPLEMENTED		rpn_unimplemented(op->op_name, __func__, __FILE__, __LINE__)
#if 0
#define   RPN_MALLOC(size)         (printf("MALLOC(%6lu) %s (%d) [%s]\n", size, __FILE__, __LINE__, __func__),  \
                                   rpn_malloc(size))
#define   RPN_FREE(mem)            (printf("FREE          %s (%d) [%s]\n", __FILE__, __LINE__, __func__),  \
                                   rpn_free(mem))
#define	RPN_STRDUP(str)		(printf("STRDUP(%s)        %s (%d) [%s]\n", str, __FILE__, __LINE__, __func__),  \
							rpn_strdup(str))
#else
#define   RPN_MALLOC(size)         rpn_malloc(size)
#define   RPN_FREE(mem)            rpn_free(mem)
#define	RPN_STRDUP(str)		rpn_strdup(str)
#endif

#define   RPN_TRACE_LEX(...)            { if (G.debug_level & RPN_DBG_LEX)  printf("LEX  : " __VA_ARGS__); }
#define   RPN_TRACE_YACC(...)           { if (G.debug_level & RPN_DBG_YACC) printf("YACC : " __VA_ARGS__); }
#define   RPN_TRACE_VERBOSE(...)        { if (G.debug_level & RPN_DBG_VERBOSE) printf(__VA_ARGS__); }

/* Debug macros }}} */
/* Generic macros {{{
   ~~~~~~~~~~~~~~ */
#if ! defined(MAX)
#define   MAX(a, b)                     ((a) < (b) ? (b) : (a))
#endif
#if ! defined(MIN)
#define   MIN(a, b)                     ((a) < (b) ? (a) : (b))
#endif

#if defined(__GNUC__)
#define likely(expr)				__builtin_expect(!!(expr), 1)
#define unlikely(expr)				__builtin_expect((expr), 0)
#else
#define likely(expr)				(expr)
#define unlikely(expr)				(expr)
#endif

/* ... to define functions and methods
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define	RPN_DEFN_INIT(PREF)			int   PREF ## _init(dl_module *module_desc) \
{													\
	int					*_p;							\
													\
	for (_p = PREF ## _managed_types; *_p != 0; _p++) {		\
		rpn_methods[*_p]		= &PREF ## _methods;		\
	}												\
													\
	return TRUE;										\
}

#define	RPN_DEFN_METHODS(PREF)		struct rpn_methods PREF ##_methods = {	\
	PREF ## _disp_elt,			\
	PREF ## _clone_elt,			\
	PREF ## _type_to_string,		\
	PREF ## _free_elt			\
}

#define	RPN_DEFN_DISP(PREF)			void  PREF ## _disp_elt(rpn_elt *elt, int disp_flags)
#define	RPN_DEFN_CLONE(PREF)		void  PREF ## _clone_elt(rpn_elt *elt, rpn_elt *clone)
#define	RPN_DEFN_TYPE_TO_STR(PREF)	char *PREF ## _type_to_string(int type)
#define	RPN_DEFN_FREE(PREF)			void  PREF ## _free_elt(rpn_elt *elt, int type)

/* ... to declare functions and methods
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define	RPN_DECL_INIT(PREF)			int			 PREF ## _init(dl_module *);

#define	RPN_DECL_METHODS(PREF)		void			 PREF ## _disp_elt(rpn_elt *, int);	\
	void			 PREF ## _clone_elt(rpn_elt *, rpn_elt *);		\
	char			*PREF ## _type_to_string(int);		\
	void			 PREF ## _free_elt(rpn_elt *, int);

/* Generic macros }}} */
/* Boolean values {{{
   ~~~~~~~~~~~~~~ */
#define   BOOL_UNDEF                    (-1)
// #define   FALSE                         (0)
// #define   TRUE                          (1)

/* Boolean values }}} */
/* Exit codes {{{
   ~~~~~~~~~~ */
#define   RPN_EXIT_OK                   (0)
#define   RPN_EXIT_USAGE                (1)
#define   RPN_EXIT_NO_MEM               (2)
#define   RPN_EXIT_OP_UNKNOWN           (3)
#define   RPN_EXIT_OP_ERROR             (4)
#define   RPN_EXIT_INVALID_ELT          (5)
#define   RPN_EXIT_SYNTAX               (6)
#define   RPN_EXIT_UNIMPLEMENTED        (7)
#define   RPN_EXIT_READ_ERROR           (8)
#define   RPN_EXIT_SIGNAL_ERROR         (9)
#define   RPN_EXIT_MKDIR_ERROR         (10)
#define   RPN_EXIT_WGET_ERROR          (11)
#define   RPN_EXIT_GETCWD_ERROR        (12)
#define   RPN_EXIT_CHDIR_ERROR         (13)
#define   RPN_EXIT_SYSTEM_ERROR        (14)
#define   RPN_EXIT_GET_TIME            (15)
#define   RPN_EXIT_OPEN_ERROR          (16)
#define   RPN_EXIT_SEGV                (17)
#define   RPN_EXIT_PIPE_ERROR          (18)
#define   RPN_EXIT_FORK_ERROR          (19)
#define   RPN_EXIT_EXEC_ERROR          (20)
#define   RPN_EXIT_LINK_ERROR          (21)
#define   RPN_EXIT_NONEXISTENT         (22)
#define   RPN_EXIT_DUPLICATE           (23)
#define   RPN_EXIT_MISSING_PARAM       (24)
#define   RPN_EXIT_OP_NOT_FOUND        (25)
#define   RPN_EXIT_REGCOMP_ERROR       (26)
#define   RPN_EXIT_INTERNAL_ERR       (126)

/* }}} */
/* Return codes {{{
   ~~~~~~~~~~~~ */
#define   RPN_RET_INVALID_ELT          (-2)
#define   RPN_RET_ERROR                (-1)
#define   RPN_RET_OK                    (0)
#define   RPN_RET_INVALID_STACK         (1)
#define   RPN_RET_INVALID_STR           (2)
#define   RPN_RET_INVALID_X             (3)
#define   RPN_RET_INVALID_Y             (4)
#define   RPN_RET_INVALID_Z             (5)
#define   RPN_RET_INVALID_X_TYPE        (6)
#define   RPN_RET_INVALID_Y_TYPE        (7)
#define   RPN_RET_INVALID_Z_TYPE        (8)
#define   RPN_RET_INVALID_TYPE          (9)
#define   RPN_RET_INVALID_TYPES        (10)
#define   RPN_RET_TOO_BIG              (11)
#define   RPN_RET_NOT_ENOUGH_ELTS      (12)
#define   RPN_RET_INCOMPATIBLE_ELTS    (13)
#define   RPN_RET_INCOMPATIBLE_DIM     (14)
#define   RPN_RET_NO_START_MARKER      (15)
#define   RPN_RET_NO_MORE_ELT          (16)
#define   RPN_RET_INVALID_INDEX        (17)
#define   RPN_RET_INVALID_LIST         (18)
#define   RPN_RET_NOT_HOMOGENEOUS      (19)
#define   RPN_RET_DUPLICATE            (20)
#define   RPN_RET_NONEXISTENT          (21)
#define   RPN_RET_NOT_READABLE         (22)
#define   RPN_RET_CANNOT_LINK          (23)
#define   RPN_RET_NOT_FOUND            (24)
#define   RPN_RET_OP_NOT_FOUND         (25)
#define   RPN_RET_OPEN_ERROR           (26)
#define   RPN_RET_INTERNAL_ERROR      (127)

/* }}} */
/* Debug levels {{{ */
#define   RPN_DBG_LEX                   (0x0001)
#define   RPN_DBG_YACC                  (0x0002)
#define   RPN_DBG_LEX_2                 (0x0004)
#define   RPN_DBG_YACC_2                (0x0008)
#define   RPN_DBG_DYN_MODULE            (0x0010)
#define   RPN_DBG_DYN_OP                (0x0020)
#define   RPN_DBG_VERBOSE               (0x0040)
#define   RPN_DBG_DEBUG                 (0x0080)
#define	RPN_DBG_DISP_CMDS			(0x0100)

/* Debug levels }}} */
/* Display flags {{{ */
#define   RPN_DISP_NIL                  (0x0000)
#define   RPN_DISP_NEWLINE              (0x0001)
#define   RPN_DISP_NO_TYPE              (0x0002)
#define   RPN_DISP_VALUE                (RPN_DISP_NO_TYPE)
#define   RPN_DISP_SYNTH                (0x0004)
#define   RPN_DISP_INFOS                (0x0008)

/* Display flasg }}} */
/* ELEMENTS TYPES {{{
   ~~~~~~~~~~~~~~ */
#define   RPN_TYPE_UNDEFINED            (0)
#define   RPN_TYPE_UNUSED               (1)
#define   RPN_TYPE_ANY_TYPE             (2)
#define   RPN_TYPE_NIL                  (3)
#define   RPN_TYPE_INT                  (4)
#define   RPN_TYPE_FLOAT                (5)
#define   RPN_TYPE_DOUBLE               (6)
#define   RPN_TYPE_STRING               (7)
#define   RPN_TYPE_VECTOR_3             (8)
#define   RPN_TYPE_MATRIX               (9)
#define   RPN_TYPE_LITTERAL            (10)
#define   RPN_TYPE_START_MARKER        (11)
#define   RPN_TYPE_NEURAL_MLP          (12)
#define   RPN_TYPE_FILENAME            (13)
#define   RPN_TYPE_TRAINING_ELT        (14)
#define   RPN_TYPE_TRAINING_SET        (15)
#define   RPN_TYPE_TEST_ELT            (16)
#define   RPN_TYPE_TEST_SET            (17)
#define   RPN_TYPE_TEXT_FILE           (18)
#define   RPN_TYPE_LIST                (19)
#define   RPN_TYPE_TUPLE               (20)
#define   RPN_TYPE_OPAIR               (21)
#define   RPN_TYPE_TEXT                (22)
#define   RPN_TYPE_CLONE               (23)
#define   RPN_TYPE_COEF_A_B            (24)
#define   RPN_TYPE_MIN_MAX             (25)
#define   RPN_TYPE_IPv4                (26)
#define   RPN_TYPE_REGEX               (27)
#define   RPN_TYPE_HOSTSFILE           (28)
#define   RPN_TYPE_HOSTS               (29)

#define	RPN_MAX_TYPE				(RPN_TYPE_HOSTS)

/* }}} */
/* Operators names {{{
   ~~~~~~~~~~~~~~~ */
#define   RPN_OP_NAME_NULL              RPN_OP_NAME_UNDEF
#define   RPN_OP_NAME_UNDEF             "UNDEFINED OPERATOR NAME"

// RPN config
#define   RPN_OP_NAME_SW_ON             "sw_on"
#define   RPN_OP_NAME_SW_OFF            "sw_off"

// Stack
#define   RPN_OP_NAME_ENTER             "enter"
#define   RPN_OP_NAME_CLX               "clx"
#define   RPN_OP_NAME_DELX              "delx"
#define   RPN_OP_NAME_DEL_L             "del_l"
#define   RPN_OP_NAME_CLST              "clst"
#define   RPN_OP_NAME_SWAP_XY           "x<>y"
#define   RPN_OP_NAME_SWAP_XZ           "x<>z"
#define   RPN_OP_NAME_SWAP_XT           "x<>t"
#define   RPN_OP_NAME_SWAP_XL           "x<>l"
#define   RPN_OP_NAME_RDN               "rdn"
#define   RPN_OP_NAME_RUP               "rup"
#define   RPN_OP_NAME_LASTX             "lastx"
#define   RPN_OP_NAME_RCL_X             "rcl_x"
#define   RPN_OP_NAME_PRX               "prx"
#define   RPN_OP_NAME_PRSTK             "prstk"
#define   RPN_OP_NAME_STK               "stk"
#define   RPN_OP_NAME_DUPX              "dupx"
#define   RPN_OP_NAME_DUMP              "dump"
#define   RPN_OP_NAME_CATALOG           "catalog"
#define   RPN_OP_NAME_MODULES           "modules"

// Lists
#define   RPN_OP_NAME_PUSH              "push"
#define   RPN_OP_NAME_POP               "pop"
#define   RPN_OP_NAME_PUSH_EXTRACT      "push_extract"
#define   RPN_OP_NAME_POP_INSERT        "pop_insert"
#define   RPN_OP_NAME_GET_HEAD          "get_head"
#define   RPN_OP_NAME_GET_TAIL          "get_tail"
#define   RPN_OP_NAME_GET               "get"
#define   RPN_OP_NAME_GET_IDX           "get_idx"
#define   RPN_OP_NAME_GET_IGNORED       "get_ignored"
#define   RPN_OP_NAME_GET_USED          "get_used"
#define   RPN_OP_NAME_SET_IDX           "set_idx"
#define   RPN_OP_NAME_LIST              "list"

// Tuples
#define   RPN_OP_NAME_TUPLE             "tuple"
#define   RPN_OP_NAME_NTUPLE            "ntuple"
#define   RPN_OP_NAME_XTUPLE            "Xtuple"

// Pairs
#define   RPN_OP_NAME_OPAIR             "opair"   // Ordered pair

// Tests
#define   RPN_OP_NAME_TEST_LT           "lt"      // x<y?
#define   RPN_OP_NAME_TEST_LE           "le"      // x<=y?
#define   RPN_OP_NAME_TEST_GE           "ge"      // x>=y?
#define   RPN_OP_NAME_TEST_GT           "gt"      // x>y?
#define   RPN_OP_NAME_TEST_EQ           "eq"      // x=y?
#define   RPN_OP_NAME_TEST_NE           "ne"      // x!=y?

#define   RPN_OP_NAME_TEST_LTZ          "ltz"
#define   RPN_OP_NAME_TEST_LEZ          "lez"
#define   RPN_OP_NAME_TEST_GEZ          "gez"
#define   RPN_OP_NAME_TEST_GTZ          "gtz"
#define   RPN_OP_NAME_TEST_EQZ          "eqz"
#define   RPN_OP_NAME_TEST_NEZ          "nez"

// Math
#define   RPN_OP_NAME_ADD               "+"
#define   RPN_OP_NAME_SUB               "-"
#define   RPN_OP_NAME_MUL               "*"
#define   RPN_OP_NAME_DIV               "/"
#define   RPN_OP_NAME_MATH_MUL          "m*"
#define   RPN_OP_NAME_RECIPROCAL        "1/x"
#define   RPN_OP_NAME_POWER             "y^x"
#define   RPN_OP_NAME_SQRT              "sqrt"
#define   RPN_OP_NAME_SQUARE            "x^2"
#define   RPN_OP_NAME_LN                "ln"
#define   RPN_OP_NAME_EXP               "exp"
#define   RPN_OP_NAME_LOG10             "log"
#define   RPN_OP_NAME_10_POWER_X        "10^x"
#define   RPN_OP_NAME_PERCENT           "%"
#define   RPN_OP_NAME_DELTA_PERCENT     "delta%"
#define   RPN_OP_NAME_SINE              "sin"
#define   RPN_OP_NAME_COSINE            "cos"
#define   RPN_OP_NAME_TANGENT           "tan"
#define   RPN_OP_NAME_ASINE             "asin"
#define   RPN_OP_NAME_ACOSINE           "acos"
#define   RPN_OP_NAME_ATANGENT          "atan"
#define   RPN_OP_NAME_SINH              "sinh"
#define   RPN_OP_NAME_COSH              "cosh"
#define   RPN_OP_NAME_TANH              "tanh"
#define   RPN_OP_NAME_ASINH             "asinh"
#define   RPN_OP_NAME_ACOSH             "acosh"
#define   RPN_OP_NAME_ATANH             "atanh"
#define   RPN_OP_NAME_SIGMOID           "sigmoid"
#define   RPN_OP_NAME_MATRIX            "mat"
#define   RPN_OP_NAME_EXPLODE           "expl"
#define   RPN_OP_NAME_VEC2              "vec2"
#define   RPN_OP_NAME_VEC3              "vec3"
#define   RPN_OP_NAME_TRANSPOSE         "t"
#define   RPN_OP_NAME_CROSS_PRODUCT     "^"
#define   RPN_OP_NAME_SCALAR_MATRIX     "scalar_matrix"
#define   RPN_OP_NAME_SEQ               "seq"
#define   RPN_OP_NAME_DIM               "dim"
#define   RPN_OP_NAME_CHANGE_SIGN       "chs"
#define   RPN_OP_NAME_H_CAT             "hcat"
#define   RPN_OP_NAME_V_CAT             "vcat"
#define   RPN_OP_NAME_Z_MAT             "zmat"
#define   RPN_OP_NAME_H_REV             "hrev"
#define   RPN_OP_NAME_V_REV             "vrev"
#define   RPN_OP_NAME_DIAG_MAT          "diagmat"
#define   RPN_OP_NAME_V_PAD             "vpad"
#define   RPN_OP_NAME_MAT_NIL_DIAG      "mat_nil_diag"

// Strings
#define   RPN_OP_NAME_STR_LENGTH        "length"
#define   RPN_OP_NAME_STR_CATENATE      "cat"
#define   RPN_OP_NAME_STR_DUPL          "dupl"


// Statistics
#define   RPN_OP_NAME_SUM_ADD           "sigma+"
#define   RPN_OP_NAME_SUM_SUB           "sigma-"
#define   RPN_OP_NAME_MEAN              "mean"
#define   RPN_OP_NAME_SDEV              "sdev"
#define   RPN_OP_NAME_MEANX             "meanx"
#define   RPN_OP_NAME_MMM               "mmm"                    /* Min, Max, Mean */
#define   RPN_OP_NAME_LINE_DATA_SET     "line_set"
#define   RPN_OP_NAME_EXP_DATA_SET      "exp_set"
#define   RPN_OP_NAME_LOG_DATA_SET      "log_set"
#define   RPN_OP_NAME_POW_DATA_SET      "pow_set"

// Neural Networks
#define   RPN_OP_NAME_NN_CREAT_MLP      "create_mlp"
#define   RPN_OP_NAME_NN_LEARN          "learn"
#define   RPN_OP_NAME_NN_ANALYZE        "classify"
#define   RPN_OP_NAME_NN_SET_LAYER_W    "set_layer_weights"
#define   RPN_OP_NAME_NN_ALEA_LAYER_W   "alea_layer_weights"
#define   RPN_OP_NAME_NN_SET_MLP_W      "set_mlp_weights"
#define   RPN_OP_NAME_NN_ALEA_MLP_W     "alea_mlp_weights"
#define   RPN_OP_NAME_NN_READ_MNIST     "read_mnist"
#define   RPN_OP_NAME_NN_MNIST_PIC      "mnist_pic"
#define   RPN_OP_NAME_NN_MNIST_PICS     "mnist_pics"
#define   RPN_OP_NAME_NN_READ_ELT       "read_elt"
#define   RPN_OP_NAME_NN_MAT_7_SEG      "mat7seg"
#define   RPN_OP_NAME_NN_LABEL          "label"
#define   RPN_OP_NAME_NN_INDEX          "index"
#define   RPN_OP_NAME_NN_ORIG_INDEX     "orig_index"
#define   RPN_OP_NAME_NN_DISPATCH       "dispatch"
#define   RPN_OP_NAME_NN_IGNORE         "ignore"
#define   RPN_OP_NAME_NN_USE            "use"

// Backup / restore
#define   RPN_OP_NAME_SAVE              "save"
#define   RPN_OP_NAME_LOAD              "load"
#define   RPN_OP_NAME_RESTORE           "restore"
#define   RPN_OP_NAME_AUTOSAVE          "autosave"
#define   RPN_OP_NAME_NO_AUTOSAVE       "no_autosave"
#define   RPN_OP_NAME_WRITE             "write"
#define   RPN_OP_NAME_WRITE_CSV         "write_csv"

// Computer science
#define   RPN_OP_NAME_CS_GEN_C          "gen_C"

// System
#define   RPN_OP_NAME_MEMORY            "mem"
#define   RPN_OP_NAME_PAUSE             "pause"
#define   RPN_OP_NAME_PING              "ping"
#define   RPN_OP_NAME_IPERF             "iperf"

// Types
#define   RPN_OP_NAME_TYPE_FILENAME     "filename"

/* }}} */
/* Operators ID {{{
   ~~~~~~~~~~~~ */
#define   RPN_OP_ID_NULL                (0)

// Undef
#define   RPN_OP_BASE_UNDEF             (0)
#define   RPN_OP_END_UNDEF              RPN_OP_ID_UNDEF

#define   RPN_OP_ID_UNDEF               RPN_OP_ID_NULL

// Config
#define   RPN_OP_BASE_CONFIG            (RPN_OP_END_UNDEF + 1)
#define   RPN_OP_END_CONFIG              RPN_OP_ID_SW_OFF

#define   RPN_OP_ID_SW_ON               (RPN_OP_BASE_CONFIG + 0)
#define   RPN_OP_ID_SW_OFF              (RPN_OP_BASE_CONFIG + 1)

// Stack
#define   RPN_OP_BASE_STACK             (RPN_OP_END_CONFIG + 1)
#define   RPN_OP_END_STACK               RPN_OP_ID_DUMP

#define   RPN_OP_ID_ENTER               (RPN_OP_BASE_STACK + 0)
#define   RPN_OP_ID_CLX                 (RPN_OP_BASE_STACK + 1)
#define   RPN_OP_ID_DELX                (RPN_OP_BASE_STACK + 2)
#define   RPN_OP_ID_DEL_L               (RPN_OP_BASE_STACK + 3)
#define   RPN_OP_ID_CLST                (RPN_OP_BASE_STACK + 4)
#define   RPN_OP_ID_SWAP_XY             (RPN_OP_BASE_STACK + 5)
#define   RPN_OP_ID_SWAP_XZ             (RPN_OP_BASE_STACK + 6)
#define   RPN_OP_ID_SWAP_XT             (RPN_OP_BASE_STACK + 7)
#define   RPN_OP_ID_SWAP_XL             (RPN_OP_BASE_STACK + 8)
#define   RPN_OP_ID_RDN                 (RPN_OP_BASE_STACK + 9)
#define   RPN_OP_ID_RUP                 (RPN_OP_BASE_STACK + 10)
#define   RPN_OP_ID_LASTX               (RPN_OP_BASE_STACK + 11)
#define   RPN_OP_ID_RCL_X               (RPN_OP_BASE_STACK + 12)
#define   RPN_OP_ID_PRX                 (RPN_OP_BASE_STACK + 13)
#define   RPN_OP_ID_PRSTK               (RPN_OP_BASE_STACK + 14)
#define   RPN_OP_ID_STK                 (RPN_OP_BASE_STACK + 15)
#define   RPN_OP_ID_DUPX                (RPN_OP_BASE_STACK + 16)
#define   RPN_OP_ID_DUMP                (RPN_OP_BASE_STACK + 17)

// Lists
#define   RPN_OP_BASE_LISTS             (RPN_OP_END_STACK + 1)
#define   RPN_OP_END_LISTS               RPN_OP_ID_LIST

#define   RPN_OP_ID_PUSH                (RPN_OP_BASE_LISTS + 0)
#define   RPN_OP_ID_POP                 (RPN_OP_BASE_LISTS + 1)
#define   RPN_OP_ID_PUSH_EXTRACT        (RPN_OP_BASE_LISTS + 2)
#define   RPN_OP_ID_POP_INSERT          (RPN_OP_BASE_LISTS + 3)
#define   RPN_OP_ID_GET_HEAD            (RPN_OP_BASE_LISTS + 4)
#define   RPN_OP_ID_GET_TAIL            (RPN_OP_BASE_LISTS + 5)
#define   RPN_OP_ID_GET                 (RPN_OP_BASE_LISTS + 6)
#define   RPN_OP_ID_GET_IDX             (RPN_OP_BASE_LISTS + 7)
#define   RPN_OP_ID_GET_IGNORED         (RPN_OP_BASE_LISTS + 8)
#define   RPN_OP_ID_GET_USED            (RPN_OP_BASE_LISTS + 9)
#define   RPN_OP_ID_SET_IDX             (RPN_OP_BASE_LISTS + 10)
#define   RPN_OP_ID_LIST                (RPN_OP_BASE_LISTS + 11)

// Tuples
//#define   RPN_OP_BASE_TUPLES            (RPN_OP_END_LISTS + 1)
#define   RPN_OP_BASE_TUPLES            (RPN_OP_END_CONFIG + 1)
#define   RPN_OP_END_TUPLES              RPN_OP_ID_XTUPLE

#define   RPN_OP_ID_TUPLE               (RPN_OP_BASE_TUPLES + 0)
#define   RPN_OP_ID_NTUPLE              (RPN_OP_BASE_TUPLES + 1)
#define   RPN_OP_ID_XTUPLE              (RPN_OP_BASE_TUPLES + 2)

// Pairs
#define   RPN_OP_BASE_PAIRS             (RPN_OP_END_TUPLES + 1)
#define   RPN_OP_END_PAIRS               RPN_OP_ID_OPAIR

#define   RPN_OP_ID_OPAIR               (RPN_OP_BASE_PAIRS + 0)

// Tests
#define   RPN_OP_BASE_TESTS             (RPN_OP_END_PAIRS + 1)
#define   RPN_OP_END_TESTS               RPN_OP_ID_TEST_NEZ

#define   RPN_OP_ID_TEST_LT             (RPN_OP_BASE_TESTS + 0)
#define   RPN_OP_ID_TEST_LE             (RPN_OP_BASE_TESTS + 1)
#define   RPN_OP_ID_TEST_GE             (RPN_OP_BASE_TESTS + 2)
#define   RPN_OP_ID_TEST_GT             (RPN_OP_BASE_TESTS + 3)
#define   RPN_OP_ID_TEST_EQ             (RPN_OP_BASE_TESTS + 4)
#define   RPN_OP_ID_TEST_NE             (RPN_OP_BASE_TESTS + 5)
#define   RPN_OP_ID_TEST_LTZ            (RPN_OP_BASE_TESTS + 6)
#define   RPN_OP_ID_TEST_LEZ            (RPN_OP_BASE_TESTS + 7)
#define   RPN_OP_ID_TEST_GEZ            (RPN_OP_BASE_TESTS + 8)
#define   RPN_OP_ID_TEST_GTZ            (RPN_OP_BASE_TESTS + 9)
#define   RPN_OP_ID_TEST_EQZ            (RPN_OP_BASE_TESTS + 10)
#define   RPN_OP_ID_TEST_NEZ            (RPN_OP_BASE_TESTS + 11)

// Math
#define   RPN_OP_BASE_MATH              (RPN_OP_END_TESTS + 1)
#define   RPN_OP_END_MATH                RPN_OP_ID_MAT_NIL_DIAG

#define   RPN_OP_ID_ADD                 (RPN_OP_BASE_MATH + 0)
#define   RPN_OP_ID_SUB                 (RPN_OP_BASE_MATH + 1)
#define   RPN_OP_ID_MUL                 (RPN_OP_BASE_MATH + 2)
#define   RPN_OP_ID_DIV                 (RPN_OP_BASE_MATH + 3)
#define   RPN_OP_ID_MATH_MUL            (RPN_OP_BASE_MATH + 4)
#define   RPN_OP_ID_RECIPROCAL          (RPN_OP_BASE_MATH + 5)
#define   RPN_OP_ID_POWER               (RPN_OP_BASE_MATH + 6)
#define   RPN_OP_ID_SQRT                (RPN_OP_BASE_MATH + 7)
#define   RPN_OP_ID_SQUARE              (RPN_OP_BASE_MATH + 8)
#define   RPN_OP_ID_LN                  (RPN_OP_BASE_MATH + 9)
#define   RPN_OP_ID_EXP                 (RPN_OP_BASE_MATH + 10)
#define   RPN_OP_ID_LOG10               (RPN_OP_BASE_MATH + 11)
#define   RPN_OP_ID_10_POWER_X          (RPN_OP_BASE_MATH + 12)
#define   RPN_OP_ID_PERCENT             (RPN_OP_BASE_MATH + 13)
#define   RPN_OP_ID_DELTA_PERCENT       (RPN_OP_BASE_MATH + 14)
#define   RPN_OP_ID_SINE                (RPN_OP_BASE_MATH + 15)
#define   RPN_OP_ID_COSINE              (RPN_OP_BASE_MATH + 16)
#define   RPN_OP_ID_TANGENT             (RPN_OP_BASE_MATH + 17)
#define   RPN_OP_ID_ASINE               (RPN_OP_BASE_MATH + 18)
#define   RPN_OP_ID_ACOSINE             (RPN_OP_BASE_MATH + 19)
#define   RPN_OP_ID_ATANGENT            (RPN_OP_BASE_MATH + 20)
#define   RPN_OP_ID_SINH                (RPN_OP_BASE_MATH + 21)
#define   RPN_OP_ID_COSH                (RPN_OP_BASE_MATH + 22)
#define   RPN_OP_ID_TANH                (RPN_OP_BASE_MATH + 23)
#define   RPN_OP_ID_ASINH               (RPN_OP_BASE_MATH + 24)
#define   RPN_OP_ID_ACOSH               (RPN_OP_BASE_MATH + 25)
#define   RPN_OP_ID_ATANH               (RPN_OP_BASE_MATH + 26)
#define   RPN_OP_ID_SIGMOID             (RPN_OP_BASE_MATH + 27)
#define   RPN_OP_ID_MATRIX              (RPN_OP_BASE_MATH + 28)
#define   RPN_OP_ID_EXPLODE             (RPN_OP_BASE_MATH + 29)
#define   RPN_OP_ID_VEC2                (RPN_OP_BASE_MATH + 30)
#define   RPN_OP_ID_VEC3                (RPN_OP_BASE_MATH + 31)
#define   RPN_OP_ID_TRANSPOSE           (RPN_OP_BASE_MATH + 32)
#define   RPN_OP_ID_CROSS_PRODUCT       (RPN_OP_BASE_MATH + 33)
#define   RPN_OP_ID_SCALAR_MATRIX       (RPN_OP_BASE_MATH + 34)
#define   RPN_OP_ID_SEQ                 (RPN_OP_BASE_MATH + 35)
#define   RPN_OP_ID_DIM                 (RPN_OP_BASE_MATH + 36)
#define   RPN_OP_ID_CHANGE_SIGN         (RPN_OP_BASE_MATH + 37)
#define   RPN_OP_ID_H_CAT               (RPN_OP_BASE_MATH + 38)
#define   RPN_OP_ID_V_CAT               (RPN_OP_BASE_MATH + 39)
#define   RPN_OP_ID_Z_MAT               (RPN_OP_BASE_MATH + 40)
#define   RPN_OP_ID_H_REV               (RPN_OP_BASE_MATH + 41)
#define   RPN_OP_ID_V_REV               (RPN_OP_BASE_MATH + 42)
#define   RPN_OP_ID_DIAG_MAT            (RPN_OP_BASE_MATH + 43)
#define   RPN_OP_ID_V_PAD               (RPN_OP_BASE_MATH + 44)
#define   RPN_OP_ID_MAT_NIL_DIAG        (RPN_OP_BASE_MATH + 45)

// Neural Networks
#define   RPN_OP_BASE_NEURAL            (RPN_OP_END_MATH + 1)
#define   RPN_OP_END_NEURAL              RPN_OP_ID_NN_USE

#define   RPN_OP_ID_NN_CREAT_MLP        (RPN_OP_BASE_NEURAL + 0)
#define   RPN_OP_ID_NN_LEARN            (RPN_OP_BASE_NEURAL + 1)
#define   RPN_OP_ID_NN_ANALYZE          (RPN_OP_BASE_NEURAL + 2)
#define   RPN_OP_ID_NN_SET_LAYER_W      (RPN_OP_BASE_NEURAL + 3)
#define   RPN_OP_ID_NN_ALEA_LAYER_W     (RPN_OP_BASE_NEURAL + 4)
#define   RPN_OP_ID_NN_SET_MLP_W        (RPN_OP_BASE_NEURAL + 5)
#define   RPN_OP_ID_NN_ALEA_MLP_W       (RPN_OP_BASE_NEURAL + 6)
#define   RPN_OP_ID_NN_READ_MNIST       (RPN_OP_BASE_NEURAL + 7)
#define   RPN_OP_ID_NN_MNIST_PIC        (RPN_OP_BASE_NEURAL + 8)
#define   RPN_OP_ID_NN_MNIST_PICS       (RPN_OP_BASE_NEURAL + 9)
#define   RPN_OP_ID_NN_READ_ELT         (RPN_OP_BASE_NEURAL + 10)
#define   RPN_OP_ID_NN_MAT_7_SEG        (RPN_OP_BASE_NEURAL + 11)
#define   RPN_OP_ID_NN_LABEL            (RPN_OP_BASE_NEURAL + 12)
#define   RPN_OP_ID_NN_INDEX            (RPN_OP_BASE_NEURAL + 13)
#define   RPN_OP_ID_NN_ORIG_INDEX       (RPN_OP_BASE_NEURAL + 14)
#define   RPN_OP_ID_NN_DISPATCH         (RPN_OP_BASE_NEURAL + 15)
#define   RPN_OP_ID_NN_IGNORE           (RPN_OP_BASE_NEURAL + 16)
#define   RPN_OP_ID_NN_USE              (RPN_OP_BASE_NEURAL + 17)

// Backup / restore
#define   RPN_OP_BASE_BACKUP            (RPN_OP_END_NEURAL + 1)
#define   RPN_OP_END_BACKUP              RPN_OP_ID_WRITE_CSV

#define   RPN_OP_ID_SAVE                (RPN_OP_BASE_BACKUP + 0)
#define   RPN_OP_ID_LOAD                (RPN_OP_BASE_BACKUP + 1)
#define   RPN_OP_ID_RESTORE             (RPN_OP_BASE_BACKUP + 2)
#define   RPN_OP_ID_AUTOSAVE            (RPN_OP_BASE_BACKUP + 3)
#define   RPN_OP_ID_NO_AUTOSAVE         (RPN_OP_BASE_BACKUP + 4)
#define   RPN_OP_ID_WRITE               (RPN_OP_BASE_BACKUP + 5)
#define   RPN_OP_ID_WRITE_CSV           (RPN_OP_BASE_BACKUP + 6)

// Conputer science
//#define   RPN_OP_BASE_COMPUTER          (RPN_OP_END_BACKUP + 1)
#define   RPN_OP_BASE_COMPUTER          (RPN_OP_END_PAIRS + 1)
#define   RPN_OP_END_COMPUTER            RPN_OP_ID_CS_GEN_C

#define   RPN_OP_ID_CS_GEN_C            (RPN_OP_BASE_COMPUTER + 0)

// Types
#define   RPN_OP_BASE_TYPES             (RPN_OP_END_COMPUTER + 1)
#define   RPN_OP_END_TYPES               RPN_OP_ID_TYPE_FILENAME

#define   RPN_OP_ID_TYPE_FILENAME       (RPN_OP_BASE_TYPES + 0)

// END OF MODULES GROUPS
#define   RPN_OP_BASE_END_OF_GROUPS     (0)
#define   RPN_OP_END_END_OF_GROUPS      (0)
#define   RPN_MSG_OP_END_OF_GROUPS      (0)

/* }}} */
/* Sigma constants {{{
   ~~~~~~~~~~~~~~~ */
#define   RPN_NB_SIGMA                  (6)

#define   RPN_SIGMA_IDX_SUM_X           (0)
#define   RPN_SIGMA_IDX_SUM_X2          (1)
#define   RPN_SIGMA_IDX_SUM_Y           (2)
#define   RPN_SIGMA_IDX_SUM_Y2          (3)
#define   RPN_SIGMA_IDX_SUM_XY          (4)
#define   RPN_SIGMA_IDX_N               (5)

#define   RPN_OP(fct)                   RPN_OP_NAME_ ## fct, RPN_OP_ID_ ## fct
#define   RPN_OP_DESC(fct)              (&rpn_operators[RPN_OP_ID_ ## fct])
//#define   RPN_DEF_OP(fct)               int fct(rpn_stack *stack, rpn_operator *op)
#define   RPN_DEF_OP(fct)               int fct(rpn_stack *stack, dl_operator *op)
//#define   RPN_DECL_OP(fct)              int fct(rpn_stack *, rpn_operator *)
#define   RPN_DECL_OP(fct)              int fct(rpn_stack *, dl_operator *)

/* }}} */
/* Neural networks : activation functions {{{
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   RPN_NN_AF_SIGMOID             "AF_SIGMOID"
#define   RPN_NN_AF_TANH                "AF_TANH"
#define   RPN_NN_AF_ARC_TAN             "AF_ARC_TAN"
#define   RPN_NN_AF_RELU                "AF_RELU"

#define   RPN_OP_GROUP(name)            RPN_MSG_OP_##name, RPN_OP_END_##name

/* }}} */
/* Macros to define and declare a "new" function {{{
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   RPN_NEW(name)                                                              \
struct rpn_##name *rpn_new_##name(void)                                              \
{                                                                                    \
     struct rpn_##name              *_##name;                                        \
                                                                                     \
     if ((_##name = (struct rpn_##name *) RPN_MALLOC(sizeof(*_##name))) == 0) {      \
          rpn_err_msg_no_mem();                                                      \
          exit(RPN_EXIT_NO_MEM);                                                     \
     }                                                                               \
     bzero(_##name, sizeof(*_##name));                                               \
                                                                                     \
     return _##name;                                                                 \
}
#define   RPN_DECL_NEW(name)             struct rpn_##name *rpn_new_##name(void)

#define   RPN_PREF_NEW(pref, name)                                                   \
struct pref##_##name *pref##_new_##name(void)                                        \
{                                                                                    \
     struct pref##_##name           *_##name;                                        \
                                                                                     \
     if ((_##name = (struct pref##_##name *) RPN_MALLOC(sizeof(*_##name))) == 0) {   \
          rpn_err_msg_no_mem();                                                      \
          exit(RPN_EXIT_NO_MEM);                                                     \
     }                                                                               \
     bzero(_##name, sizeof(*_##name));                                               \
                                                                                     \
     return _##name;                                                                 \
}
#define   RPN_DECL_PREF_NEW(pref, name)  struct pref##_##name *pref##_new_##name(void)
#define   RPN_DEFN_NEW(pref, name)                                                   \
struct pref##_##name *pref##_new_##name(void)                                        \

/* }}} */
/* Macros to define a "free" function {{{
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   RPN_DEF_FREE(name)                                                    \
void rpn_free_##name(void *elt)                                             \
{                                                                           \
     free(elt);                                                             \
}
#define   RPN_DECL_FREE(name)            void *rpn_free_##name(void *)

/* }}} */
/* Compute index of element in an n x p matrix {{{
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   RPN_MATRIX_IDX(i, j, n, p)      ((((i) - 1) * p) + ((j) - 1))

/* Compute index of element in an n x p matrix }}} */
/* Elements that can be backed up {{{ */

/* Types of elements in backup files
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   RPN_BKP_TYPE_MLP                (1)
#define   RPN_BKP_TYPE_MLP_LAYER          (2)
#define   RPN_BKP_TYPE_MLP_AF             (3)
#define   RPN_BKP_TYPE_MLP_NEURON         (4)
#define   RPN_BKP_TYPE_MATRIX             (5)
#define   RPN_BKP_TYPE_LABEL              (6)

/* Elements that can be backed up }}} */
/* MNIST {{{ */
/* URL of MNIST files
   ~~~~~~~~~~~~~~~~~~ */
#define   RPN_MNIST_URL_TRAIN_IMG       "http://yann.lecun.com/exdb/mnist/train-images-idx3-ubyte.gz"
#define   RPN_MNIST_URL_TRAIN_LBL       "http://yann.lecun.com/exdb/mnist/train-labels-idx1-ubyte.gz"
#define   RPN_MNIST_URL_TESTS_IMG       "http://yann.lecun.com/exdb/mnist/t10k-images-idx3-ubyte.gz"
#define   RPN_MNIST_URL_TESTS_LBL       "http://yann.lecun.com/exdb/mnist/t10k-labels-idx1-ubyte.gz"

/* Directory names
   ~~~~~~~~~~~~~~~ */
#define   RPN_MNIST_DIR_DATA            "MNIST"
#define   RPN_MNIST_DIR_TEXTS           "TEXTS"
#define   RPN_MNIST_DIR_IMAGES          "IMAGES"
#define   RPN_MNIST_DIR_TOOLS           "TOOLS"
#define   RPN_MNIST_SUBDIR_GROUPS       "GROUPS"

/* Directory mode
   ~~~~~~~~~~~~~~ */
#define   RPN_MNIST_DIR_MODE            (0755)

/* Gnuplot script names
   ~~~~~~~~~~~~~~~~~~~~ */
#define   RPN_MNIST_PLOT_PIC            "plot_digit_640"
#define   RPN_MNIST_PLOT_PICS           "plot_digits_1920"

/* Subdirectory max name length
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   RPN_MNIST_SUBDIR_LG           (256)

/* Dimensions of MNIST matrices
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   RPN_MNIST_ROWS                 (28)
#define   RPN_MNIST_COLS                 (28)

/* Default dimensions of MNIST pics (in pixels)
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   RPN_DEFLT_MNIST_WIDTH_PIX     (1024)
#define   RPN_DEFLT_MNIST_HEIGHT_PIX    ( 768)

/* Default dimensions of MNIST arrays (in digits)
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   RPN_DEFLT_MNIST_NROWS           (25)
#define   RPN_DEFLT_MNIST_NCOLS           (40)

/* MNIST }}} */
/* System interface parameters {{{ */
#define   RPN_SYS_CMD_SIZE              (1024)

/* System interface parameters }}} */
/* Messages associated to operators groups {{{ */
/* Indexes of groups messages
   ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define   RPN_MSG_OP_UNDEF               (0)
#define   RPN_MSG_OP_CONFIG              (1)
#define   RPN_MSG_OP_STACK               (2)
#define   RPN_MSG_OP_LISTS               (3)
#define   RPN_MSG_OP_TUPLES              (4)
#define   RPN_MSG_OP_PAIRS               (5)
#define   RPN_MSG_OP_TESTS               (6)
#define   RPN_MSG_OP_MATH                (7)
#define   RPN_MSG_OP_STRINGS             (8)
#define   RPN_MSG_OP_STATS               (9)
#define   RPN_MSG_OP_NEURAL             (10)
#define   RPN_MSG_OP_BACKUP             (11)
#define   RPN_MSG_OP_COMPUTER           (12)
#define   RPN_MSG_OP_SYSTEM             (13)
#define   RPN_MSG_OP_TYPES              (14)
#define   RPN_MSG_OP_END_OF_GROUPS       (0)

/* Messages associated to operators groups }}} */
/* Structures definitions {{{
   ~~~~~~~~~~~~~~~~~~~~~~ */
/* Global structure {{{ */
struct global_struct {
     char                               *progname;
     char                               *prompt;
     char                               *version;
     rpn_stack                          *stack;
     cc_uint32                           module_lg;
     struct rpn_sigma                   *sigma;
     char                               *CSV_sep;
     int                                 sw_on,
                                         debug_level,
                                         silent,
                                         show_prompt;
     unsigned long long                  allocated_current,
                                         allocated_peak,
                                         allocated_total,
                                         allocated_nb,
                                         free_nb;
     int                                 err_no,
								 exit_code;
     int                                 cflags,                 // Flags for regcomp()
								 eflags;				// Flag for regexec()
     char                               *err_msg;
     char                               *libpath;
     struct ci_root                      modules_tree;
     struct ci_root                      ops_tree;
     struct ci_root                      ops_tree_v2;
     /* MNIST */
     void                               *mnist;
};

/* Global structure }}} */
/* IP v4 {{{ */
union rpn_IP {
     byte                 b[4];
     unsigned int         i;
};

/* IP v4 }}} */
/* ELEMENT {{{ */
struct rpn_elt {
     char                               *name;
     uint32_t                            type;
     int                                 clone_level;
     union {
          int                            i;
          long                           l;
          long long                     ll;
//        float                          f;
          double                         d;
          long double                   ld;
          bool                           b;
          char                          *s;
          void                          *obj;
     }                                   value;
     struct rpn_elt                     *previous_elt,
                                        *next_elt;
};
typedef struct rpn_elt                  rpn_elt;

/* ELEMENT }}} */
/* METHODS {{{ */
struct rpn_methods {
	void							 (*disp_elt)(rpn_elt *, int);
	void							 (*clone_elt)(rpn_elt *, rpn_elt *);
	char							*(*type_to_string)(int);
	void							 (*free_elt)(rpn_elt *, int);
};
/* METHODS }}} */
/* Operator {{{ */
struct rpn_operator {
     char                               *op_name;
     int                                 op_id;
     uint32_t                            nb_args_in;
     uint32_t                            nb_args_out;
     bool                                activated;
//   int                                (*func)(rpn_stack *, rpn_operator *);
     int                                (*func)(rpn_stack *, dl_operator *);
     struct rpn_st_types                *st_types;
};

typedef struct rpn_operator             rpn_operator;

/* Operator }}} */
/* Stack {{{ */
struct rpn_stack {
     char                               *name;
     uint32_t                            num_elts;
     struct rpn_elt                     *lastx;
//   rpn_operator                       *last_op;
     dl_operator                        *last_op;
     int                                 clear_x;
     int                                 new_value;
     int                                 nb_begin;
     int                                 test_result;
     rpn_elt                            *base_elt,
                                        *top_elt;
//   AJOUTER print_mode
};
typedef struct rpn_stack                 rpn_stack;

/* Stack }}} */
/* Litteral {{{ */
struct rpn_litteral {
     char                               *value;
     int                                 need_parentheses;
};
typedef struct rpn_litteral              rpn_litteral;

/* Litteral }}} */
/* List {{{ */
struct rpn_list {
     char                               *name;
     uint32_t                            num_elts;
     int                                 type;
     int                                 homogeneous;
     rpn_elt                            *base_elt,     // XXX Replace with *head
                                        *top_elt;      // XXX Replace with *tail
};
typedef struct rpn_list                 rpn_list;

/* List }}} */
/* Pair {{{ */
struct rpn_pair {
     char                               *name;
     rpn_elt                            *elt_a,
                                        *elt_b;
};
typedef struct rpn_pair                 rpn_pair;

/* Pair }}} */
/* Coef_a_b {{{ */
struct rpn_coef_a_b {
     double                              a;
     double                              b;
};
typedef struct rpn_coef_a_b             rpn_coef_a_b;

/* Coef_a_b }}} */
/* Min_max {{{ */
struct rpn_min_max {
     double                              min;
     double                              max;
};
typedef struct rpn_min_max               rpn_min_max;

/* Coef_a_b }}} */
/* Regex {{{ */
struct rpn_regex {
     char                               *expr;         // Regular Expression
     regex_t                             RE;           // Compiled regexp
     int                                 cflags,       // Flags used to compile expr
                                         eflags;       // Flags uesd to execute expr
};
typedef struct rpn_regex                 rpn_regex;
/* Regex }}} */
/* Matrix {{{ */
struct rpn_matrix {
     uint32_t                            n;
     uint32_t                            p;
     void                               *base[1];
};
typedef struct rpn_matrix                rpn_matrix;

/* Matrix }}} */
/* Types {{{ */
struct rpn_st_types {
     int                                 nb_operands;
     int                                 nb_results;
     int                                 X_type;
     int                                 Y_type;
};
typedef struct rpn_types                 rpn_types;

/* Types }}} */
/* Sigma {{{ */
struct rpn_sigma {
     char                               *sigma_name;
     struct rpn_elt                     *reg[RPN_NB_SIGMA];
};
typedef struct rpn_sigma                 rpn_sigma;

/* Sigma }}} */
/* Neural network {{{ */
typedef double                          rpn_weight;
typedef double                          rpn_input;
typedef double                          rpn_output;

/* Input neuron {{{ */
struct rpn_input_neuron {
     rpn_input                           input;
};

/* Input neuron }}} */
/* Input layer {{{ */
struct rpn_input_layer {
     int                                 nb_inputs;
     struct rpn_input_neuron            *inputs[0];
};

/* Input layer }}} */
/* Neuron {{{ */
struct rpn_neuron {
     rpn_output                          output;
     int                                 nb_weights;
     rpn_weight                          bias;
     rpn_weight                          weights[0];
};

/* Neuron }}} */
/* Perceptron {{{ */
struct rpn_perceptron {
     int                                 nb_neurons;
     rpn_weight                        (*activation)(rpn_weight);
     rpn_weight                        (*derivative)(rpn_weight);
     struct rpn_neuron                  *neurons[0];
};

/* Perceptron }}} */
/* Multi-layer perceptron {{{ */
struct rpn_mlp {
     int                                 nb_layers;
     int                                 nb_neurons;
     int                                 nb_weights;
     char                               *str_af;
     rpn_weight                        (*activation)(rpn_weight);     // TODO : A SUPPRIMER !!!!!!!!!
     char                               *comment;
     char                               *version;
     struct rpn_input_layer             *input;
     struct rpn_perceptron              *layers[0];
};

/* Multi-layer perceptron }}} */
/* Mnist_module descriptor {{{ */
struct rpn_mnist_desc {
     char                               *data_dir,
                                        *texts_dir,
                                        *images_dir,
                                        *groups_dir,
                                        *tools_dir,
                                        *subdir_texts,
                                        *subdir_images;

     char                               *plot_pic,
                                        *plot_pics;
};
typedef struct rpn_mnist_desc            rpn_mnist_desc;

/* Mnist_module descriptor }}} */
/* Training data {{{ */
struct rpn_training_data {
     int                                 index;        // Index of the element in the set
     int                                 orig_index;
     int                                 ignore;
     int                                 nb_elts;
     int                                 elt_size;
     int                                 num_rows,
                                         num_cols;
     void                               *vector;
};
typedef struct rpn_training_data         rpn_training_data;

struct rpn_mnist_set {
     int                                 set_type,
                                         elt_type;

     char                               *images,
                                        *labels;
     char                               *name;    /* MNIST_TRAIN, MNIST_TEST */
     int                                 nrows,
                                         ncols,
                                         width_pix,
                                         height_pix;
};
typedef struct rpn_mnist_set             rpn_mnist_set;

/* Training data }}} */
/* Training label {{{ */
struct rpn_training_label {
     int                                 index;        // Index of the element in the set
     int                                 value;
};
typedef struct rpn_training_label        rpn_training_label;

/* Training label }}} */
/* Training element {{{ */
struct rpn_training_elt {
     struct rpn_training_data            *data;
     struct rpn_training_label           *label;
};
typedef struct rpn_training_elt           rpn_training_elt;

/* Training element }}} */
/* Training set {{{ */
struct rpn_training_set {
     char                                *name;
     int                                  nb_elts;
     int                                  current_idx;
     int                                  nrows,
                                          ncols;
     int                                  width_pix,
                                          height_pix;
     int                                  elt_width,
                                          elt_height;
     rpn_elt                             *array[1];
};
typedef struct rpn_training_set          rpn_training_set;

/* Training set }}} */
/* 7 segments digit {{{ */
struct rpn_7_seg_digit {
     int                                  digit[RPN_MNIST_ROWS * RPN_MNIST_COLS];
};

/* 7 segments digit }}} */
/* Neural network }}} */
/* Backup ctx {{{ */
struct rpn_backup_ctx {
     char                                *filename;
     char                                *mem;
     void                                *offset;
};
typedef struct rpn_backup_ctx            rpn_backup_ctx;

/* Backup ctx }}} */
/* Text files {{{ */
struct rpn_text_file {
     int                                 static_data;
     int                                 filemode;
     char                               *dirname;
     char                               *filename;
     char                               *text[];
};
typedef struct rpn_text_file             rpn_text_file;

/* Text files }}} */
#define   T(nb_ops, nb_res, X_type, Y_type)       \
     {    nb_ops,   nb_res, RPN_TYPE_## X_type,   RPN_TYPE_ ## Y_type      }

/* Operator groups {{{ */
struct rpn_op_groups {
     int                                 desc_idx;
     int                                 end;
};

/* Operator groups }}} */
/* Message groups {{{ */
struct rpn_mesg_groups {
     char                               *mesg[3];
     char                               *descr[3];
};

/* Message groups }}} */
/* Dynamic module descriptor {{{ */
struct rpn_module {
     void                               *handle;
     char                               *name,
                                        *version;
     cc_uint32                           link_ID;
     struct ci_node                      node;
};
typedef struct rpn_module                rpn_module;

/* Dynamic module descriptor }}} */
/* Dynamic operator descriptor {{{ */
struct rpn_dyn_op {
     struct rpn_module                  *module;
     struct rpn_operator                *operator;
     struct ci_node                      node;
};

/* Dynamic operator descriptor }}} */
/* }}} */

/* External variables
   ~~~~~~~~~~~~~~~~~~ */
extern struct global_struct              G;

#endif    // _RPN_HEADER_H
