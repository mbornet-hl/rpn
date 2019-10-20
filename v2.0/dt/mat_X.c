// {{{
               {
                    struct rpn_matrix        *_mat_B, *_mat_C;
                    int                       _n, _p, _i, _j, _idx, _size;
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
                              _elt      = dt_mat_extract_elt(_mat_B, _i, _j);
                              rpn_push(_stack, _elt);

                              _scalar   = rpn_clone_elt(_stk_x);
                              rpn_push(_stack, _scalar);

                              if ((_retcode = (*op->func)(_stack, op)) != 0) {
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

