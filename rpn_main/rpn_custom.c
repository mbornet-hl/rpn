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
 *   @(#)  [MB] cy_rpn_custom.c Version 1.90 du 19/06/05 - 
 */

#include  <stdio.h>
#include  <fcntl.h>
#include  "rpn_header.h"
#include  "rpn_epri.h"
#include  "rpn_proto.h"
#include  "../df/df_epub.h"
#define   _GNU_SOURCE
#include  <math.h>
#include  <sys/stat.h>
#include  <sys/types.h>
#include  <errno.h>
#include  <unistd.h>
#include  <libgen.h>

/* Operators descriptors
   ~~~~~~~~~~~~~~~~~~~~~ */
struct rpn_operator      rpn_operators[]           = {
     // undef {{{
     {    RPN_OP(UNDEF),                     0,     0, TRUE,  rpn_op_undef,          NULL                        },

     // }}}
     // Config {{{
     {    RPN_OP(SW_ON),                     0,     0, TRUE,  rpn_op_sw_on,          NULL                        },
     {    RPN_OP(SW_OFF),                    0,     0, TRUE,  rpn_op_sw_off,         NULL                        },

     // }}}
     // Stack {{{
     {    RPN_OP(ENTER),                     1,     0, TRUE,  rpn_op_enter,          NULL                        },
     {    RPN_OP(CLX),                       1,     1, TRUE,  rpn_op_clx,            NULL                        },
     {    RPN_OP(DELX),                      1,     0, TRUE,  rpn_op_delx,           NULL                        },
     {    RPN_OP(DEL_L),                     1,     0, TRUE,  rpn_op_del_l,          NULL                        },
     {    RPN_OP(CLST),                      0,     0, TRUE,  rpn_op_clst,           NULL                        },
     {    RPN_OP(SWAP_XY),                   2,     2, TRUE,  rpn_op_swap_xy,        NULL                        },
     {    RPN_OP(SWAP_XZ),                   2,     2, TRUE,  rpn_op_swap_xz,        NULL                        },
     {    RPN_OP(SWAP_XT),                   2,     2, TRUE,  rpn_op_swap_xt,        NULL                        },
     {    RPN_OP(SWAP_XL),                   2,     2, TRUE,  rpn_op_swap_xl,        NULL                        },
     {    RPN_OP(RDN),                       0,     0, TRUE,  rpn_op_roll_down,      NULL                        },
     {    RPN_OP(RUP),                       0,     0, TRUE,  rpn_op_roll_up,        NULL                        },
     {    RPN_OP(LASTX),                     0,     1, TRUE,  rpn_op_lastx,          NULL                        },
     {    RPN_OP(RCL_X),                     0,     1, TRUE,  rpn_op_rcl_x,          NULL                        },
     {    RPN_OP(PRX),                       1,     1, TRUE,  rpn_op_prx,            NULL                        },
     {    RPN_OP(PRSTK),                     1,     0, TRUE,  rpn_op_prstk,          NULL                        },
     {    RPN_OP(STK),                       1,     0, TRUE,  rpn_op_stk,            NULL                        },
     {    RPN_OP(DUPX),                      1,     0, TRUE,  rpn_op_dupx,           NULL                        },
     {    RPN_OP(DUMP),                      1,     0, TRUE,  rpn_op_dump,           NULL                        },
//   {    RPN_OP(CATALOG),                   1,     0, TRUE,  rpn_op_catalog,        NULL                        },

     // }}}
     // Lists {{{
     {    RPN_OP(PUSH),                      2,     0, TRUE,  rpn_op_push,           NULL                        },
     {    RPN_OP(POP),                       1,     1, TRUE,  rpn_op_pop,            NULL                        },
     {    RPN_OP(PUSH_EXTRACT),              2,     0, TRUE,  rpn_op_undef,          NULL                        },
     {    RPN_OP(POP_INSERT),                1,     1, TRUE,  rpn_op_undef,          NULL                        },
     {    RPN_OP(GET_HEAD),                  2,     1, FALSE, rpn_op_undef,          NULL                        },
     {    RPN_OP(GET_QUEUE),                 2,     1, FALSE, rpn_op_undef,          NULL                        },
     {    RPN_OP(GET),                      -1,     1, TRUE,  rpn_op_get,            NULL                        },
     {    RPN_OP(GET_IDX),                  -1,     1, FALSE, rpn_op_undef,          NULL                        },
     {    RPN_OP(GET_IGNORED),              -1,     1, FALSE, rpn_op_undef,          NULL                        },
     {    RPN_OP(GET_USED),                 -1,     1, FALSE, rpn_op_undef,          NULL                        },
     {    RPN_OP(SET_IDX),                  -1,     1, FALSE, rpn_op_undef,          NULL                        },
     {    RPN_OP(LIST),                     -1,     1, TRUE,  rpn_op_list,           NULL                        },

     // }}}
     // Tuples {{{
     {    RPN_OP(TUPLE),                     2,     1, TRUE,  rpn_op_tuples,         NULL                        },
     {    RPN_OP(NTUPLE),                   -1,     1, TRUE,  rpn_op_tuples,         NULL                        },
     {    RPN_OP(XTUPLE),                   -1,     1, TRUE,  rpn_op_tuples,         NULL                        },

     // }}}
     // Pairs {{{
     {    RPN_OP(OPAIR),                     2,     1, TRUE,  rpn_op_pairs,          NULL                        },

     // }}}
     // Tests {{{
     {    RPN_OP(TEST_LT),                   2,     0, TRUE,  rpn_op_test,           NULL                        },
     {    RPN_OP(TEST_LE),                   2,     0, TRUE,  rpn_op_test,           NULL                        },
     {    RPN_OP(TEST_GE),                   2,     0, TRUE,  rpn_op_test,           NULL                        },
     {    RPN_OP(TEST_GT),                   2,     0, TRUE,  rpn_op_test,           NULL                        },
     {    RPN_OP(TEST_EQ),                   2,     0, TRUE,  rpn_op_test,           NULL                        },
     {    RPN_OP(TEST_NE),                   2,     0, TRUE,  rpn_op_test,           NULL                        },

     // Comparison to zero
     {    RPN_OP(TEST_LTZ),                  1,     0, TRUE,  rpn_op_test,           NULL                        },
     {    RPN_OP(TEST_LEZ),                  1,     0, TRUE,  rpn_op_test,           NULL                        },
     {    RPN_OP(TEST_GEZ),                  1,     0, TRUE,  rpn_op_test,           NULL                        },
     {    RPN_OP(TEST_GTZ),                  1,     0, TRUE,  rpn_op_test,           NULL                        },
     {    RPN_OP(TEST_EQZ),                  1,     0, TRUE,  rpn_op_test,           NULL                        },
     {    RPN_OP(TEST_NEZ),                  1,     0, TRUE,  rpn_op_test,           NULL                        },

     // }}}
     // Math {{{
     {    RPN_OP(ADD),                       2,     1, TRUE,  rpn_op_generic2,       op_types_add                },
     {    RPN_OP(SUB),                       2,     1, TRUE,  rpn_op_generic2,       op_types_sub                },
     {    RPN_OP(MUL),                       2,     1, TRUE,  rpn_op_generic2,       op_types_mul                },
     {    RPN_OP(DIV),                       2,     1, TRUE,  rpn_op_generic2,       op_types_div                },
     {    RPN_OP(MATH_MUL),                  2,     1, TRUE,  rpn_op_math_mul,       op_types_mul                },
     {    RPN_OP(RECIPROCAL),                1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(POWER),                     1,     1, TRUE,  rpn_op_generic2,       op_types_power              },
     {    RPN_OP(SQRT),                      1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(SQUARE),                    1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(LN),                        1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(EXP),                       1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(LOG10),                     1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(10_POWER_X),                1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(PERCENT),                   1,     1, FALSE, rpn_op_percent,        NULL                        },
     {    RPN_OP(DELTA_PERCENT),             1,     1, TRUE,  rpn_op_generic2,       op_types_delta_percent      },
     {    RPN_OP(SINE),                      1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(COSINE),                    1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(TANGENT),                   1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(ASINE),                     1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(ACOSINE),                   1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(ATANGENT),                  1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(SINH),                      1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(COSH),                      1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(TANH),                      1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(ASINH),                     1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(ACOSH),                     1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(ATANH),                     1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(SIGMOID),                   1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(MATRIX),                    2,     1, TRUE,  rpn_op_matrix,         NULL                        },
     {    RPN_OP(EXPLODE),                   1,     0, TRUE,  rpn_op_explode,        NULL                        },
     {    RPN_OP(VEC2),                      2,     1, TRUE,  rpn_op_vec2,           NULL                        },
     {    RPN_OP(VEC3),                      2,     1, TRUE,  rpn_op_vec3,           NULL                        },
     {    RPN_OP(TRANSPOSE),                 1,     1, TRUE,  rpn_op_transpose,      NULL                        },
     {    RPN_OP(CROSS_PRODUCT),             2,     1, TRUE,  rpn_op_cross_prdct,    NULL                        },
     {    RPN_OP(SCALAR_MATRIX),             1,     1, FALSE, rpn_op_scalar_matrix,  NULL                        },
     {    RPN_OP(SEQ),                       2,    -1, TRUE,  rpn_op_seq,            NULL                        },
     {    RPN_OP(DIM),                       1,     2, TRUE,  rpn_op_dim,            NULL                        },
     {    RPN_OP(CHANGE_SIGN),               1,     1, TRUE,  rpn_op_generic1,       op_types_gen_math1          },
     {    RPN_OP(H_CAT),                     2,     1, TRUE,  rpn_op_generic2,       op_types_h_cat              },
     {    RPN_OP(V_CAT),                     2,     1, TRUE,  rpn_op_generic2,       op_types_v_cat              },
     {    RPN_OP(Z_MAT),                    -1,     1, TRUE,  rpn_op_z_mat,          op_types_z_mat              },
     {    RPN_OP(H_REV),                     1,     1, TRUE,  rpn_op_h_rev,          NULL                        },
     {    RPN_OP(V_REV),                     1,     1, TRUE,  rpn_op_generic1,       NULL                        },
     {    RPN_OP(DIAG_MAT),                 -1,     1, FALSE, rpn_op_diag_mat,       NULL                        },
     {    RPN_OP(V_PAD),                     2,     1, TRUE,  rpn_op_generic2,       op_types_v_cat              },
     {    RPN_OP(MAT_NIL_DIAG),              2,     1, TRUE,  rpn_op_mat_nil_diag,   NULL                        },

     // }}}
     // Strings {{{
     {    RPN_OP(STR_LENGTH),                1,     1, TRUE,  rpn_op_str_length,     NULL                        },
     {    RPN_OP(STR_CATENATE),              2,     1, TRUE,  rpn_op_str_catenate,   NULL                        },
     {    RPN_OP(STR_DUPL),                  2,     1, TRUE,  rpn_op_str_dupl,       NULL                        },

     // }}}
     // Statistics {{{
     {    RPN_OP(SUM_ADD),                   2,     1, TRUE,  rpn_op_sum_add,        NULL                        },
     {    RPN_OP(SUM_SUB),                   2,     1, FALSE, rpn_op_sum_sub,        NULL                        },
     {    RPN_OP(MEAN),                      0,     2, TRUE,  rpn_op_mean,           NULL                        },
     {    RPN_OP(SDEV),                      0,     2, TRUE,  rpn_op_sdev,           NULL                        },
     {    RPN_OP(MEANX),                     1,     1, TRUE,  rpn_op_meanx,          NULL                        },
     {    RPN_OP(MMM),                       1,     1, TRUE,  rpn_op_mmm,            NULL                        },
     {    RPN_OP(LINE_DATA_SET),             0,     2, TRUE,  rpn_op_gen_data_set,   NULL                        },
     {    RPN_OP(EXP_DATA_SET),              0,     2, TRUE,  rpn_op_gen_data_set,   NULL                        },
     {    RPN_OP(LOG_DATA_SET),              0,     2, TRUE,  rpn_op_gen_data_set,   NULL                        },
     {    RPN_OP(POW_DATA_SET),              0,     2, TRUE,  rpn_op_gen_data_set,   NULL                        },

     // }}}
     // Neural networks {{{
     {    RPN_OP(NN_CREAT_MLP),             -1,     1, TRUE,  rpn_op_generic_list,   op_types_gen_nn             },
     {    RPN_OP(NN_LEARN),                  2,     1, FALSE, rpn_op_generic2,       op_types_gen_nn             },
     {    RPN_OP(NN_ANALYZE),                2,     1, FALSE, rpn_op_generic2,       op_types_gen_nn             },
     {    RPN_OP(NN_SET_LAYER_W),            2,     1, FALSE, rpn_op_generic2,       op_types_gen_nn             },
     {    RPN_OP(NN_ALEA_LAYER_W),           1,     1, FALSE, rpn_op_generic1,       op_types_gen_nn             },
     {    RPN_OP(NN_SET_MLP_W),              2,     1, TRUE,  rpn_set_mlp_weights,   op_types_gen_nn             },
     {    RPN_OP(NN_ALEA_MLP_W),             1,     1, FALSE, rpn_op_generic1,       op_types_gen_nn             },
     {    RPN_OP(NN_READ_MNIST),             0,     1, TRUE,  rpn_op_read_mnist,     NULL                        },
     {    RPN_OP(NN_MNIST_PIC),              0,     1, TRUE,  rpn_op_mnist_pic,      NULL                        },
     {    RPN_OP(NN_MNIST_PICS),            -1,     1, TRUE,  rpn_op_mnist_pics,     NULL                        },
     {    RPN_OP(NN_READ_ELT),               0,     1, TRUE,  rpn_op_read_elt,       NULL                        },
     {    RPN_OP(NN_MAT_7_SEG),              1,     1, TRUE,  rpn_op_mat_7_seg,      NULL                        },
     {    RPN_OP(NN_LABEL),                  1,     1, TRUE,  rpn_op_nn_label,       NULL                        },
     {    RPN_OP(NN_INDEX),                  1,     1, TRUE,  rpn_op_nn_train_index, NULL                        },
     {    RPN_OP(NN_ORIG_INDEX),             1,     1, TRUE,  rpn_op_nn_train_index, NULL                        },
     {    RPN_OP(NN_DISPATCH),               1,     1, TRUE,  rpn_op_nn_dispatch,    NULL                        },
     {    RPN_OP(NN_IGNORE),                 1,     1, TRUE,  rpn_op_nn_ignore,      NULL                        },
     {    RPN_OP(NN_USE),                    1,     1, TRUE,  rpn_op_nn_ignore,      NULL                        },

     // }}}
     // Backup / restore {{{
     {    RPN_OP(SAVE),                      2,     1, FALSE, rpn_op_generic2,       op_types_backup             },
     {    RPN_OP(LOAD),                      2,     1, FALSE, rpn_op_generic2,       op_types_backup             },
     {    RPN_OP(RESTORE),                   2,     1, FALSE, rpn_op_generic2,       op_types_backup             },
     {    RPN_OP(AUTOSAVE),                  1,     0, FALSE, rpn_op_undef,          NULL                        },
     {    RPN_OP(NO_AUTOSAVE),               1,     0, FALSE, rpn_op_undef,          NULL                        },
     {    RPN_OP(WRITE),                     2,     0, TRUE,  rpn_op_write,          NULL                        },
     {    RPN_OP(WRITE_CSV),                 2,     0, TRUE,  rpn_op_write_csv,      NULL                        },

     // }}}
     // Computer science tools {{{
     {    RPN_OP(CS_GEN_C),                  1,     1, TRUE,  rpn_op_cs_gen_C,       NULL                        },

     // }}}
     // System {{{
     {    RPN_OP(MEMORY),                    1,     1, TRUE,  rpn_op_sys_memory,     NULL                        },
     {    RPN_OP(PAUSE),                     1,     1, TRUE,  rpn_op_sys_pause,      NULL                        },
     {    RPN_OP(PING),                      1,     1, TRUE,  rpn_op_sys_ping,       NULL                        },
     {    RPN_OP(IPERF),                     1,     1, TRUE,  rpn_op_sys_iperf,      NULL                        },

     // }}}
     // Types {{{
     {    RPN_OP(TYPE_FILENAME),             1,     1, TRUE,  rpn_op_generic1,       op_types_type               },

     // }}}
     // END OF OPERATORS DESCRIPTORS {{{
     {    RPN_OP(NULL),                      0,     0, FALSE, 0,                     0                           }
     // }}}
};
/* Operators groups
   ~~~~~~~~~~~~~~~~ */
struct rpn_op_groups     rpn_op_groups[] = {
     /* Operators groups {{{ */
     {    RPN_OP_GROUP(UNDEF)           },
     {    RPN_OP_GROUP(CONFIG)          },
     {    RPN_OP_GROUP(STACK)           },
     {    RPN_OP_GROUP(LISTS)           },
     {    RPN_OP_GROUP(TUPLES)          },
     {    RPN_OP_GROUP(PAIRS)           },
     {    RPN_OP_GROUP(TESTS)           },
     {    RPN_OP_GROUP(MATH)            },
     {    RPN_OP_GROUP(STRINGS)         },
     {    RPN_OP_GROUP(STATS)           },
     {    RPN_OP_GROUP(NEURAL)          },
     {    RPN_OP_GROUP(BACKUP)          },
     {    RPN_OP_GROUP(COMPUTER)        },
     {    RPN_OP_GROUP(SYSTEM)          },
     {    RPN_OP_GROUP(TYPES)           },
     {    RPN_OP_GROUP(END_OF_GROUPS)   }
     /* Operators groups }}} */
};

struct rpn_mesg_groups   rpn_mesg_groups[] = {
     /* Operator groups messages {{{ */
     {
          { "UNDEF",                    "INDEFINI",                   NULL      },
          {
               "Undefined operator",
               "Operateur indefini",
               NULL
          }
     },
     {
          { "CONFIG",                   "CONFIGURATION",              NULL      },
          {
               "atack operators",
               "Operateurs de configuration",
               NULL
          }
     },
     {
          { "STACK",                    "PILE",                       NULL      },
          {
               "Stack operators",
               "Operateurs de gestion de piles",
               NULL
          }
     },
     {
          { "LISTS",                    "LISTES",                     NULL      },
          {    "Lists operators",
               "Operateurs de gestion de listes",
               NULL
          },
     },
     {
          { "TUPLES",                    "N-UPLETS",                  NULL      },
          {    "Tuples operators",
               "Operateurs de gestion de n-uplets",
               NULL
          },
     },
     {
          { "PAIRS",                     "PAIRES",                    NULL      },
          {    "Pairs operators",
               "Operateurs de gestion de paires/couples",
               NULL
          },
     },
     {
          { "TESTS",                    "TESTS",                      NULL      },
          {    "Test operators",
               "Operateurs de tests",
               NULL
          },
     },
     {
          { "MATHS",                    "MATHS",                      NULL      },
          {    "Maths operators",
               "Operateurs mathematiques",
               NULL
          },
     },
     {
          { "STRINGS",                  "CHAINES DE CARACTERES",      NULL      },
          {    "Maths operators",
               "Operateurs mathematiques",
               NULL
          },
     },
     {
          { "STATS",                    "STATISTIQUES",               NULL      },
          {    "Maths operators",
               "Operateurs mathematiques",
               NULL
          },
     },
     {
          { "NEURAL NETWORKS",          "RESEAUX DE NEURONES",        NULL      },
          {    "Neural networks operators",
               "Operateurs de reseaux de neurones",
               NULL
          },
     },
     {
          { "BACKUP",                   "SAUVEGARDES",                NULL      },
          {    "Backups operators",
               "Operateurs de sauvegardes",
               NULL
          },
     },
     {
          { "COMPUTER",                 "INFORMATIQUE",               NULL      },
          {    "Computer operators",
               "Operateurs informatique",
               NULL
          },
     },
     {
          { "SYSTEM",                   "SYSTEME",                    NULL      },
          {    "system operators",
               "Operateurs systeme",
               NULL
          },
     },
     {
          { "TYPES",                    "TYPES",                      NULL      },
          {    "Types operators",
               "Operateurs de gestion de types",
               NULL
          },
     },
     {
          { NULL,                       NULL,                         NULL      },
          { NULL,                       NULL,                         NULL      }
     }
     /* Operator groups messages }}} */
};
/* rpn_op_undef() {{{ */

/******************************************************************************

                              RPN_OP_UNDEF

******************************************************************************/
RPN_DEF_OP(rpn_op_undef)
{
     RPN_INTERNAL_ERROR;
}
/* rpn_op_undef() }}} */
/* rpn_op_generic1() {{{ */
/******************************************************************************

                              RPN_OP_GENERIC1

******************************************************************************/
RPN_DEF_OP(rpn_op_generic1)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _x, _result, _lg, _X_type;
     double                    _xd, _resultd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     /* Check types
        ~~~~~~~~~~~ */
     if ((_retcode = rpn_check_types(op, stack)) != RPN_RET_OK) {
          goto end;
     }

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     /* Initialize result assumed type
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _stk_result              = rpn_new_elt(RPN_TYPE_INT);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _x                       = _stk_x->value.i;

          switch (op->op_id) {

          case RPN_OP_ID_SQUARE:
               _result                  = _x * _x;
               break;

          case RPN_OP_ID_CHANGE_SIGN:
               _result                  = - _x;
               break;

          default:
               rpn_set_type(_stk_result, RPN_TYPE_UNDEFINED);
               break;
          }

          if (rpn_get_type(_stk_result) == RPN_TYPE_INT) {
               _stk_result->value.i          = _result;
          }
          else {
               /* Initialize result assumed type
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_set_type(_stk_result, RPN_TYPE_DOUBLE);

               switch (op->op_id) {

               case RPN_OP_ID_RECIPROCAL:
                    _resultd                 = 1.0 / (double) _x;
                    break;

               case RPN_OP_ID_SQRT:
                    _resultd                 = sqrt((double) _x);
                    break;

               case RPN_OP_ID_LN:
                    _resultd                 = log((double) _x);
                    break;

               case RPN_OP_ID_EXP:
                    _resultd                 = exp((double) _x);
                    break;

               case RPN_OP_ID_LOG10:
                    _resultd                 = log10((double) _x);
                    break;

               case RPN_OP_ID_10_POWER_X:
                    _resultd                 = exp10((double) _x);
                    break;

               case RPN_OP_ID_SINE:
                    _resultd                 = sin((double) _x);
                    break;

               case RPN_OP_ID_COSINE:
                    _resultd                 = cos((double) _x);
                    break;

               case RPN_OP_ID_TANGENT:
                    _resultd                 = tan((double) _x);
                    break;

               case RPN_OP_ID_ASINE:
                    _resultd                 = asin((double) _x);
                    break;

               case RPN_OP_ID_ACOSINE:
                    _resultd                 = acos((double) _x);
                    break;

               case RPN_OP_ID_ATANGENT:
                    _resultd                 = atan((double) _x);
                    break;

               case RPN_OP_ID_SINH:
                    _resultd                 = (exp((double) _x) - exp(-(double) _x)) / 2.0;
                    break;

               case RPN_OP_ID_COSH:
                    _resultd                 = (exp((double) _x) + exp(-(double) _x)) / 2.0;
                    break;

               case RPN_OP_ID_TANH:
                    _resultd                 = (exp((double) _x) - exp(-(double) _x)) / (exp((double) _x) + exp(-(double) _x));
                    break;

               case RPN_OP_ID_ASINH:
                    _resultd                 = log((double)_x + sqrt(((double) _x * (double) _x) + 1.0));
                    break;

               case RPN_OP_ID_ACOSH:
                    _resultd                 = log((double)_x + sqrt(((double) _x * (double) _x) - 1.0));
                    break;

               case RPN_OP_ID_ATANH:
                    _resultd                 = 0.5 * log((1.0 + (double) _x) / (1.0 - (double) _x));
                    break;

               case RPN_OP_ID_SIGMOID:
                    _resultd                 = (1.0 / (1.0 + exp((double) -_x)));
                    break;

               default:
                    fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
                    RPN_INTERNAL_ERROR;
                    break;
               }

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;
          }

          rpn_set_lastx(stack, _stk_x);

          rpn_push(stack, _stk_result);
          break;
// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;

          switch (op->op_id) {

          case RPN_OP_ID_RECIPROCAL:
               _resultd                 = 1.0 / (double) _xd;
               break;

          case RPN_OP_ID_SQUARE:
               _resultd                 = (double) _xd * (double) _xd;
               break;

          case RPN_OP_ID_SQRT:
               _resultd                 = sqrt((double) _xd);
               break;

          case RPN_OP_ID_LN:
               _resultd                 = log((double) _xd);
               break;

          case RPN_OP_ID_EXP:
               _resultd                 = exp((double) _xd);
               break;

          case RPN_OP_ID_LOG10:
               _resultd                 = log10(_xd);
               break;

          case RPN_OP_ID_10_POWER_X:
               _resultd                 = exp10(_xd);
               break;

          case RPN_OP_ID_SINE:
               _resultd                 = sin((double) _xd);
               break;

          case RPN_OP_ID_COSINE:
               _resultd                 = cos((double) _xd);
               break;

          case RPN_OP_ID_TANGENT:
               _resultd                 = tan((double) _xd);
               break;

          case RPN_OP_ID_ASINE:
               _resultd                 = asin((double) _xd);
               break;

          case RPN_OP_ID_ACOSINE:
               _resultd                 = acos((double) _xd);
               break;

          case RPN_OP_ID_ATANGENT:
               _resultd                 = atan((double) _xd);
               break;

          case RPN_OP_ID_SINH:
               _resultd                 = (exp((double) _xd) - exp(-(double) _xd)) / 2.0;
               break;

          case RPN_OP_ID_COSH:
               _resultd                 = (exp((double) _xd) + exp(-(double) _xd)) / 2.0;
               break;

          case RPN_OP_ID_TANH:
               _resultd                 = (exp((double) _xd) - exp(-(double) _xd)) / (exp((double) _xd) + exp(-(double) _xd));
               break;

          case RPN_OP_ID_ASINH:
               _resultd                 = log((double) _xd + sqrt(((double) _xd * (double) _xd) + 1.0));
               break;

          case RPN_OP_ID_ACOSH:
               _resultd                 = log((double) _xd + sqrt(((double) _xd * (double) _xd) - 1.0));
               break;

          case RPN_OP_ID_ATANH:
               _resultd                 = 0.5 * log((1.0 + (double) _xd) / (1.0 - (double) _xd));
               break;

          case RPN_OP_ID_SIGMOID:
               _resultd                 = 1.0 / (1.0 + exp((double) - _xd));
               break;

          case RPN_OP_ID_CHANGE_SIGN:
               _resultd                 = - _xd;
               break;

          default:
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               RPN_INTERNAL_ERROR;
               break;
          }

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = _resultd;

          rpn_push(stack, _stk_result);
          break;
// }}}
     case RPN_TYPE_MATRIX:
// {{{
          {
               struct rpn_matrix        *_mat_A, *_mat_C;
               int                       _n, _m, _p, _i, _j, _idx, _sum, _size;
               struct rpn_elt           *_elt, *_nil;
               struct rpn_stack         *_stack;

               _mat_A                   = (struct rpn_matrix *) _stk_x->value.obj;

               _n                       = _mat_A->n;
               _p                       = _mat_A->p;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
               _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
               _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
               _mat_C->n                = _n;
               _mat_C->p                = _p;
               _stk_result->value.obj   = _mat_C;

               _stack                   = rpn_new_stack(__func__);

               for (_i = 1; _i <= _n; _i++) {
                    for (_j = 1; _j <= _p; _j++) {
                         _nil      = rpn_new_elt(RPN_TYPE_INT);
                         rpn_push(_stack, _nil);

                         _elt      = rpn_mat_get_elt_ref(_mat_A, _i, _j);
                         rpn_push(_stack, _elt);
                         if ((_retcode = rpn_op_generic1(_stack, op)) != 0) {
                              rpn_err_msg_op_error(op->op_name, _retcode);
                              exit(RPN_EXIT_OP_ERROR);
                         }
                         _elt                = rpn_pop(_stack);
                         _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
                         _mat_C->base[_idx]  = (void *) _elt;
                    }
               }

               if ((_mat_C->n == 1)
               &&  (_mat_C->p == 1)) {
                    _stk_result              = rpn_mat_get_elt_ref(_mat_C, 1, 1);
               }

               rpn_free_stack(_stack);
               rpn_push(stack, _stk_result);
          }
          break;
// }}}
     case RPN_TYPE_STRING:
// {{{
          _stk_result              = rpn_new_elt(RPN_TYPE_UNDEFINED);

          switch (op->op_id) {

          case RPN_OP_ID_TYPE_FILENAME:
               rpn_set_type(_stk_result, RPN_TYPE_FILENAME);
               _stk_result->value.s     = strdup(_stk_x->value.s);
               break;

          default:
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               RPN_INTERNAL_ERROR;
               break;
          }
          rpn_set_lastx(stack, _stk_x);
          rpn_push(stack, _stk_result);
          break;

// }}}
     case RPN_TYPE_LITTERAL:
// {{{
          _stk_result              = rpn_new_elt(RPN_TYPE_UNDEFINED);

          switch (op->op_id) {

          case RPN_OP_ID_TYPE_FILENAME:
               rpn_set_type(_stk_result, RPN_TYPE_FILENAME);
               _stk_result->value.s     = strdup(_stk_x->value.s);
               break;

          default:
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               RPN_INTERNAL_ERROR;
               break;
          }

          rpn_set_lastx(stack, _stk_x);
          rpn_push(stack, _stk_result);
          break;
// }}}
     default:
// {{{
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_TYPES;
          break;
// }}}
     }

end:
     return _retcode;
}
/* rpn_op_generic1() }}} */
/* rpn_op_generic2() {{{ */

/******************************************************************************

                         RPN_OP_GENERIC2

******************************************************************************/
RPN_DEF_OP(rpn_op_generic2)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _X_type, _Y_type;
     int                       _x, _y, _result, _lg;
     double                    _xd, _yd, _resultd;
     char                     *_s1, *_s2, *_s;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     /* Check types
        ~~~~~~~~~~~ */
     if ((_retcode = rpn_check_types(op, stack)) != RPN_RET_OK) {
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(rpn_get_X_type(stack)));
          fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(rpn_get_Y_type(stack)));
          goto end;
     }

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);
// printf("%s [%s (%d)] Y : type = %s\n", __func__, __FILE__, __LINE__,  rpn_type_to_string(_stk_y->type));
// printf("%s [%s (%d)] X : type = %s\n", __func__, __FILE__, __LINE__,  rpn_type_to_string(_stk_x->type));

     _X_type                  = rpn_get_type(_stk_x);
     _Y_type                  = rpn_get_type(_stk_y);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _x                       = _stk_x->value.i;

          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               _y                       = _stk_y->value.i;

               switch (op->op_id) {

               case RPN_OP_ID_ADD:
                    _result                  = _y + _x;
                    break;

               case RPN_OP_ID_SUB:
                    _result                  = _y - _x;
                    break;

               case RPN_OP_ID_MUL:
               case RPN_OP_ID_MATH_MUL:
// printf("OPERATION : %3d %s %3d\n", _y, op->op_name, _x);
                    _result                  = _y * _x;
                    break;

               case RPN_OP_ID_DIV:
                    _result                  = _y / _x;
                    break;

               case RPN_OP_ID_DELTA_PERCENT:
                    _result                  = (_x - _y) * 100 / _y;
                    break;

               case RPN_OP_ID_POWER:
                    _result                  = (int) pow((double) _y, (double) _x);
                    break;

               default:
                    fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
                    fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
                    RPN_INTERNAL_ERROR;
                    break;
               }

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_INT);
               _stk_result->value.i     = _result;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_DOUBLE:
