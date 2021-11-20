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
 *   @(#)  [MB] cy_rpn_custom.c Version 1.106 du 21/11/20 - 
 */

#include  <stdio.h>
#include  <fcntl.h>
#include  "cy_rpn_header.h"
#include  "cy_epri.h"
#include  "cy_rpn_proto.h"
#include	"../ci/ci_cpub.h"
#include  "../df/df_epub.h"
#define   _GNU_SOURCE
#include  <math.h>
#include  <sys/stat.h>
#include  <sys/types.h>
#include  <errno.h>
#include  <unistd.h>
#include  <libgen.h>

/* Operators descriptors tree
   ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
struct ci_root			rpn_ops_tree			 = { 0 };

/* Operators descriptors
   ~~~~~~~~~~~~~~~~~~~~~ */
struct rpn_operator      rpn_operators[]           = {
#if 0
     // Tuples {{{
     {    RPN_OP(TUPLE),                     2,     1, TRUE,  rpn_op_tuples,         NULL                        },
     {    RPN_OP(NTUPLE),                   -1,     1, TRUE,  rpn_op_tuples,         NULL                        },
     {    RPN_OP(XTUPLE),                   -1,     1, TRUE,  rpn_op_tuples,         NULL                        },

     // }}}
#endif
#if 0
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
#endif
#if 0
     // Computer science tools {{{
     {    RPN_OP(CS_GEN_C),                  1,     1, TRUE,  rpn_op_cs_gen_C,       NULL                        },

     // }}}
#endif
#if 0
     // Types {{{
     {    RPN_OP(TYPE_FILENAME),             1,     1, TRUE,  rpn_op_generic1,       op_types_type               },

     // }}}
#endif
     // END OF OPERATORS DESCRIPTORS {{{
     {    RPN_OP(NULL),                      0,     0, FALSE, 0,                     0                           }
     // }}}
};

/* rpn_new_list() {{{ */

/******************************************************************************

                         RPN_NEW_LIST

******************************************************************************/
rpn_list *rpn_new_list(char *name)
{
     rpn_list                      *_list;

     if ((_list = (rpn_list *) RPN_MALLOC(sizeof(*_list))) == 0) {
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

     if ((_pair = (rpn_pair *) RPN_MALLOC(sizeof(*_pair))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }
     bzero(_pair, sizeof(*_pair));

     _pair->name                   = strdup(name);

     return _pair;
}

/* rpn_new_pair() }}} */
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
#if 0
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

//end:
     return _retcode;
}

/* rpn_op_write() }}} */
#endif
#if 0
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
                    int                       _n, _p, _i, _j, _idx;
                    FILE                     *_fp;
                    char                     *_filename;
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

//end:
     return _retcode;
}

/* rpn_op_write_csv() }}} */
#endif
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
          fprintf(fp, "%15.5e", elt->value.d);
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
//               int                  _n, _p, _i, _j, _idx;
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
               rpn_pair            *_pair;
#if 0
               rpn_list            *_list_a, *_list_b;
               char                *_str_a, *_str_b;
               int                  _a_type, _b_type;
               char                *_a_str_type, *_b_str_type;
#endif	/* 0 */
               rpn_elt             *_elt_a, *_elt_b;

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
#endif	/* 0 */
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
#if 0
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
#endif
/* GROUP : Computer science }}} */