// {{{
               _yd                      = _stk_y->value.d;
               _xd                      = (double) _x;

               switch (op->op_id) {

               case RPN_OP_ID_ADD:
                    _resultd                 = _yd + _xd;
                    break;

               case RPN_OP_ID_SUB:
                    _resultd                 = _yd - _xd;
                    break;

               case RPN_OP_ID_MUL:
               case RPN_OP_ID_MATH_MUL:
                    _resultd                 = _yd * _xd;
                    break;

               case RPN_OP_ID_DIV:
                    _resultd                 = _yd / _xd;
                    break;

               case RPN_OP_ID_DELTA_PERCENT:
                    _resultd                 = (_xd - _yd) * 100.0 / _yd;
                    break;

               case RPN_OP_ID_POWER:
                    _resultd                 = pow(_yd, _xd);
                    break;

               default:
                    fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
                    fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
                    RPN_INTERNAL_ERROR;
                    break;
               }

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
               {
                    struct rpn_matrix        *_mat_B, *_mat_C;
                    int                       _n, _p, _i, _j, _idx, _sum, _size;
                    struct rpn_elt           *_elt, *_scalar;
                    struct rpn_stack         *_stack;
                    _mat_B                   = (struct rpn_matrix *) _stk_y->value.obj;
                    _n                       = _mat_B->n;
                    _p                       = _mat_B->p;

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
                    _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                    _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
                    _mat_C->n                = _n;
                    _mat_C->p                = _p;
                    _stk_result->value.obj   = _mat_C;

                    _stack                   = rpn_new_stack(__func__);
                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _elt      = rpn_mat_extract_elt(_mat_B, _i, _j);
                              rpn_push(_stack, _elt);

                              _scalar   = rpn_clone_elt(_stk_x);
                              rpn_push(_stack, _scalar);

                              if ((_retcode = rpn_op_generic2(_stack, op)) != 0) {
                                   rpn_err_msg_op_error(op->op_name, _retcode);
                                   exit(RPN_EXIT_OP_ERROR);
                              }

                              _elt                = rpn_pop(_stack);
                              _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
                              _mat_C->base[_idx]  = (void *) _elt;
                         }
                    }

                    rpn_free_stack(_stack);
                    rpn_push(stack, _stk_result);
               }
               break;
// }}}
          case RPN_TYPE_VECTOR_3:
// {{{
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
// }}}
          case RPN_TYPE_LITTERAL:
// {{{
               fprintf(stderr, "%s (op = %s) :\n", __func__, op->op_name);
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
// }}}
          case RPN_TYPE_NIL:
// {{{
               rpn_push(stack, _stk_x);
               break;
// }}}
          case RPN_TYPE_NEURAL_MLP:
// {{{
               switch (op->op_id) {

               case RPN_OP_ID_NN_SET_MLP_W:
                    rpn_set_mlp_weights(stack, op);
                    break;

               default:
                    fprintf(stderr, "*** NOT IMPLEMENTED !\n");
                    exit(RPN_EXIT_UNIMPLEMENTED);
                    break;
               }

               rpn_set_lastx(stack, _stk_x);

// XXX XXX XXX XXX
               _stk_result              = _stk_x;
// XXX XXX XXX XXX

               rpn_push(stack, _stk_result);
               break;

// }}}
          default:
// {{{
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
// }}}
          }
          break;
// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;

          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               _yd                       = (double) _stk_y->value.i;


               switch (op->op_id) {

               case RPN_OP_ID_ADD:
                    _resultd                 = _yd + _xd;
                    break;

               case RPN_OP_ID_SUB:
                    _resultd                 = _yd - _xd;
                    break;

               case RPN_OP_ID_MUL:
               case RPN_OP_ID_MATH_MUL:
                    _resultd                 = _yd * _xd;
                    break;

               case RPN_OP_ID_DIV:
                    _resultd                 = _yd / _xd;
                    break;

               case RPN_OP_ID_DELTA_PERCENT:
                    _resultd                 = (_xd - _yd) * 100.0 / _yd;
                    break;

               case RPN_OP_ID_POWER:
                    _resultd                 = pow(_yd, _xd);
                    break;

               default:
                    fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
                    fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
                    RPN_INTERNAL_ERROR;
                    break;
               }

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_DOUBLE:
// {{{
               _yd                      = _stk_y->value.d;

               switch (op->op_id) {

               case RPN_OP_ID_ADD:
                    _resultd                 = _yd + _xd;
                    break;

               case RPN_OP_ID_SUB:
                    _resultd                 = _yd - _xd;
                    break;

               case RPN_OP_ID_MUL:
               case RPN_OP_ID_MATH_MUL:
                    _resultd                 = _yd * _xd;
                    break;

               case RPN_OP_ID_DIV:
                    _resultd                 = _yd / _xd;
                    break;

               case RPN_OP_ID_DELTA_PERCENT:
                    _resultd                 = (_xd - _yd) * 100.0 / _yd;
                    break;

               case RPN_OP_ID_POWER:
                    _resultd                 = pow(_yd, _xd);
                    break;

               default:
                    fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
                    fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
                    RPN_INTERNAL_ERROR;
                    break;
               }

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
               {
                    struct rpn_matrix        *_mat_B, *_mat_C;
                    int                       _n, _p, _i, _j, _idx, _sum, _size;
                    struct rpn_elt           *_elt, *_scalar;
                    struct rpn_stack         *_stack;
                    _mat_B                   = (struct rpn_matrix *) _stk_y->value.obj;
                    _n                       = _mat_B->n;
                    _p                       = _mat_B->p;

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
                    _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                    _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
                    _mat_C->n                = _n;
                    _mat_C->p                = _p;
                    _stk_result->value.obj   = _mat_C;

                    _stack                   = rpn_new_stack(__func__);
                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _elt      = rpn_mat_extract_elt(_mat_B, _i, _j);
                              rpn_push(_stack, _elt);

                              _scalar   = rpn_clone_elt(_stk_x);
                              rpn_push(_stack, _scalar);

                              if ((_retcode = rpn_op_generic2(_stack, op)) != 0) {
                                   rpn_err_msg_op_error(op->op_name, _retcode);
                                   exit(RPN_EXIT_OP_ERROR);
                              }

                              _elt                = rpn_pop(_stack);
                              _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
                              _mat_C->base[_idx]  = (void *) _elt;
                         }
                    }

                    rpn_free_stack(_stack);
                    rpn_push(stack, _stk_result);
               }
               break;
// }}}
          case RPN_TYPE_NIL:
// {{{
               rpn_push(stack, _stk_x);
               break;
// }}}
          default:
// {{{
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
// }}}
          }
          break;
// }}}
     case RPN_TYPE_STRING:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_STRING:
// {{{
               _s1                      = _stk_y->value.s;
               _s2                      = _stk_x->value.s;

               switch (op->op_id) {

               case RPN_OP_ID_ADD:
                    _lg                      = strlen(_s1) + strlen(_s2);
                    _s                       = RPN_MALLOC(_lg + 1);

                    strcpy(_s, _s1);
                    strcat(_s, _s2);
                    break;

               case RPN_OP_ID_SUB:
               case RPN_OP_ID_MUL:
               case RPN_OP_ID_DIV:
               default:
                    RPN_INTERNAL_ERROR;
                    break;
               }

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_STRING);
               _stk_result->value.s     = _s;

               rpn_push(stack, _stk_result);
               break;
// }}}

          case RPN_TYPE_NIL:
               rpn_push(stack, _stk_x);
               break;

          case RPN_TYPE_INT:
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;

          default:
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
          }
          break;
// }}}
     case RPN_TYPE_LITTERAL:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_NIL:
               rpn_push(stack, _stk_x);
               break;

          case RPN_TYPE_LITTERAL:
               _s1                      = _stk_y->value.s;
               _s2                      = _stk_x->value.s;

               _lg                      = strlen(_s1) + strlen(_s2);
               _s                       = RPN_MALLOC(_lg + 8);

               sprintf(_s, "(%s) %s (%s)", _s1, op->op_name, _s2);

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_LITTERAL);
               _stk_result->value.s     = _s;

               rpn_push(stack, _stk_result);
               break;

          default:
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
          }
          break;
// }}}
     case RPN_TYPE_MATRIX:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               {
                    struct rpn_matrix        *_mat_A, *_mat_C;
                    int                       _n, _p, _i, _j, _idx, _sum, _size;
                    struct rpn_elt           *_elt, *_scalar;
                    struct rpn_stack         *_stack;
                    _mat_A                   = (struct rpn_matrix *) _stk_x->value.obj;
                    _n                       = _mat_A->n;
                    _p                       = _mat_A->p;

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
                    _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                    _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
                    _mat_C->n                = _n;
                    _mat_C->p                = _p;
                    _stk_result->value.obj   = _mat_C;

                    _stack                   = rpn_new_stack(__func__);
                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _elt      = rpn_mat_get_elt_ref(_mat_A, _i, _j);
                              rpn_push(_stack, _elt);

                              _scalar   = rpn_clone_elt(_stk_y);
                              rpn_push(_stack, _scalar);

                              if ((_retcode = rpn_op_generic2(_stack, op)) != 0) {
                                   rpn_err_msg_op_error(op->op_name, _retcode);
                                   exit(RPN_EXIT_OP_ERROR);
                              }

                              _elt                = rpn_pop(_stack);
                              _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
                              _mat_C->base[_idx]  = (void *) _elt;
                         }
                    }

                    rpn_free_stack(_stack);
                    rpn_push(stack, _stk_result);
               }
               break;

// }}}
          case RPN_TYPE_DOUBLE:
// {{{
               {
                    struct rpn_matrix        *_mat_A, *_mat_C;
                    int                       _n, _p, _i, _j, _idx, _sum, _size;
                    struct rpn_elt           *_elt, *_scalar;
                    struct rpn_stack         *_stack;
                    _mat_A                   = (struct rpn_matrix *) _stk_x->value.obj;
                    _n                       = _mat_A->n;
                    _p                       = _mat_A->p;

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
                    _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                    _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
                    _mat_C->n                = _n;
                    _mat_C->p                = _p;
                    _stk_result->value.obj   = _mat_C;

                    _stack                   = rpn_new_stack(__func__);
                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _elt      = rpn_mat_get_elt_ref(_mat_A, _i, _j);
                              rpn_push(_stack, _elt);

                              _scalar   = rpn_clone_elt(_stk_y);
                              rpn_push(_stack, _scalar);

                              if ((_retcode = rpn_op_generic2(_stack, op)) != 0) {
                                   rpn_err_msg_op_error(op->op_name, _retcode);
                                   exit(RPN_EXIT_OP_ERROR);
                              }

                              _elt                = rpn_pop(_stack);
                              _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
                              _mat_C->base[_idx]  = (void *) _elt;
                         }
                    }

                    rpn_free_stack(_stack);
                    rpn_push(stack, _stk_result);
               }
               break;

// }}}
          case RPN_TYPE_MATRIX:
// {{{
               switch (op->op_id) {

#define   HCAT_VERSION   2
#if (HCAT_VERSION == 1)
               case RPN_OP_ID_H_CAT:    /* VERSION 1 */
// {{{
                    {
                         struct rpn_matrix        *_mat_A, *_mat_B, *_mat_C;
                         int                       _n, _m, _pA, _pB, _i, _j, _k, _idx, _sum, _size;
                         struct rpn_elt           *_elt, *_nil;
                         struct rpn_stack         *_stack;

                         _mat_A                   = (struct rpn_matrix *) _stk_y->value.obj;
                         _mat_B                   = (struct rpn_matrix *) _stk_x->value.obj;

                         /* Check dimensions : A : n x p;   B : n x p'
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         if ((_mat_A->n != _mat_B->n)) {
                              // Incompatible dimensions
                              rpn_push(stack, _stk_y);
                              rpn_push(stack, _stk_x);

                              _retcode                 = RPN_RET_INCOMPATIBLE_DIM;
                              goto end;
                         }

                         _n                       = _mat_A->n;
                         _pA                      = _mat_A->p;
                         _pB                      = _mat_B->p;

                         rpn_set_lastx(stack, _stk_x);

                         _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
                         _size                    = sizeof(*_mat_C) + (((_n * (_pA + _pB)) - 1) * sizeof (void *));
                         _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
                         _mat_C->n                = _n;
                         _mat_C->p                = _pA + _pB;
                         _stk_result->value.obj   = _mat_C;

                         _stack                   = rpn_new_stack(__func__);

                         for (_i = 1, _idx = 0; _i <= _n; _i++) {
                              for (_j = 1; _j <= _pA; _j++) {
                                   _elt                     = rpn_mat_get_elt_ref(_mat_A, _i, _j);
                                   _mat_C->base[_idx++]     = (void *) rpn_clone_elt(_elt);
                              }

                              for (_j = 1; _j <= _pB; _j++) {
                                   _elt                     = rpn_mat_get_elt_ref(_mat_B, _i, _j);
                                   _mat_C->base[_idx++]     = (void *) rpn_clone_elt(_elt);
                              }
                         }

                         rpn_free_stack(_stack);
                         rpn_push(stack, _stk_result);
                    }
                    break;

#elif (HCAT_VERSION == 2)
               case RPN_OP_ID_H_CAT:    /* VERSION 2 */
                    {
                         struct rpn_matrix        *_mat_A, *_mat_B, *_mat_C;
                         int                       _n, _m, _pA, _pB, _i, _j, _k, _idx, _sum, _size;
                         struct rpn_elt           *_elt, *_nil;
                         struct rpn_stack         *_stack;

                         _mat_A                   = (struct rpn_matrix *) _stk_y->value.obj;
                         _mat_B                   = (struct rpn_matrix *) _stk_x->value.obj;

                         /* Check dimensions : A : n x p;   B : n x p'
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         if ((_mat_A->n != _mat_B->n)) {
                              // Incompatible dimensions
                              rpn_push(stack, _stk_y);
                              rpn_push(stack, _stk_x);

                              _retcode                 = RPN_RET_INCOMPATIBLE_DIM;
                              goto end;
                         }

                         _n                       = _mat_A->n;
                         _pA                      = _mat_A->p;
                         _pB                      = _mat_B->p;

                         rpn_set_lastx(stack, _stk_x);

                         _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
                         _size                    = sizeof(*_mat_C) + (((_n * (_pA + _pB)) - 1) * sizeof (void *));
                         _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
                         _mat_C->n                = _n;
                         _mat_C->p                = _pA + _pB;
                         _stk_result->value.obj   = _mat_C;

                         _stack                   = rpn_new_stack(__func__);

                         for (_i = 1, _idx = 0; _i <= _n; _i++) {
                              for (_j = 1; _j <= _pA; _j++) {
                                   _mat_C->base[_idx++]     = rpn_mat_extract_elt(_mat_A, _i, _j);
                              }

                              for (_j = 1; _j <= _pB; _j++) {
                                   _mat_C->base[_idx++]     = rpn_mat_extract_elt(_mat_B, _i, _j);
                              }
                         }

                         rpn_free_stack(_stack);
                         rpn_push(stack, _stk_result);
                    }
                    break;
#endif
// }}}

#define   VCAT_VERSION   2
#if (VCAT_VERSION == 1)
               case RPN_OP_ID_V_CAT:    /* VERSION 1 */
// {{{
                    {
                         struct rpn_matrix        *_mat_A, *_mat_B, *_mat_C;
                         int                       _nA, _nB, _m, _p, _i, _j, _k, _idx, _sum, _size;
                         struct rpn_elt           *_elt, *_nil;
                         struct rpn_stack         *_stack;

                         _mat_A                   = (struct rpn_matrix *) _stk_y->value.obj;
                         _mat_B                   = (struct rpn_matrix *) _stk_x->value.obj;

                         /* Check dimensions : A : n x p;   B : n' x p
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         if ((_mat_A->p != _mat_B->p)) {
                              // Incompatible dimensions
                              rpn_push(stack, _stk_y);
                              rpn_push(stack, _stk_x);

                              _retcode                 = RPN_RET_INCOMPATIBLE_DIM;
                              goto end;
                         }

                         _nA                      = _mat_A->n;
                         _nB                      = _mat_B->n;
                         _p                       = _mat_A->p;

                         rpn_set_lastx(stack, _stk_x);

                         _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
                         _size                    = sizeof(*_mat_C) + ((((_nA + _nB) * _p) - 1) * sizeof (void *));
                         _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
                         _mat_C->n                = _nA + _nB;
                         _mat_C->p                = _p;
                         _stk_result->value.obj   = _mat_C;

                         _stack                   = rpn_new_stack(__func__);

                         for (_i = 1, _idx = 0; _i <= _nA; _i++) {
                              for (_j = 1; _j <= _p; _j++) {
                                   _elt                     = rpn_mat_get_elt_ref(_mat_A, _i, _j);
                                   _mat_C->base[_idx++]     = (void *) rpn_clone_elt(_elt);
                              }
                         }

                         for (_i = 1; _i <= _nB; _i++) {
                              for (_j = 1; _j <= _p; _j++) {
                                   _elt                     = rpn_mat_get_elt_ref(_mat_B, _i, _j);
                                   _mat_C->base[_idx++]     = (void *) rpn_clone_elt(_elt);
                              }
                         }

                         rpn_free_stack(_stack);
                         rpn_push(stack, _stk_result);
                    }
                    break;

#elif (VCAT_VERSION == 2)

               case RPN_OP_ID_V_CAT:    /* VERSION 2 */
                    {
                         struct rpn_matrix        *_mat_A, *_mat_B, *_mat_C;
                         int                       _nA, _nB, _m, _p, _i, _j, _k, _idx, _sum, _size;
                         struct rpn_elt           *_elt, *_nil;
                         struct rpn_stack         *_stack;

                         _mat_A                   = (struct rpn_matrix *) _stk_y->value.obj;
                         _mat_B                   = (struct rpn_matrix *) _stk_x->value.obj;

                         /* Check dimensions : A : n x p;   B : n' x p
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         if ((_mat_A->p != _mat_B->p)) {
                              // Incompatible dimensions
                              rpn_push(stack, _stk_y);
                              rpn_push(stack, _stk_x);

                              _retcode                 = RPN_RET_INCOMPATIBLE_DIM;
                              goto end;
                         }

                         _nA                      = _mat_A->n;
                         _nB                      = _mat_B->n;
                         _p                       = _mat_A->p;

                         rpn_set_lastx(stack, _stk_x);

                         _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
                         _size                    = sizeof(*_mat_C) + ((((_nA + _nB) * _p) - 1) * sizeof (void *));
                         _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
                         _mat_C->n                = _nA + _nB;
                         _mat_C->p                = _p;
                         _stk_result->value.obj   = _mat_C;

                         _stack                   = rpn_new_stack(__func__);

                         for (_i = 1, _idx = 0; _i <= _nA; _i++) {
                              for (_j = 1; _j <= _p; _j++) {
                                   _mat_C->base[_idx++]     = rpn_mat_extract_elt(_mat_A, _i, _j);
                              }
                         }

                         for (_i = 1; _i <= _nB; _i++) {
                              for (_j = 1; _j <= _p; _j++) {
                                   _mat_C->base[_idx++]     = rpn_mat_extract_elt(_mat_B, _i, _j);
                              }
                         }

                         rpn_free_stack(_stack);
                         rpn_push(stack, _stk_result);
                    }
                    break;
#endif
// }}}

#define   VPAD_VERSION   2
#if (VPAD_VERSION == 1)
               case RPN_OP_ID_V_PAD:    /* VERSION 1 */
// {{{
                    {
                         struct rpn_matrix        *_mat_A, *_mat_B, *_mat_C, *_mat_P;
                         int                       _nA, _nB, _nC, _nP, _pA, _pB, _pC, _pP,
                                                   _i, _j, _k, _idx, _sum, _size;
                         struct rpn_elt           *_elt, *_nil, *_stk_vP, *_stk_nP, *_stk_pP,
                                                  *_stk_P, *_stk_C;
                         struct rpn_stack         *_stack;

                         _mat_A                   = (struct rpn_matrix *) _stk_y->value.obj;
                         _mat_B                   = (struct rpn_matrix *) _stk_x->value.obj;

                         _nA                      = _mat_A->n;
                         _pA                      = _mat_A->p;

                         _nB                      = _mat_B->n;
                         _pB                      = _mat_B->p;

                         if ((_pB > _pA)) {
                              // _pB must be less than or equal to _pA
                              rpn_push(stack, _stk_y);
                              rpn_push(stack, _stk_x);

                              _retcode                 = RPN_RET_INCOMPATIBLE_DIM;
                              goto end;
                         }

                         if (_pB == _pA) {
                              /* Nothing to pad
                                 ~~~~~~~~~~~~~~ */
                              rpn_push(stack, _stk_y);
                              rpn_push(stack, _stk_x);

                              goto end;
                         }

                         _nP                      = _nB;
                         _pP                      = _pA - _pB;

                         rpn_set_lastx(stack, _stk_x);

                         /* Value of the elements of the padding matrix */
                         _stk_vP                  = rpn_new_elt(RPN_TYPE_INT);
                         _stk_vP->value.i         = 0;

                         /* Number of lines of the padding matrix */
                         _stk_nP                  = rpn_new_elt(RPN_TYPE_INT);
                         _stk_nP->value.i         = _nP;

                         /* Number of columns of the padding matrix */
                         _stk_pP                  = rpn_new_elt(RPN_TYPE_INT);
                         _stk_pP->value.i         = _pP;

                         _stack                   = rpn_new_stack(__func__);
                         rpn_push(_stack, _stk_vP);
                         rpn_push(_stack, _stk_nP);
                         rpn_push(_stack, _stk_pP);

                         if ((_retcode = rpn_op_z_mat(_stack, RPN_OP_DESC(Z_MAT))) != 0) {
                              rpn_err_msg_op_error(RPN_OP_DESC(Z_MAT)->op_name, _retcode);
                              exit(RPN_EXIT_OP_ERROR);
                         }

                         _stk_P                   = rpn_pop(_stack);
                         _mat_P                   = (struct rpn_matrix *) _stk_P->value.obj;

                         _stk_C                   = rpn_new_elt(RPN_TYPE_MATRIX);
                         _size                    = sizeof(*_mat_C) + (((_nB * (_pB + _pP)) - 1) * sizeof (void *));
                         _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
                         _mat_C->n                = _nB;
                         _mat_C->p                = _pB + _pP;
                         _stk_C->value.obj        = _mat_C;


                         for (_i = 1, _idx = 0; _i <= _nB; _i++) {
                              for (_j = 1; _j <= _pB; _j++) {
                                   _mat_C->base[_idx++]     = rpn_mat_get_elt_ref(_mat_B, _i, _j);
                              }

                              for (_j = 1; _j <= _pP; _j++) {
                                   _elt                     = rpn_mat_get_elt_ref(_mat_P, _i, _j);
                                   _mat_C->base[_idx++]     = (void *) rpn_clone_elt(_elt);
                              }
                         }

                         rpn_free_stack(_stack);

                         /* Let matrix A in the stack (Y register) */
                         rpn_push(stack, _stk_y);

                         /* Put padded matrix in the X register */
                         rpn_push(stack, _stk_C);
                    }
                    break;

#elif (VPAD_VERSION == 2)
               case RPN_OP_ID_V_PAD:    /* VERSION 2 */
                    {
                         struct rpn_matrix        *_mat_A, *_mat_B, *_mat_C, *_mat_P;
                         int                       _nA, _nB, _nC, _nP, _pA, _pB, _pC, _pP,
                                                   _i, _j, _k, _idx, _sum, _size;
                         struct rpn_elt           *_elt, *_nil, *_stk_vP, *_stk_nP, *_stk_pP,
                                                  *_stk_P, *_stk_C;
                         struct rpn_stack         *_stack;

                         _mat_A                   = (struct rpn_matrix *) _stk_y->value.obj;
                         _mat_B                   = (struct rpn_matrix *) _stk_x->value.obj;

                         _nA                      = _mat_A->n;
                         _pA                      = _mat_A->p;

                         _nB                      = _mat_B->n;
                         _pB                      = _mat_B->p;

                         if ((_pB > _pA)) {
                              // _pB must be less than or equal to _pA
                              rpn_push(stack, _stk_y);
                              rpn_push(stack, _stk_x);

                              _retcode                 = RPN_RET_INCOMPATIBLE_DIM;
                              goto end;
                         }

                         if (_pB == _pA) {
                              /* Nothing to pad
                                 ~~~~~~~~~~~~~~ */
                              rpn_push(stack, _stk_y);
                              rpn_push(stack, _stk_x);

                              goto end;
                         }

                         _nP                      = _nB;
                         _pP                      = _pA - _pB;

                         rpn_set_lastx(stack, _stk_x);

                         /* Value of the elements of the padding matrix */
                         _stk_vP                  = rpn_new_elt(RPN_TYPE_INT);
                         _stk_vP->value.i         = 0;

                         /* Number of lines of the padding matrix */
                         _stk_nP                  = rpn_new_elt(RPN_TYPE_INT);
                         _stk_nP->value.i         = _nP;

                         /* Number of columns of the padding matrix */
                         _stk_pP                  = rpn_new_elt(RPN_TYPE_INT);
                         _stk_pP->value.i         = _pP;

                         _stack                   = rpn_new_stack(__func__);
                         rpn_push(_stack, _stk_vP);
                         rpn_push(_stack, _stk_nP);
                         rpn_push(_stack, _stk_pP);

//rpn_disp_stack(_stack);
                         if ((_retcode = rpn_op_z_mat(_stack, RPN_OP_DESC(Z_MAT))) != 0) {
                              rpn_err_msg_op_error(RPN_OP_DESC(Z_MAT)->op_name, _retcode);
                              exit(RPN_EXIT_OP_ERROR);
                         }

                         _stk_P                   = rpn_pop(_stack);
                         _mat_P                   = (struct rpn_matrix *) _stk_P->value.obj;

                         _stk_C                   = rpn_new_elt(RPN_TYPE_MATRIX);
                         _size                    = sizeof(*_mat_C) + (((_nB * (_pB + _pP)) - 1) * sizeof (void *));
                         _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
                         _mat_C->n                = _nB;
                         _mat_C->p                = _pB + _pP;
                         _stk_C->value.obj        = _mat_C;


                         for (_i = 1, _idx = 0; _i <= _nB; _i++) {
                              for (_j = 1; _j <= _pB; _j++) {
                                   _mat_C->base[_idx++]     = rpn_mat_get_elt_ref(_mat_B, _i, _j);
                              }

                              for (_j = 1; _j <= _pP; _j++) {
                                   _elt                     = rpn_mat_get_elt_ref(_mat_P, _i, _j);
                                   _mat_C->base[_idx++]     = (void *) rpn_clone_elt(_elt);
                              }
                         }

                         rpn_free_stack(_stack);

                         /* Let matrix A in the stack (Y register) */
                         rpn_push(stack, _stk_y);

                         /* Put padded matrix in the X register */
                         rpn_push(stack, _stk_C);
                    }
                    break;
#endif
// }}}

               default:
// {{{
                    {
                         struct rpn_matrix        *_mat_A, *_mat_B, *_mat_C;
                         int                       _n, _m, _p, _i, _j, _k, _idx, _sum, _size;
                         struct rpn_elt           *_elt, *_nil;
                         struct rpn_stack         *_stack;

// printf("%s [%s (%d)] Y : type = %s\n", __func__, __FILE__, __LINE__,  rpn_type_to_string(_stk_y->type));
// printf("%s [%s (%d)] X : type = %s\n", __func__, __FILE__, __LINE__,  rpn_type_to_string(_stk_x->type));
                         _mat_A                   = (struct rpn_matrix *) _stk_y->value.obj;
                         _mat_B                   = (struct rpn_matrix *) _stk_x->value.obj;

                         /* Check dimensions : A : n x p;   B : n x p
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         if ((_mat_A->n != _mat_B->n)
                         ||  (_mat_A->p != _mat_B->p)) {
                              // Incompatible dimensions
                              rpn_push(stack, _stk_y);
                              rpn_push(stack, _stk_x);

                              _retcode                 = RPN_RET_INCOMPATIBLE_ELTS;
                              goto end;
                         }

                         _n                       = _mat_A->n;
                         _p                       = _mat_A->p;

                         rpn_set_lastx(stack, _stk_x);

                         _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
                         _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                         _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
                         _mat_C->n                = _n;
                         _mat_C->p                = _p;
                         _stk_result->value.obj   = _mat_C;

                         _stack                   = rpn_new_stack(__func__);

                         for (_i = 1; _i <= _n; _i++) {
                              for (_j = 1; _j <= _p; _j++) {
                                   _elt      = rpn_mat_extract_elt(_mat_A, _i, _j);
                                   rpn_push(_stack, _elt);

                                   _elt      = rpn_mat_extract_elt(_mat_B, _i, _j);
                                   rpn_push(_stack, _elt);

                                   if ((_retcode = rpn_op_generic2(_stack, op)) != 0) {
                                        rpn_err_msg_op_error(op->op_name, _retcode);
                                        exit(RPN_EXIT_OP_ERROR);
                                   }

                                   _elt                = rpn_pop(_stack);
                                   _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
                                   _mat_C->base[_idx]  = (void *) _elt;
                              }
                         }

                         if ((_mat_C->n == 1)
                         &&  (_mat_C->p == 1)) {
                              _stk_result              = rpn_mat_get_elt_ref(_mat_C, 1, 1);
                         }

                         rpn_free_stack(_stack);
                         rpn_push(stack, _stk_result);
                    }
                    break;
// }}}
               }
          break;
// }}}
          case RPN_TYPE_NIL:
// {{{
               rpn_push(stack, _stk_x);
               break;
// }}}
          default:
// {{{
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
          }
          break;
// }}}
// }}}
     case RPN_TYPE_NIL:
// {{{
          rpn_push(stack, _stk_y);
          break;
// }}}
     case RPN_TYPE_FILENAME:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_MATRIX:
               {
                    struct rpn_backup_ctx     _backup_ctx;

                    _backup_ctx.filename      = _stk_x->value.s;
                    rpn_backup_elt(_stk_y, &_backup_ctx);
               }
               break;

          case RPN_TYPE_NEURAL_MLP:
               switch (op->op_id) {

               case RPN_OP_ID_SAVE:
                    printf("SAVE NEURAL NETWORK ...\n");
                    rpn_save_mlp(_stk_y);
                    break;

               case RPN_OP_ID_LOAD:
                    printf("LOAD NEURAL NETWORK ...\n");
                    break;

               case RPN_OP_ID_RESTORE:
                    printf("RESTORE NEURAL NETWORK ...\n");
                    break;
               }
               break;

          default:
               break;
          }
          break;

// }}}
     case RPN_TYPE_LIST:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_LIST:

               switch (op->op_id) {

               case RPN_OP_ID_MUL:
                    {
                         rpn_elt                  *_stk_mat;
// TODO : Create matrix whose components are n-tuples XXX XXX XXX XXX XXX
                         RPN_INTERNAL_ERROR;
                    }
                    break;

               default:
                    fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
                    fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
                    RPN_INTERNAL_ERROR;
                    break;
               }
               break;

          default:
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
          }
          break;

// }}}
     default:
// {{{
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_TYPES;

          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
          RPN_INTERNAL_ERROR;
          break;
// }}}
     }

// printf("%s [%s (%d)] Y : type = %s\n", __func__, __FILE__, __LINE__,  rpn_type_to_string(_stk_y->type));
// printf("%s [%s (%d)] X : type = %s\n", __func__, __FILE__, __LINE__,  rpn_type_to_string(_stk_x->type));
// rpn_disp_stack(stack);

     rpn_free_elt(&_stk_x);
// printf("%s [%s (%d)] Y : type = %s\n", __func__, __FILE__, __LINE__,  rpn_type_to_string(_stk_y->type));
     if (_Y_type != RPN_TYPE_NIL) {
// printf("%s [%s (%d)] Y : type = %s\n", __func__, __FILE__, __LINE__,  rpn_type_to_string(_stk_y->type));
          rpn_free_elt(&_stk_y);
     }

end:

     return _retcode;
}
/* rpn_op_generic2() }}} */
/* rpn_op_generic_list() {{{ */

/******************************************************************************

                         RPN_OP_GENERIC_LIST

******************************************************************************/
RPN_DEF_OP(rpn_op_generic_list)
{
     rpn_elt                  *_stk_x, *_stk_result;
     struct rpn_stack         *_stack;
     int                       _nb_layers, _X_type, _size, _prev_dim, _dim,
                               _layer, _i, _nb_neurons, _nb_weights,
                               _nb_litteral = 0;
     int                       _retcode;
     struct rpn_mlp           *_mlp;
     struct rpn_perceptron    *_perceptron;
     struct rpn_neuron        *_neuron;

     _retcode                 = RPN_RET_OK;

     /* Control that there is a BEGIN marker in the stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (stack->nb_begin == 0) {
          _retcode  = RPN_RET_NO_START_MARKER;
          rpn_err_msg_op_error(op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     /* Allocate temporary new stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _stack                   = rpn_new_stack(__func__);
     _nb_layers               = 0;

     /* Transfer elements into the new stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     for (;;) {
          _stk_x                   = rpn_pop(stack);
          _X_type                  =  rpn_get_type(_stk_x);
//printf("X TYPE = %s\n", rpn_type_to_string(_X_type));
          if (_X_type == RPN_TYPE_START_MARKER) {
               rpn_dec_begin(stack);
               break;
          }

          switch (op->op_id) {

          case RPN_OP_ID_NN_CREAT_MLP:
               /* Only integers are allowed to specifiy dimensions
                * and only one litteral is allowed to specify which
                * activation function will be used
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               switch (_X_type) {

               case RPN_TYPE_INT:
                    break;

               case RPN_TYPE_LITTERAL:
                    if (++_nb_litteral > 1) {
                         /* Only one litteral is allowed
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _retcode                 = RPN_RET_INVALID_TYPES;
                         goto end;
                    }
                    break;

               default:
                    _retcode                 = RPN_RET_INVALID_TYPES;
                    goto end;
               }

               _nb_layers++;
               break;

          default:
               RPN_INTERNAL_ERROR;
               break;
          }

          rpn_push(_stack, _stk_x);
          rpn_disp_stack(_stack);
     }

     /* Decrease nb_layers by 1 to take the input layer into account
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _nb_layers--;

     switch (op->op_id) {

     case RPN_OP_ID_NN_CREAT_MLP:
          if (_nb_layers < 2) {
               /* Not enough layers
                  ~~~~~~~~~~~~~~~~~ */
               _retcode                 = RPN_RET_NOT_ENOUGH_ELTS;

               /* Restore stack contents
                  ~~~~~~~~~~~~~~~~~~~~~~ */
               for (; _stack->num_elts > 0; ) {
                    _stk_x    = rpn_pop(_stack);
                    rpn_push(stack, _stk_x);
               }

               goto end;
          }

          /* Allocate and create a neural network
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          _stk_result              = rpn_new_elt(RPN_TYPE_NEURAL_MLP);

          _layer                   = 0;
          _prev_dim                = 0;
          _dim                     = 0;
          _mlp                     = rpn_new_mlp(_nb_layers);
          _stk_result->value.obj   = _mlp;

          for (; _stack->num_elts > 0; ) {
               _stk_x                   = rpn_pop(_stack);
               _X_type                  =  rpn_get_type(_stk_x);
//printf("X TYPE = %s\n", rpn_type_to_string(_X_type));
               if (_X_type == RPN_TYPE_INT) {
                    _prev_dim                = _dim;
                    _dim                     = _stk_x->value.i;
                    _nb_neurons              = _dim;
                    _nb_weights              = _prev_dim;

                    if (_prev_dim == 0) {
                         _mlp->input              = rpn_new_input_layer(_dim);
                         _perceptron              = NULL;
                    }
                    else {
                         _perceptron              = rpn_new_perceptron(_nb_neurons);

                         for (_i = 0; _i < _nb_neurons; _i++) {
                              _neuron                  = rpn_new_neuron(_nb_weights);
                              _perceptron->neurons[_i] = _neuron;
                              _mlp->nb_weights         += _nb_weights + 1;
                         }
                         _mlp->nb_neurons         += _nb_neurons;
                         _mlp->layers[_layer++]   = _perceptron;
//fprintf(stderr, "mlp->layers[%d] = 0x%08X\n", _layer - 1, _mlp->layers[_layer - 1]);
                    }

               }
               else if (_X_type == RPN_TYPE_LITTERAL) {
                    char                     *_af_function_str;

                    /* Decode activation function specifier
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _af_function_str         = _stk_x->value.s;

                    if (!strcmp(_af_function_str, RPN_NN_AF_SIGMOID)) {
                         _mlp->activation         = rpn_nn_af_sigmoid;
                    }
                    else if (!strcmp(_af_function_str, RPN_NN_AF_TANH)) {
                         _mlp->activation         = rpn_nn_af_tanh;
                    }
                    else if (!strcmp(_af_function_str, RPN_NN_AF_ARC_TAN)) {
                         _mlp->activation         = rpn_nn_af_arc_tan;
                    }
                    else if (!strcmp(_af_function_str, RPN_NN_AF_RELU)) {
                         _mlp->activation         = rpn_nn_af_relu;
                    }
                    else {
                         fprintf(stderr, "%s (%d) : unknown activation function (\"%s\')\n",
                                 _af_function_str);
                    }
                    _mlp->str_af             = strdup(_af_function_str);
               }
//printf("\n");
          }
          break;

     default:
          fprintf(stderr, "%s (%s) (%d) : not implemented for \"%s\"\n",
                  G.progname, __func__, __LINE__, op->op_id);
          RPN_INTERNAL_ERROR;
     }

//printf("MLP : %10d layers\n",  _mlp->nb_layers);
//printf("MLP : %10d neurons\n", _mlp->nb_neurons);
//printf("MLP : %10d weights\n",   _mlp->nb_weights);
//printf("\n");

     rpn_free_stack(_stack);
     rpn_push(stack, _stk_result);

end:
     return _retcode;
}
/* rpn_op_generic_list() }}} */
/* rpn_new_list() {{{ */

/******************************************************************************

                         RPN_NEW_LIST

******************************************************************************/
rpn_list *rpn_new_list(char *name)
{
     rpn_list                      *_list;

     if ((_list = (rpn_list *) malloc(sizeof(*_list))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     bzero(_list, sizeof(*_list));

     _list->name                   = strdup(name);
     _list->type                   = RPN_TYPE_UNDEFINED;
     _list->homogeneous            = TRUE;

     return _list;
}
/* rpn_new_list() }}} */
/* rpn_new_pair() {{{ */

/******************************************************************************

                         RPN_NEW_PAIR

******************************************************************************/
rpn_pair *rpn_new_pair(char *name)
{
     rpn_pair                      *_pair;

     if ((_pair = (rpn_pair *) malloc(sizeof(*_pair))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     bzero(_pair, sizeof(*_pair));

     _pair->name                   = strdup(name);

     return _pair;
}

/* rpn_new_pair() }}} */
/* rpn_op_push() {{{ */

/******************************************************************************

                         RPN_OP_PUSH

******************************************************************************/
RPN_DEF_OP(rpn_op_push)
{
     int                       _retcode      = RPN_RET_OK,
                               _X_type, _Y_type;
     rpn_elt                  *_stk_x, *_stk_y;
     rpn_list                 *_list;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     _X_type                  = rpn_get_type(_stk_x);
     _Y_type                  = rpn_get_type(_stk_y);

     switch (_Y_type) {

     case RPN_TYPE_LIST:
          _list                    = _stk_y->value.obj;

          rpn_set_lastx(stack, _stk_x);

          /* Push X onto Y
             ~~~~~~~~~~~~~ */
          rpn_list_push_tail(_list, _stk_x);

          /* Push Y back on the stack
             ~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_y);

          break;

     default:
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));

          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_Y;
          break;
     }

     return _retcode;
}

/* rpn_op_push() }}} */
/* rpn_op_pop() {{{ */

/******************************************************************************

                         RPN_OP_POP

******************************************************************************/
RPN_DEF_OP(rpn_op_pop)
{
     int                       _retcode      = RPN_RET_OK,
                               _X_type;
     rpn_elt                  *_stk_x, *_elt;
     rpn_list                 *_list;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_LIST:
          _list                    = _stk_x->value.obj;

          rpn_set_lastx(stack, _stk_x);

          _elt                     = rpn_list_pop_head(_list);

          rpn_push(stack, _stk_x);
          rpn_push(stack, _elt);
          break;

     default:
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));

          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_X;
          break;
     }

     return _retcode;
}

/* rpn_op_pop() }}} */
/* GROUP : Tests {{{ */
/* rpn_op_test() {{{ */

/******************************************************************************

                         RPN_OP_TEST

******************************************************************************/
RPN_DEF_OP(rpn_op_test)
{
     rpn_elt                  *_stk_x, *_stk_y;
     int                       _result, _X_type, _Y_type;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     if (op->op_id <= RPN_OP_ID_TEST_NE) {
          /* Test with 2 operands
             ~~~~~~~~~~~~~~~~~~~~ */
          _stk_y                   = rpn_pop(stack);
          _Y_type                  = rpn_get_type(_stk_y);

          switch (_X_type) {

          case RPN_TYPE_INT:
               switch (_Y_type) {

               case RPN_TYPE_INT:
                    switch (op->op_id) {

                    case RPN_OP_ID_TEST_LT:
                         if (_stk_x->value.i < _stk_y->value.i) {
                              rpn_set_test_result(TRUE);
                         }
                         else {
                              rpn_set_test_result(FALSE);
                         }
                         break;

                    case RPN_OP_ID_TEST_LE:
                         if (_stk_x->value.i <= _stk_y->value.i) {
                              rpn_set_test_result(TRUE);
                         }
                         else {
                              rpn_set_test_result(FALSE);
                         }
                         break;

                    case RPN_OP_ID_TEST_GE:
                         if (_stk_x->value.i >= _stk_y->value.i) {
                              rpn_set_test_result(TRUE);
                         }
                         else {
                              rpn_set_test_result(FALSE);
                         }
                         break;

                    case RPN_OP_ID_TEST_GT:
                         if (_stk_x->value.i > _stk_y->value.i) {
                              rpn_set_test_result(TRUE);
                         }
                         else {
                              rpn_set_test_result(FALSE);
                         }
                         break;

                    case RPN_OP_ID_TEST_EQ:
                         if (_stk_x->value.i == _stk_y->value.i) {
                              rpn_set_test_result(TRUE);
                         }
                         else {
                              rpn_set_test_result(FALSE);
                         }
                         break;

                    case RPN_OP_ID_TEST_NE:
                         if (_stk_x->value.i != _stk_y->value.i) {
                              rpn_set_test_result(TRUE);
                         }
                         else {
                              rpn_set_test_result(FALSE);
                         }
                         break;

                    default:
                         RPN_INTERNAL_ERROR;
                         break;
                    }
                    break;

               default:
                    RPN_INTERNAL_ERROR;
                    break;
               }
               break;

          default:
               RPN_INTERNAL_ERROR;
               break;
          }

          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);
     }
     else {
          /* Test with 1 operands
             ~~~~~~~~~~~~~~~~~~~~ */
          _stk_x                   = rpn_pop(stack);

          switch (op->op_id) {

          case RPN_OP_ID_TEST_LTZ:
               
               break;

          default:
               RPN_INTERNAL_ERROR;
               break;
          }

          rpn_push(stack, _stk_x);
     }

     return RPN_RET_OK;
}

/* rpn_op_test() }}} */
/* rpn_set_test_result() {{{ */

/******************************************************************************

                         RPN_SET_TEST_RESULT

******************************************************************************/
void rpn_set_test_result(int result)
{
     switch (result) {

     case TRUE:
          printf("YES\n");
          G.test_result            = TRUE;
          break;

     case FALSE:
          printf("NO\n");
          G.test_result            = FALSE;
          break;

     default:
          printf("UNKNOWN TEST RESULT\n");
          G.test_result            = BOOL_UNDEF;
          break;
     }
}

/* rpn_set_test_result() }}} */
/* GROUP : Tests }}} */
/* GROUP : Strings {{{ */
/* rpn_op_str_length() {{{ */

/******************************************************************************

                         RPN_OP_STR_LENGTH

******************************************************************************/
RPN_DEF_OP(rpn_op_str_length)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     char                     *_s;
     int                       _result;

     _stk_x                   = rpn_pop(stack);

     _s                       = _stk_x->value.s;

     _result                  = strlen(_s);

     rpn_set_lastx(stack, _stk_x);

     _stk_result              = rpn_new_elt(RPN_TYPE_INT);
     _stk_result->value.i     = _result;

     rpn_push(stack, _stk_result);

     return RPN_RET_OK;
}
/* rpn_op_str_length() }}} */
/* rpn_op_str_catenate() {{{ */

/******************************************************************************

                         RPN_OP_STR_CATENATE

******************************************************************************/
RPN_DEF_OP(rpn_op_str_catenate)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     char                     *_s1, *_s2;
     char                     *_result;
     int                       _lg, _retcode      = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     if (rpn_get_type(_stk_x) == RPN_TYPE_STRING) {
          if (rpn_get_type(_stk_y) == RPN_TYPE_STRING) {
               _s1                      = _stk_y->value.s;
               _s2                      = _stk_x->value.s;

               _lg                      = strlen(_s1) + strlen(_s2);
               _result                  = RPN_MALLOC(_lg + 1);

               strcpy(_result, _s1);
               strcat(_result, _s2);

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_STRING);
               _stk_result->value.s     = _result;

               rpn_push(stack, _stk_result);
          }
          else {
               _retcode                 = RPN_RET_INVALID_Y;
          }
     }
     else {
          _retcode                 = RPN_RET_INVALID_X;
     }

     if (_retcode != RPN_RET_OK) {
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);
     }

     return _retcode;
}
/* rpn_op_str_catenate() }}} */
/* rpn_op_str_dupl() {{{ */

/******************************************************************************

                         RPN_OP_STR_DUPL

******************************************************************************/
RPN_DEF_OP(rpn_op_str_dupl)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     char                     *_s;
     int                       _n, _total_size, _i;
     char                     *_result;
     int                       _lg, _retcode      = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     if (rpn_get_type(_stk_x) == RPN_TYPE_INT) {
          if (rpn_get_type(_stk_y) == RPN_TYPE_STRING) {
               _s                       = _stk_y->value.s;
               _n                       = _stk_x->value.i;

               _lg                      = strlen(_s);
               _total_size              = _lg * _n;

#define   MAX_SIZE       (10240)
               if (_total_size > MAX_SIZE) {
                    /* Result would not be reasonable !
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _retcode                 = RPN_RET_TOO_BIG;
               }
               else {
                    _result                  = RPN_MALLOC(_total_size + 1);

                    strcpy(_result, _s);
                    for (_i = 0; _i < (_n - 1); _i++) {
                         strcat(_result, _s);
                    }

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt(RPN_TYPE_STRING);
                    _stk_result->value.s     = _result;

                    rpn_push(stack, _stk_result);
               }
          }
          else {
               _retcode                 = RPN_RET_INVALID_Y;
          }
     }
     else {
          _retcode                 = RPN_RET_INVALID_X;
     }

     if (_retcode != RPN_RET_OK) {
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);
     }

     return _retcode;
}
/* rpn_op_str_dupl() }}} */
/* GROUP : Strings }}} */
/* GROUP : Maths {{{ */
/* rpn_op_math_mul() {{{ */

/******************************************************************************

                         RPN_OP_MATH_MUL

******************************************************************************/
RPN_DEF_OP(rpn_op_math_mul)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _X_type, _Y_type;
     int                       _x, _y, _result, _lg, _n, _total_size, _i;
     double                    _xd, _yd, _resultd;
     char                     *_s1, *_s2, *_s, *_result_s;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     /* Check types
        ~~~~~~~~~~~ */
     if ((_retcode = rpn_check_types(op, stack)) != RPN_RET_OK) {
          goto end;
     }

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     _X_type                  = rpn_get_type(_stk_x);
     _Y_type                  = rpn_get_type(_stk_y);

// printf("%s [%s (%d)] Y : type = %s\n", __func__, __FILE__, __LINE__,  rpn_type_to_string(_stk_y->type));
// printf("%s [%s (%d)] X : type = %s\n", __func__, __FILE__, __LINE__,  rpn_type_to_string(_stk_x->type));
     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _x                       = _stk_x->value.i;
          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               _y                       = _stk_y->value.i;

               _result                  = _y * _x;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_INT);
               _stk_result->value.i     = _result;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_DOUBLE:
// {{{
               _xd                      = (double) _x;;
               _yd                      = _stk_y->value.d;

               _resultd                 = _yd * _xd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_STRING:
// {{{
               _n                       = _x;
               _s                       = _stk_y->value.s;

               _lg                      = strlen(_s);
               _total_size              = _lg * _n;

#define   MAX_SIZE       (10240)
               if (_total_size > MAX_SIZE) {
                    /* Result would not be reasonable !
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _retcode                 = RPN_RET_TOO_BIG;
               }
               else {
                    _result_s                = RPN_MALLOC(_total_size + 1);

                    strcpy(_result_s, _s);
                    for (_i = 0; _i < (_n - 1); _i++) {
                         strcat(_result_s, _s);
                    }

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt(RPN_TYPE_STRING);
                    _stk_result->value.s     = _result_s;

                    rpn_push(stack, _stk_result);
               }
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
               {
                    struct rpn_matrix        *_mat_A, *_mat_C;
                    int                       _n, _p, _i, _j, _k, _idx, _sum, _size;
                    struct rpn_elt           *_elt, *_scalar;
                    struct rpn_stack         *_stack;

                    _mat_A                   = (struct rpn_matrix *) _stk_y->value.obj;
                    _n                       = _mat_A->n;
                    _p                       = _mat_A->p;

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
                    _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                    _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
                    _mat_C->n                = _n;
                    _mat_C->p                = _p;
                    _stk_result->value.obj   = _mat_C;

                    _stack                   = rpn_new_stack(__func__);
                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _scalar   = rpn_clone_elt(_stk_x);
                              rpn_push(_stack, _scalar);

                              _elt      = rpn_mat_get_elt_ref(_mat_A, _i, _j);
                              rpn_push(_stack, _elt);

                              if ((_retcode = rpn_op_math_mul(_stack, op)) != 0) {
                                   rpn_err_msg_op_error("*", _retcode);
                                   exit(RPN_EXIT_OP_ERROR);
                              }

                              _elt                = rpn_pop(_stack);
                              _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
                              _mat_C->base[_idx]  = (void *) _elt;
                         }
                    }

                    rpn_free_stack(_stack);
                    rpn_push(stack, _stk_result);
               }
               break;
// }}}
          default:
// {{{
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
// }}}
          }
          break;
// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               _yd                      = (double)_stk_y->value.i;

               _resultd                 = _yd * _xd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_DOUBLE:
// {{{
               _yd                      = _stk_y->value.d;

               _resultd                 = _yd * _xd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
               {
                    struct rpn_matrix        *_mat_A, *_mat_C;
                    int                       _n, _p, _i, _j, _k, _idx, _sum, _size;
                    struct rpn_elt           *_elt, *_scalar;
                    struct rpn_stack         *_stack;

                    _mat_A                   = (struct rpn_matrix *) _stk_y->value.obj;
                    _n                       = _mat_A->n;
                    _p                       = _mat_A->p;

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
                    _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                    _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
                    _mat_C->n                = _n;
                    _mat_C->p                = _p;
                    _stk_result->value.obj   = _mat_C;

                    _stack                   = rpn_new_stack(__func__);
                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _scalar   = rpn_clone_elt(_stk_x);
                              rpn_push(_stack, _scalar);

                              _elt      = rpn_mat_get_elt_ref(_mat_A, _i, _j);
                              rpn_push(_stack, _elt);

                              if ((_retcode = rpn_op_math_mul(_stack, op)) != 0) {
                                   rpn_err_msg_op_error(op->op_name, _retcode);
                                   exit(RPN_EXIT_OP_ERROR);
                              }

                              _elt                = rpn_pop(_stack);
                              _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
                              _mat_C->base[_idx]  = (void *) _elt;
                         }
                    }

                    rpn_free_stack(_stack);
                    rpn_push(stack, _stk_result);
               }
               break;
// }}}
          default:
// {{{
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
// }}}
          }
          break;
// }}}
     case RPN_TYPE_LITTERAL:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_LITTERAL:
               _s1                      = _stk_y->value.s;
               _s2                      = _stk_x->value.s;

               _lg                      = strlen(_s1) + strlen(_s2);
               _s                       = RPN_MALLOC(_lg + 8);

               sprintf(_s, "(%s) * (%s)", _s1, _s2);

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_LITTERAL);
               _stk_result->value.s     = _s;

               rpn_push(stack, _stk_result);
               break;

          default:
// {{{
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
// }}}
          }
          break;
// }}}
     case RPN_TYPE_MATRIX:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_INT:
          case RPN_TYPE_DOUBLE:
// {{{
               {
                    struct rpn_matrix        *_mat_B, *_mat_C;
                    int                       _n, _p, _i, _j, _k, _idx, _sum, _size;
                    struct rpn_elt           *_elt, *_scalar;
                    struct rpn_stack         *_stack;

                    _mat_B                   = (struct rpn_matrix *) _stk_x->value.obj;
                    _n                       = _mat_B->n;
                    _p                       = _mat_B->p;

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
                    _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                    _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
                    _mat_C->n                = _n;
                    _mat_C->p                = _p;
                    _stk_result->value.obj   = _mat_C;

                    _stack                   = rpn_new_stack(__func__);
                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _elt      = rpn_mat_get_elt_ref(_mat_B, _i, _j);
                              rpn_push(_stack, _elt);

                              _scalar   = rpn_clone_elt(_stk_y);
                              rpn_push(_stack, _scalar);

                              if ((_retcode = rpn_op_math_mul(_stack, op)) != 0) {
                                   rpn_err_msg_op_error(op->op_name, _retcode);
                                   exit(RPN_EXIT_OP_ERROR);
                              }

                              _elt                = rpn_pop(_stack);
                              _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
                              _mat_C->base[_idx]  = (void *) _elt;
                         }
                    }

                    rpn_free_stack(_stack);
                    rpn_push(stack, _stk_result);
               }
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
               {
                    struct rpn_matrix        *_mat_A, *_mat_B, *_mat_C;
                    int                       _n, _m, _p, _i, _j, _k, _idx, _sum, _size;
                    struct rpn_elt           *_elt, *_nil, *_clone;
                    struct rpn_stack         *_stack;

                    _mat_A                   = (struct rpn_matrix *) _stk_y->value.obj;
                    _mat_B                   = (struct rpn_matrix *) _stk_x->value.obj;

                    /* Check dimensions : A : n x m;   B : m x p
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    if (_mat_A->p != _mat_B->n) {
                         // Incompatible dimensions
                         rpn_push(stack, _stk_y);
                         rpn_push(stack, _stk_x);

                         _retcode                 = RPN_RET_INCOMPATIBLE_ELTS;
                         goto end;
                    }

                    _n                       = _mat_A->n;
                    _m                       = _mat_A->p;
                    _p                       = _mat_B->p;

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
                    _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                    _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
                    _mat_C->n                = _n;
                    _mat_C->p                = _p;
                    _stk_result->value.obj   = _mat_C;

                    _stack                   = rpn_new_stack(__func__);

                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              for (_k = 1; _k <= _m; _k++) {
                                   _elt                = rpn_mat_get_elt_ref(_mat_A, _i, _k);
                                   _clone              = rpn_clone_elt(_elt);
                                   rpn_push(_stack, _clone);

                                   _elt                = rpn_mat_get_elt_ref(_mat_B, _k, _j);
                                   _clone              = rpn_clone_elt(_elt);
                                   rpn_push(_stack, _clone);

// printf("AVANT MULT:\n");
// rpn_disp_stack(_stack);
                                   if ((_retcode = rpn_op_math_mul(_stack, op)) != 0) {
                                        rpn_err_msg_op_error(op->op_name, _retcode);
                                        exit(RPN_EXIT_OP_ERROR);
                                   }

// printf("APRES MULT, AVANT ADD :\n");
// rpn_disp_stack(_stack);
                                   if (_k > 1) {
                                        if ((_retcode = rpn_op_generic2(_stack, RPN_OP_DESC(ADD))) != 0) {
                                             rpn_err_msg_op_error(RPN_OP_DESC(ADD)->op_name, _retcode);
                                             exit(RPN_EXIT_OP_ERROR);
                                        }
// printf("APRES ADD :\n");
// rpn_disp_stack(_stack);
                                   }
                              }

                              _elt                = rpn_pop(_stack);
                              _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
                              _mat_C->base[_idx]  = (void *) _elt;
                         }
                    }
//                  rpn_op_clst(_stack, NULL);

                    if ((_mat_C->n == 1)
                    &&  (_mat_C->p == 1)) {
                         _stk_result              = rpn_mat_get_elt_ref(_mat_C, 1, 1);
                    }

                    rpn_free_stack(_stack);
                    rpn_push(stack, _stk_result);
               }
               break;
// }}}
          default:
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
          }
          break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
          RPN_INTERNAL_ERROR;
          break;
// }}}
     }

end:
     return _retcode;
}
/* rpn_op_math_mul() }}} */
/* rpn_op_matrix() {{{ */

/******************************************************************************

                         RPN_OP_MATRIX

******************************************************************************/
RPN_DEF_OP(rpn_op_matrix)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_matrix, *_stk_elt,
                              *_elt;
     int                       _n, _p, _size, _i, _j, _idx;
     struct rpn_matrix        *_matrix;
     struct rpn_training_elt  *_train;
     struct rpn_training_data *_data;
     int                       _X_type, _Y_type = RPN_TYPE_UNDEFINED;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
          _p                       = _stk_x->value.i;
          if (_p <= 0) {
               /* p is invalid
                  ~~~~~~~~~~~~ */
               rpn_push(stack, _stk_x);

               _retcode                 = RPN_RET_INVALID_X;
               goto end;
          }

          _stk_y                   = rpn_pop(stack);
          _Y_type                  = rpn_get_type(_stk_y);

          switch (_Y_type) {

          case RPN_TYPE_INT:
               _n                       = _stk_y->value.i;
               if (_n <= 0) {
                    /* n is invalid
                       ~~~~~~~~~~~~ */
                    rpn_push(stack, _stk_y);
                    rpn_push(stack, _stk_x);

                    _retcode                 = RPN_RET_INVALID_Y;
                    goto end;
               }

               /* Check number of elements in the stack
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if (stack->num_elts < (_n * _p)) {
                    /* Too few elements in the stack
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    rpn_push(stack, _stk_y);
                    rpn_push(stack, _stk_x);

                    _retcode                 = RPN_RET_NOT_ENOUGH_ELTS;
                    goto end;
               }

               /* Create matrix element
                  ~~~~~~~~~~~~~~~~~~~~~ */
               _stk_matrix              = rpn_new_elt(RPN_TYPE_MATRIX);

               _size                    = sizeof(*_matrix) + (((_n * _p) - 1) * sizeof (void *));
               _matrix                  = (struct rpn_matrix *) RPN_MALLOC(_size);
               _matrix->n               = _n;
               _matrix->p               = _p;
               _stk_matrix->value.obj   = _matrix;

               for (_i = _n; _i > 0; _i--) {
                    for (_j = _p; _j > 0; _j--) {
                         _stk_elt            = rpn_pop(stack);
                         _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
                         _matrix->base[_idx] = (void *) _stk_elt;
// printf("M(%2d, %2d) => IDX = %3d\n", _i, _j, _idx);
// rpn_disp_elt(_stk_elt, RPN_DISP_VALUE|RPN_DISP_NEWLINE);
                    }
               }

               RPN_FREE(_stk_x);
               RPN_FREE(_stk_y);

               break;

          default:
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));

               rpn_push(stack, _stk_y);
               rpn_push(stack, _stk_x);
               break;
          }
          break;

     case RPN_TYPE_TRAINING_ELT:
     case RPN_TYPE_TEST_ELT:
          _train                   = _stk_x->value.obj;
          _data                    = _train->data;
//rpn_disp_elt(_stk_x, RPN_DISP_NIL|RPN_DISP_NEWLINE);
          _n                       = _data->num_rows;
          _p                       = _data->num_cols;

          _stk_matrix              = rpn_new_elt(RPN_TYPE_MATRIX);

          _size                    = sizeof(*_matrix) + (((_n * _p) - 1) * sizeof (void *));
          _matrix                  = (struct rpn_matrix *) RPN_MALLOC(_size);
          _matrix->n               = _n;
          _matrix->p               = _p;
          _stk_matrix->value.obj   = _matrix;

          for (_i = 1; _i <= _n; _i++) {
               for (_j = 1; _j <= _p; _j++) {
                    _elt                = rpn_new_elt(RPN_TYPE_INT);
                    _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
                    _elt->value.i       = (int) ((uint8_t *)_data->vector)[_idx];
                    _matrix->base[_idx] = (void *) _elt;
               }
          }
          break;

     default:
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
          break;
     }

     rpn_push(stack, _stk_matrix);

end:
     return _retcode;
}
/* rpn_op_matrix() }}} */
/* rpn_op_mat_nil_diag() {{{ */

/******************************************************************************

                         RPN_OP_MAT_NIL_DIAG

******************************************************************************/
RPN_DEF_OP(rpn_op_mat_nil_diag)
{
     rpn_elt                  *_stk_x, *_stk_matrix,
                              *_elt, *_elt_a, *_elt_b;
     int                       _n, _size, _i, _j, _idx;
     struct rpn_matrix        *_matrix;
     int                       _X_type;
     int                       _retcode;
     rpn_list                 *_list_a, *_list_b;
     rpn_pair                 *_pair;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_LIST:
          /* Create matrix element
             ~~~~~~~~~~~~~~~~~~~~~ */
          _stk_matrix              = rpn_new_elt(RPN_TYPE_MATRIX);

          _list_a                  = _stk_x->value.obj;
          _list_b                  = _list_a;
          _n                       = _list_a->num_elts;
          _size                    = sizeof(*_matrix) + (((_n * _n) - 1) * sizeof (void *));
          _matrix                  = (struct rpn_matrix *) RPN_MALLOC(_size);
          _matrix->n               = _n;
          _matrix->p               = _n;
          _stk_matrix->value.obj   = _matrix;

          _elt_a                   = _list_a->top_elt;
          for (_i = 1; _i <= _n; _i++) {
               _elt_b                   =  _list_b->top_elt;
               for (_j = 1; _j <= _n; _j++) {
                    if (_i == _j) {
                         _elt                = rpn_new_elt(RPN_TYPE_NIL);
                    }
                    else {
                         _elt                = rpn_new_elt(RPN_TYPE_OPAIR);
                         _pair               = rpn_new_pair((char *) __func__);

                         _pair->elt_a        = rpn_clone_elt(_elt_a);
                         _pair->elt_b        = rpn_clone_elt(_elt_b);
                         _elt->value.obj     = _pair;
                    }

                    _idx                = RPN_MATRIX_IDX(_i, _j, _n, _n);
                    _matrix->base[_idx] = (void *) _elt;
//X
//rpn_disp_elt(_elt, RPN_DISP_VALUE|RPN_DISP_NEWLINE);

                    _elt_b              = _elt_b->previous_elt;
               }

               _elt_a              = _elt_a->previous_elt;
          }

//X
//rpn_disp_elt(_stk_matrix, RPN_DISP_VALUE|RPN_DISP_NEWLINE);
          RPN_FREE(_stk_x);
          break;

     default:
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          break;
     }

     rpn_push(stack, _stk_matrix);

end:
     return _retcode;
}
/* rpn_op_mat_nil_diag() }}} */
/* rpn_op_explode() {{{ */

/******************************************************************************

                         RPN_OP_EXPLODE

******************************************************************************/
RPN_DEF_OP(rpn_op_explode)
{
     rpn_elt                  *_stk_x, *_elt;
     int                       _n, _p, _size, _i, _j, _idx;
     struct rpn_matrix        *_matrix;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     switch (_stk_x->type) {

     case RPN_TYPE_MATRIX:
          _matrix                  = (struct rpn_matrix *) _stk_x->value.obj;
          _n                       = _matrix->n;
          _p                       = _matrix->p;

          for (_i = 1; _i <= _n; _i++) {
               for (_j = 1; _j <= _p; _j++) {
                     _idx          = RPN_MATRIX_IDX(_i, _j, _n, _p);
                     _elt          = (struct rpn_elt *) _matrix->base[_idx];
                    rpn_push(stack, _elt);
               }
          }
          break;

     case RPN_TYPE_LIST:
     case RPN_TYPE_TUPLE:
          {
               rpn_list                 *_list;

               for (_list = _stk_x->value.obj; _list->num_elts > 0; ) {
                    _elt                     = rpn_list_pop_head(_list);
                    rpn_push(stack, _elt);
               }

               RPN_FREE(_list->name);
               RPN_FREE(_stk_x);
          }
          break;

     default:
          rpn_push(stack, _stk_x);
          _retcode                 = RPN_RET_INVALID_X;
          break;
     }

     return _retcode;
}
/* rpn_op_explode() }}} */
/* rpn_op_vec2() {{{ */

/******************************************************************************

                         RPN_OP_VEC2

******************************************************************************/
RPN_DEF_OP(rpn_op_vec2)
{
     rpn_elt                  *_stk_n, *_stk_p;
     int                       _n, _p;

     /* Vectors of dimension 2 are defined as matrices 2x1
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _stk_n                   = rpn_new_elt(RPN_TYPE_INT);
     _stk_n->value.i          = 2;      // 2 lines
     rpn_push(stack, _stk_n);

     _stk_p                   = rpn_new_elt(RPN_TYPE_INT);
     _stk_p->value.i          = 1;      // 1 column
     rpn_push(stack, _stk_p);

     return rpn_op_matrix(stack, RPN_OP_DESC(MATRIX));
}
/* rpn_op_vec2() }}} */
/* rpn_op_vec3() {{{ */

/******************************************************************************

                         RPN_OP_VEC3

******************************************************************************/
RPN_DEF_OP(rpn_op_vec3)
{
     rpn_elt                  *_stk_n, *_stk_p;
     int                       _n, _p;

     /* Vectors of dimension 3 are defined as matrices 3x1
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _stk_n                   = rpn_new_elt(RPN_TYPE_INT);
     _stk_n->value.i          = 3;      // 3 lines
     rpn_push(stack, _stk_n);

     _stk_p                   = rpn_new_elt(RPN_TYPE_INT);
     _stk_p->value.i          = 1;      // 1 column
     rpn_push(stack, _stk_p);

     return rpn_op_matrix(stack, RPN_OP_DESC(MATRIX));
}
/* rpn_op_vec3() }}} */
/* rpn_op_transpose() {{{ */

/******************************************************************************

                         RPN_OP_TRANSPOSE

******************************************************************************/
RPN_DEF_OP(rpn_op_transpose)
{
     rpn_elt                  *_stk_x;
     int                       _n, _p, _X_type, _i, _j, _idx_src, _idx_dst;
     int                       _retcode, _size;
     struct rpn_matrix        *_matrix, *_transpose;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_MATRIX:
          _matrix        = (struct rpn_matrix *) _stk_x->value.obj;
          _n             = _matrix->n;
          _p             = _matrix->p;

          if ((_matrix->n == 1) || (_matrix->p == 1)) {
               _matrix->n     = _p;
               _matrix->p     = _n;
          }
          else {
               _size                    = sizeof(*_transpose) + (((_n * _p) - 1)
                                          * sizeof (void *));
               _transpose               = (struct rpn_matrix *) RPN_MALLOC(_size);
               for (_i = 1; _i <= _n; _i++) {
                    for (_j = 1; _j <= _p; _j++) {
                         _idx_src                 = RPN_MATRIX_IDX(_i, _j, _n, _p);
                         _idx_dst                 = RPN_MATRIX_IDX(_j, _i, _p, _n);
                         _transpose->base[_idx_dst]  = _matrix->base[_idx_src];
                    }
               }
               _transpose->n       = _p;
               _transpose->p       = _n;

               RPN_FREE(_stk_x->value.obj);
               _stk_x->value.obj   = _transpose;
          }
          break;

     default:
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode       = RPN_RET_INVALID_X;
          goto end;
          break;
     }

end:
     rpn_push(stack, _stk_x);

     return _retcode;
}
/* rpn_op_transpose() }}} */
/* rpn_op_cross_prdct() {{{ */

/******************************************************************************

                         RPN_OP_CROSS_PRDCT

******************************************************************************/
RPN_DEF_OP(rpn_op_cross_prdct)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _n, _p;
     int                       _retcode;
     struct rpn_matrix        *_mat_x, *_mat_y;
     struct rpn_elt           *_a1, *_a2, *_a3,
                              *_b1, *_b2, *_b3,
                              *_c1, *_c2, *_c3;
     struct rpn_stack         *_stack;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     if (rpn_get_type(_stk_x) != RPN_TYPE_MATRIX) {
          rpn_push(stack, _stk_x);
          _retcode       = RPN_RET_INVALID_X;
          goto end;
     }

     _mat_x         = (struct rpn_matrix *) _stk_x->value.obj;
     if ((_mat_x->n != 3) && (_mat_x->p != 1)) {
          rpn_push(stack, _stk_x);
          _retcode       = RPN_RET_INVALID_X;
          goto end;
     }

     _stk_y                   = rpn_pop(stack);
     if (rpn_get_type(_stk_y) != RPN_TYPE_MATRIX) {
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);
          _retcode       = RPN_RET_INVALID_Y;
          goto end;
     }

     _mat_y         = (struct rpn_matrix *) _stk_y->value.obj;
     if ((_mat_y->n != 3) && (_mat_y->p != 1)) {
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);
          _retcode       = RPN_RET_INVALID_Y;
          goto end;
     }

     _stack                   = rpn_new_stack(__func__);

     _a1  = rpn_mat_get_elt_ref(_mat_y, 1, 1);
     _a2  = rpn_mat_get_elt_ref(_mat_y, 2, 1);
     _a3  = rpn_mat_get_elt_ref(_mat_y, 3, 1);

     _b1  = rpn_mat_get_elt_ref(_mat_x, 1, 1);
     _b2  = rpn_mat_get_elt_ref(_mat_x, 2, 1);
     _b3  = rpn_mat_get_elt_ref(_mat_x, 3, 1);

     rpn_push(_stack, _a2);
     rpn_push(_stack, _b3);
     rpn_op_generic2(_stack, RPN_OP_DESC(MUL));
     rpn_push(_stack, _a3);
     rpn_push(_stack, _b2);
     rpn_op_generic2(_stack, RPN_OP_DESC(MUL));
     rpn_op_generic2(_stack, RPN_OP_DESC(SUB));

     rpn_push(_stack, _a3);
     rpn_push(_stack, _b1);
     rpn_op_generic2(_stack, RPN_OP_DESC(MUL));
     rpn_push(_stack, _a1);
     rpn_push(_stack, _b3);
     rpn_op_generic2(_stack, RPN_OP_DESC(MUL));
     rpn_op_generic2(_stack, RPN_OP_DESC(SUB));

     rpn_push(_stack, _a1);
     rpn_push(_stack, _b2);
     rpn_op_generic2(_stack, RPN_OP_DESC(MUL));
     rpn_push(_stack, _a2);
     rpn_push(_stack, _b1);
     rpn_op_generic2(_stack, RPN_OP_DESC(MUL));
     rpn_op_generic2(_stack, RPN_OP_DESC(SUB));

     rpn_op_vec3(_stack, RPN_OP_DESC(VEC3));

     rpn_set_lastx(stack, _stk_x);
     _stk_result              = rpn_pop(_stack);

     rpn_free_elt(&_stk_x);
     rpn_free_elt(&_stk_y);

     rpn_free_stack(_stack);
     rpn_push(stack, _stk_result);

end:
     return _retcode;
}
/* rpn_op_cross_prdct() }}} */
/* rpn_mat_get_i() {{{ */

/******************************************************************************

                              RPN_MAT_GET_I

******************************************************************************/
int rpn_mat_get_i(struct rpn_matrix *matrix, int i, int j)
{
     struct rpn_elt      *_elt;
     int                  _idx;

     _idx                = RPN_MATRIX_IDX(i, j, matrix->n, matrix->p);
     _elt                = (struct rpn_elt *) matrix->base[_idx];

     return _elt->value.i;
}
/* rpn_mat_get_i() }}} */
/* rpn_mat_get_elt_ref() {{{ */

/******************************************************************************

                              RPN_MAT_GET_ELT_REF

******************************************************************************/
struct rpn_elt *rpn_mat_get_elt_ref(struct rpn_matrix *matrix, int i, int j)
{
     struct rpn_elt      *_elt;
     int                  _idx;

     _idx                = RPN_MATRIX_IDX(i, j, matrix->n, matrix->p);
     _elt                = (struct rpn_elt *) matrix->base[_idx];

     return _elt;
}
/* rpn_mat_get_elt_ref() }}} */
/* rpn_mat_extract_elt() {{{ */

/******************************************************************************

                              RPN_MAT_EXTRACT_ELT

******************************************************************************/
struct rpn_elt *rpn_mat_extract_elt(struct rpn_matrix *matrix, int i, int j)
{
     struct rpn_elt      *_elt;
     int                  _idx;

     _idx                = RPN_MATRIX_IDX(i, j, matrix->n, matrix->p);
     _elt                = (struct rpn_elt *) matrix->base[_idx];

     /* Reset original pointer since the element is extracted
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     matrix->base[_idx]  = NULL;

     return _elt;
}
/* rpn_mat_extract_elt() }}} */
/* rpn_op_seq() {{{ */

/******************************************************************************

                              RPN_OP_SEQ

******************************************************************************/
RPN_DEF_OP(rpn_op_seq)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _x, _y, _i;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     if ((rpn_get_type(_stk_x) == RPN_TYPE_INT)
     &&  (rpn_get_type(_stk_y) == RPN_TYPE_INT)) {
          _x                       = _stk_x->value.i;
          _y                       = _stk_y->value.i;

          if (_y <= _x) {
               for (_i = _y; _i <= _x; _i++) {
                    _stk_result              = rpn_new_elt(RPN_TYPE_INT);
                    _stk_result->value.i     = _i;

                    rpn_push(stack, _stk_result);
               }
          }
          else {
               for (_i = _y; _i >= _x; _i--) {
                    _stk_result              = rpn_new_elt(RPN_TYPE_INT);
                    _stk_result->value.i     = _i;

                    rpn_push(stack, _stk_result);
               }
          }

          rpn_set_lastx(stack, _stk_x);

          rpn_free_elt(&_stk_x);
          rpn_free_elt(&_stk_y);
     }
     else {
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_TYPES;
     }

     return _retcode;
}
/* rpn_op_seq() }}} */
/* rpn_op_dim() {{{ */

/******************************************************************************

                              RPN_OP_DIM

******************************************************************************/
RPN_DEF_OP(rpn_op_dim)
{
     rpn_elt                  *_stk_x, *_dim_row, *_dim_col;
     int                       _x, _row, _col;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     switch (rpn_get_type(_stk_x)) {

     case RPN_TYPE_NIL:
          _row                     = 0;
          _col                     = 0;

          rpn_push(stack, _stk_x);

          _dim_row                 = rpn_new_elt(RPN_TYPE_INT);
          _dim_row->value.i        = _row;
          rpn_push(stack, _dim_row);

          _dim_col                 = rpn_new_elt(RPN_TYPE_INT);
          _dim_col->value.i        = _col;
          rpn_push(stack, _dim_col);

          rpn_set_lastx(stack, _stk_x);

          rpn_free_elt(&_stk_x);
          break;

     case RPN_TYPE_MATRIX:
          {
               struct rpn_matrix        *_mat_A;
               int                       _idx, _sum, _size;
               struct rpn_elt           *_elt, *_nil;
               struct rpn_stack         *_stack;

               rpn_push(stack, _stk_x);

               _mat_A                   = (struct rpn_matrix *) _stk_x->value.obj;

               _row                     = _mat_A->n;
               _col                     = _mat_A->p;

               _dim_row                 = rpn_new_elt(RPN_TYPE_INT);
               _dim_row->value.i        = _row;
               rpn_push(stack, _dim_row);

               _dim_col                 = rpn_new_elt(RPN_TYPE_INT);
               _dim_col->value.i        = _col;
               rpn_push(stack, _dim_col);

               rpn_set_lastx(stack, _stk_x);
          }
          break;

     case RPN_TYPE_INT:
     case RPN_TYPE_DOUBLE:
          _row                     = 1;
          _col                     = 1;

          rpn_push(stack, _stk_x);

          _dim_row                 = rpn_new_elt(RPN_TYPE_INT);
          _dim_row->value.i        = _row;
          rpn_push(stack, _dim_row);

          _dim_col                 = rpn_new_elt(RPN_TYPE_INT);
          _dim_col->value.i        = _col;
          rpn_push(stack, _dim_col);

          rpn_set_lastx(stack, _stk_x);

          rpn_free_elt(&_stk_x);
          break;

     default:
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_TYPES;
          break;
     }

     return _retcode;
}
/* rpn_op_dim() }}} */
#if 0
/* rpn_op_log10() {{{ */

/******************************************************************************

                              RPN_OP_LOG10

******************************************************************************/
RPN_DEF_OP(rpn_op_log10)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _x, _result, _lg;
     double                    _xd, _resultd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     rpn_push(stack, _stk_x);

     _retcode                 = RPN_RET_INVALID_TYPES;

     return _retcode;
}
/* rpn_op_log10() }}} */
#endif    /* 0 */
/* rpn_op_10_power_x() {{{ */

/******************************************************************************

                              RPN_OP_10_POWER_X

******************************************************************************/
RPN_DEF_OP(rpn_op_10_power_x)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _x, _result, _lg;
     double                    _xd, _resultd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     rpn_push(stack, _stk_x);

     _retcode                 = RPN_RET_INVALID_TYPES;

     return _retcode;
}
/* rpn_op_10_power_x() }}} */
/* rpn_op_percent() {{{ */

/******************************************************************************

                              RPN_OP_PERCENT

******************************************************************************/
RPN_DEF_OP(rpn_op_percent)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _x, _result, _lg;
     double                    _xd, _resultd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     rpn_push(stack, _stk_x);

     _retcode                 = RPN_RET_INVALID_TYPES;

     return _retcode;
}
/* rpn_op_percent() }}} */
/* rpn_op_h_rev() {{{ */

/******************************************************************************

                         RPN_OP_H_REV

******************************************************************************/
RPN_DEF_OP(rpn_op_h_rev)
{
     rpn_elt                  *_stk_x, *_stk_result, *_elt_i;
     int                       _x, _X_type, _n, _p, _idx_src, _idx_dst;
     int                       _retcode, _size, _row, _col;
     struct rpn_stack         *_stack;
     rpn_operator             *_op;
     struct rpn_matrix        *_mat, *_mat_dst;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     /* Check types
        ~~~~~~~~~~~ */
     if (_X_type != RPN_TYPE_MATRIX) {
          _retcode                 = RPN_RET_INVALID_X;
          rpn_push(stack, _stk_x);
          goto end;
     }

     _mat                     = _stk_x->value.obj;
     _n                       = _mat->n;
     _p                       = _mat->p;

     _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);

     _size                    = sizeof(*_mat_dst) + (((_n * _p) - 1) * sizeof (void *));
     _mat_dst                 = (struct rpn_matrix *) RPN_MALLOC(_size);
     _mat_dst->n              = _n;
     _mat_dst->p              = _p;
     _stk_result->value.obj   = _mat_dst;

     /* Copy the pointers of the elements from _stk_x to _stk_result
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     for (_row = 1; _row <= _n; _row++) {
          for (_col = 1; _col <= _p; _col++) {
               _idx_src                 = RPN_MATRIX_IDX(_row, _col, _n, _p);
               _idx_dst                 = RPN_MATRIX_IDX(_n - _row + 1, _col, _n, _p);

               _mat_dst->base[_idx_dst] = _mat->base[_idx_src];

               /* Clear the pointer in the source matrix
                * since it has been moved into the destination matrix
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _mat->base[_idx_src]     = NULL;
          }
     }

     /* Free the source matrix
        ~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_free_elt(&_stk_x);

     rpn_push(stack, _stk_result);

end:
     return _retcode;
}
/* rpn_op_h_rev() }}} */
/* rpn_op_diag_mat() {{{ */

/******************************************************************************

                         RPN_OP_DIAG_MAT

******************************************************************************/
RPN_DEF_OP(rpn_op_diag_mat)
{

}
/* rpn_op_diag_mat() }}} */
/* rpn_op_scalar_matrix() {{{ */

/******************************************************************************

                              RPN_OP_SCALAR_MATRIX

******************************************************************************/
RPN_DEF_OP(rpn_op_scalar_matrix)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _x, _result, _lg;
     double                    _xd, _resultd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     rpn_push(stack, _stk_x);

     _retcode                 = RPN_RET_INVALID_TYPES;

     return _retcode;
}
/* rpn_op_scalar_matrix() }}} */
/* rpn_op_z_mat() {{{ */

/******************************************************************************

                              RPN_OP_Z_MAT

******************************************************************************/
RPN_DEF_OP(rpn_op_z_mat)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_z = NULL, *_stk_result;
     rpn_elt                  *_stk_n, *_stk_p, *_stk_value, *_stk_matrix,
                              *_stk_elt;
     int                       _n, _p, _size, _i, _j, _idx;
     struct rpn_matrix        *_matrix;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     /* Check number of elements in the stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (stack->num_elts < 3) {
          /* Too few elements in the stack
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          _retcode                 = RPN_RET_NOT_ENOUGH_ELTS;
          goto end;
     }

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     if ((rpn_get_type(_stk_y)) != RPN_TYPE_INT) {
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_Y_TYPE;
          goto end;
     }

     if ((rpn_get_type(_stk_x)) != RPN_TYPE_INT) {
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_X_TYPE;
          goto end;
     }

     if ((_p  = _stk_x->value.i) <= 0) {
          /* p is invalid
             ~~~~~~~~~~~~ */
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_X;
          goto end;
     }

     if ((_n = _stk_y->value.i) <= 0) {
          /* n is invalid
             ~~~~~~~~~~~~ */
          rpn_push(stack, _stk_z);
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_Y;
          goto end;
     }

     /* Create matrix element
        ~~~~~~~~~~~~~~~~~~~~~ */
     _stk_matrix              = rpn_new_elt(RPN_TYPE_MATRIX);

     _size                    = sizeof(*_matrix) + (((_n * _p) - 1) * sizeof (void *));
     _matrix                  = (struct rpn_matrix *) RPN_MALLOC(_size);
     _matrix->n               = _n;
     _matrix->p               = _p;
     _stk_matrix->value.obj   = _matrix;

     _stk_value               = rpn_pop(stack);

     for (_i = _n; _i > 0; _i--) {
          for (_j = _p; _j > 0; _j--) {
               _stk_elt            = rpn_clone_elt(_stk_value);
               _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
               _matrix->base[_idx] = (void *) _stk_elt;
          }
     }

     rpn_push(stack, _stk_matrix);

end:
     return _retcode;
}

/* rpn_op_z_mat() }}} */
/* GROUP : Maths }}} */
/* GROUP : Stats {{{ */
/* rpn_op_sum_add() {{{ */

/******************************************************************************

                              RPN_OP_SUM_ADD

******************************************************************************/
RPN_DEF_OP(rpn_op_sum_add)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result,
                              *_sum_x, *_sum_x2,
                              *_sum_y, *_sum_y2,
                              *_sum_xy, *_N, *_constant;
     rpn_stack                *_stack;
     int                       _retcode;
     struct rpn_operator      *_op;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     _stack                   = rpn_new_stack("sigma+ sum(x)");

     /* Sum(x)
        ~~~~~~ */
     _sum_x                   = G.sigma->reg[RPN_SIGMA_IDX_SUM_X];
     rpn_push(_stack, _sum_x);
     rpn_push(_stack, _stk_x);

     _op                      = RPN_OP_DESC(ADD);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_SUM_X]);

     /* Sum(x^2)
        ~~~~~~ */
     rpn_op_lastx(_stack, RPN_OP_DESC(LASTX));
     _sum_x2                  = G.sigma->reg[RPN_SIGMA_IDX_SUM_X2];
     rpn_push(_stack, _sum_x2);
     rpn_op_lastx(_stack, RPN_OP_DESC(LASTX));
     rpn_op_lastx(_stack, RPN_OP_DESC(LASTX));

     _op                      = RPN_OP_DESC(MATH_MUL);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _op                      = RPN_OP_DESC(ADD);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_SUM_X2]);
     rpn_op_swap_xy(_stack, RPN_OP_DESC(SWAP_XY));

     /* Sum(y)
        ~~~~~~ */
     _sum_y                   = G.sigma->reg[RPN_SIGMA_IDX_SUM_Y];
     rpn_push(_stack, _sum_y);
     rpn_push(_stack, _stk_y);

     _op                      = RPN_OP_DESC(ADD);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_SUM_Y]);

     /* Sum(y^2)
        ~~~~~~ */
     rpn_op_swap_xy(_stack, RPN_OP_DESC(SWAP_XY));
     rpn_op_lastx(_stack, RPN_OP_DESC(LASTX));
     _sum_y2                  = G.sigma->reg[RPN_SIGMA_IDX_SUM_Y2];
     rpn_push(_stack, _sum_y2);
     rpn_op_lastx(_stack, RPN_OP_DESC(LASTX));
     rpn_op_lastx(_stack, RPN_OP_DESC(LASTX));

     _op                      = RPN_OP_DESC(MATH_MUL);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _op                      = RPN_OP_DESC(ADD);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_SUM_Y2]);
     rpn_op_roll_down(_stack, RPN_OP_DESC(RDN));

     /* Sum(x*y)
        ~~~~~~~~ */
     _op                      = RPN_OP_DESC(MATH_MUL);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _sum_xy                  = G.sigma->reg[RPN_SIGMA_IDX_SUM_XY];
     rpn_push(_stack, _sum_xy);

     _op                      = RPN_OP_DESC(ADD);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_SUM_XY]);

     /* N
        ~ */
     _N                       = G.sigma->reg[RPN_SIGMA_IDX_N];
     rpn_push(_stack, _N);
     _constant                = rpn_new_int(1);
     rpn_push(_stack, _constant);

     _op                      = RPN_OP_DESC(ADD);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }
     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_N]);

//rpn_disp_sigma();

     rpn_free_stack(_stack);
//     rpn_op_clst(_stack, NULL);

     _retcode                 = RPN_RET_OK;

     return _retcode;
}
/* rpn_op_sum_add() }}} */
/* rpn_op_sum_sub() {{{ */

/******************************************************************************

                              RPN_OP_SUM_SUB

******************************************************************************/
RPN_DEF_OP(rpn_op_sum_sub)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _x, _result, _lg;
     double                    _xd, _resultd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     rpn_push(stack, _stk_x);

     _retcode                 = RPN_RET_INVALID_TYPES;

     return _retcode;
}
/* rpn_op_sum_sub() }}} */
/* rpn_op_mean() {{{ */

/******************************************************************************

                              RPN_OP_MEAN

******************************************************************************/
RPN_DEF_OP(rpn_op_mean)
{
     rpn_elt                  *_stk_x, *_stk_result,
                              *_sum_x, *_sum_y, *_N;
     rpn_stack                *_stack;
     int                       _retcode;
     struct rpn_operator      *_op;

     _retcode                 = RPN_RET_OK;

     _stack                   = rpn_new_stack("mean");

     /* Y mean
        ~~~~~~ */
     _sum_y                   = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_Y]);
     rpn_push(_stack, _sum_y);

     _N                       = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_N]);
     rpn_push(_stack, _N);

     _op                      = RPN_OP_DESC(DIV);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _stk_result              = rpn_pop(_stack);
     rpn_push(stack, _stk_result);

     /* X mean
        ~~~~~ */
     _sum_x                   = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_X]);
     rpn_push(_stack, _sum_x);

     rpn_op_lastx(_stack, RPN_OP_DESC(LASTX));

     _op                      = RPN_OP_DESC(DIV);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _stk_result              = rpn_pop(_stack);
     rpn_free_stack(_stack);
     rpn_push(stack, _stk_result);

     _retcode                 = RPN_RET_OK;

     return _retcode;
}
/* rpn_op_mean() }}} */
/* rpn_op_sdev() {{{ */

/******************************************************************************

                              RPN_OP_SDEV

******************************************************************************/
RPN_DEF_OP(rpn_op_sdev)
{
     rpn_elt                  *_stk_x, *_stk_result,
                              *_sum_x, *_sum_y,
                              *_sum_x2, *_sum_y2, *_N,
                              *_constant;
     rpn_stack                *_stack;
     struct rpn_operator      *_op;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stack                   = rpn_new_stack("sdev");

     /* Y sdev
        ~~~~~~ */
     _sum_y2                   = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_Y2]);
     rpn_push(_stack, _sum_y2);

     _sum_y                    = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_Y]);
     rpn_push(_stack, _sum_y);

     _op                      = RPN_OP_DESC(SQUARE);
     if ((_retcode = rpn_op_generic1(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _N                       = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_N]);
     rpn_push(_stack, _N);

     _op                      = RPN_OP_DESC(DIV);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _op                      = RPN_OP_DESC(SUB);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _N                       = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_N]);
     rpn_push(_stack, _N);

     _constant                = rpn_new_int(1);
     rpn_push(_stack, _constant);

     _op                      = RPN_OP_DESC(SUB);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _op                      = RPN_OP_DESC(DIV);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _op                      = RPN_OP_DESC(SQRT);
     if ((_retcode = rpn_op_generic1(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _stk_result              = rpn_pop(_stack);
     rpn_push(stack, _stk_result);

     /* X sdev
        ~~~~~~ */
     _sum_x2                   = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_X2]);
     rpn_push(_stack, _sum_x2);

     _sum_x                    = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_X]);
     rpn_push(_stack, _sum_x);

     _op                      = RPN_OP_DESC(SQUARE);
     if ((_retcode = rpn_op_generic1(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _N                       = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_N]);
     rpn_push(_stack, _N);

     _op                      = RPN_OP_DESC(DIV);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _op                      = RPN_OP_DESC(SUB);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _N                       = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_N]);
     rpn_push(_stack, _N);

     _constant                = rpn_new_int(1);
     rpn_push(_stack, _constant);

     _op                      = RPN_OP_DESC(SUB);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _op                      = RPN_OP_DESC(DIV);
     if ((_retcode = rpn_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _op                      = RPN_OP_DESC(SQRT);
     if ((_retcode = rpn_op_generic1(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _stk_result              = rpn_pop(_stack);
     rpn_free_stack(_stack);
     rpn_push(stack, _stk_result);

     _retcode                 = RPN_RET_OK;

     return _retcode;
}
/* rpn_op_sdev() }}} */
/* rpn_op_meanx() {{{ */

/******************************************************************************

                              RPN_OP_MEANX

******************************************************************************/
RPN_DEF_OP(rpn_op_meanx)
{
     rpn_elt                  *_stk_x, *_stk_result, *_sum_x, *_elt;
     rpn_stack                *_stack;
     int                       _retcode, _N, _X_type;
     struct rpn_operator      *_op;
     rpn_list                 *_list;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_LIST:
          _list                    = _stk_x->value.obj;
          if (!_list->homogeneous) {
               _retcode                 = RPN_RET_NOT_HOMOGENEOUS;
               _stk_result              = _stk_x;
               goto end;
          }

          _N                       = _list->num_elts;
          _sum_x                   = rpn_new_elt(_list->type);

          for ( ; _list->num_elts > 0; ) {
               _elt                     = rpn_list_pop_head(_list);

               switch (_list->type) {

               case RPN_TYPE_INT:
                    _sum_x->value.i               += _elt->value.i;
                    break;

               case RPN_TYPE_DOUBLE:
                    _sum_x->value.d               += _elt->value.d;
                    break;

               default:
                    RPN_INTERNAL_ERROR;
                    break;

               }

               rpn_free_elt(&_elt);
          }
          rpn_free_elt(&_stk_x);

          switch (_list->type) {

          case RPN_TYPE_INT:
               _sum_x->value.i               /= _N;
               break;

          case RPN_TYPE_DOUBLE:
               _sum_x->value.d               /= _N;
               break;

          default:
               RPN_INTERNAL_ERROR;
               break;

          }

          _stk_result              = _sum_x;
          break;

     default:
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          _stk_result              = _stk_x;
          goto end;
          break;
     }

end:

     rpn_push(stack, _stk_result);

     return _retcode;
}
/* rpn_op_mean() }}} */
/* rpn_op_mmm() {{{ */

/******************************************************************************

                              RPN_OP_MMM

******************************************************************************/
RPN_DEF_OP(rpn_op_mmm)
{
     rpn_elt                  *_stk_x, *_stk_result, *_sum_x, *_elt,
                              *_elt_min, *_elt_max;
     rpn_stack                *_stack;
     int                       _retcode, _N, _X_type;
     struct rpn_operator      *_op;
     rpn_list                 *_list, *_result_list;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_LIST:
          _list                    = _stk_x->value.obj;
          if (!_list->homogeneous) {
               _retcode                 = RPN_RET_NOT_HOMOGENEOUS;
               _stk_result              = _stk_x;
               goto end;
          }

          _N                       = _list->num_elts;
          if (_N == 0) {
               /* Empty list
                  ~~~~~~~~~~ */
               _stk_result              = rpn_new_elt(RPN_TYPE_NIL);
               goto end;
          }

          _sum_x                   = rpn_new_elt(_list->type);

          _elt_min                 = NULL;
          _elt_max                 = NULL;

          for ( ; _list->num_elts > 0; ) {
               _elt                     = rpn_list_pop_head(_list);
               if (_elt_min == NULL) {
#if 0
                    _elt_min                 = rpn_new_elt(_list->type);
                    _elt_min->value          = _elt->value;
#else
                    _elt_min                 = rpn_clone_elt(_elt);
#endif
               }
               if (_elt_max == NULL) {
#if 0
                    _elt_max                 = rpn_new_elt(_list->type);
                    _elt_max->value          = _elt->value;
#else
                    _elt_max                 = rpn_clone_elt(_elt);
#endif
               }

               switch (_list->type) {

               case RPN_TYPE_INT:
                    _sum_x->value.i               += _elt->value.i;
                    if (_elt_max->value.i < _elt->value.i) {
                         _elt_max->value.i         = _elt->value.i;
                    }
                    if (_elt_min->value.i > _elt->value.i) {
                         _elt_min->value.i         = _elt->value.i;
                    }
                    break;

               case RPN_TYPE_DOUBLE:
                    _sum_x->value.d               += _elt->value.d;
                    if (_elt_max->value.d < _elt->value.d) {
                         _elt_max->value.d         = _elt->value.d;
                    }
                    if (_elt_min->value.d > _elt->value.d) {
                         _elt_min->value.d         = _elt->value.d;
                    }
                    break;

               default:
                    RPN_INTERNAL_ERROR;
                    break;

               }

               rpn_free_elt(&_elt);
          }

          rpn_set_lastx(stack, _stk_x);

          switch (_list->type) {

          case RPN_TYPE_INT:
               _sum_x->value.i               /= _N;
               break;

          case RPN_TYPE_DOUBLE:
               _sum_x->value.d               /= _N;
               break;

          default:
               RPN_INTERNAL_ERROR;
               break;

          }

          _stk_result              = _sum_x;
          break;

     default:
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          _stk_result              = _stk_x;
          goto end;
          break;
     }


     _stk_result              = rpn_new_elt(RPN_TYPE_LIST);
     _result_list             = rpn_new_list((char *) __func__);
     _stk_result->value.obj   = _result_list;

     rpn_list_push_tail(_result_list, _elt_min);
     rpn_list_push_tail(_result_list, _elt_max);
     rpn_list_push_tail(_result_list, _sum_x);

     rpn_free_elt(&_stk_x);
// XXX TODO : free _list

end:
     rpn_push(stack, _stk_result);

     return _retcode;
}
/* rpn_op_mean() }}} */
/* rpn_op_gen_data_set() {{{ */

/******************************************************************************

                              RPN_OP_GEN_DATA_SET

******************************************************************************/
RPN_DEF_OP(rpn_op_gen_data_set)
{
     /* Generate data set : linear, exponential, logarithmic, power
      */
     rpn_elt                  *_stk_a, *_stk_b, *_stk_x_min, *_stk_x_max,
                              *_stk_n, *_stk_delta_y;
     double                    _a, _b, _x_min, _x_max, _delta_y, _delta_x,
                               _x, _y, _delta;
     int                       _n, _retcode, _i, _rnd, _sign;

     _retcode                 = RPN_RET_OK;

     _stk_n                   = rpn_pop(stack);
     _stk_delta_y             = rpn_pop(stack);
     _stk_x_max               = rpn_pop(stack);
     _stk_x_min               = rpn_pop(stack);
     _stk_b                   = rpn_pop(stack);
     _stk_a                   = rpn_pop(stack);

     /* Check elements types
        ~~~~~~~~~~~~~~~~~~~~ */
     if ((rpn_get_type(_stk_a)       != RPN_TYPE_DOUBLE)
     ||  (rpn_get_type(_stk_b)       != RPN_TYPE_DOUBLE)
     ||  (rpn_get_type(_stk_x_min)   != RPN_TYPE_DOUBLE)
     ||  (rpn_get_type(_stk_x_max)   != RPN_TYPE_DOUBLE)
     ||  (rpn_get_type(_stk_delta_y) != RPN_TYPE_DOUBLE)
     ||  (rpn_get_type(_stk_n)       != RPN_TYPE_INT)) {
          rpn_push(stack, _stk_a);
          rpn_push(stack, _stk_b);
          rpn_push(stack, _stk_x_min);
          rpn_push(stack, _stk_x_max);
          rpn_push(stack, _stk_delta_y);
          rpn_push(stack, _stk_n);

          _retcode                      = RPN_RET_INVALID_TYPES;
          goto end;
     }

     _a                       = _stk_a->value.d;
     _b                       = _stk_b->value.d;
     _x_min                   = _stk_x_min->value.d;
     _x_max                   = _stk_x_max->value.d;
     _delta_y                 = _stk_delta_y->value.d;
     _n                       = _stk_n->value.i;

     _delta_x                 = (_x_max - _x_min) / (_n - 1);
     _x                       = _x_min;

     // XXX TODO : Put a BEGIN marker in the stack
     for (_i = 0; _i < _n; _i++, _x += _delta_x) {
          _sign                    = rand() < (RAND_MAX / 2) ? -1 : 1;
          _rnd                     = rand();
          _delta                   = _delta_y * _rnd / RAND_MAX;

          switch (op->op_id) {

          case RPN_OP_ID_LINE_DATA_SET:
               _y                       = (_a * _x) + _b + (_sign * _delta);
               break;

          case RPN_OP_ID_EXP_DATA_SET:
               _y                       = (_b * exp(_a * _x)) + (_sign * _delta);
//printf("%6f a * x = %6.9f\n", _x, _a * _x);
//printf("%6f %6.9f\n", _x, (_b * exp(_a * _x)));
               break;

          case RPN_OP_ID_LOG_DATA_SET:
               _y                       = (_a * log(_x)) + _b + (_sign * _delta);
               break;

          case RPN_OP_ID_POW_DATA_SET:
               _y                       = _b * pow(_x, _a) + (_sign * _delta);
               break;

          default:
               RPN_INTERNAL_ERROR;
               break;
          }

          // XXX TODO : Create an n-tuple in the stack
          printf("%6f %6.9f\n", _x, _y);
     }

     // XXX TODO : Create a list of n-tuples in the stack

end:
     return _retcode;
}

/* rpn_op_gen_data_set() }}} */
/* GROUP : Stats }}} */
/* rpn_op_sto() {{{ */

/******************************************************************************

                              RPN_STO

******************************************************************************/
void rpn_sto(rpn_stack *stack, struct rpn_elt **dest_elt)
{
     struct rpn_elt           *_stk_x, *_X_clone;

     /* Pop X, clone element, Push X
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _stk_x                   = rpn_pop(stack);
     _X_clone                 = rpn_clone_elt(_stk_x);
     rpn_push(stack, _stk_x);
     if (*dest_elt != NULL) {
          /* Free previous content
             ~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(dest_elt);
     }

     /* Store clone address into destination register
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     *dest_elt                = _X_clone;
}
/* rpn_op_sto() }}} */
/* GROUP : Neural networks {{{ */
/* rpn_new_mlp() {{{ */

/******************************************************************************

                              RPN_NEW_MLP

******************************************************************************/
struct rpn_mlp *rpn_new_mlp(int nb_layers)
{
     /* The input layer is counted in the number of layers
      * of the multilayer perceptron, but isn't associated
      * to a real layer of neurons
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     int                       _size;
     struct rpn_mlp           *_mlp;

     _size                    = sizeof(struct rpn_mlp)
                              + ((nb_layers - 1) * sizeof(struct rpn_perceptron *));

     if ((_mlp = (struct rpn_mlp *) RPN_MALLOC(_size)) == NULL) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _mlp->nb_layers               = nb_layers;
     _mlp->nb_neurons              = 0;
     _mlp->nb_weights              = 0;

     return _mlp;
}
/* rpn_new_mlp() }}} */
/* rpn_new_input_layer() {{{ */

/******************************************************************************

                              RPN_NEW_INPUT_LAYER

******************************************************************************/
struct rpn_input_layer *rpn_new_input_layer(int nb_inputs)
{
     int                       _size, _i;
     struct rpn_input_layer   *_input_layer;

     _size                    = sizeof(struct rpn_input_layer)
                              + (nb_inputs * sizeof(struct rpn_input_neuron));

     if ((_input_layer = (struct rpn_input_layer *) RPN_MALLOC(_size)) == NULL) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _input_layer->nb_inputs = nb_inputs;
     for (_i = 0; _i < nb_inputs; _i++) {
          _input_layer->inputs[_i]      = rpn_new_input_neuron();
     }

     return _input_layer;
}
/* rpn_new_input_layer() }}} */
/* rpn_new_perceptron() {{{ */

/******************************************************************************

                              RPN_NEW_PERCEPTRON

******************************************************************************/
struct rpn_perceptron *rpn_new_perceptron(int nb_neurons)
{
     int                       _size, _i;
     struct rpn_perceptron    *_perceptron;

     _size                    = sizeof(struct rpn_perceptron)
                              + (nb_neurons * sizeof(struct rpn_neuron));

     if ((_perceptron = (struct rpn_perceptron *) RPN_MALLOC(_size)) == NULL) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _perceptron->nb_neurons  = nb_neurons;
     for (_i = 0; _i < nb_neurons; _i++) {
          /* Neurons will be allocated later, so reset their pointers
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          _perceptron->neurons[_i]      = NULL;
     }

     return _perceptron;
}
/* rpn_new_perceptron() }}} */
/* rpn_new_neuron() {{{ */

/******************************************************************************

                              RPN_NEW_NEURON

******************************************************************************/
struct rpn_neuron *rpn_new_neuron(int nb_weights)
{
     int                       _size, _i;
     struct rpn_neuron        *_neuron;

     _size                    = sizeof(struct rpn_neuron)
                              + (nb_weights * sizeof(rpn_weight));

     if ((_neuron = (struct rpn_neuron *) RPN_MALLOC(_size)) == NULL) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _neuron->nb_weights      = nb_weights;
     _neuron->bias            = 0;

     for (_i = 0; _i < nb_weights; _i++) {
          _neuron->weights[_i]          = 0;
     }

     return _neuron;
}
/* rpn_new_neuron() }}} */
/* rpn_nn_af_sigmoid() {{{ */

/******************************************************************************

                              RPN_NN_AF_SIGMOID

******************************************************************************/
rpn_weight rpn_nn_af_sigmoid(rpn_weight x)
{
     rpn_weight                _result;

     _result                  = (rpn_weight) 1.0 / (1.0 + exp((double) -x));

     return _result;
}
/* rpn_nn_af_sigmoid() }}} */
/* rpn_nn_af_tanh() {{{ */

/******************************************************************************

                              RPN_NN_AF_TANH

******************************************************************************/
rpn_weight rpn_nn_af_tanh(rpn_weight x)
{
     rpn_weight                _result;

     _result                  = (exp((double) x) - exp(-(double) x)) / (exp((double) x) + exp(-(double) x));

     return _result;
}
/* rpn_nn_af_tanh() }}} */
/* rpn_nn_af_arc_tan() {{{ */

/******************************************************************************

                              RPN_NN_AF_ARC_TAN

******************************************************************************/
rpn_weight rpn_nn_af_arc_tan(rpn_weight x)
{
     rpn_weight                _result;

     _result                  = atan((double) x);

     return _result;
}
/* rpn_nn_af_arc_tan() }}} */
/* rpn_nn_af_relu() {{{ */

/******************************************************************************

                              RPN_NN_AF_RELU

******************************************************************************/
rpn_weight rpn_nn_af_relu(rpn_weight x)
{
     rpn_weight                _result;

     if (x >= 0) {
          _result                  = x;
     }
     else {
          _result                  = 0;
     }

     return _result;
}
/* rpn_nn_af_relu() }}} */
/* rpn_nn_rand() {{{ */

/******************************************************************************

                              RPN_NN_RAND

******************************************************************************/
rpn_weight rpn_nn_rand(void)
{
     return (rpn_weight) rand();
}
/* rpn_nn_rand() }}} */
/* rpn_set_layer_weights() {{{ */

/******************************************************************************

                              RPN_SET_LAYER_WEIGHTS

******************************************************************************/
RPN_DEF_OP( rpn_set_layer_weights)
{
     RPN_INTERNAL_ERROR;
}
/* rpn_set_layer_weights() }}} */
/* rpn_set_mlp_weights() {{{ */

/******************************************************************************

                              RPN_SET_MLP_WEIGHTS

******************************************************************************/
RPN_DEF_OP(rpn_set_mlp_weights)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _X_type, _Y_type;
     int                       _i, _result, _layer, _n_num, _w_num;
     double                    _fixed_weight;
     int                       _retcode;
     struct rpn_mlp           *_mlp;
     struct rpn_perceptron    *_perceptron;
     struct rpn_neuron        *_neuron;

     _retcode                 = RPN_RET_OK;

     /* Check types
        ~~~~~~~~~~~ */
     if ((_retcode = rpn_check_types(op, stack)) != RPN_RET_OK) {
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(rpn_get_X_type(stack)));
          fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(rpn_get_Y_type(stack)));
          goto end;
     }

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     _X_type                  = rpn_get_type(_stk_x);
     _Y_type                  = rpn_get_type(_stk_y);

     switch (_X_type) {

     case RPN_TYPE_INT:
          _fixed_weight            = (double) _stk_x->value.i;
          break;

     case RPN_TYPE_DOUBLE:
          _fixed_weight            = _stk_x->value.d;
          break;

     case RPN_TYPE_NEURAL_MLP:
          _mlp                     = _stk_x->value.obj;
          _stk_result              = _stk_x;
          break;

     default:
          RPN_INTERNAL_ERROR;
          break;
     }

     switch (_Y_type) {

     case RPN_TYPE_INT:
          _fixed_weight            = (double) _stk_y->value.i;
          break;

     case RPN_TYPE_DOUBLE:
          _fixed_weight            = _stk_y->value.d;
          break;

     case RPN_TYPE_NEURAL_MLP:
          _mlp                     = _stk_y->value.obj;
          _stk_result              = _stk_y;
          break;

     default:
          RPN_INTERNAL_ERROR;
          break;
     }

     for (_layer = 0; _layer < _mlp->nb_layers - 1; _layer++) {
//fprintf(stderr, "mlp->layers[%d] = 0x%08X\n", _layer, _mlp->layers[_layer]);
          _perceptron                   = _mlp->layers[_layer];

          for (_n_num = 0; _n_num < _perceptron->nb_neurons; _n_num++) {
               _neuron                  = _perceptron->neurons[_n_num];
               for (_w_num = 0; _w_num < _neuron->nb_weights; _w_num++) {
//printf("VALUE = %f\n", _fixed_weight);
                    _neuron->weights[_w_num]     = _fixed_weight;
               }
               _neuron->bias            = _fixed_weight;
//printf("BIAS = %f\n", _neuron->bias);
          }
     }
     rpn_push(stack, _stk_result);

end:
     return _retcode;
}
/* rpn_set_mlp_weights() }}} */
/* rpn_op_read_mnist() {{{ */

/******************************************************************************

                              RPN_OP_READ_MNIST

******************************************************************************/
RPN_DEF_OP(rpn_op_read_mnist)
{
     char                          *_train_images, *_train_labels,
                                   *_test_images,  *_test_labels,
                                   *_image, _cwd[RPN_SYS_CMD_SIZE];
     rpn_mnist_desc                *_mnist;
     rpn_mnist_set                  _train,  _test;

     /* Initalize the module
        ~~~~~~~~~~~~~~~~~~~~ */
     _mnist                        = rpn_mnist_init();

     /* Get current working directory
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (getcwd(_cwd, sizeof(_cwd)) == NULL) {
          fprintf(stderr, "%s: cannot get current working directory\n",
                  G.progname);
          perror("getcwd");
          exit(RPN_EXIT_GETCWD_ERROR);
     }

     /* Change current directory to the specified directory
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (chdir(_mnist->data_dir) < 0) {
          fprintf(stderr, "%s: cannot chdir to \"%s\" !\n",
                  G.progname, _mnist->data_dir);
          perror("chdir");
          exit(RPN_EXIT_CHDIR_ERROR);
     }

     /* Download and uncompress files if not present
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _train.name         = "MNIST_TRAIN";
     _train.images       = rpn_mnist_install_file(RPN_MNIST_URL_TRAIN_IMG);
     _train.labels       = rpn_mnist_install_file(RPN_MNIST_URL_TRAIN_LBL);
     _train.set_type     = RPN_TYPE_TRAINING_SET;
     _train.elt_type     = RPN_TYPE_TRAINING_ELT;

     _test.name          = "MNIST_TESTS";
     _test.images        = rpn_mnist_install_file(RPN_MNIST_URL_TESTS_IMG);
     _test.labels        = rpn_mnist_install_file(RPN_MNIST_URL_TESTS_LBL);
     _test.set_type      = RPN_TYPE_TEST_SET;
     _test.elt_type      = RPN_TYPE_TEST_ELT;

     rpn_create_set(stack, &_test);
     rpn_create_set(stack, &_train);

     /* Go back to the previous directory
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (chdir(_cwd) < 0) {
          fprintf(stderr, "%s: cannot chdir to \"%s\" !\n",
                  G.progname, _cwd);
          perror("chdir");
          exit(RPN_EXIT_CHDIR_ERROR);
     }

     return 0;
}
/* rpn_op_read_mnist() }}} */
/* rpn_create_set() {{{ */

/******************************************************************************

                         RPN_CREATE_SET

******************************************************************************/
void rpn_create_set(rpn_stack *stack, rpn_mnist_set *set)
{
     uint32_t                       _magic, _nb_images, _dim1, _dim2, _size,
                                    _nb_labels;
     int                            _fd_images, _fd_labels, _type, _nb;
     rpn_elt                       *_elt, *_set, *_marker;
     rpn_training_elt              *_train;
     rpn_training_set              *_train_set;

     if ((_fd_images = open(set->images, O_RDONLY)) < 0) {
          fprintf(stderr, "%s : cannot open \"%s\" !\n", G.progname, set->images);
          exit(RPN_EXIT_OPEN_ERROR);
     }

     if (read(_fd_images, &_magic, sizeof(_magic)) < 0) {
          fprintf(stderr, "%s : error while reading magic number !\n", G.progname);
          exit(RPN_EXIT_READ_ERROR);
     }

     printf("Magic number  = 0x%08x\n", df_convert_uint32(_magic));

     if (read(_fd_images, &_nb_images, sizeof(_nb_images)) < 0) {
          fprintf(stderr, "%s : error while reading images number !\n", G.progname);
          exit(RPN_EXIT_READ_ERROR);
     }

     printf("Images number = %10d  (0x%08x)\n", df_convert_uint32(_nb_images), df_convert_uint32(_nb_images));
     _nb_images          = df_convert_uint32(_nb_images);

     if (read(_fd_images, &_dim1, sizeof(_dim1)) < 0) {
          fprintf(stderr, "%s : error while reading dimension 1 !\n", G.progname);
          exit(RPN_EXIT_READ_ERROR);
     }

     if (read(_fd_images, &_dim2, sizeof(_dim2)) < 0) {
          fprintf(stderr, "%s : error while reading dimension 2 !\n", G.progname);
          exit(RPN_EXIT_READ_ERROR);
     }

     _dim1               = df_convert_uint32(_dim1);
     _dim2               = df_convert_uint32(_dim2);

     printf("Dimension 1   = %10d  (0x%08x)\n", _dim1, _dim1);
     printf("Dimension 2   = %10d  (0x%08x)\n", _dim2, _dim2);

     _size               = _dim1 * _dim2;
     if ((_fd_labels = open(set->labels, O_RDONLY)) < 0) {
          fprintf(stderr, "%s : cannot open \"%s\" !\n", G.progname, set->labels);
          exit(RPN_EXIT_OPEN_ERROR);
     }

     if (read(_fd_labels, &_magic, sizeof(_magic)) < 0) {
          fprintf(stderr, "%s : error while reading magic number !\n", G.progname);
          exit(RPN_EXIT_READ_ERROR);
     }

     printf("Magic number  = 0x%08x\n", df_convert_uint32(_magic));

     if (read(_fd_labels, &_nb_labels, sizeof(_nb_labels)) < 0) {
          fprintf(stderr, "%s : error while reading labels number !\n", G.progname);
          exit(RPN_EXIT_READ_ERROR);
     }

     printf("Labels number = %10d  (0x%08x)\n", df_convert_uint32(_nb_labels), df_convert_uint32(_nb_labels));

     /* Allocate element descriptors for start marker
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_inc_begin(stack);
     _elt                = rpn_new_elt(RPN_TYPE_START_MARKER);

     /* Push element on the stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~ */
     stack->new_value  = TRUE;
     rpn_push(stack, _elt);
     for (_nb = 0; _nb < _nb_images; _nb++) {
          struct rpn_training_data      *_data;
          struct rpn_training_label     *_label;
          uint8_t                        _mnist_label;

          _data               = rpn_new_training_data();
          _data->orig_index   = 
          _data->index        = _nb + 1;
          _data->ignore       = FALSE;
          _data->nb_elts      = _dim1 * _dim2;
          _data->elt_size     = sizeof(uint8_t);
          _data->num_rows     = _dim1;
          _data->num_cols     = _dim2;
          _size               = _data->nb_elts * _data->elt_size;

          if ((_data->vector = (unsigned char *) RPN_MALLOC(_size)) == NULL) {
               fprintf(stderr, "%s : cannot allocate %d bytes for the images !\n", G.progname, _size);
               exit(RPN_EXIT_NO_MEM);
          }

          if (read(_fd_images, _data->vector, _size) <= 0) {
               fprintf(stderr, "%s : error while reading image !\n", G.progname);
               exit(RPN_EXIT_READ_ERROR);
          }

          _label              = rpn_new_training_label();
          _label->index       = _nb + 1;

          if (read(_fd_labels, (char *) &_mnist_label, sizeof(_mnist_label)) <= 0) {
               fprintf(stderr, "%s : error while reading label !\n", G.progname);
               exit(RPN_EXIT_READ_ERROR);
          }
          _label->value       = _mnist_label;

          _train              = rpn_new_training_elt();
          _train->data        = _data;
          _train->label       = _label;

          _elt                = rpn_new_elt(set->elt_type);
          _elt->value.obj     = _train;

          /* Push element on the stack
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          stack->new_value  = TRUE;
          rpn_push(stack, _elt);

//          printf("Image %10d read (size = %10d) ...\n", _nb, _size);
     }

     close(_fd_labels);
     close(_fd_images);

     /* Create the SET element
        ~~~~~~~~~~~~~~~~~~~~~~ */
     _set                     = rpn_new_elt(set->set_type);
     _size                    = sizeof(*_train_set) + ((_nb_images - 1) * sizeof(struct rpn_elt *));
     _train_set               = (struct rpn_training_set *) RPN_MALLOC(_size);
#if 0
printf("sizeof(rpn_training_set) = %6d\n", sizeof(rpn_training_set));
printf("sizeof(train_set)        = %6d\n", _size);
printf("sizeof(rpn_elt *)        = %6d\n", sizeof(struct rpn_elt *));
#endif
     _train_set->name         = strdup(set->name);
     _train_set->nb_elts      = _nb_images;
     _train_set->current_idx  = 1;
     _train_set->nrows        = RPN_DEFLT_MNIST_NROWS;
     _train_set->ncols        = RPN_DEFLT_MNIST_NCOLS;
     _train_set->width_pix    = RPN_DEFLT_MNIST_WIDTH_PIX;
     _train_set->height_pix   = RPN_DEFLT_MNIST_HEIGHT_PIX;
     _train_set->elt_width    = _dim1;
     _train_set->elt_height   = _dim2;

     _set->value.obj          = _train_set;

     for (_nb = _nb_images - 1; _nb >= 0; _nb--) {
          _elt                     = rpn_pop(stack);
          _train_set->array[_nb]   = _elt;
     }

     _marker             = rpn_pop(stack);
     if ((_type = rpn_get_type(_marker)) != RPN_TYPE_START_MARKER) {
          RPN_INTERNAL_ERROR;
     }

     rpn_push(stack, _set);
}

/* rpn_create_set() }}} */
/* rpn_new_training_elt() {{{ */

/******************************************************************************

                         RPN_NEW_TRAINING_ELT

******************************************************************************/
RPN_NEW(training_elt)
/* rpn_new_training_elt() }}} */
/* rpn_new_training_data() {{{ */

/******************************************************************************

                         RPN_NEW_TRAINING_DATA

******************************************************************************/
RPN_NEW(training_data)
/* rpn_new_training_data() }}} */
/* rpn_new_training_label() {{{ */

/******************************************************************************

                         RPN_NEW_TRAINING_LABEL

******************************************************************************/
RPN_NEW(training_label)
/* rpn_new_training_label() }}} */
/* rpn_new_input_neuron() {{{ */

/******************************************************************************

                         RPN_NEW_INPUT_NEURON

******************************************************************************/
RPN_NEW(input_neuron)
/* rpn_new_input_neuron() }}} */
/* rpn_nn_learn() {{{ */

/******************************************************************************

                         RPN_NN_LEARN

******************************************************************************/
void rpn_nn_learn()
{
}
/* rpn_nn_learn() }}} */
/* rpn_nn_analyse() {{{ */

/******************************************************************************

                         RPN_NN_ANALYZE

******************************************************************************/
void rpn_nn_analyze()
{
}
/* rpn_nn_analyse() }}} */
/* rpn_save_mlp() {{{ */

/******************************************************************************

                         RPN_SAVE_MLP

******************************************************************************/
void rpn_save_mlp(struct rpn_elt *mlp)
{
     struct rpn_mlp           *_mlp;
     struct rpn_perceptron    *_perceptron;
     struct rpn_neuron        *_neuron;
     int                       _l_num, _n_num, _w_num;

     _mlp           = (struct rpn_mlp *) mlp->value.obj;

     printf("Nb layers : %d nb_neurons : %d nb_weights : %d\n",
            _mlp->nb_layers, _mlp->nb_neurons, _mlp->nb_weights);
     for (_l_num = 0; _l_num < _mlp->nb_layers - 1; _l_num++) {
          _perceptron         = _mlp->layers[_l_num];
          for (_n_num = 0; _n_num < _perceptron->nb_neurons; _n_num++) {
               _neuron        = _perceptron->neurons[_n_num];
               for (_w_num = 0; _w_num < _neuron->nb_weights; _w_num++) {
                    printf("Layer %3d, neuron %5d : weight %5d = %f\n",
                           _l_num, _n_num, _w_num, _neuron->weights[_w_num]);
               }
               printf("Layer %3d, neuron %5d : bias         = %f\n",
                      _l_num, _n_num, _w_num, _neuron->bias);
          }
     }
}
/* rpn_save_mlp() }}} */
/* rpn_op_read_elt() {{{ */

/******************************************************************************

                         RPN_OP_READ_ELT

******************************************************************************/
RPN_DEF_OP(rpn_op_read_elt)
{
     rpn_elt                  *_stk_x, *_stk_result;
     struct rpn_training_elt  *_train_elt;
     int                       _X_type, _retcode;

     _retcode                 = RPN_RET_OK;

     /* Check_type
        ~~~~~~~~~~ */
     _stk_x                   = rpn_pop(stack);

     _X_type                  = rpn_get_type(_stk_x);

     if ((_X_type != RPN_TYPE_TRAINING_SET)
     &&  (_X_type != RPN_TYPE_TEST_SET)) {
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(rpn_get_X_type(stack)));
          goto end;
     }

end:
     return _retcode;
}
/* rpn_op_read_elt() }}} */
/* rpn_op_mnist_pic() {{{ */

/******************************************************************************

                         RPN_OP_MNIST_PIC

******************************************************************************/
/**
 *   \brief    Generate a PNG image derived from MNIST hand written digit.
 *             If X type is RPN_TYPE_TRAINING_ELT, then the image is the image
 *             of the digit of the MNIST element.
 *             If X type is RPN_TYPE_INT and Y type is RPN_TRAINING_ELT, then
 *             the image is the image of the digit of the mnist element and the
 *             digit in X is displayed by the side of the MNIST digit as a
 *             7 segments digit.
 */
RPN_DEF_OP(rpn_op_mnist_pic)
{
     rpn_elt                  *_stk_x, *_stk_result;
     struct rpn_training_elt  *_train_elt;
     int                       _X_type, _retcode;

     _retcode                 = RPN_RET_OK;

     /* Check_type
        ~~~~~~~~~~ */
     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_TRAINING_ELT:
     case RPN_TYPE_TEST_ELT:
          {
               /* Generate image with MNIST digit with a 7 segments digit
                * by its side
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               struct rpn_stack         *_stack;
               char                      _txt[64], _png[64], _base[64],
                                        *_template, *_template_base, *_template_dir,
                                        *_dirname;
               int                       _index, _orig_index, _label, _size;
               rpn_elt                  *_stk_file, *_stk_label;
               rpn_training_elt         *_train;
               rpn_training_data        *_st_data;
               rpn_training_label       *_st_label;

               _template_base           = "%02d/mnist_%d_%05d";
               _template                = "%s/%s.%s";
               _template_dir            = "%s/%02d";

               _stack                   = rpn_new_stack(__func__);

               _train_elt               = _stk_x->value.obj;
               _st_data                 = _train_elt->data;
               _st_label                = _train_elt->label;
               _index                   = _st_data->index;
               _orig_index              = _st_data->orig_index;
               _label                   = _st_label->value;

               rpn_push(_stack, _stk_x);

               /* Convert MNIST digit to a matrix */
               rpn_op_matrix(_stack, RPN_OP_DESC(MATRIX));

               /* Generate 7 segments digit */
               _stk_label               = rpn_new_elt(RPN_TYPE_INT);
               _stk_label->value.i      = _label;
               rpn_push(_stack, _stk_label);
               rpn_op_mat_7_seg(_stack, RPN_OP_DESC(NN_MAT_7_SEG));

               /* Concatenate 7 segment matrix to the digit matrix */
               rpn_op_generic2(_stack, RPN_OP_DESC(H_CAT));

               /* Revert lines for correct plotting */
               rpn_op_h_rev(_stack, RPN_OP_DESC(H_REV));

               /* Generate filenames */
               sprintf(_base, _template_base, _orig_index / 1000, _label, _orig_index);

               _size                    = strlen(RPN_MNIST_DIR_TEXTS) + 1
                                        + 2 + 1
                                        + strlen(_base) + 1 + 3 + 1;
               _dirname                 = RPN_MALLOC(_size);
               sprintf(_dirname, _template_dir, RPN_MNIST_DIR_TEXTS, _orig_index / 1000);
// printf("DIRNAME = [%s]\n", _dirname);
               rpn_mkdir(_dirname);
               strcpy(_dirname, RPN_MNIST_DIR_TEXTS);
               sprintf(_txt, _template, _dirname,  _base, "txt");
               rpn_free(_dirname);

               _size                    = strlen(RPN_MNIST_DIR_IMAGES) + 1
                                        + 2 + 1
                                        + strlen(_base) + 1 + 3 + 1;
               _dirname                 = RPN_MALLOC(_size);
               sprintf(_dirname, _template_dir, RPN_MNIST_DIR_IMAGES, _orig_index / 1000);
// printf("DIRNAME = [%s]\n", _dirname);
               rpn_mkdir(_dirname);
               strcpy(_dirname, RPN_MNIST_DIR_IMAGES);
               sprintf(_png, _template, _dirname, _base, "png");

               printf("TXT FILE : %s\n", _txt);
               printf("PNG FILE : %s\n", _png);

               _stk_file                = rpn_new_elt(RPN_TYPE_STRING);    // XXX
               _stk_file->value.s       = strdup(_txt);

               rpn_push(_stack, _stk_file);

               /* Generate output filename */
               rpn_op_generic1(_stack, RPN_OP_DESC(TYPE_FILENAME));

               /* Write matrix to file */
               rpn_op_write(_stack, RPN_OP_DESC(WRITE));

               /* Call the plot command
                  ~~~~~~~~~~~~~~~~~~~~~ */
               rpn_plot_pic(_base, G.mnist);

               _stk_result              = rpn_pop(_stack);
               rpn_free_stack(_stack);
          }
          break;

     case RPN_TYPE_INT:
          {
#if 0
               struct rpn_stack         *_stack;
               char                      _txt[32], _png[32], _base[32],
                                        *_template, *_template_base;
               int                       _index, _label, _value;
               rpn_elt                  *_file;

               _template_base           = "mnist_%05d_%d_%d";
               _template                = "%s/%s.%s";

               _stack                   = rpn_new_stack(__func__);
               _index                   = _stk_x->value.i;

               /* Generate image with MNIST digit with a 7 segments digit
                * by its side
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_push(_stack, _stk_x);
               _train_elt               = _stk_x->value.obj;
               printf("==========> Label = %d\n", _train_elt->label->value);

               rpn_op_mat_7_seg(_stack, RPN_OP_DESC(NN_MAT_7_SEG));
               rpn_op_swap_xy(_stack, RPN_OP_DESC(SWAP_XY));
               rpn_op_matrix(_stack, RPN_OP_DESC(MATRIX));
               rpn_op_swap_xy(_stack, RPN_OP_DESC(SWAP_XY));
               rpn_op_generic2(_stack, RPN_OP_DESC(H_CAT));
               rpn_op_h_rev(_stack, RPN_OP_DESC(H_REV));

               /* Generate filenames */
               sprintf(_base, _template_base, _index, _value, _label);

               sprintf(_txt, _template, RPN_MNIST_DIR_TEXTS,  _base, "txt");
               sprintf(_png, _template, RPN_MNIST_DIR_IMAGES, _base, "png");

               printf("TXT FILE : %s\n", _txt);
               printf("PNG FILE : %s\n", _png);

               _file                    = rpn_new_elt(RPN_TYPE_STRING);
               _file->value.s           = strdup(_txt);

               rpn_push(_stack, _file);

               /* Generate output filename */
               rpn_op_generic1(_stack, RPN_OP_DESC(TYPE_FILENAME));

               /* Write matrix to file */
               rpn_op_write(_stack, RPN_OP_DESC(WRITE));

               /* Call the plot command
                  ~~~~~~~~~~~~~~~~~~~~~ */
               rpn_plot_pic(_base, G.mnist);

               _stk_result              = rpn_pop(_stack);
               rpn_free_stack(_stack);
#endif    /* 0 */
          }
          break;

     default:
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(rpn_get_X_type(stack)));
          goto end;
          break;
     }

     rpn_set_lastx(stack, _stk_result);

end:
     return _retcode;
}
/* rpn_op_mnist_pic() }}} */
/* rpn_op_mnist_pics() {{{ */

/******************************************************************************

                         RPN_OP_MNIST_PICS

******************************************************************************/
RPN_DEF_OP(rpn_op_mnist_pics)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_z, *_stk_t, *_stk_u, *_stk_result,
                              *_train_elt;;
     struct rpn_training_set  *_train;
     int                       _X_type, _Y_type, _Z_type, _T_type, _U_type, _retcode,
                               _first_idx, _last_idx, _n, _p, _w, _h,
                               _first, _last;

     _retcode                 = RPN_RET_OK;

     /* Check types
      * ~~~~~~~~~~~
      * Admissible stack contents :
      *
      *  1st possibility     2nd possibility                     3rd possibility
      *  ~~~~~~~~~~~~~~~     ~~~~~~~~~~~~~~~                     ~~~~~~~~~~~~~~~
      *                                                          U : TRAINING_SET
      *                                                          T : INT  Index of first element
      *                      Z : TRAINING_SET                    Z : INT  Index of last element
      *                      Y : INT  Index of first element     Y : INT  n (number of rows)
      * X: TRAINING_SET      X : INT  Index of last element      X : INT  p (number of columns
      *
      * Dimensions of the generated image (in pixels) :
      * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      * - width_pix
      * - height_pix
      */

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     /* Try first possibility
        ~~~~~~~~~~~~~~~~~~~~~ */
     if ((_X_type == RPN_TYPE_TRAINING_SET)
     ||  (_X_type == RPN_TYPE_TEST_SET)) {
          _train_elt               = _stk_x;
          _first_idx               = 0;
          _last_idx                = 0;
          _n                       = 0;
          _p                       = 0;
          _w                       = 0;
          _h                       = 0;
     }
     else {
          /* Try second possibility
             ~~~~~~~~~~~~~~~~~~~~~~ */
          _stk_y                   = rpn_pop(stack);
          _stk_z                   = rpn_pop(stack);

          _Y_type                  = rpn_get_type(_stk_y);
          _Z_type                  = rpn_get_type(_stk_z);

          if ((_Z_type == RPN_TYPE_TRAINING_SET)
          ||  (_Z_type == RPN_TYPE_TEST_SET)) {
               if (_Y_type == RPN_TYPE_INT
               &&  _X_type == RPN_TYPE_INT) {
                    _train_elt               = _stk_z;
                    _first_idx               = _stk_y->value.i;
                    _last_idx                = _stk_x->value.i;
                    _n                       = 0;
                    _p                       = 0;
                    _w                       = 0;
                    _h                       = 0;
               }
               else {
                    /* Invalid types
                       ~~~~~~~~~~~~~ */
                    rpn_push(stack, _stk_z);
                    rpn_push(stack, _stk_y);
                    rpn_push(stack, _stk_x);

                    _retcode                 = RPN_RET_INVALID_TYPES;
                    goto end;
               }
          }
          else {
               /* Try third possibility
                  ~~~~~~~~~~~~~~~~~~~~~ */
               _stk_t                   = rpn_pop(stack);
               _stk_u                   = rpn_pop(stack);

               _T_type                  = rpn_get_type(_stk_t);
               _U_type                  = rpn_get_type(_stk_u);

               if ((_U_type == RPN_TYPE_TRAINING_SET || _U_type == RPN_TYPE_TEST_SET)
               &&  _T_type == RPN_TYPE_INT
               &&  _Z_type == RPN_TYPE_INT
               &&  _Y_type == RPN_TYPE_INT
               &&  _X_type == RPN_TYPE_INT) {
                    _train_elt               = _stk_u;
                    _first_idx               = _stk_t->value.i;
                    _last_idx                = _stk_z->value.i;
                    _n                       = _stk_y->value.i;
                    _p                       = _stk_x->value.i;
                    _w                       = 0;
                    _h                       = 0;
               }
               else {
                    /* Invalid types
                       ~~~~~~~~~~~~~ */
                    rpn_push(stack, _stk_u);
                    rpn_push(stack, _stk_t);
                    rpn_push(stack, _stk_z);
                    rpn_push(stack, _stk_y);
                    rpn_push(stack, _stk_x);

                    _retcode                 = RPN_RET_INVALID_TYPES;
                    goto end;
               }
          }
     }

     _train                   = _train_elt->value.obj;
     
     if (_n == 0) {
          _n                       = _train->nrows;
     }
     if (_p == 0) {
          _p                       = _train->ncols;
     }
     if (_w == 0) {
          _w                       = _train->width_pix;
     }
     if (_h == 0) {
          _h                       = _train->height_pix;
     }
     if (_first_idx == 0) {
          _first_idx               = _train->current_idx;
     }
     if (_last_idx == 0) {
          _last_idx                = _train->current_idx
                                   + (RPN_DEFLT_MNIST_NROWS * RPN_DEFLT_MNIST_NCOLS) -1;
     }
     if (_last_idx > _train->nb_elts) {
          _last_idx                = _train->nb_elts;
     }

     /* Keep the training set in the stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_push(stack, _train_elt);

     for (_first = _first_idx; _first <= _last_idx; _first += _n * _p) {
          _last = MIN(_first + (_n * _p) - 1, _last_idx);
          printf("%5d - %5d\n", _first, _last);

#define   MNIST_PICS_VERSION  5
          /* VERSION 1 : HCAT_VERSION = 1
           * ~~~~~~~~~   VCAT_VERSION = 1
           *             VPAD_VERSION = 1
           *
           * VERSION 2 : HCAT_VERSION = 2
           * ~~~~~~~~~   VCAT_VERSION = 2
           *             VPAD_VERSION = 2
           *
           * VERSION 3 : creation of the target matrix, rpn_clone_elt(), rpn_free(matrix)
           * ~~~~~~~~~
           * 
           * VERSION 4 : creation of the target matrix, copy of the element pointers, free(matrix)
           * ~~~~~~~~~
           *
           * VERSION 5 : creation of the target matrix, copy of the integers, free(matrix)
           * ~~~~~~~~~
           */

#if (MNIST_PICS_VERSION == 1)
          {    /* VERSION 1 and 2 */
               struct rpn_stack              *_stack, *_stack2;
               char                           _txt[64], _png[64], _base[64],
                                             *_template, *_template_base;
               int                            _index, _label, _value, _row, _col;
               rpn_elt                       *_file, *_elt_i, *_elt_set, *_mat,
                                             *_stk_result;

               _template_base           = "mnist_%s_%05d-%05d_%dx%d";
               _template                = "%s/GROUPS/%s.%s";

               _stack                   = rpn_new_stack(__func__);
               _stack2                  = rpn_new_stack("MNIST_MATRIX");
               _elt_set                 = rpn_clone_elt(_train_elt);
               rpn_push(_stack, _elt_set);

               _index                   = _first;

               for (_row = 1; _row <= _n; _row++) {
                    for (_col = 1; _col <= _p; _col++) {

                         _elt_i              = rpn_new_elt(RPN_TYPE_INT);
                         _elt_i->value.i     = _index;

                         /* Push index on stack
                            ~~~~~~~~~~~~~~~~~~~ */
                         rpn_push(_stack, _elt_i);

                         /* Get element
                            ~~~~~~~~~~~ */
                         if ((_retcode = rpn_op_get(_stack, RPN_OP_DESC(GET))) != 0) {
                              rpn_err_msg_op_error(op->op_name, _retcode);
                              exit(RPN_EXIT_OP_ERROR);
                         }

                         /* Convert element into a matrix
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         if ((_retcode = rpn_op_matrix(_stack, RPN_OP_DESC(MATRIX))) != 0) {
                              rpn_err_msg_op_error(op->op_name, _retcode);
                              exit(RPN_EXIT_OP_ERROR);
                         }

                         /* Reverse lines
                            ~~~~~~~~~~~~~ */
                         if ((_retcode = rpn_op_h_rev(_stack, RPN_OP_DESC(H_REV))) != 0) {
                              rpn_err_msg_op_error(op->op_name, _retcode);
                              exit(RPN_EXIT_OP_ERROR);
                         }

                         /* Get the matrix
                            ~~~~~~~~~~~~~~ */
                         _mat                = rpn_pop(_stack);

                         /* Push the matrix on the second stack
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         rpn_push(_stack2, _mat);

                         if (_col > 1) {
                              /* Concatenate the current matrix with the previous one (column)
                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                              if ((_retcode = rpn_op_generic2(_stack2, RPN_OP_DESC(H_CAT))) != 0) {
                                   rpn_err_msg_op_error(op->op_name, _retcode);
                                   exit(RPN_EXIT_OP_ERROR);
                              }
                         }

                         _index++;
                         if (_index > _last) {
                              break;
                         }
                    }

                    if (_row > 1) {
                         /* Eventually pad the matrix
                            ~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         if ((_retcode = rpn_op_generic2(_stack2, RPN_OP_DESC(V_PAD))) != 0) {
                              rpn_err_msg_op_error(op->op_name, _retcode);
                              exit(RPN_EXIT_OP_ERROR);
                         }

                         /* Swap lines
                            ~~~~~~~~~~ */
                         rpn_op_swap_xy(_stack2, RPN_OP_DESC(SWAP_XY));

                         /* Concatenate the current matrix with the previous one (row)
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         if ((_retcode = rpn_op_generic2(_stack2, RPN_OP_DESC(V_CAT))) != 0) {
                              rpn_err_msg_op_error(op->op_name, _retcode);
                              exit(RPN_EXIT_OP_ERROR);
                         }
                    }


                    if (_index > _last) {
                         break;
                    }
               }

               _stk_result              = rpn_pop(_stack2);
               rpn_push(_stack, _stk_result);

               /* Generate filenames */
               sprintf(_base, _template_base, _train->name, _first, _last, _n, _p);

               sprintf(_txt, _template, RPN_MNIST_DIR_TEXTS,  _base, "txt");
               sprintf(_png, _template, RPN_MNIST_DIR_IMAGES, _base, "png");

               printf("TXT FILE : %s\n", _txt);
               printf("PNG FILE : %s\n", _png);

               _file                    = rpn_new_elt(RPN_TYPE_STRING);
               _file->value.s           = strdup(_txt);

               rpn_push(_stack, _file);

               /* Generate output filename */
               rpn_op_generic1(_stack, RPN_OP_DESC(TYPE_FILENAME));

               /* Write matrix to file */
               rpn_op_write(_stack, RPN_OP_DESC(WRITE));

               /* Call the plot command
                  ~~~~~~~~~~~~~~~~~~~~~ */
               rpn_plot_pics(_base, G.mnist);

               /* Free temporary stacks
                  ~~~~~~~~~~~~~~~~~~~~~ */
               rpn_free_stack(_stack);
               rpn_free_stack(_stack2);
          }

#elif (MNIST_PICS_VERSION == 5)

          {    /* VERSION 5 */
               char                           _txt[64], _png[64], _base[64],
                                             *_template, *_template_base;
               unsigned char                 *_vector;
               rpn_elt                       *_elt;
               rpn_training_elt              *_elt_train;
               rpn_training_data             *_elt_data;
               FILE                          *_fp;
               char                          *_fmt = "%3d";
               char                          *_tab = "     ";
               int                            _idx, _val;
               int                            _R, _C;

               /* Line / column of the matrix containing all digits,
                * in digit units
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               int                            _row, _col;

               /* Dimensions of the matrix, in cells
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               int                            _N, _P, _N0, _P0;

               /* Size of the matrix, in bytes
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               int                            _size;

               /* Index of the element to copy
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               int                            _index;

               int                            _i0, _j0, _i, _j, _dst_idx, _pixel_idx;

               int                            _label, _value;
               rpn_elt                       *_file, *_elt_i, *_elt_set, *_mat,
                                             *_stk_result;
               unsigned char                 *_matrix;

               _template_base           = "mnist_v5_%s_%05d-%05d_%dx%d";
               _template                = "%s/GROUPS/%s.%s";

               _N0                      = (_n + 3) * _train->elt_height;
               _P0                      = (_p + 3) * _train->elt_width;

// printf("N0 = %4d P0 = %4d  (%3d x %3d)\n", _N0, _P0, _n + 3, _p + 3);

               _N                       = _n * _train->elt_height;
               _P                       = _p * _train->elt_width;
// printf("N  = %4d P  = %4d  (%3d x %3d)\n", _N, _P, _n, _p);

               _size                    = _N0 * _P0 * sizeof(_matrix[0]);
//               printf("N = %5d  P = %5d  size = %10d\n", _N0, _P0, _size);

               /* Allocate memory for a simplified matrix (only bytes values)
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _matrix                  = RPN_MALLOC(_size);

               /* Fill the matrix with 0
                  ~~~~~~~~~~~~~~~~~~~~~~ */
               (void) memset(_matrix, 0, _size);

               /* Initialize the reference borders :
                * 1. Initialize columns numbering (first two lines)
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _row                     = 1;
               for (_col = 1 + 3; _col <= _p + 1 + 3; _col += 10) {
                    _value              = _col / 10;
                    _R                  = _row;
                    _C                  = _col - ((_col / 10) > 0);

                    /* Compute the position of the upper left corner of the destination matrix
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _i0                 = (_R - 1) * _train->elt_height;
                    _j0                 = (_C - 1) * _train->elt_width;

                    for (_i = 0; _i < _train->elt_width; _i++) {
                         for (_j = 0; _j < _train->elt_width; _j++) {
                              _dst_idx            = RPN_MATRIX_IDX(_i0 + _i + 1, _j0 + _j + 1,  N0, _P0);
                              _pixel_idx          = RPN_MATRIX_IDX(_i + 1, _j + 1,
                                                                   _train->elt_height, _train->elt_width);

                              _matrix[_dst_idx]   = rpn_7_segments[_value].digit[_pixel_idx];
                              if (_i == 0 && _j == 0) {
//                                 printf("DIGIT 7 segments %d : pos = (%2d, %2d) coord :(%3d, %3d) [%3d] => (%6d, %6d) [%6d]\n",
//                                        _value, _R, _C, _i, _j, _pixel_idx, _i0 + _i, _j0 + _j, _dst_idx);
                              }
                              if (_i == _train->elt_height - 1 && _j == _train->elt_height - 1) {
//                                 printf("DIGIT 7 segments %d : pos = (%2d, %2d) coord :(%3d, %3d) [%3d] => (%6d, %6d) [%6d]\n",
//                                        _value, _R, _C, _i, _j, _pixel_idx, _i0 + _i, _j0 + _j, _dst_idx);
                              }
                         }
                    }
               }

               _row                     = 2;
               for (_col = 1 + 3; _col < _p + 1 + 3; _col++) {

                    _value              = (_col - 3) % 10;
                    _R                  = _row;
                    _C                  = _col;

                    /* Compute the position of the upper left corner of the destination matrix
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _i0                 = (_R - 1) * _train->elt_height;
                    _j0                 = (_C - 1) * _train->elt_width;

                    for (_i = 0; _i < _train->elt_width; _i++) {
                         for (_j = 0; _j < _train->elt_width; _j++) {
                              _dst_idx            = RPN_MATRIX_IDX(_i0 + _i + 1, _j0 + _j + 1,  N0, _P0);
                              _pixel_idx          = RPN_MATRIX_IDX(_i + 1, _j + 1,
                                                                   _train->elt_height, _train->elt_width);

                              _matrix[_dst_idx]   = rpn_7_segments[_value].digit[_pixel_idx];
                         }
                    }
               }

               /* 2. Initialize lines numbering (first tow columns)
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _col                     = 1;
               for (_row = 1 + 3; _row <= _n + 1 + 3; _row += 10) {
                    _value              = _row / 10;
                    _R                  = _row - ((_row / 10) > 0);
                    _C                  = _col;

                    /* Compute the position of the upper left corner of the destination matrix
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _i0                 = (_R - 1) * _train->elt_height;
                    _j0                 = (_C - 1) * _train->elt_width;

                    for (_i = 0; _i < _train->elt_width; _i++) {
                         for (_j = 0; _j < _train->elt_width; _j++) {
                              _dst_idx            = RPN_MATRIX_IDX(_i0 + _i + 1, _j0 + _j + 1,  N0, _P0);
                              _pixel_idx          = RPN_MATRIX_IDX(_i + 1, _j + 1,
                                                                   _train->elt_height, _train->elt_width);

                              _matrix[_dst_idx]   = rpn_7_segments[_value].digit[_pixel_idx];
                         }
                    }
               }

               _col                     = 2;
               for (_row = 1 + 3; _row < _n + 1 + 3; _row++) {
//                  printf(" ROW %2d COL %2d : %d\n", _row, _col, (_row - 3 - 1) % 10);
                    _value              = (_row - 3) % 10;
                    _R                  = _row;
                    _C                  = _col;

                    /* Compute the position of the upper left corner of the destination matrix
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _i0                 = (_R - 1) * _train->elt_height;
                    _j0                 = (_C - 1) * _train->elt_width;

                    for (_i = 0; _i < _train->elt_width; _i++) {
                         for (_j = 0; _j < _train->elt_width; _j++) {
                              _dst_idx            = RPN_MATRIX_IDX(_i0 + _i + 1, _j0 + _j + 1,  N0, _P0);
                              _pixel_idx          = RPN_MATRIX_IDX(_i + 1, _j + 1,
                                                                   _train->elt_height, _train->elt_width);

                              _matrix[_dst_idx]   = rpn_7_segments[_value].digit[_pixel_idx];
                         }
                    }
               }

               /* Copy loop
                  ~~~~~~~~~ */
               _index                   = _first - 1;
               for (_row = 1; _row <= _n; _row++) {
                    for (_col = 1; _col <= _p; _col++) {
                         /* Get element reference
                            ~~~~~~~~~~~~~~~~~~~~~ */
                         _elt                = _train->array[_index];
                         _elt_train          = _elt->value.obj;
                         _elt_data           = _elt_train->data;
                         _vector             = _elt_data->vector;

                         /* Compute the position of the upper left corner of the destination matrix
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _i0                 = (_row - 1 + 3) * _train->elt_height;
                         _j0                 = (_col - 1 + 3) * _train->elt_width;

                         /* Copy the handwritten digit into the matrix
                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                         _pixel_idx          = 0;
                         for (_i = 0; _i < _train->elt_width; _i++) {
                              for (_j = 0; _j < _train->elt_width; _j++) {
                                   _dst_idx            = RPN_MATRIX_IDX(_i0 + _i + 1, _j0 + _j + 1,  N0, _P0);
                                   _pixel_idx          = RPN_MATRIX_IDX(_i + 1, _j + 1,
                                                                        _train->elt_height, _train->elt_width);

                                   if (!_elt_data->ignore) {
                                        _matrix[_dst_idx]   = _vector[_pixel_idx];
                                   }
                                   else {
                                        /* Hand written digit is not used
                                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                                        _matrix[_dst_idx]   = 0;
                                   }
                              }
                         }

                         _index++;
                         if (_index > _last - 1) {
                              break;
                         }
                    }

                    if (_index > _last - 1) {
                         break;
                    }
               }

               /* Generate filenames */
               sprintf(_base, _template_base, _train->name, _first, _last, _n, _p);

               sprintf(_txt, _template, RPN_MNIST_DIR_TEXTS,  _base, "txt");
               sprintf(_png, _template, RPN_MNIST_DIR_IMAGES, _base, "png");

               printf("TXT FILE : %s\n", _txt);
               printf("PNG FILE : %s\n", _png);

               if ((_fp = fopen(_txt, "w")) == NULL) {
                    fprintf(stderr, "%s (%d) : cannot open \"%s\" !\n",
                            __func__, __LINE__, _txt);
                    RPN_INTERNAL_ERROR;
               }

               for (_i = _N0; _i >= 1; _i--) {
                    for (_j = 1; _j <= _P0; _j++) {
                         if (_j == 1 && _i < _N0) {
                              fprintf(_fp, "\n");
                         }
                         if (_j == 1) {
                              fprintf(_fp, "%s", _tab);
                         }

                         _idx                     = RPN_MATRIX_IDX(_i, _j, _N0, _P0);
                         _val                     = _matrix[_idx];

                         if (_j > 1) {
                              fprintf(_fp, " ");
                         }
                         fprintf(_fp, _fmt, _val);
                    }
               }
               fprintf(_fp, "\n");
               fclose(_fp);

               /* Increment current_index
                  ~~~~~~~~~~~~~~~~~~~~~~~ */
               if (_last >= _train->nb_elts) {
                    _train->current_idx      = 1;
               }
               else {
                    _train->current_idx      = _first + (_n * _p);
               }

               /* Call the plot command
                  ~~~~~~~~~~~~~~~~~~~~~ */
               rpn_plot_pics(_base, G.mnist);

               RPN_FREE(_matrix);
#endif
          }
     }

// rpn_disp_stk(stack);

//     rpn_set_lastx(stack, _stk_result);

end:
     return _retcode;
}

/* rpn_op_mnist_pics() }}} */
/* rpn_op_get() {{{ */

/******************************************************************************

                         RPN_OP_GET

******************************************************************************/
RPN_DEF_OP(rpn_op_get)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result, *_elt, *_clone;
     int                       _X_type, _Y_type;
     int                       _x, _y, _idx;
     int                       _retcode;
     rpn_training_set         *_train_set;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
          _stk_y                   = rpn_pop(stack);
          _Y_type                  = rpn_get_type(_stk_y);

          if ((_Y_type == RPN_TYPE_TRAINING_SET)
          ||  (_Y_type == RPN_TYPE_TEST_SET)) {
               _train_set               = _stk_y->value.obj;
               _idx                     = _stk_x->value.i - 1;

               if (_idx < 0 || _idx >= _train_set->nb_elts) {
                    /* Invalid X
                       ~~~~~~~~~ */
                    _retcode                 = RPN_RET_INVALID_X;

                    /* Restore the stack
                       ~~~~~~~~~~~~~~~~~ */
                    rpn_push(stack, _stk_y);
                    rpn_push(stack, _stk_x);
                    goto end;
               }

               rpn_set_lastx(stack, _stk_x);

               _elt                     = _train_set->array[_idx];
               _stk_result              = rpn_clone_elt(_elt);        // XXX
               _train_set->current_idx  = _idx + 1 + 1;

               /* Keep set element in the stack
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, _stk_y);

               /* Push the extracted element into the stack
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, _stk_result);
          }
          else {
               /* Invalid Y
                  ~~~~~~~~~ */
               _retcode                 = RPN_RET_INVALID_Y;

               /* Restore the stack
                  ~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, _stk_y);
               rpn_push(stack, _stk_x);
               goto end;
          }

          rpn_free_elt(&_stk_x);

          break;

     case RPN_TYPE_TRAINING_SET:
     case RPN_TYPE_TEST_SET:
          _train_set               = _stk_x->value.obj;
          _idx                     = _train_set->current_idx - 1;
          if (_idx >= _train_set->nb_elts) {
               /* No more element
                  ~~~~~~~~~~~~~~~ */
               _retcode                 = RPN_RET_NO_MORE_ELT;

               /* Restore the stack
                  ~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, _stk_x);
               goto end;
          }
          if (_idx < 0) {
//printf("current_idx = %d\n", _idx);
               /* Invalid X
                  ~~~~~~~~~ */
               _retcode                 = RPN_RET_INVALID_X;

               /* Restore the stack
                  ~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, _stk_x);
               goto end;
          }

//        _clone                   = rpn_clone_elt(_stk_x);
//        rpn_set_lastx(stack, _clone);
//        rpn_set_lastx(stack, _stk_x);

          _elt                     = _train_set->array[_idx];
          _stk_result              = rpn_clone_elt(_elt);
          _train_set->current_idx++;

          /* Keep set element in the stack
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_x);

          /* Push the extracted element into the stack
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
          break;

     default:
          _retcode                 = RPN_RET_INVALID_TYPES;

          /* Restore the stack
             ~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_x);
          break;
     }

end:
     return _retcode;
}

/* rpn_op_get() }}} */
/* rpn_op_mat_7seg() {{{ */

/******************************************************************************

                         RPN_OP_MAT_7_SEG

******************************************************************************/
RPN_DEF_OP(rpn_op_mat_7_seg)
{
     rpn_elt                  *_stk_x, *_stk_result, *_elt_i;
     int                       _x, _X_type, _row, _col, _idx;
     int                       _retcode;
     int                      *_digit_w;
     struct rpn_stack         *_stack;
     rpn_operator             *_op;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     /* Check types
        ~~~~~~~~~~~ */
     if (_X_type != RPN_TYPE_INT) {
          _retcode                 = RPN_RET_INVALID_X;
          rpn_push(stack, _stk_x);
          goto end;
     }

     _digit_w                 = rpn_7_segments[_stk_x->value.i].digit;
     _stack                   = rpn_new_stack(__func__);
     for (_row = 0, _idx = 0; _row < RPN_MNIST_ROWS; _row++) {
          for (_col = 0; _col < RPN_MNIST_COLS; _col++) {
               _elt_i              = rpn_new_elt(RPN_TYPE_INT);
               _elt_i->value.i     = _digit_w[_idx++];

               rpn_push(_stack, _elt_i);
          }
     }

#define PRINT_7_SEG      0
#if (PRINT_7_SEG == 1)
     for (_row = 1; _row <= RPN_MNIST_ROWS; _row++) {
          for (_col = 1; _col <= RPN_MNIST_COLS; _col++) {
               _idx                = RPN_MATRIX_IDX(_row, _col, RPN_MNIST_ROWS, RPN_MNIST_COLS);
               if (_col == 0) {
                    printf("%3d", _digit_w[_idx]);
               }
               else {
                    printf(" %3d", _digit_w[_idx]);
               }
          }
          printf("\n");
     }
#endif

     _elt_i              = rpn_new_elt(RPN_TYPE_INT);
     _elt_i->value.i     = RPN_MNIST_ROWS;
     rpn_push(_stack, _elt_i);

     _elt_i              = rpn_new_elt(RPN_TYPE_INT);
     _elt_i->value.i     = RPN_MNIST_COLS;
     rpn_push(_stack, _elt_i);

//rpn_disp_stack(_stack);
     /* Initialize operator
        ~~~~~~~~~~~~~~~~~~~ */
     _op                 = RPN_OP_DESC(MATRIX);

     /* Execute operator
        ~~~~~~~~~~~~~~~~ */
     if ((_retcode = rpn_op_matrix(_stack, _op)) != RPN_RET_OK) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     rpn_set_lastx(stack, _stk_x);

     _stk_result         = rpn_pop(_stack);
     rpn_free_stack(_stack);
     rpn_push(stack, _stk_result);

end:
     return _retcode;
}
/* rpn_op_mat_7seg() }}} */
/* rpn_op_nn_label() {{{ */

/******************************************************************************

                         RPN_OP_NN_LABEL

******************************************************************************/
RPN_DEF_OP(rpn_op_nn_label)
{
     rpn_elt              *_stk_x, *_stk_result;
     int                  _X_type;
     int                  _retcode;
     rpn_training_elt    *_train;
     rpn_training_label  *_label;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_TRAINING_ELT:
     case RPN_TYPE_TEST_ELT:
          _train                   = _stk_x->value.obj;
          _label                   = _train->label;

          _stk_result              = rpn_new_elt(RPN_TYPE_INT);
          _stk_result->value.i     = _label->value;
          break;

     default:
          _retcode                 = RPN_RET_INVALID_X;

          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          RPN_INTERNAL_ERROR;
          break;
     }

     rpn_set_lastx(stack, _stk_x);
     rpn_free_elt(&_stk_x);

     rpn_push(stack, _stk_result);

end:

     return _retcode;
}

/* rpn_op_nn_label() }}} */
/* rpn_op_nn_train_index() {{{ */

/******************************************************************************

                         RPN_OP_NN_TRAIN_INDEX

******************************************************************************/
RPN_DEF_OP(rpn_op_nn_train_index)
{
     rpn_elt             *_stk_x, *_stk_result;
     int                  _X_type;
     int                  _retcode;
     rpn_training_elt    *_train;
     rpn_training_data   *_data;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_TRAINING_ELT:
     case RPN_TYPE_TEST_ELT:
          _train                   = _stk_x->value.obj;
          _data                    = _train->data;

          _stk_result              = rpn_new_elt(RPN_TYPE_INT);

          switch (op->op_id) {

          case RPN_OP_ID_NN_INDEX:
               _stk_result->value.i     = _data->index;
               break;

          case RPN_OP_ID_NN_ORIG_INDEX:
               _stk_result->value.i     = _data->orig_index;
               break;

          default:
               _retcode                 = RPN_RET_INVALID_X;

               fprintf(stderr, "%s: operator ID = %d (%s)\n",
                       G.progname, op->op_id, op->op_name);
               RPN_INTERNAL_ERROR;
               break;
          }
          break;

     default:
          _retcode                 = RPN_RET_INVALID_X;

          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          RPN_INTERNAL_ERROR;
          break;
     }

     rpn_set_lastx(stack, _stk_x);

     rpn_push(stack, _stk_result);

     return _retcode;
}

/* rpn_op_nn_train_index() }}} */
/* rpn_mnist_install_file() {{{ */

/******************************************************************************

                         RPN_MNIST_INSTALL_FILE

******************************************************************************/
char *rpn_mnist_install_file(char *url)
{
     char                     *_gz_file, *_basename;
     int                       _ret;

     _gz_file            = basename(strdup(url));
     _basename           = strdup(_gz_file);
     rpn_remove_ext(_basename);

     /* Check for the existence of the file
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (access(_basename, F_OK) < 0) {
          /* File does not exist : uncompress the file
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
fprintf(stderr, "%s (%d) : file \"%s\" does not exist => uncompress ...\n",
        __func__, __LINE__, _basename);
          rpn_mnist_uncompress_file(_gz_file, url);
     }

     return _basename;
}

/* rpn_mnist_install_file() }}} */
/* rpn_mnist_uncompress_file() {{{ */

/******************************************************************************

                         RPN_MNIST_UNCOMPRESS_FILE

******************************************************************************/
void rpn_mnist_uncompress_file(char *gz_file, char *url)
{
     char                     *_basename, _cmd[RPN_SYS_CMD_SIZE];
     int                       _ret;

     /* Check for the existence of the compressed file
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (access(gz_file, F_OK) < 0) {
          /* File does not exist : download the file
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
fprintf(stderr, "%s (%d) : file \"%s\" does not exist => download ...\n",
        __func__, __LINE__, gz_file);
          rpn_mnist_download_file(url);
     }

     /* Uncompress the downloaded file
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     sprintf(_cmd, "gunzip %s", gz_file);
fprintf(stderr, "%s (%d) : uncompress (cmd = \"%s\" ...\n",
        __func__, __LINE__, gz_file);

     if ((_ret = system(_cmd) < 0)) {
          fprintf(stderr, "%s: uncompress error, command (%s) returned an error (%d)!\n",
                  G.progname, _cmd, _ret);
          perror("system");
          exit(RPN_EXIT_SYSTEM_ERROR);
     }
}

/* rpn_mnist_uncompress_file() }}} */
/* rpn_mnist_download_file() {{{ */

/******************************************************************************

                         RPN_MNIST_DOWNLOAD_FILE

******************************************************************************/
void rpn_mnist_download_file(char *url)
{
     char                      _cmd[RPN_SYS_CMD_SIZE];
     int                       _ret;

     /* Prepare download command
        ~~~~~~~~~~~~~~~~~~~~~~~~ */
     sprintf(_cmd, "wget %s", url);
     
     /* Execute the download command
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if ((_ret = system(_cmd) < 0)) {
          fprintf(stderr, "%s: download command (%s) returned an error (%d)!\n",
                  G.progname, _cmd, _ret);
          perror("system");
          exit(RPN_EXIT_SYSTEM_ERROR);
     }
}

/* rpn_mnist_download_file() }}} */
/* rpn_remove_ext() {{{ */

/*******************************************************************************

                         RPN_REMOVE_EXT

*******************************************************************************/
void rpn_remove_ext(char *s)
{
     int                 _i, _j;

     _i = _j = 0;

     /* Search for the last '.' character
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     while (s[_i] != '\0') {
          if (s[_i] == '.') {
               _j = _i;
          }
          _i++;
     }

     /* Reset the last '.' character
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (_j != 0) {
          s[_j] = '\0';
     }
}

/* rpn_remove_ext() }}} */
/* rpn_plot_pic() {{{ */

/******************************************************************************

                         RPN_PLOT_PIC

******************************************************************************/
void rpn_plot_pic(char *basename, rpn_mnist_desc *mnist)
{
     char                 _cmd[RPN_SYS_CMD_SIZE];
     int                  _ret;

     sprintf(_cmd, "gnuplot -e RADIX='\"%s\"' %s/%s",
              basename, RPN_MNIST_DIR_TOOLS, mnist->plot_pic);
printf("PLOT COMMAND [%s]\n", _cmd);

     /* Execute the plot command
        ~~~~~~~~~~~~~~~~~~~~~~~~ */
     if ((_ret = system(_cmd) < 0)) {
          fprintf(stderr, "%s: plot command (%s) returned an error (%d)!\n",
                  G.progname, _cmd, _ret);
          perror("system");
          exit(RPN_EXIT_SYSTEM_ERROR);
     }
}

/* rpn_plot_pic() }}} */
/* rpn_plot_pics() {{{ */

/******************************************************************************

                         RPN_PLOT_PICS

******************************************************************************/
void rpn_plot_pics(char *basename, rpn_mnist_desc *mnist)
{
     char                 _cmd[RPN_SYS_CMD_SIZE];
     int                  _ret;

     sprintf(_cmd, "gnuplot -e RADIX='\"%s\"' %s/%s",
              basename, RPN_MNIST_DIR_TOOLS, mnist->plot_pics);
printf("PLOT COMMAND [%s]\n", _cmd);

     /* Execute the plot command
        ~~~~~~~~~~~~~~~~~~~~~~~~ */
     if ((_ret = system(_cmd) < 0)) {
          fprintf(stderr, "%s: plot command (%s) returned an error (%d)!\n",
                  G.progname, _cmd, _ret);
          perror("system");
          exit(RPN_EXIT_SYSTEM_ERROR);
     }
}


/* rpn_plot_pics() }}} */
/* rpn_op_nn_dispatch() {{{ */

/******************************************************************************

                         RPN_OP_NN_DISPATCH

******************************************************************************/
RPN_DEF_OP(rpn_op_nn_dispatch)
{
     rpn_elt                  *_stk_x;
     int                       _X_type, _i, _index, _label, _size, _num_elts;
     struct rpn_stack         *_lists[10];
     int                       _retcode;
     struct rpn_training_set  *_train;
     rpn_training_elt         *_elt_train;
     rpn_training_data        *_elt_data;
     rpn_training_label       *_elt_label;
     rpn_training_set         *_train_set;
     rpn_elt                  *_elt, *_clone, *_new_set;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_TRAINING_SET:
     case RPN_TYPE_TEST_SET:
          /* Push the element back on the stack
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_x);

          _train                   = _stk_x->value.obj;

          /* Allocate the stacks / FIFO descriptors
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          for (_i = 0; _i < sizeof(_lists) / sizeof(_lists[0]); _i++) {
               _lists[_i]               = rpn_new_stack(__func__);
          }

          /* Dispatch the clones of the elements into the stacks / FIFO
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          for (_index = 0; _index < _train->nb_elts; _index++) {
               _elt                = _train->array[_index];
               _elt_train          = _elt->value.obj;
               _elt_data           = _elt_train->data;
               _elt_label          = _elt_train->label;
               _label              = _elt_label->value;

               _clone              = rpn_clone_elt(_elt);
               rpn_push(_lists[_label], _clone);
          }

#if 0
          /* Display counters for each label
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          for (_label = 0; _label < sizeof(_lists) / sizeof(_lists[0]); _label++) {
               printf("LABEL %d : %5d elements\n", _label, _lists[_label]->num_elts);
          }
#endif

          /* Create the training sets for each label
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//        for (_label = 0; _label < sizeof(_lists) / sizeof(_lists[0]); _label++) {
          for (_label = (sizeof(_lists) / sizeof(_lists[0])) - 1; _label >= 0; _label--) {
               char                      _name[64];

               /* Create the _SET element
                  ~~~~~~~~~~~~~~~~~~~~~~~ */
               _new_set                 = rpn_new_elt(_X_type);
               _size                    = sizeof(*_train_set)
                                        + ((_lists[_label]->num_elts - 1) * sizeof(struct rpn_elt *));
               _train_set               = (struct rpn_training_set *) RPN_MALLOC(_size);
               sprintf(_name, "%s-%d", _train->name, _label);
               _train_set->name         = strdup(_name);
               _train_set->nb_elts      = _lists[_label]->num_elts;
               _train_set->current_idx  = 1;
               _train_set->nrows        = _train->nrows;
               _train_set->ncols        = _train->ncols;
               _train_set->width_pix    = _train->width_pix;
               _train_set->height_pix   = _train->height_pix;
               _train_set->elt_width    = _train->elt_width;
               _train_set->elt_height   = _train->elt_height;

#if 0
printf("Name                     = %s\n", _name);
printf("sizeof(rpn_training_set) = %6d\n", sizeof(rpn_training_set));
printf("sizeof(train_set)        = %6d\n", _size);
printf("sizeof(rpn_elt *)        = %6d\n", sizeof(struct rpn_elt *));
#endif

               _new_set->value.obj      = _train_set;


               /* Copy the number of the elements in the stack so that
                * it stays constant during the complete loop.
                * rpn_base_pop(_lists[_label]) decrements the member num_elts,
                * so it cannot be use directly to control the loop
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               _num_elts                = _lists[_label]->num_elts;
               for (_i = 0; _i < _num_elts; _i++) {
                    _elt                     = rpn_base_pop(_lists[_label]);
                    _elt_train               = _elt->value.obj;
                    _elt_data                = _elt_train->data;
                    _elt_data->index         = _i + 1;
                    _train_set->array[_i]    = _elt;
               }

               /* Push the new training set on the stack
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, _new_set);
          }

          for (_i = 0; _i < sizeof(_lists) / sizeof(_lists[0]); _i++) {
               /* Free the temporary stacks / FIFO
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//             rpn_free_stack(_lists[_i]);
          }

          break;

     default:
          _retcode                 = RPN_RET_INVALID_X_TYPE;

          /* Restore the stack
             ~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_x);
          goto end;
          break;
     }

end:
     return _retcode;
}

/* rpn_op_nn_dispatch() }}} */
/* rpn_op_nn_ignore() {{{ */

/******************************************************************************

                         RPN_OP_NN_IGNORE

******************************************************************************/
RPN_DEF_OP(rpn_op_nn_ignore)
{
     int                       _retcode, _X_type, _Y_type, _idx;
     rpn_elt                  *_stk_x, *_stk_y, *_elt;
     rpn_training_set         *_train_set;
     rpn_training_elt         *_train_elt;
     rpn_training_data        *_data;

     _retcode                 = RPN_RET_OK;

     _X_type                  = rpn_get_X_type(stack);
     _Y_type                  = rpn_get_Y_type(stack);

     switch (_X_type) {

     case RPN_TYPE_INT:
          switch (_Y_type) {

          case RPN_TYPE_TRAINING_SET:
               /* Retrieve index
                  ~~~~~~~~~~~~~~ */
               _stk_x                   = rpn_pop(stack);
               rpn_set_lastx(stack, _stk_x);
               _idx                     = _stk_x->value.i;

               _stk_y                   = rpn_pop(stack);

               /* TRAINING_SET is persistent, so push it back on the stack
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, _stk_y);

               _train_set               = _stk_y->value.obj;

               /* Check validity of the specified index
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if (_idx <= 0 || _idx > _train_set->nb_elts) {
                    rpn_push(stack, _stk_x);
                    _retcode                 = RPN_RET_INVALID_X;
                    goto end;
               }

               _elt                     = _train_set->array[_idx - 1];
               _train_elt               = _elt->value.obj;
               _data                    = _train_elt->data;

               switch (op->op_id) {

               case RPN_OP_ID_NN_IGNORE:
                    _data->ignore            = TRUE;
                    break;

               case RPN_OP_ID_NN_USE:
                    _data->ignore            = FALSE;
                    break;

               default:
                    RPN_INTERNAL_ERROR;
                    break;
               }
               break;

          default:
               printf("*** Invalid Y type (%s) for operator \"%s\" !\n", rpn_type_to_string(_Y_type), op->op_name);
               break;
          }
          break;

     case RPN_TYPE_TRAINING_ELT:
          _stk_x                   = rpn_pop(stack);
          rpn_set_lastx(stack, _stk_x);

          _train_elt               = _stk_x->value.obj;
          _data                    = _train_elt->data;

          switch (op->op_id) {

          case RPN_OP_ID_NN_IGNORE:
               _data->ignore            = TRUE;
               break;

          case RPN_OP_ID_NN_USE:
               _data->ignore            = FALSE;
               break;

          default:
               RPN_INTERNAL_ERROR;
               break;
          }
          rpn_push(stack, _stk_x);
          break;

     default:
          printf("*** Invalid X type (%s) for operator \"%s\" !\n", rpn_type_to_string(_X_type), op->op_name);
          break;
     }

end:
     return _retcode;
}
/* rpn_op_nn_ignore() }}} */

struct rpn_text_file           rpn_text_plot_pic = {
     TRUE,
     0444,
     NULL,               /* Dirname  : dynamically initialized */
     NULL,               /* Filename : dynamically initialized */
     {
          "#!/usr/bin/gnuplot",
          "#",
          "#  @(#) [MB] cy_plot_heatmap_gray_640 Version 1.2 du 19/01/01 - ",
          "#",
          "",
          "reset",
          "",
          "set terminal pngcairo size 640,480 enhanced font 'Verdana,10'",
          "INPUT    = sprintf(\"TEXTS/%s.%s\",  RADIX, \"txt\")",
          "OUTPUT   = sprintf(\"IMAGES/%s.%s\", RADIX, \"png\")",
          "print \"RADIX  = \", RADIX",
          "print \"INPUT  = \", INPUT",
          "print \"OUTPUT = \", OUTPUT",
          "set output OUTPUT",
          "",
          "set border linewidth 0",
          "unset key",
          "unset colorbox",
          "unset tics",
          "set lmargin screen 0.1",
          "set rmargin screen 0.9",
          "set tmargin screen 0.9",
          "set bmargin screen 0.1",
          "set xlabel noenhanced  OUTPUT",
          "# set palette grey",
          "set palette grey negative",
          "",
          "set pm3d map",
          "splot INPUT matrix",
          NULL
     }
};

struct rpn_text_file           rpn_text_plot_pics = {
     TRUE,
     0444,
     NULL,               /* Dirname  : dynamically initialized */
     NULL,               /* Filename : dynamically initialized */
     {
          "#!/usr/bin/gnuplot",
          "#",
          "#    @(#) [MB] cy_plot_heatmap_gray_1920     Version 1.6 du 18/12/31 -",
          "#",
          "",
          "reset",
          "",
          "set terminal pngcairo size 1920,1080 enhanced font 'Verdana,10'",
          "INPUT    = sprintf(\"TEXTS/GROUPS/%s.%s\",  RADIX, \"txt\")",
          "OUTPUT   = sprintf(\"IMAGES/GROUPS/%s.%s\", RADIX, \"png\")",
          "print \"RADIX  = \", RADIX",
          "print \"INPUT  = \", INPUT",
          "print \"OUTPUT = \", OUTPUT",
          "set output OUTPUT",
          "set border linewidth 0",
          "unset key",
          "unset colorbox",
          "unset tics",
          "set lmargin screen 0.1",
          "set rmargin screen 0.9",
          "set tmargin screen 0.9",
          "set bmargin screen 0.1",
          "set xlabel noenhanced  OUTPUT",
          "set palette grey negative",
          "set pm3d map",
          "splot INPUT matrix",
          NULL
     }
};

/* rpn_tools_plot() {{{ */

/******************************************************************************

                         RPN_TOOLS_PLOT

******************************************************************************/
void rpn_tools_plot(rpn_text_file *text_file)
{

     
}
/* rpn_tools_plot() }}} */
/*rpn_new_mnist_desc() {{{ */
/******************************************************************************

                         RPN_NEW_MNIST_DESC

******************************************************************************/
RPN_NEW(mnist_desc)

/*rpn_new_mnist_desc() }}} */
/* rpn_mnist_init() {{{ */
/******************************************************************************

                         RPN_MNIST_INIT

******************************************************************************/
rpn_mnist_desc *rpn_mnist_init()
{
     struct rpn_mnist_desc         *_desc;
     int                            _size;
     char                          *_subdir_fmt   = "%s/%s";
     rpn_stack                     *_stack;
     rpn_elt                       *_elt;

     _desc                         = rpn_new_mnist_desc();
     G.mnist                       = _desc;

     /* Make sure that necessary directories are created
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _desc->data_dir               = RPN_MNIST_DIR_DATA;
     _desc->texts_dir              = RPN_MNIST_DIR_TEXTS;
     _desc->images_dir             = RPN_MNIST_DIR_IMAGES;
     _desc->tools_dir              = RPN_MNIST_DIR_TOOLS;
     _desc->groups_dir             = RPN_MNIST_SUBDIR_GROUPS;

     _desc->plot_pic               = RPN_MNIST_PLOT_PIC;
     _desc->plot_pics              = RPN_MNIST_PLOT_PICS;

     _size                         = strlen(_desc->texts_dir) + 1
                                   + strlen(_desc->groups_dir) + 1;

     if ((_desc->subdir_texts = RPN_MALLOC(_size)) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     sprintf(_desc->subdir_texts, _subdir_fmt, _desc->texts_dir,  _desc->groups_dir);

     _size                         = strlen(_desc->images_dir) + 1
                                   + strlen(_desc->groups_dir) + 1;

     if ((_desc->subdir_images = RPN_MALLOC(_size)) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     sprintf(_desc->subdir_images, _subdir_fmt, _desc->images_dir, _desc->groups_dir);

     /* Check existence of directories
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_mkdir(_desc->data_dir);
     rpn_mkdir(_desc->texts_dir);
     rpn_mkdir(_desc->images_dir);
     rpn_mkdir(_desc->tools_dir);

     rpn_mkdir(_desc->subdir_texts);
     rpn_mkdir(_desc->subdir_images);

     _stack                        = rpn_new_stack(__func__);

     /* Create plot scripts
        ~~~~~~~~~~~~~~~~~~~ */
     rpn_text_plot_pic.dirname     = _desc->tools_dir;
     rpn_text_plot_pic.filename    = _desc->plot_pic;
     _elt                          = rpn_new_elt(RPN_TYPE_TEXT_FILE);
     _elt->value.obj               = &rpn_text_plot_pic;

     rpn_push(_stack, _elt);

     /* Write plot script to file
        ~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_op_write(_stack, RPN_OP_DESC(WRITE));

     rpn_text_plot_pics.dirname    = _desc->tools_dir;
     rpn_text_plot_pics.filename   = _desc->plot_pics;
     _elt                          = rpn_new_elt(RPN_TYPE_TEXT_FILE);
     _elt->value.obj               = &rpn_text_plot_pics;

     rpn_push(_stack, _elt);

     /* Write plot script to file
        ~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_op_write(_stack, RPN_OP_DESC(WRITE));

     /* Free temporary stack
        ~~~~~~~~~~~~~~~~~~~~ */
     rpn_free_stack(_stack);

     return _desc;
}

/* rpn_mnist_init() }}} */
/* GROUP : Neural networks }}} */
/* GROUP : Backup / restore {{{ */
/* rpn_backup_elt() {{{ */

/******************************************************************************

                         RPN_BACKUP_ELT

******************************************************************************/
int rpn_backup_elt(rpn_elt *elt, struct rpn_backup_ctx *bkp_ctx)
{
     int                       _retcode, _elt_type;

     _retcode                 = RPN_RET_OK;
     _elt_type                = rpn_get_type(elt);

     switch (_elt_type) {

     case RPN_TYPE_MATRIX:
fprintf(stderr, "%s (%d) : type = RPN_TYPE_MATRIX\n", __func__, __LINE__);
          break;

     default:
fprintf(stderr, "%s (%d) : type unknown (%d)\n", __func__, __LINE__, _elt_type);
          break;
     }

     return _retcode;
}
/* rpn_backup_elt() }}} */
/* rpn_op_write() {{{ */

/******************************************************************************

                         RPN_OP_WRITE

******************************************************************************/
RPN_DEF_OP(rpn_op_write)
{
     rpn_elt                  *_stk_x, *_stk_y;
     int                       _X_type, _Y_type = RPN_TYPE_UNDEFINED;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_FILENAME:
          _stk_y                   = rpn_pop(stack);
          _Y_type                  = rpn_get_type(_stk_y);

          switch (_Y_type) {

          case RPN_TYPE_MATRIX:
               {
                    rpn_matrix               *_mat;
                    int                       _n, _p, _i, _j, _idx, _val, _type;
                    FILE                     *_fp;
                    char                     *_filename;
                    char                     *_fmt = "%3d";
                    char                     *_tab = "     ";
                    rpn_elt                  *_elt;

                    _filename                = _stk_x->value.s;
                    if ((_fp = fopen(_filename, "w")) == NULL) {
                         fprintf(stderr, "%s (%d) : cannot open \"%s\" !\n",
                                 __func__, __LINE__, _filename);
                         RPN_INTERNAL_ERROR;
                    }

                    _mat                     = (struct rpn_matrix *) _stk_y->value.obj;
                    _n                       = _mat->n;
                    _p                       = _mat->p;

                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              if (_j == 1 && _i > 1) {
                                   fprintf(_fp, "\n");
                              }
                              if (_j == 1) {
                                   fprintf(_fp, "%s", _tab);
                              }

                              _idx                     = RPN_MATRIX_IDX(_i, _j, _n, _p);
                              _elt                     = _mat->base[_idx];
                              if ((_type = rpn_get_type(_elt)) != RPN_TYPE_INT) {
                                   printf("Type = %d (%s) !\n",
                                         _type, rpn_type_to_string(_type));
                                   RPN_INTERNAL_ERROR;
                              }
                              _val                     = _elt->value.i;

                              if (_j > 1) {
                                   fprintf(_fp, " ");
                              }
                              fprintf(_fp, _fmt, _val);
                         }
                    }
                    fprintf(_fp, "\n");
// X
                    fclose(_fp);
               }

               rpn_free_elt(&_stk_x);
               rpn_free_elt(&_stk_y);
               break;

          default:
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
          }
          break;

     case RPN_TYPE_TEXT_FILE:
          {
               FILE                     *_fp;
               rpn_text_file            *_text_file;
               int                       _size, _i;
               char                     *_pathname, *_line;

               _text_file                    = _stk_x->value.obj;

               /* Eventually create the directory containing the file
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if ((_text_file->dirname != 0)) {
                    rpn_mkdir(_text_file->dirname);

                    _size                    = strlen(_text_file->dirname) + 1
                                             + strlen(_text_file->filename) + 1;
                    _pathname                = RPN_MALLOC(_size);
                    sprintf(_pathname, "%s/%s", _text_file->dirname, _text_file->filename);
               }
               else {
                    _pathname                = strdup(_text_file->filename);
               }

               if (access(_pathname, F_OK) < 0) {
                    if ((_fp = fopen(_pathname, "w")) == NULL) {
                         fprintf(stderr, "%s (%d) : cannot open \"%s\" !\n",
                                 __func__, __LINE__, _pathname);
                         RPN_INTERNAL_ERROR;
                    }

                    for (_i = 0; _text_file->text[_i] != NULL; _i++) {
                         _line                         = _text_file->text[_i];
                         fprintf(_fp, "%s\n", _line);
                    }
                    fclose(_fp);
               }

               /* Initialize file mode
                  ~~~~~~~~~~~~~~~~~~~~ */
               if (chmod(_pathname, _text_file->filemode) < 0) {
                    fprintf(stderr, "%s: cannot set mode of \"%s\" to %04o !\n",
                            G.progname, _pathname, _text_file->filemode);
               }

               RPN_FREE(_pathname);
          }

          /* TODO : initialize lastx
             ~~~~~~~~~~~~~~~~~~~~~~~ */
 //       rpn_set_lastx(stack, _stk_x);      // XXX

          rpn_free(_stk_x);
          break;

     default:
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
          RPN_INTERNAL_ERROR;
          break;
     }

end:
     return _retcode;
}

/* rpn_op_write() }}} */
/* rpn_op_write_csv() {{{ */

/******************************************************************************

                         RPN_OP_WRITE_CSV

******************************************************************************/
RPN_DEF_OP(rpn_op_write_csv)
{
     rpn_elt                  *_stk_x, *_stk_y;
     int                       _X_type, _Y_type = RPN_TYPE_UNDEFINED;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_FILENAME:
          _stk_y                   = rpn_pop(stack);
          _Y_type                  = rpn_get_type(_stk_y);

          switch (_Y_type) {

          case RPN_TYPE_MATRIX:
               {
                    rpn_matrix               *_mat;
                    int                       _n, _p, _i, _j, _idx, _val, _type;
                    FILE                     *_fp;
                    char                     *_filename;
                    char                     *_fmt = "%3d";
                    char                     *_tab = "     ";
                    rpn_elt                  *_elt;

                    _filename                = _stk_x->value.s;
                    if ((_fp = fopen(_filename, "w")) == NULL) {
                         fprintf(stderr, "%s (%d) : cannot open \"%s\" !\n",
                                 __func__, __LINE__, _filename);
                         RPN_INTERNAL_ERROR;
                    }

                    _mat                     = (struct rpn_matrix *) _stk_y->value.obj;
                    _n                       = _mat->n;
                    _p                       = _mat->p;

                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _idx                     = RPN_MATRIX_IDX(_i, _j, _n, _p);
                              _elt                     = _mat->base[_idx];
                              if (rpn_get_type(_elt) != RPN_TYPE_NIL) {
                                   rpn_write_csv(_elt, _fp);
                                   fprintf(_fp, "\n");
                              }
                         }
                    }
//                  fprintf(_fp, "\n");
// X
                    fclose(_fp);
               }

               rpn_free_elt(&_stk_x);
               rpn_free_elt(&_stk_y);
               break;

          default:
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
          }
          break;

     case RPN_TYPE_TEXT_FILE:
          {
               FILE                     *_fp;
               rpn_text_file            *_text_file;
               int                       _size, _i;
               char                     *_pathname, *_line;

               _text_file                    = _stk_x->value.obj;

               /* Eventually create the directory containing the file
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if ((_text_file->dirname != 0)) {
                    rpn_mkdir(_text_file->dirname);

                    _size                    = strlen(_text_file->dirname) + 1
                                             + strlen(_text_file->filename) + 1;
                    _pathname                = RPN_MALLOC(_size);
                    sprintf(_pathname, "%s/%s", _text_file->dirname, _text_file->filename);
               }
               else {
                    _pathname                = strdup(_text_file->filename);
               }

               if (access(_pathname, F_OK) < 0) {
                    if ((_fp = fopen(_pathname, "w")) == NULL) {
                         fprintf(stderr, "%s (%d) : cannot open \"%s\" !\n",
                                 __func__, __LINE__, _pathname);
                         RPN_INTERNAL_ERROR;
                    }

                    for (_i = 0; _text_file->text[_i] != NULL; _i++) {
                         _line                         = _text_file->text[_i];
                         fprintf(_fp, "%s\n", _line);
                    }
                    fclose(_fp);
               }

               /* Initialize file mode
                  ~~~~~~~~~~~~~~~~~~~~ */
               if (chmod(_pathname, _text_file->filemode) < 0) {
                    fprintf(stderr, "%s: cannot set mode of \"%s\" to %04o !\n",
                            G.progname, _pathname, _text_file->filemode);
               }

               RPN_FREE(_pathname);
          }

          /* TODO : initialize lastx
             ~~~~~~~~~~~~~~~~~~~~~~~ */
 //       rpn_set_lastx(stack, _stk_x);      // XXX

          rpn_free(_stk_x);
          break;

     default:
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
          RPN_INTERNAL_ERROR;
          break;
     }

end:
     return _retcode;
}

/* rpn_op_write_csv() }}} */
/* rpn_write_csv() {{{ */

/******************************************************************************

                         RPN_WRITE_CSV

******************************************************************************/
void rpn_write_csv(rpn_elt *elt, FILE *fp)
{
     int                       _type;
     char                      _sep;

     _sep                     = ',';
     _type                    = rpn_get_type(elt);

     switch (_type) {

     case RPN_TYPE_NIL :
          fprintf(fp, "NIL");
          fprintf(fp, "%c", _sep);
          break;

     case RPN_TYPE_INT :
          fprintf(fp, "%10d", elt->value.i);
          fprintf(fp, "%c", _sep);
          break;

     case RPN_TYPE_DOUBLE :
          fprintf(fp, "%15.5f", elt->value.d);
          fprintf(fp, "%c", _sep);
          break;

     case RPN_TYPE_STRING :
          fprintf(fp, "%s", elt->value.s);
          fprintf(fp, "%c", _sep);
          break;

     case RPN_TYPE_LITTERAL :
          fprintf(fp, "'%s'", elt->value.s);
          fprintf(fp, "%c", _sep);
          break;

     case RPN_TYPE_MATRIX :
          {
               int                       _n, _p, _i, _j, _idx;
               struct rpn_elt           *_elt;
               rpn_matrix               *_matrix;

               _matrix        = (struct rpn_matrix *) elt->value.obj;
               _n             = _matrix->n;
               _p             = _matrix->p;
//             printf("MATRIX    [%d x %d]\n", _n, _p);
               for (_i = 1; _i <= _n; _i++) {
                    for (_j = 1; _j <= _p; _j++) {
                         _idx           = ((_i - 1) * _p) + (_j - 1);
                         _elt           = (struct rpn_elt *) _matrix->base[_idx];
                         rpn_write_csv(_elt, fp);
                    }
               }
//                  printf("]\n");
          }
          break;

     case RPN_TYPE_FILENAME:
          fprintf(fp, "'%s'", elt->value.s);
          fprintf(fp, "%c", _sep);
          break;

#if 0
     case RPN_TYPE_START_MARKER:
          fprintf(fp, "BEGIN");
          break;

     case RPN_TYPE_NEURAL_MLP:
          {
               struct rpn_mlp      *_mlp;

               _mlp                = (struct rpn_mlp *) elt->value.obj;
               fprintf(fp, "NEURAL MLP (%d layers, %d neurons, %d weights, activation function : %s)",
                      _mlp->nb_layers, _mlp->nb_neurons, _mlp->nb_weights, _mlp->str_af);
          }
          break;

     case RPN_TYPE_TRAINING_ELT:
          {
               struct rpn_training_elt       *_train;
               struct rpn_training_data      *_data;
               struct rpn_training_label     *_label;

               _train         = elt->value.obj;
               _data          = _train->data;
               _label         = _train->label;

               printf("%-*s [idx = %7d, orig = %7d] '%d' %6d -- [%3d x %3d] %-7s",
                      _sz, "TRAINING_ELT",
                      _data->index, _data->orig_index,
                      _label->value, _data->nb_elts * _data->elt_size,
                      _data->num_rows, _data->num_cols,
                      _data->ignore ? "IGNORED" : "USED");
          }
          break;

     case RPN_TYPE_TRAINING_SET:
          {
               rpn_training_set         *_train_set;

               _train_set     = elt->value.obj;

               printf("%-*s '%s'", _sz, "TRAINING_SET", _train_set->name);
               printf(" %5d elts, current idx = %d",
                      _train_set->nb_elts, _train_set->current_idx);
          }
          break;

     case RPN_TYPE_TEST_ELT:
          {
               struct rpn_training_elt       *_train;
               struct rpn_training_data      *_data;
               struct rpn_training_label     *_label;

               _train         = elt->value.obj;
               _data          = _train->data;
               _label         = _train->label;

               printf("%-*s [idx = %7d, orig = %7d] '%d' %6d -- [%3d x %3d]",
                      _sz, "TEST_ELT    ",
                      _data->index, _data->orig_index,
                      _label->value, _data->nb_elts * _data->elt_size,
                      _data->num_rows, _data->num_cols);
          }
          break;

     case RPN_TYPE_TEST_SET:
          {
               rpn_training_set         *_train_set;

               _train_set     = elt->value.obj;

               printf("%-*s '%s'", _sz, "TEST_SET    ", _train_set->name);
               printf(" %5d elts, current idx = %d",
                      _train_set->nb_elts, _train_set->current_idx);
          }
          break;
#endif    /* 0 */

     case RPN_TYPE_LIST:
          {
               int                  _n, _p, _i, _j, _idx;
               rpn_list            *_list;
               rpn_elt             *_elt;

               _list               = elt->value.obj;

//             printf("LIST      [%s] %6d elts (%s)\n",
//                    _list->name, _list->num_elts, _list->homogeneous ? "Homogeneous" : "Heterogeneous");
               for (_elt = _list->top_elt; _elt != 0; _elt = _elt->previous_elt) {
                    rpn_write_csv(_elt, fp);
               }

//             printf("\n");
          }
          break;

#if 0
     case RPN_TYPE_TUPLE:
          {
               int                  _n, _p, _i, _j, _idx;
               rpn_list            *_list;

               _list               = elt->value.obj;

               printf("TUPLE     [%s] %6d elts (%s)\n",
                      _list->name, _list->num_elts, _list->homogeneous ? "Homogeneous" : "Heterogeneous");

               printf(" elt1 ... elt%d\n", _list->num_elts);
               printf("\n");
               
          }
          break;
#endif /* 0 */

     case RPN_TYPE_OPAIR:
          {
               int                  _n, _p, _i, _j, _idx;
               rpn_pair            *_pair;
               rpn_list            *_list_a, *_list_b;
               char                *_str_a, *_str_b;
               rpn_elt             *_elt_a, *_elt_b;
               int                  _a_type, _b_type;
               char                *_a_str_type, *_b_str_type;

               _pair               = elt->value.obj;

//             printf("OPAIR     [%s]\n", _pair->name);
               _elt_a              = _pair->elt_a;
               _elt_b              = _pair->elt_b;

               rpn_write_csv(_elt_a, fp);
               rpn_write_csv(_elt_b, fp);

#if 0
               _a_type             = rpn_get_type(_elt_a);
               _b_type             = rpn_get_type(_elt_b);

               _a_str_type         = rpn_type_to_string(_a_type);
               _b_str_type         = rpn_type_to_string(_b_type);

               _list_a             = _elt_a->value.obj;
               _list_b             = _elt_b->value.obj;

               _str_a              =  _list_a->top_elt->value.s;
               _str_b              =  _list_b->top_elt->value.s;

               printf("(%s, %s) ", _str_a, _str_b);
               printf("\n");
#endif
          }
          break;

     default :
          fprintf(stderr, "%s: (%s) unknown element type (%d)\n",
                  G.progname, __func__, _type);
          exit(RPN_EXIT_INTERNAL_ERR);
     }
}

/* rpn_write_csv() }}} */

/* GROUP : Backup / restore }}} */
/* GROUP : Computer science {{{ */
/* rpn_op_cs_gen_C() {{{ */

/******************************************************************************

                         RPN_OP_CS_GEN_C

******************************************************************************/
RPN_DEF_OP(rpn_op_cs_gen_C)
{
     struct rpn_elt           *_stk_x;
     int                       _retcode, _X_type;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

#if 0
     case RPN_TYPE_TRAINING_ELT:
          {
               // TODO : TRAINING_ELT => C
               fprintf(stderr, "%s (%d) : type unknown (%d)\n",
                       __func__, __LINE__, _X_type);
               RPN_INTERNAL_ERROR;
          }
          break;
#endif

     case RPN_TYPE_MATRIX:
          {
               struct rpn_matrix        *_matrix;
               struct rpn_elt           *_elt;
               int                       _n, _p, _i, _j, _idx, _val;
               char                     *_fmt = "%3d";
               char                     *_tab = "     ";

               _matrix                  = (struct rpn_matrix *) _stk_x->value.obj;

               _n                       = _matrix->n;
               _p                       = _matrix->p;

               printf("{\n");
               for (_i = 1; _i <= _n; _i++) {
                    for (_j = 1; _j <= _p; _j++) {
                         if (_j == 1 && _i > 1) {
                              printf(",\n");
                         }
                         if (_j == 1) {
                              printf("%s", _tab);
                         }

                         _idx                     = RPN_MATRIX_IDX(_i, _j, _n, _p);
                         _elt                     = _matrix->base[_idx];
                         if (rpn_get_type(_elt) != RPN_TYPE_INT) {
                              RPN_INTERNAL_ERROR;
                         }
                         _val                     = _elt->value.i;

                         if (_j > 1) {
                              printf(", ");
                         }
                         printf(_fmt, _val);
                    }
               }
               printf("\n};\n");
          }
          break;

     default:
fprintf(stderr, "%s (%d) : type unknown (%d)\n", __func__, __LINE__, _X_type);
          break;
     }

     rpn_push(stack, _stk_x);

     return _retcode;
}

/* rpn_op_cs_gen_C() }}} */
/* GROUP : Computer science }}} */
/* GROUP : System{{{ */
/* rpn_op_sys_memory() {{{ */

/******************************************************************************

                         RPN_OP_SYS_MEMORY

******************************************************************************/
RPN_DEF_OP(rpn_op_sys_memory)
{
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     M;

     return _retcode;
}

/* rpn_op_sys_memory() }}} */
/* rpn_op_sys_pause() {{{ */

/******************************************************************************

                         RPN_OP_SYS_PAUSE

******************************************************************************/
RPN_DEF_OP(rpn_op_sys_pause)
{
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     pause();

     return _retcode;
}
/* rpn_op_sys_pause() }}} */
/* rpn_op_sys_ping() {{{ */

/******************************************************************************

                         RPN_OP_SYS_PING

******************************************************************************/
RPN_DEF_OP(rpn_op_sys_ping)
{
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     RPN_INTERNAL_ERROR;

     return _retcode;
}
/* rpn_op_sys_ping() }}} */
/* rpn_op_sys_iperf() {{{ */

/******************************************************************************

                         RPN_OP_SYS_IPERF

******************************************************************************/
RPN_DEF_OP(rpn_op_sys_iperf)
{
     int                       _retcode, _X_type;
     rpn_elt                  *_stk_x, *_stk_result;
     char                     *_options_a, *_options_b;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
// printf("&_stk_x = %p, _stk_x = %p\n", &_stk_x, _stk_x);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_NIL:
// {{{
          _stk_result              = rpn_new_elt(RPN_TYPE_NIL);
          rpn_set_lastx(stack, _stk_x);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          {
               struct rpn_matrix        *_mat_A, *_mat_C;
               int                       _n, _p, _i, _j, _idx, _sum, _size;
               struct rpn_elt           *_elt, *_nil, *_elt_clone;
               struct rpn_stack         *_stack;

               rpn_set_lastx(stack, _stk_x);

               _mat_A                   = (struct rpn_matrix *) _stk_x->value.obj;

               _n                       = _mat_A->n;
               _p                       = _mat_A->p;


               _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
               _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
               _mat_C                   = (struct rpn_matrix *) RPN_MALLOC(_size);
               _mat_C->n                = _n;
               _mat_C->p                = _p;
               _stk_result->value.obj   = _mat_C;

               _stack                   = rpn_new_stack(__func__);

               for (_i = 1; _i <= _n; _i++) {
                    for (_j = 1; _j <= _p; _j++) {
                         _elt                = rpn_mat_get_elt_ref(_mat_A, _i, _j);
                         _elt_clone          = rpn_clone_elt(_elt);
                         rpn_push(_stack, _elt_clone);

                         if ((_retcode = rpn_op_sys_iperf(_stack, op)) != 0) {
                              rpn_err_msg_op_error(op->op_name, _retcode);
                              exit(RPN_EXIT_OP_ERROR);
                         }

                         _elt                = rpn_pop(_stack);
// printf("LOOP : [%d, %d] : stack num = %d\n", _i, _j, _stack->num_elts);
                         _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
                         _mat_C->base[_idx]  = (void *) _elt;
                    }
               }

               if ((_mat_C->n == 1)
               &&  (_mat_C->p == 1)) {
                    _stk_result              = rpn_mat_get_elt_ref(_mat_C, 1, 1);
               }

               rpn_free_stack(_stack);
          }
          break;

// }}}
     case RPN_TYPE_TUPLE:
// {{{
          RPN_INTERNAL_ERROR;
          break;

// }}}
     case RPN_TYPE_OPAIR:
// {{{
          {
               rpn_elt                  *_elt_a, *_elt_b, *_elt_srv_a, *_elt_srv_b, *_elt_list;
               rpn_pair                 *_pair;
               rpn_list                 *_a_list, *_b_list, *_result_list;
               int                       _a_type, _b_type, _a_srv_type, _b_srv_type,
                                         _lg_a, _lg_b, _pid_a, _pid_b, _status_a, _status_b;
               char                     *_server_a, *_server_b, *_cmd_a, *_cmd_b;
               char                     *_argp_a[64], *_argp_b[64];   // XXX Replace constant with mnemonic
               rpn_elt                  *_elt_res_srv_a, *_elt_res_srv_b, *_elt_res_pair, *_begin;
               rpn_pair                 *_res_pair;
               int                       _pipe[2], _i;

// printf("===== OPAIR =================================================\n");
               rpn_set_lastx(stack, _stk_x);

               _pair                    = _stk_x->value.obj;
               _elt_a                   = _pair->elt_a;
               _elt_b                   = _pair->elt_b;

               _a_type                  = rpn_get_type(_elt_a);
               _b_type                  = rpn_get_type(_elt_b);

               /* Server A
                  ~~~~~~~~ */
               switch (_a_type) {
               
               case RPN_TYPE_TUPLE:
// {{{
                    _a_list                  = _elt_a->value.obj;
                    _elt_srv_a               = rpn_list_pop_head(_a_list);
                    _a_srv_type              = rpn_get_type(_elt_srv_a);
                    _elt_res_srv_a           = rpn_clone_elt(_elt_srv_a);

                    switch (_a_srv_type) {

                    case RPN_TYPE_STRING:
// {{{
                         _server_a                = _elt_srv_a->value.s;
                         printf("Server A : \"%s\"\n", _server_a);
                         break;

// }}}
                    default:
// {{{
                         fprintf(stderr, "_a_srv_type = %s !\n", rpn_type_to_string(_a_srv_type));
                         RPN_INTERNAL_ERROR;
                         break;
// }}}
                    }
                    break;

// }}}
               default:
// {{{
                    RPN_INTERNAL_ERROR;
                    break;
// }}}
               }

               /* Server B
                  ~~~~~~~~ */
               switch (_b_type) {

               case RPN_TYPE_TUPLE:
// {{{
                    _b_list                  = _elt_b->value.obj;
                    _elt_srv_b               = rpn_list_pop_head(_b_list);
                    _b_srv_type              = rpn_get_type(_elt_srv_b);
                    _elt_res_srv_b           = rpn_clone_elt(_elt_srv_b);

                    switch (_b_srv_type) {

                    case RPN_TYPE_STRING:
// {{{
                         _server_b                = _elt_srv_b->value.s;
                         printf("Server B : \"%s\"\n", _server_b);
                         break;

// }}}
                    default:
// {{{
                         fprintf(stderr, "_b_srv_type = %s !\n", rpn_type_to_string(_b_srv_type));
                         RPN_INTERNAL_ERROR;
                         break;
// }}}
                    }
                    break;

// }}}
               default:
// {{{
                    RPN_INTERNAL_ERROR;
                    break;
// }}}
               }

               _options_a                    = rpn_gen_command(_elt_a);
               _options_b                    = rpn_gen_command(_elt_b);

               _lg_a                         = strlen(_options_a);
               _lg_b                         = strlen(_options_b);

               _cmd_a                        = malloc(_lg_a + 64);
               _cmd_b                        = malloc(_lg_b + 64);


//             sprintf(_cmd_b, "ssh %s 'iperf -s %s' &", _server_b, _options_a);
//             sprintf(_cmd_a, "ssh %s 'iperf -c %s %s'", _server_a, _server_b, _options_a);

//             printf("     %s\n", _cmd_b);
//             printf("     %s\n", _cmd_a);

//             system(_cmd_b);
//             system(_cmd_a);

               _i                            = 0;
               _argp_b[_i++]                 = "ssh";
               _argp_b[_i++]                 = _server_b;
               sprintf(_cmd_b, "iperf -s %s > /dev/null 2>&1 & echo $!", _options_a);
               _argp_b[_i++]                 = _cmd_b;
               _argp_b[_i++]                 = (char *) 0;

               _i                            = 0;
               _argp_a[_i++]                 = "ssh";
               _argp_a[_i++]                 = _server_a;
               sprintf(_cmd_a, "iperf -c %s %s | tee -a i.out", _server_b, _options_a);        // XXX : tee !!! XXX
               _argp_a[_i++]                 = _cmd_a;
               _argp_a[_i++]                 = (char *) 0;

               _pid_b                        = rpn_exec("/usr/bin/ssh", _argp_b, NULL);

               usleep(500000L);

               if (pipe(_pipe) == -1) {
                    fprintf(stderr, "%s: pipe error !\n", G.progname);
                    exit(RPN_EXIT_PIPE_ERROR);
               }

               _pid_a                        = rpn_exec("/usr/bin/ssh", _argp_a, _pipe);
               close(_pipe[1]);

               if (G.iperf.elt_list == NULL) {
                    G.iperf.elt_list              = rpn_new_elt(RPN_TYPE_LIST);
                    _result_list                  = rpn_new_list((char *)__func__);
                    G.iperf.elt_list->value.obj   = _result_list;
               }
               else {
                    RPN_INTERNAL_ERROR;
               }

               /* Run parser
                  ~~~~~~~~~~ */
               iperf_out_in                  = fdopen(_pipe[0], "r");
//             iperf_out_lex();
               iperf_out_parse();

               waitpid(_pid_a, &_status_a, 0);

               close(_pipe[0]);

               /* Create the resulting list :
                * - start marker
                * - OPAIR(server A, server B)
                * - list(min throughput, max throughput, mean throughput)
                * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

               /* Push start marker on the stack
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_inc_begin(stack);
               _begin                   = rpn_new_elt(RPN_TYPE_START_MARKER);
               stack->new_value         = TRUE;
               rpn_push(stack, _begin);

               _elt_res_pair            = rpn_new_elt(RPN_TYPE_OPAIR);
               _res_pair                = rpn_new_pair("result_servers");
               _res_pair->elt_a         = _elt_res_srv_a;
               _res_pair->elt_b         = _elt_res_srv_b;
               _elt_res_pair->value.obj = _res_pair;
               rpn_push(stack, _elt_res_pair);

               /* Push the list of throughputs on the stack
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               rpn_push(stack, G.iperf.elt_list);
               G.iperf.elt_list              = NULL;
// printf("PAIR before MMM : stack num = %d\n", stack->num_elts);
               if (rpn_get_X_type(stack) == RPN_TYPE_LIST) {
//rpn_disp_elt(stack->top_elt, RPN_DISP_VALUE);
                    if ((_retcode = rpn_op_mmm(stack, RPN_OP_DESC(MMM))) != 0) {
                         rpn_err_msg_op_error(RPN_OP_DESC(MMM)->op_name, _retcode);
                         exit(RPN_EXIT_OP_ERROR);
                    }
//rpn_disp_stk(stack);
//rpn_disp_elt(stack->top_elt, RPN_DISP_VALUE);
               }

               /* Create resulting list
                  ~~~~~~~~~~~~~~~~~~~~~ */
               if ((_retcode = rpn_op_list(stack, RPN_OP_DESC(LIST))) != 0) {
                    rpn_err_msg_op_error(RPN_OP_DESC(LIST)->op_name, _retcode);
                    exit(RPN_EXIT_OP_ERROR);
               }

               _stk_result                   = rpn_pop(stack);
// printf("PAIR after MMM : stack num = %d\n", stack->num_elts);

               RPN_FREE(_cmd_a);
               RPN_FREE(_cmd_b);

               printf("\n");
               printf("\n");
               printf("\n");

               sleep(2);

//             fprintf(stderr, "     ssh %s 'iperf -s %s'\n", _server_b, _options_a);
//             fprintf(stderr, "     ssh %s 'iperf -c %s %s'\n", _server_a, _server_b, _options_a);
          }
          break;

// }}}
     default:
// {{{
          RPN_INTERNAL_ERROR;
          break;
// }}}
     }

     rpn_free_elt(&_stk_x);

     rpn_push(stack, _stk_result);
// printf("END of op_iperf()\n");

end:

     return _retcode;
}
/* rpn_op_sys_iperf() }}} */
/* GROUP : Computer science }}} */

/* Definitions of 7 segments digits in MNIST digits dimensions
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
struct rpn_7_seg_digit             rpn_7_segments[10] = {
     {
/* 7 segments digits {{{ */
/* 7 segments : 0 {{{ */
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
     },
/* 7 segments : 0 }}} */
/* 7 segments : 1 {{{ */
     {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
     },
/* 7 segments : 1 }}} */
/* 7 segments : 2 {{{ */
     {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
     },
/* 7 segments : 2 }}} */
/* 7 segments : 3 {{{ */
     {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
     },
/* 7 segments : 3 }}} */
/* 7 segments : 4 {{{ */
     {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
     },
/* 7 segments : 4 }}} */
/* 7 segments : 5 {{{ */
     {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
     },
/* 7 segments : 5 }}} */
/* 7 segments : 6 {{{ */
     {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
     },
/* 7 segments : 6 }}} */
/* 7 segments : 7 {{{ */
     {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
     },
/* 7 segments : 7 }}} */
/* 7 segments : 8 {{{ */
     {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
     },
/* 7 segments : 8 }}} */
/* 7 segments : 9 {{{ */
     {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 127, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
     }
/* 7 segments : 9 }}} */
};
/* 7 segments digits }}} */
