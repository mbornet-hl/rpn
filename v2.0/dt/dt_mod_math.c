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
 *   @(#)  [MB] dt_mod_math.c Version 1.18 du 21/11/30 - 
 */

#include  <math.h>
#include  "../cc/cc_types.h"
#include  "../cy/cy_rpn_header.h"
#include  "../cy/cy_rpn_proto.h"
#include  "../cy/cy_rpn_epub.h"
#include  "../ci/ci_cpub.h"
#include  "../dl/dl_cpub.h"
#include  "../dl/dl_epub.h"
#include  "dt_serial.h"
#include  "dt_cpub.h"
#include  "dt_epub.h"
#include	"dt_epri.h"

/* Methods
   ~~~~~~~ */
RPN_DEFN_METHODS(dt);
   
/* List of types managed by the module
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static int                               dt_managed_types[] = {
     RPN_TYPE_MATRIX,
     0
};

/* Module description {{{ */
static char						*dt_module_label[] = {
	"Mathematical functions",
	"Fonctions mathematiques",
	0
};

/* Module description }}} */
/* Help messages {{{ */
char                               *dt_help_add[] = {
     "Add X to Y",
     0
},
                                   *dt_help_add_mat[] = {
     "Hadamard : add matrix X to matrix Y",
     0
},
                                   *dt_help_cross_product[] = {
     "Cross product Y ^ X",
     0
},
                                   *dt_help_sub[] = {
     "Substract X from Y",
     0
},
                                   *dt_help_sub_mat[] = {
     "Hadamard : substract matrix X from matrix Y",
     0
},
                                   *dt_help_math_mul[] = {
     "Multiply Y by X",
     0
},
                                   *dt_help_math_mul_mat[] = {
     "Multiply matrix Y by matrix X",
     0
},
                                   *dt_help_mul[] = {
     "Multiply Y by X",
     0
},
                                   *dt_help_mul_mat[] = {
     "Hadamard : multiply matrix Y by matrix X",
     0
},
                                   *dt_help_div[] = {
     "Divide Y by X",
     0
},
                                   *dt_help_div_mat[] = {
     "Hadamard : divide matrix Y by matrix X",
     0
},
                                   *dt_help_diagmat[] = {
     "Create an X x X matrix, Z on the diagonal, Y elsewhere",
     0
},
							*dt_help_percent[] = {
	"Replace X with X percent of Y",
	0
},
							*dt_help_percent_mat_mat[] = {
	"Hadamard : replace X with X percent of Y",
	0
},
							*dt_help_sine[] = {
	"Sine of X",
	0
},
							*dt_help_sine_mat[] = {
	"Sine of elements of X",
	0
},
							*dt_help_cosine[] = {
	"Cosine of X",
	0
},
							*dt_help_cosine_mat[] = {
	"Cosine of elements of X",
	0
},
							*dt_help_tangent[] = {
	"Tangent of X",
	0
},
							*dt_help_tangent_mat[] = {
	"Tangent of elements of X",
	0
},
							*dt_help_arcsine[] = {
	"Arc sine of X",
	0
},
							*dt_help_arcsine_mat[] = {
	"Arc sine of elements of X",
	0
},
							*dt_help_arccosine[] = {
	"Arc cosine of X",
	0
},
							*dt_help_arccosine_mat[] = {
	"Arc cosine of elements of X",
	0
},
							*dt_help_arctangent[] = {
	"Arc tangent of X",
	0
},
							*dt_help_arctangent_mat[] = {
	"Arc tangent of elements of X",
	0
},
							*dt_help_sinh[] = {
	"Hyperbolic sine of X",
	0
},
							*dt_help_sinh_mat[] = {
	"Hyperbolic sine elements of of X",
	0
},
							*dt_help_cosh[] = {
	"Hyperbolic cosine of X",
	0
},
							*dt_help_cosh_mat[] = {
	"Hyperbolic cosine of elements of X",
	0
},
							*dt_help_tanh[] = {
	"Hyperbolic tangent of X",
	0
},
							*dt_help_tanh_mat[] = {
	"Hyperbolic tangent of elements of X",
	0
},
							*dt_help_asinh[] = {
	"Hyperbolic arc sine of X",
	0
},
							*dt_help_asinh_mat[] = {
	"Hyperbolic arc sine of elements of X",
	0
},
							*dt_help_acosh[] = {
	"Hyperbolic arc cosine of X",
	0
},
							*dt_help_acosh_mat[] = {
	"Hyperbolic arc cosine of elements of X",
	0
},
							*dt_help_atanh[] = {
	"Hyperbolic arc tangent of X",
	0
},
							*dt_help_atanh_mat[] = {
	"Hyperbolic arc tangent of elements of X",
	0
},
							*dt_help_matrix[] = {
	"Create a Y x X matrix with stack elements",
	0
},
							*dt_help_delta_percent[] = {
	"Percent change between X and Y",
	0
},
							*dt_help_delta_percent_mat_mat[] = {
	"Hadamard : percent change between X and Y",
	0
},
							*dt_help_dim[] = {
	"Dimensions of X",
	0
},
							*dt_help_exp[] = {
	"Exponential of X",
	0
},
							*dt_help_explode[] = {
	"Explode : extract elements of X",
	0
},
							*dt_help_hcat[] = {
	"Horizontal contatenation",
	0
},
							*dt_help_hrev[] = {
	"Horizontal reverse : reverse lines",
	0
},
							*dt_help_ln[] = {
	"Logarithm of X",
	0
},
							*dt_help_log[] = {
	"Logarithm, base 10, of X",
	0
},
							*dt_help_reciprocal[] = {
	"Reciprocal of X",
	0
},
							*dt_help_reciprocal_mat[] = {
	"Reciprocal of elements of X",
	0
},
							*dt_help_seq[] = {
	"Create a sequence of numbers between Y and X",
	0
},
							*dt_help_sigmoid[] = {
	"Sigmoid of X (1/(1 + e^-X))",
	0
},
							*dt_help_sigmoid_mat[] = {
	"Sigmoid of elements of X (1/(1 + e^-X))",
	0
},
							*dt_help_square[] = {
	"Square of X",
	0
},
							*dt_help_square_mat[] = {
	"Square of elements of X",
	0
},
							*dt_help_sqrt[] = {
	"Square root of X",
	0
},
							*dt_help_sqrt_mat[] = {
	"Square root of elements of X",
	0
},
							*dt_help_power_x[] = {
	"Y to the power of X",
	0
},
							*dt_help_10_power_x[] = {
	"10 to the power of X",
	0
},
							*dt_help_10_power_x_mat[] = {
	"10 to the power of elements of X",
	0
},
							*dt_help_power_x_mat_mat[] = {
	"Hadamard : Y to the power of X",
	0
},
							*dt_help_transpose[] = {
	"Transpose matrix in X",
	0
},
							*dt_help_vcat[] = {
	"Vertical concatenation",
	0
},
							*dt_help_vec2[] = {
	"Create a 2-dimensional vector with Y and X",
	0
},
							*dt_help_vec3[] = {
	"Create a 3-dimensional vector with Z, Y and X",
	0
},
							*dt_help_vrev[] = {
	"Vertical reverse : reverse columns",
	0
},
							*dt_help_zmat[] = {
	"Create a Y x X matrix filled with Z",
	0
};

/* Help messages }}} */
/* Module descriptor {{{ */
struct dl_module         math_module = {
     "math",
     "2.0",
     DT_LINK_ID,
     0,
     dt_ops_array,
	dt_init,
	dt_module_label
};

/* Module descriptor }}} */
/* Operator parameters descriptions {{{ */
static dl_op_params                      dt_params_add[] = {
     DL_OP_DEF2H(dt_op_math_add, 1, INT,      INT,      dt_help_add),
     DL_OP_DEF2H(dt_op_math_add, 1, INT,      DOUBLE,   dt_help_add),
     DL_OP_DEF2H(dt_op_math_add, 1, INT,      MATRIX,   dt_help_add),
     DL_OP_DEF2H(dt_op_math_add, 1, INT,      LITTERAL, dt_help_add),
     DL_OP_DEF2H(dt_op_math_add, 1, DOUBLE,   INT,      dt_help_add),
     DL_OP_DEF2H(dt_op_math_add, 1, DOUBLE,   DOUBLE,   dt_help_add),
     DL_OP_DEF2H(dt_op_math_add, 1, DOUBLE,   MATRIX,   dt_help_add),
     DL_OP_DEF2H(dt_op_math_add, 1, MATRIX,   INT,      dt_help_add),
     DL_OP_DEF2H(dt_op_math_add, 1, MATRIX,   DOUBLE,   dt_help_add),
     DL_OP_DEF2H(dt_op_math_add, 1, MATRIX,   MATRIX,   dt_help_add_mat),
     DL_OP_DEF2H(dt_op_math_add, 1, LITTERAL, INT,      dt_help_add),
     DL_OP_DEF2H(dt_op_math_add, 1, LITTERAL, LITTERAL, dt_help_add),
     DL_OP_DEF2H(dt_op_math_add, 1, LITTERAL, MATRIX,   dt_help_add),
     DL_OP_DEF2H(dt_op_math_add, 1, MATRIX,   LITTERAL, dt_help_add),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_sub[] = {
     DL_OP_DEF2H(dt_op_math_sub, 1, INT,      INT,      dt_help_sub),
     DL_OP_DEF2H(dt_op_math_sub, 1, INT,      DOUBLE,   dt_help_sub),
     DL_OP_DEF2H(dt_op_math_sub, 1, INT,      MATRIX,   dt_help_sub),
     DL_OP_DEF2H(dt_op_math_sub, 1, DOUBLE,   INT,      dt_help_sub),
     DL_OP_DEF2H(dt_op_math_sub, 1, DOUBLE,   DOUBLE,   dt_help_sub),
     DL_OP_DEF2H(dt_op_math_sub, 1, DOUBLE,   MATRIX,   dt_help_sub),
     DL_OP_DEF2H(dt_op_math_sub, 1, MATRIX,   INT,      dt_help_sub),
     DL_OP_DEF2H(dt_op_math_sub, 1, MATRIX,   DOUBLE,   dt_help_sub),
     DL_OP_DEF2H(dt_op_math_sub, 1, MATRIX,   MATRIX,   dt_help_sub_mat),
     DL_OP_DEF2H(dt_op_math_sub, 1, LITTERAL, LITTERAL, dt_help_sub),
     DL_OP_DEF2H(dt_op_math_sub, 1, LITTERAL, MATRIX,   dt_help_sub),
     DL_OP_DEF2H(dt_op_math_sub, 1, MATRIX,   LITTERAL, dt_help_sub),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_mul[] = {
     DL_OP_DEF2H(dt_op_math_mul, 1, INT,      INT,      dt_help_mul),
     DL_OP_DEF2H(dt_op_math_mul, 1, INT,      DOUBLE,   dt_help_mul),
     DL_OP_DEF2H(dt_op_math_mul, 1, INT,      MATRIX,   dt_help_mul),
     DL_OP_DEF2H(dt_op_math_mul, 1, DOUBLE,   INT,      dt_help_mul),
     DL_OP_DEF2H(dt_op_math_mul, 1, DOUBLE,   DOUBLE,   dt_help_mul),
     DL_OP_DEF2H(dt_op_math_mul, 1, DOUBLE,   MATRIX,   dt_help_mul),
     DL_OP_DEF2H(dt_op_math_mul, 1, MATRIX,   INT,      dt_help_mul),
     DL_OP_DEF2H(dt_op_math_mul, 1, MATRIX,   DOUBLE,   dt_help_mul),
     DL_OP_DEF2H(dt_op_math_mul, 1, MATRIX,   MATRIX,   dt_help_mul_mat),
     DL_OP_DEF2H(dt_op_math_mul, 1, LITTERAL, LITTERAL, dt_help_mul),
     DL_OP_DEF2H(dt_op_math_mul, 1, LITTERAL, MATRIX,   dt_help_mul),
     DL_OP_DEF2H(dt_op_math_mul, 1, MATRIX,   LITTERAL, dt_help_mul),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_div[] = {
     DL_OP_DEF2H(dt_op_math_div, 1, INT,      INT,      dt_help_div),
     DL_OP_DEF2H(dt_op_math_div, 1, INT,      DOUBLE,   dt_help_div),
     DL_OP_DEF2H(dt_op_math_div, 1, INT,      MATRIX,   dt_help_div),
     DL_OP_DEF2H(dt_op_math_div, 1, DOUBLE,   INT,      dt_help_div),
     DL_OP_DEF2H(dt_op_math_div, 1, DOUBLE,   DOUBLE,   dt_help_div),
     DL_OP_DEF2H(dt_op_math_div, 1, DOUBLE,   MATRIX,   dt_help_div),
     DL_OP_DEF2H(dt_op_math_div, 1, MATRIX,   INT,      dt_help_div),
     DL_OP_DEF2H(dt_op_math_div, 1, MATRIX,   DOUBLE,   dt_help_div),
     DL_OP_DEF2H(dt_op_math_div, 1, MATRIX,   MATRIX,   dt_help_div_mat),
     DL_OP_DEF2H(dt_op_math_div, 1, LITTERAL, LITTERAL, dt_help_div),
     DL_OP_DEF2H(dt_op_math_div, 1, LITTERAL, MATRIX,   dt_help_div),
     DL_OP_DEF2H(dt_op_math_div, 1, MATRIX,   LITTERAL, dt_help_div),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_math_mul[] = {
     DL_OP_DEF2H(dt_op_math_math_mul, 1, INT,    INT,    dt_help_math_mul),
     DL_OP_DEF2H(dt_op_math_math_mul, 1, INT,    DOUBLE, dt_help_math_mul),
     DL_OP_DEF2H(dt_op_math_math_mul, 1, INT,    MATRIX, dt_help_math_mul),
     DL_OP_DEF2H(dt_op_math_math_mul, 1, DOUBLE, INT,    dt_help_math_mul),
     DL_OP_DEF2H(dt_op_math_math_mul, 1, DOUBLE, DOUBLE, dt_help_math_mul),
     DL_OP_DEF2H(dt_op_math_math_mul, 1, DOUBLE, MATRIX, dt_help_math_mul),
     DL_OP_DEF2H(dt_op_math_math_mul, 1, MATRIX, MATRIX, dt_help_math_mul_mat),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_reciprocal[] = {
     DL_OP_DEF1H(dt_op_math_reciprocal, 1, INT,      dt_help_reciprocal),
     DL_OP_DEF1H(dt_op_math_reciprocal, 1, DOUBLE,   dt_help_reciprocal),
     DL_OP_DEF1H(dt_op_math_reciprocal, 1, MATRIX,   dt_help_reciprocal_mat),
     DL_OP_DEF1H(dt_op_math_reciprocal, 1, LITTERAL, dt_help_reciprocal),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_power[] = {
     DL_OP_DEF2H(dt_op_math_power, 1, INT,    INT,    dt_help_power_x),
     DL_OP_DEF2H(dt_op_math_power, 1, INT,    DOUBLE, dt_help_power_x),
     DL_OP_DEF2H(dt_op_math_power, 1, INT,    MATRIX, dt_help_power_x),
     DL_OP_DEF2H(dt_op_math_power, 1, DOUBLE, INT,    dt_help_power_x),
     DL_OP_DEF2H(dt_op_math_power, 1, DOUBLE, DOUBLE, dt_help_power_x),
     DL_OP_DEF2H(dt_op_math_power, 1, DOUBLE, MATRIX, dt_help_power_x),
     DL_OP_DEF2H(dt_op_math_power, 1, MATRIX, INT,    dt_help_power_x),
     DL_OP_DEF2H(dt_op_math_power, 1, MATRIX, DOUBLE, dt_help_power_x),
     DL_OP_DEF2H(dt_op_math_power, 1, MATRIX, MATRIX, dt_help_power_x_mat_mat),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_sqrt[] = {
     DL_OP_DEF1H(dt_op_math_sqrt, 1, INT,      dt_help_sqrt),
     DL_OP_DEF1H(dt_op_math_sqrt, 1, DOUBLE,   dt_help_sqrt),
     DL_OP_DEF1H(dt_op_math_sqrt, 1, MATRIX,   dt_help_sqrt_mat),
     DL_OP_DEF1H(dt_op_math_sqrt, 1, LITTERAL, dt_help_sqrt),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_square[] = {
     DL_OP_DEF1H(dt_op_math_square, 1, INT,      dt_help_square),
     DL_OP_DEF1H(dt_op_math_square, 1, DOUBLE,   dt_help_square),
     DL_OP_DEF1H(dt_op_math_square, 1, MATRIX,   dt_help_square_mat),
     DL_OP_DEF1H(dt_op_math_square, 1, LITTERAL, dt_help_square),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_ln[] = {
     DL_OP_DEF1H(dt_op_math_ln, 1, INT,      dt_help_ln),
     DL_OP_DEF1H(dt_op_math_ln, 1, DOUBLE,   dt_help_ln),
     DL_OP_DEF1H(dt_op_math_ln, 1, MATRIX,   dt_help_ln),
     DL_OP_DEF1H(dt_op_math_ln, 1, LITTERAL, dt_help_ln),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_exp[] = {
     DL_OP_DEF1H(dt_op_math_exp, 1, INT,      dt_help_exp),
     DL_OP_DEF1H(dt_op_math_exp, 1, DOUBLE,   dt_help_exp),
     DL_OP_DEF1H(dt_op_math_exp, 1, MATRIX,   dt_help_exp),
     DL_OP_DEF1H(dt_op_math_exp, 1, LITTERAL, dt_help_exp),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_log[] = {
     DL_OP_DEF1H(dt_op_math_log, 1, INT,      dt_help_log),
     DL_OP_DEF1H(dt_op_math_log, 1, DOUBLE,   dt_help_log),
     DL_OP_DEF1H(dt_op_math_log, 1, MATRIX,   dt_help_log),
     DL_OP_DEF1H(dt_op_math_log, 1, LITTERAL, dt_help_log),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_10_power_x[] = {
     DL_OP_DEF1H(dt_op_math_10_power_x, 1, INT,      dt_help_10_power_x),
     DL_OP_DEF1H(dt_op_math_10_power_x, 1, DOUBLE,   dt_help_10_power_x),
     DL_OP_DEF1H(dt_op_math_10_power_x, 1, MATRIX,   dt_help_10_power_x_mat),
     DL_OP_DEF1H(dt_op_math_10_power_x, 1, LITTERAL, dt_help_10_power_x),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_percent[] = {
     DL_OP_DEF2H(dt_op_math_percent, 1, INT, INT,       dt_help_percent),
     DL_OP_DEF2H(dt_op_math_percent, 1, INT, DOUBLE,    dt_help_percent),
     DL_OP_DEF2H(dt_op_math_percent, 1, INT, MATRIX,    dt_help_percent),
     DL_OP_DEF2H(dt_op_math_percent, 1, DOUBLE, INT,    dt_help_percent),
     DL_OP_DEF2H(dt_op_math_percent, 1, DOUBLE, DOUBLE, dt_help_percent),
     DL_OP_DEF2H(dt_op_math_percent, 1, DOUBLE, MATRIX, dt_help_percent),
     DL_OP_DEF2H(dt_op_math_percent, 1, MATRIX, INT,    dt_help_percent),
     DL_OP_DEF2H(dt_op_math_percent, 1, MATRIX, DOUBLE, dt_help_percent),
     DL_OP_DEF2H(dt_op_math_percent, 1, MATRIX, MATRIX, dt_help_percent_mat_mat),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_delta_percent[] = {
     DL_OP_DEF2H(dt_op_math_delta_percent, 1, INT, INT,       dt_help_delta_percent),
     DL_OP_DEF2H(dt_op_math_delta_percent, 1, INT, DOUBLE,    dt_help_delta_percent),
     DL_OP_DEF2H(dt_op_math_delta_percent, 1, INT, MATRIX,    dt_help_delta_percent),
     DL_OP_DEF2H(dt_op_math_delta_percent, 1, DOUBLE, INT,    dt_help_delta_percent),
     DL_OP_DEF2H(dt_op_math_delta_percent, 1, DOUBLE, DOUBLE, dt_help_delta_percent),
     DL_OP_DEF2H(dt_op_math_delta_percent, 1, DOUBLE, MATRIX, dt_help_delta_percent),
     DL_OP_DEF2H(dt_op_math_delta_percent, 1, MATRIX, INT,    dt_help_delta_percent),
     DL_OP_DEF2H(dt_op_math_delta_percent, 1, MATRIX, DOUBLE, dt_help_delta_percent),
     DL_OP_DEF2H(dt_op_math_delta_percent, 1, MATRIX, MATRIX, dt_help_delta_percent_mat_mat),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_sine[] = {
     DL_OP_DEF1H(dt_op_math_sin, 1, INT,      dt_help_sine),
     DL_OP_DEF1H(dt_op_math_sin, 1, DOUBLE,   dt_help_sine),
     DL_OP_DEF1H(dt_op_math_sin, 1, MATRIX,   dt_help_sine_mat),
     DL_OP_DEF1H(dt_op_math_sin, 1, LITTERAL, dt_help_sine),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_cosine[] = {
     DL_OP_DEF1H(dt_op_math_cos, 1, INT,      dt_help_cosine),
     DL_OP_DEF1H(dt_op_math_cos, 1, DOUBLE,   dt_help_cosine),
     DL_OP_DEF1H(dt_op_math_cos, 1, MATRIX,   dt_help_cosine_mat),
     DL_OP_DEF1H(dt_op_math_cos, 1, LITTERAL, dt_help_cosine),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_tangent[] = {
     DL_OP_DEF1H(dt_op_math_tan, 1, INT,      dt_help_tangent),
     DL_OP_DEF1H(dt_op_math_tan, 1, DOUBLE,   dt_help_tangent),
     DL_OP_DEF1H(dt_op_math_tan, 1, MATRIX,   dt_help_tangent_mat),
     DL_OP_DEF1H(dt_op_math_tan, 1, LITTERAL, dt_help_tangent),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_asine[] = {
     DL_OP_DEF1H(dt_op_math_asin, 1, INT,      dt_help_arcsine),
     DL_OP_DEF1H(dt_op_math_asin, 1, DOUBLE,   dt_help_arcsine),
     DL_OP_DEF1H(dt_op_math_asin, 1, MATRIX,   dt_help_arcsine_mat),
     DL_OP_DEF1H(dt_op_math_asin, 1, LITTERAL, dt_help_arcsine),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_acosine[] = {
     DL_OP_DEF1H(dt_op_math_acos, 1, INT,      dt_help_arccosine),
     DL_OP_DEF1H(dt_op_math_acos, 1, DOUBLE,   dt_help_arccosine),
     DL_OP_DEF1H(dt_op_math_acos, 1, MATRIX,   dt_help_arccosine_mat),
     DL_OP_DEF1H(dt_op_math_acos, 1, LITTERAL, dt_help_arccosine),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_atangent[] = {
     DL_OP_DEF1H(dt_op_math_atan, 1, INT,      dt_help_arctangent),
     DL_OP_DEF1H(dt_op_math_atan, 1, DOUBLE,   dt_help_arctangent),
     DL_OP_DEF1H(dt_op_math_atan, 1, MATRIX,   dt_help_arctangent_mat),
     DL_OP_DEF1H(dt_op_math_atan, 1, LITTERAL, dt_help_arctangent),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_sinh[] = {
     DL_OP_DEF1H(dt_op_math_sinh, 1, INT,      dt_help_sinh),
     DL_OP_DEF1H(dt_op_math_sinh, 1, DOUBLE,   dt_help_sinh),
     DL_OP_DEF1H(dt_op_math_sinh, 1, MATRIX,   dt_help_sinh_mat),
     DL_OP_DEF1H(dt_op_math_sinh, 1, LITTERAL, dt_help_sinh),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_cosh[] = {
     DL_OP_DEF1H(dt_op_math_cosh, 1, INT,      dt_help_cosh),
     DL_OP_DEF1H(dt_op_math_cosh, 1, DOUBLE,   dt_help_cosh),
     DL_OP_DEF1H(dt_op_math_cosh, 1, MATRIX,   dt_help_cosh_mat),
     DL_OP_DEF1H(dt_op_math_cosh, 1, LITTERAL, dt_help_cosh),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_tanh[] = {
     DL_OP_DEF1H(dt_op_math_tanh, 1, INT,      dt_help_tanh),
     DL_OP_DEF1H(dt_op_math_tanh, 1, DOUBLE,   dt_help_tanh),
     DL_OP_DEF1H(dt_op_math_tanh, 1, MATRIX,   dt_help_tanh_mat),
     DL_OP_DEF1H(dt_op_math_tanh, 1, LITTERAL, dt_help_tanh),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_asinh[] = {
     DL_OP_DEF1H(dt_op_math_asinh, 1, INT,      dt_help_asinh),
     DL_OP_DEF1H(dt_op_math_asinh, 1, DOUBLE,   dt_help_asinh),
     DL_OP_DEF1H(dt_op_math_asinh, 1, MATRIX,   dt_help_asinh_mat),
     DL_OP_DEF1H(dt_op_math_asinh, 1, LITTERAL, dt_help_asinh),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_acosh[] = {
     DL_OP_DEF1H(dt_op_math_acosh, 1, INT,      dt_help_acosh),
     DL_OP_DEF1H(dt_op_math_acosh, 1, DOUBLE,   dt_help_acosh),
     DL_OP_DEF1H(dt_op_math_acosh, 1, MATRIX,   dt_help_acosh_mat),
     DL_OP_DEF1H(dt_op_math_acosh, 1, LITTERAL, dt_help_acosh),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_atanh[] = {
     DL_OP_DEF1H(dt_op_math_atanh, 1, INT,      dt_help_atanh),
     DL_OP_DEF1H(dt_op_math_atanh, 1, DOUBLE,   dt_help_atanh),
     DL_OP_DEF1H(dt_op_math_atanh, 1, MATRIX,   dt_help_atanh_mat),
     DL_OP_DEF1H(dt_op_math_atanh, 1, LITTERAL, dt_help_atanh),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_sigmoid[] = {
     DL_OP_DEF1H(dt_op_math_sigmoid, 1, INT,      dt_help_sigmoid),
     DL_OP_DEF1H(dt_op_math_sigmoid, 1, DOUBLE,   dt_help_sigmoid),
     DL_OP_DEF1H(dt_op_math_sigmoid, 1, MATRIX,   dt_help_sigmoid_mat),
     DL_OP_DEF1H(dt_op_math_sigmoid, 1, LITTERAL, dt_help_sigmoid),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_seq[] = {
     DL_OP_DEF2H(dt_op_math_seq, 1, INT, INT, dt_help_seq),
//     DL_OP_DEF1(dt_op_math_seq, 1, MIN_MAX),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_seq2[] = {
     DL_OP_DEF3(dt_op_math_seq2, 1, MIN_MAX, MIN_MAX, STRING),
     DL_OP_DEF3(dt_op_math_seq2, 1, MIN_MAX, MIN_MAX, LITTERAL),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_dim[] = {
     DL_OP_DEF1H(dt_op_math_dim, 1, NIL,    dt_help_dim),
     DL_OP_DEF1H(dt_op_math_dim, 1, INT,    dt_help_dim),
     DL_OP_DEF1H(dt_op_math_dim, 1, DOUBLE, dt_help_dim),
     DL_OP_DEF1H(dt_op_math_dim, 1, MATRIX, dt_help_dim),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_matrix[] = {
     DL_OP_DEF4H(dt_op_math_matrix, 1, INT, INT, ANY_TYPE, ANY_TYPE, dt_help_matrix),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_explode[] = {
     DL_OP_DEF1H(dt_op_math_explode, 1, MATRIX, dt_help_explode),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_vec2[] = {
     DL_OP_DEF2H(dt_op_math_vec2, 1, INT,    INT,    dt_help_vec2),
     DL_OP_DEF2H(dt_op_math_vec2, 1, DOUBLE, INT,    dt_help_vec2),
     DL_OP_DEF2H(dt_op_math_vec2, 1, INT,    DOUBLE, dt_help_vec2),
     DL_OP_DEF2H(dt_op_math_vec2, 1, DOUBLE, DOUBLE, dt_help_vec2),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_vec3[] = {
     DL_OP_DEF3H(dt_op_math_vec3, 1, INT, INT, INT, dt_help_vec3),
     DL_OP_DEF3H(dt_op_math_vec3, 1, INT, INT, DOUBLE, dt_help_vec3),
     DL_OP_DEF3H(dt_op_math_vec3, 1, INT, DOUBLE, INT, dt_help_vec3),
     DL_OP_DEF3H(dt_op_math_vec3, 1, INT, DOUBLE, DOUBLE, dt_help_vec3),
     DL_OP_DEF3H(dt_op_math_vec3, 1, DOUBLE, INT, INT, dt_help_vec3),
     DL_OP_DEF3H(dt_op_math_vec3, 1, DOUBLE, INT, DOUBLE, dt_help_vec3),
     DL_OP_DEF3H(dt_op_math_vec3, 1, DOUBLE, DOUBLE, INT, dt_help_vec3),
     DL_OP_DEF3H(dt_op_math_vec3, 1, DOUBLE, DOUBLE, DOUBLE, dt_help_vec3),
     DL_OP_DEF3H(dt_op_math_vec3, 1, LITTERAL, LITTERAL, LITTERAL, dt_help_vec3),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_transpose[] = {
     DL_OP_DEF1H(dt_op_math_transpose, 1, MATRIX, dt_help_transpose),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_cross_product[] = {
     DL_OP_DEF2H(dt_op_math_cross_product, 1, MATRIX, MATRIX, dt_help_cross_product),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_hrev[] = {
     DL_OP_DEF1H(dt_op_math_hrev, 1, MATRIX, dt_help_hrev),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_vrev[] = {
     DL_OP_DEF1H(dt_op_math_vrev, 1, MATRIX, dt_help_vrev),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_diagmat[] = {
     DL_OP_DEF3H(dt_op_math_diagmat, 1, INT, ANY_TYPE, ANY_TYPE, dt_help_diagmat),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_hcat[] = {
     DL_OP_DEF2H(dt_op_math_hcat, 1, MATRIX, MATRIX, dt_help_hcat),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_vcat[] = {
     DL_OP_DEF2H(dt_op_math_vcat, 1, MATRIX, MATRIX, dt_help_vcat),
     DL_OP_DEF_END
};

static dl_op_params                      dt_params_zmat[] = {
     DL_OP_DEF3H(dt_op_math_zmat, 1, INT, INT, ANY_TYPE, dt_help_zmat),
     DL_OP_DEF_END
};

/* Operator parameters descriptions }}} */
/* Operators list {{{ */
static dl_op_desc                        dt_ops_array[] = {
     {    "+",                     dt_params_add                           },
     {    "-",                     dt_params_sub                           },
     {    "*",                     dt_params_mul                           },
     {    "/",                     dt_params_div                           },
     {    "m*",                    dt_params_math_mul                      },
     {    "1/x",                   dt_params_reciprocal                    },
     {    "y^x",                   dt_params_power                         },
     {    "sqrt",                  dt_params_sqrt                          },
     {    "x^2",                   dt_params_square                        },
     {    "ln",                    dt_params_ln                            },
     {    "exp",                   dt_params_exp                           },
     {    "log",                   dt_params_log                           },
     {    "10^x",                  dt_params_10_power_x                    },
     {    "%",                     dt_params_percent                       },
     {    "delta%",                dt_params_delta_percent                 },
     {    "sin",                   dt_params_sine                          },
     {    "cos",                   dt_params_cosine                        },
     {    "tan",                   dt_params_tangent                       },
     {    "asin",                  dt_params_asine                         },
     {    "acos",                  dt_params_acosine                       },
     {    "atan",                  dt_params_atangent                      },
     {    "sinh",                  dt_params_sinh                          },
     {    "cosh",                  dt_params_cosh                          },
     {    "tanh",                  dt_params_tanh                          },
     {    "asinh",                 dt_params_asinh                         },
     {    "acosh",                 dt_params_acosh                         },
     {    "atanh",                 dt_params_atanh                         },
     {    "sigmoid",               dt_params_sigmoid                       },
     {    "mat",                   dt_params_matrix                        },
     {    "expl",                  dt_params_explode                       },
     {    "vec2",                  dt_params_vec2                          },
     {    "vec3",                  dt_params_vec3                          },
     {    "t",                     dt_params_transpose                     },
     {    "^",                     dt_params_cross_product                 },
#if 0
     {    "scalar_matrix",         dt_params_scalar_matrix                 },
#endif
     {    "seq",                   dt_params_seq                           },
     {    "seq2",                  dt_params_seq2                          },
     {    "dim",                   dt_params_dim                           },
     {    "hcat",                  dt_params_hcat                          },
     {    "vcat",                  dt_params_vcat                          },
     {    "zmat",                  dt_params_zmat                          },
     {    "hrev",                  dt_params_hrev                          },
     {    "vrev",                  dt_params_vrev                          },
     {    "diagmat",               dt_params_diagmat                       },
#if 0
     {    "vpad",                  dt_params_vpad                          },
     {    "mat_nil_diag",          dt_params_mat_nil_diag                  },
#endif
     {    0,                       0                                       }
};

/* Operators list }}} */

// GROUP : Math {{{
// dt_init() {{{

/******************************************************************************

					DT_INIT

******************************************************************************/
RPN_DEFN_INIT(dt)

// dt_init() }}}
/* --- Methods {{{ */
/* dt_disp_elt() {{{ */

/******************************************************************************

                         DT_DISP_ELT

******************************************************************************/
RPN_DEFN_DISP(dt)
{
     int                  _type;
     struct rpn_matrix   *_matrix;

     switch (_type = rpn_get_type(elt)) {

	case RPN_TYPE_MATRIX :
		{
			int                  _n, _p, _i, _j, _idx;
			struct rpn_elt      *_elt;

			_matrix        = (struct rpn_matrix *) elt->value.obj;
			_n             = _matrix->n;
			_p             = _matrix->p;
			printf("MATRIX    [%d x %d]", _n, _p);
			if (disp_flags & RPN_DISP_NO_TYPE) {
				printf("\n");
				for (_i = 1; _i <= _n; _i++) {
					printf("|");
					for (_j = 1; _j <= _p; _j++) {
						_idx           = ((_i - 1) * _p) + (_j - 1);
						_elt           = (struct rpn_elt *) _matrix->base[_idx];
						printf(" ");
						rpn_disp_elt(_elt, disp_flags);
					}
					printf(" |\n");
				}
			}
		}
		break;

     default :
          RPN_INTERNAL_ERROR;
          break;
     }
}

/* dt_disp_elt() }}} */
/* dt_clone_elt() {{{ */

/******************************************************************************

                         DT_CLONE_ELT

******************************************************************************/
RPN_DEFN_CLONE(dt)
{
     switch (elt->type) {

     case RPN_TYPE_MATRIX:
//X
          {
               rpn_elt				*_sub_elt, *_clone_sub;
               rpn_matrix			*_mat, *_clone_mat;
               size_t                    _size;
			int					 _idx;

               _mat                	= (rpn_matrix *) elt->value.obj;
               _size               	= sizeof(rpn_matrix)
                                   	+ (((_mat->n *_mat->p) - 1) * sizeof(rpn_elt *));
               clone->value.obj   		= RPN_MALLOC(_size);
               _clone_mat          	= clone->value.obj;
               _clone_mat->n       	= _mat->n;
               _clone_mat->p       	= _mat->p;

               for (_idx = 0; _idx < (_mat->n * _mat->p); _idx++) {
                    _sub_elt                 = (rpn_elt *) _mat->base[_idx];
//X
                    _clone_sub               = rpn_clone_elt(_sub_elt);
//X
                    _clone_mat->base[_idx]   = _clone_sub;
               }
          }
          break;

     default:
          RPN_INTERNAL_ERROR;
          break;
     }
}

/* dt_clone_elt() }}} */
/* dt_type_to_string() {{{ */

/******************************************************************************

                         DT_TYPE_TO_STRING

******************************************************************************/
RPN_DEFN_TYPE_TO_STR(dt)
{
     char                     *_str_type;

     switch (type) {

     case RPN_TYPE_VECTOR_3:
          _str_type      = "VECTOR_3";
          break;

     case RPN_TYPE_MATRIX:
          _str_type      = "MATRIX";
          break;

     default:
          RPN_INTERNAL_ERROR;
          break;
     }

     return _str_type;
}

/* dt_type_to_string() }}} */
#if 0
/* dt_free_name() {{{ */

/******************************************************************************

					DT_FREE_NAME

******************************************************************************/
void dt_free_name(dt_name *name)
{
//fprintf(stderr, "%s(%s)\n", __func__, name->name);
	RPN_FREE(name->name);
	RPN_FREE(name);
}

/* dt_free_name() }}} */
/* dt_free_host() {{{ */

/******************************************************************************

					DT_FREE_HOST

******************************************************************************/
void dt_free_host(dt_host *host)
{
	ci_trek				 _trek;
	ci_node				*_node;
	dt_name				*_name;

//fprintf(stdout, "%s(%s)\n", __func__, host->IP);
	ci_reset(&_trek, &host->names_alphabetical, CI_T_LRN);

	for (_node = ci_get_next(&_trek); _node != 0;
	     _node = ci_get_next(&_trek)) {
		_name					= _node->data;

		dt_free_name(_name);
	}

	RPN_FREE(host->IP);
	RPN_FREE(host);
}

/* dt_free_host() }}} */
/* dt_free_hosts() {{{ */

/******************************************************************************

					DT_FREE_HOSTS

******************************************************************************/
void dt_free_hosts(dt_hosts_tree *hosts)
{
	ci_trek				 _trek;
	ci_node				*_node;
	dt_host				*_host;

//fprintf(stderr, "%s(%s)\n", __func__, hosts->filename);
	ci_reset(&_trek, &hosts->hosts_by_IP, CI_T_LRN);

	for (_node = ci_get_next(&_trek); _node != 0;
	     _node = ci_get_next(&_trek)) {
		_host					= _node->data;

		dt_free_host(_host);
	}

	RPN_FREE(hosts->filename);
	RPN_FREE(hosts);
}

/* dt_free_hosts() }}} */
#endif	/* 0 */
/* dt_free_elt() {{{ */

/******************************************************************************

                         DT_FREE_ELT

******************************************************************************/
RPN_DEFN_FREE(dt)
{
X
     switch (type) {

     case RPN_TYPE_MATRIX:
          {
               rpn_elt				*_sub_elt;
               rpn_matrix			*_mat;
               int                       _idx;

               _mat                = (rpn_matrix *) elt->value.obj;

               for (_idx = 0; _idx < (_mat->n * _mat->p); _idx++) {
                    _sub_elt                 = (rpn_elt *) _mat->base[_idx];

                    if (_sub_elt != NULL) {
                         /* Free sub element
                            ~~~~~~~~~~~~~~~~ */
X
                         rpn_free_elt(&_sub_elt);
                    }
               }

               /* Free matrix
                  ~~~~~~~~~~~ */
X
               RPN_FREE(_mat);
          }

          /* Free element */
X
          RPN_FREE(elt);
          break;

     default:
          RPN_INTERNAL_ERROR;
          break;
     }
}

/* dt_free_elt() }}} */
/* --- Methods }}} */

/* dt_op_math_reciprocal() {{{ */

/******************************************************************************

                         DT_OP_MATH_RECIPROCAL

******************************************************************************/
RPN_DEF_OP(dt_op_math_reciprocal)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = 1.0 / _xd;
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = 1.0 / _xd;
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;
// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_litteral, *_result_litteral;
			char					*_str;

			_litteral				= _stk_x->value.obj;
			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			if (_litteral->need_parentheses) {
				_result_litteral->value	= rpn_str_printf("1/(%s)", _str);
			}
			else {
				_result_litteral->value	= rpn_str_printf("1/%s", _str);
			}

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}

/* dt_op_math_reciprocal() }}} */
/* dt_op_math_power() {{{ */

/******************************************************************************

                         DT_OP_MATH_POWER

******************************************************************************/
RPN_DEF_OP(dt_op_math_power)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _X_type, _Y_type;
     int                       _x, _y, _result;
     double                    _xd, _yd, _resultd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

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
               _result                  = (int) pow((double) _y, (double) _x);

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
               _resultd                 = pow(_yd, _xd);

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			_stk_result			= dt_op_mat_X(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
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
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;

          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               _yd                       = (double) _stk_y->value.i;
               _resultd                 = pow(_yd, _xd);

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_DOUBLE:
// {{{
               _yd                      = _stk_y->value.d;
               _resultd                 = pow(_yd, _xd);

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			_stk_result			= dt_op_mat_X(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
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
     case RPN_TYPE_MATRIX:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_DOUBLE:
// {{{
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			if ((_retcode = dt_mat_check_dim(_stk_y, _stk_x)) == RPN_RET_OK) {
				rpn_set_lastx(stack, _stk_x);
				_stk_result			= dt_op_mat_mat(_stk_y, _stk_x, op);
				rpn_push(stack, _stk_result);
			}
			else {
				rpn_push(stack, _stk_y);
				rpn_push(stack, _stk_x);
				goto end;
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

     rpn_free_elt(&_stk_x);
	rpn_free_elt(&_stk_y);

end:
     return _retcode;
}

/* dt_op_math_power() }}} */
/* dt_op_math_square() {{{ */

/******************************************************************************

                         DT_OP_MATH_SQUARE

******************************************************************************/
RPN_DEF_OP(dt_op_math_square)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _x, _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _x                       = _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_INT);
          _stk_result->value.i     = _x * _x;
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = _xd * _xd;
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;
// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_litteral, *_result_litteral;
			char					*_str;

			_litteral				= _stk_x->value.obj;
			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			if (_litteral->need_parentheses) {
				_result_litteral->value	= rpn_str_printf("(%s)^2", _str);
			}
			else {
				_result_litteral->value	= rpn_str_printf("%s^2", _str);
			}

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_square() }}} */
/* dt_op_math_sqrt() {{{ */

/******************************************************************************

                         DT_OP_MATH_SQRT

******************************************************************************/
RPN_DEF_OP(dt_op_math_sqrt)
{
     rpn_elt                  *_stk_x, *_stk_result = 0;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = sqrt(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = sqrt(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;
// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("sqrt(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_sqrt() }}} */
/* dt_op_math_ln() {{{ */

/******************************************************************************

                         DT_OP_MATH_LN

******************************************************************************/
RPN_DEF_OP(dt_op_math_ln)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = log(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = log(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;
// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("ln(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
		rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_x);
     }

     return _retcode;
}


/* dt_op_math_ln() }}} */
/* dt_op_math_exp() {{{ */

/******************************************************************************

                         DT_OP_MATH_EXP

******************************************************************************/
RPN_DEF_OP(dt_op_math_exp)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = exp(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = exp(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;
// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("exp(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_exp() }}} */
/* dt_op_math_log() {{{ */

/******************************************************************************

                         DT_OP_MATH_LOG

******************************************************************************/
RPN_DEF_OP(dt_op_math_log)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = log10(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = log10(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;
// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("log(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_log() }}} */
/* dt_op_math_10_power_x() {{{ */

/******************************************************************************

                         DT_OP_MATH_10_POWER_X

******************************************************************************/
RPN_DEF_OP(dt_op_math_10_power_x)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = exp10(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = exp10(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;
// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_litteral, *_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);
			_litteral				= _stk_x->value.obj;

			_result_litteral		= rpn_new_litteral();
			if (_litteral->need_parentheses) {
				_result_litteral->value	= rpn_str_printf("10^(%s)", _str);
			}
			else {
				_result_litteral->value	= rpn_str_printf("10^%s", _str);
			}

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_set_lastx(stack, _stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_10_power_x() }}} */
/* dt_op_math_percent() {{{ */

/******************************************************************************

                         DT_OP_MATH_PERCENT

******************************************************************************/
RPN_DEF_OP(dt_op_math_percent)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _X_type, _Y_type;
     double                    _xd, _yd, _resultd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     _X_type                  = rpn_get_type(_stk_x);
     _Y_type                  = rpn_get_type(_stk_y);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;

          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               _yd                      = (double) _stk_y->value.i;
               _resultd                 = (_yd * _xd) / 100;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_y);
               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_DOUBLE:
// {{{
               _yd                      = _stk_y->value.d;
               _resultd                 = (_yd * _xd) / 100;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_y);
               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			_stk_result			= dt_op_keep_mat_X(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_y);
               rpn_push(stack, _stk_result);
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
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;

          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               _yd                       = (double) _stk_y->value.i;
               _resultd                 = (_yd * _xd) / 100;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_y);
               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_DOUBLE:
// {{{
               _yd                      = _stk_y->value.d;
               _resultd                 = (_yd * _xd) / 100;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_y);
               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			_stk_result			= dt_op_keep_mat_X(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_y);
               rpn_push(stack, _stk_result);
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
     case RPN_TYPE_MATRIX:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_y);
               rpn_push(stack, _stk_result);
               break;

// }}}
          case RPN_TYPE_DOUBLE:
// {{{
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_y);
               rpn_push(stack, _stk_result);
               break;

// }}}
          case RPN_TYPE_MATRIX:
// {{{
			if ((_retcode = dt_mat_check_dim(_stk_y, _stk_x)) == RPN_RET_OK) {
				rpn_set_lastx(stack, _stk_x);
				_stk_result			= dt_op_keep_mat_mat(_stk_y, _stk_x, op);
				rpn_push(stack, _stk_result);
			}
			else {
				rpn_push(stack, _stk_y);
				rpn_push(stack, _stk_x);
				goto end;
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

     rpn_free_elt(&_stk_x);

end:
     return _retcode;
}

/* dt_op_math_percent() }}} */
/* dt_op_math_delta_percent() {{{ */

/******************************************************************************

                         DT_OP_MATH_DELTA_PERCENT

******************************************************************************/
RPN_DEF_OP(dt_op_math_delta_percent)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _X_type, _Y_type;
     double                    _xd, _yd, _resultd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     _X_type                  = rpn_get_type(_stk_x);
     _Y_type                  = rpn_get_type(_stk_y);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;

          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               _yd                      = (double) _stk_y->value.i;
               _resultd                 = (_xd - _yd) * 100 / _yd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_DOUBLE:
// {{{
               _yd                      = _stk_y->value.d;
               _resultd                 = (_xd - _yd) * 100 / _yd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			_stk_result			= dt_op_keep_mat_X(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
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
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;

          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               _yd                       = (double) _stk_y->value.i;
               _resultd                 = (_xd - _yd) * 100 / _yd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_DOUBLE:
// {{{
               _yd                      = _stk_y->value.d;
               _resultd                 = (_xd - _yd) * 100 / _yd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			_stk_result			= dt_op_keep_mat_X(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
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
     case RPN_TYPE_MATRIX:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_y);
               rpn_push(stack, _stk_result);
               break;

// }}}
          case RPN_TYPE_DOUBLE:
// {{{
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_y);
               rpn_push(stack, _stk_result);
               break;

// }}}
          case RPN_TYPE_MATRIX:
// {{{
			if ((_retcode = dt_mat_check_dim(_stk_y, _stk_x)) == RPN_RET_OK) {
				rpn_set_lastx(stack, _stk_x);
				_stk_result			= dt_op_keep_mat_mat(_stk_y, _stk_x, op);
				rpn_push(stack, _stk_result);
			}
			else {
				rpn_push(stack, _stk_y);
				rpn_push(stack, _stk_x);
				goto end;
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

     rpn_free_elt(&_stk_x);

end:
     return _retcode;
}

/* dt_op_math_delta_percent() }}} */
/* dt_op_math_sin() {{{ */

/******************************************************************************

                         DT_OP_MATH_SIN

******************************************************************************/
RPN_DEF_OP(dt_op_math_sin)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = sin(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = sin(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          _stk_result              = dt_op_mat(_stk_x, op);
          break;

// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("sin(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_sin() }}} */
/* dt_op_math_cos() {{{ */

/******************************************************************************

                         DT_OP_MATH_COS

******************************************************************************/
RPN_DEF_OP(dt_op_math_cos)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = cos(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = cos(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;

// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("cos(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}

/* dt_op_math_cos() }}} */
/* dt_op_math_tan() {{{ */

/******************************************************************************

                         DT_OP_MATH_TAN

******************************************************************************/
RPN_DEF_OP(dt_op_math_tan)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = tan(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = tan(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;

// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("tan(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_tan() }}} */
/* dt_op_math_asin() {{{ */

/******************************************************************************

                         DT_OP_MATH_ASIN

******************************************************************************/
RPN_DEF_OP(dt_op_math_asin)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = asin(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = asin(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;

// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("arc_sin(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_asin() }}} */
/* dt_op_math_acos() {{{ */

/******************************************************************************

                         DT_OP_MATH_ACOS

******************************************************************************/
RPN_DEF_OP(dt_op_math_acos)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = acos(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = acos(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;

// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("arc_cos(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_acos() }}} */
/* dt_op_math_atan() {{{ */

/******************************************************************************

                         DT_OP_MATH_ATAN

******************************************************************************/
RPN_DEF_OP(dt_op_math_atan)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = atan(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = atan(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;
// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("arc_tan(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_atan() }}} */
/* dt_op_math_sinh() {{{ */

/******************************************************************************

                         DT_OP_MATH_SINH

******************************************************************************/
RPN_DEF_OP(dt_op_math_sinh)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = sinh(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = sinh(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;

// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("sinh(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_sinh() }}} */
/* dt_op_math_cosh() {{{ */

/******************************************************************************

                         DT_OP_MATH_COSH

******************************************************************************/
RPN_DEF_OP(dt_op_math_cosh)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = cosh(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = cosh(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;

// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("cosh(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_cosh() }}} */
/* dt_op_math_tanh() {{{ */

/******************************************************************************

                         DT_OP_MATH_TANH

******************************************************************************/
RPN_DEF_OP(dt_op_math_tanh)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = tanh(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = tanh(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;

// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("tanh(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_tanh() }}} */
/* dt_op_math_asinh() {{{ */

/******************************************************************************

                         DT_OP_MATH_ASINH

******************************************************************************/
RPN_DEF_OP(dt_op_math_asinh)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = asinh(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = asinh(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;

// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("arc_sinh(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_asinh() }}} */
/* dt_op_math_acosh() {{{ */

/******************************************************************************

                         DT_OP_MATH_ACOSH

******************************************************************************/
RPN_DEF_OP(dt_op_math_acosh)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = acosh(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = acosh(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;

// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("arc_cosh(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_acosh() }}} */
/* dt_op_math_atanh() {{{ */

/******************************************************************************

                         DT_OP_MATH_ATANH

******************************************************************************/
RPN_DEF_OP(dt_op_math_atanh)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = atanh(_xd);
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = atanh(_xd);
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;

// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("arc_tanh(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_atanh() }}} */
/* dt_op_math_sigmoid() {{{ */

/******************************************************************************

                         DT_OP_MATH_SIGMOID

******************************************************************************/
RPN_DEF_OP(dt_op_math_sigmoid)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type;
     double                    _xd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _xd                      = (double) _stk_x->value.i;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = (1.0 / (1.0 + exp(-_xd)));
          break;

// }}}
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;
          _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
          _stk_result->value.d     = (1.0 / (1.0 + exp(-_xd)));
          break;

// }}}
     case RPN_TYPE_MATRIX:
// {{{
          rpn_set_lastx(stack, _stk_x);
          _stk_result              = dt_op_mat(_stk_x, op);
          break;

// }}}
	case	RPN_TYPE_LITTERAL:
// {{{
		{
			rpn_litteral			*_result_litteral;
			char					*_str;

			_str					= rpn_litteral_value(_stk_x);

			_result_litteral		= rpn_new_litteral();
			_result_litteral->value	= rpn_str_printf("sigmoid(%s)", _str);

			_stk_result			= rpn_new_elt(RPN_TYPE_LITTERAL);
			_stk_result->value.obj	= _result_litteral;
		}
		break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode                 = RPN_RET_INVALID_X_TYPE;
          break;
// }}}
     }

     if (_retcode == RPN_RET_OK) {
          /* Set LastX and push result
             ~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(&_stk_x);
          rpn_push(stack, _stk_result);
     }
     else {
          /* Restore X register
             ~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_result);
     }

     return _retcode;
}


/* dt_op_math_sigmoid() }}} */
/* dt_op_math_add() {{{ */

/******************************************************************************

                         DT_OP_MATH_ADD

******************************************************************************/
RPN_DEF_OP(dt_op_math_add)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _X_type, _Y_type;
     int                       _x, _y, _result;
     double                    _xd, _yd, _resultd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

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
               _result                  = _y + _x;

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
               _resultd                 = _yd + _xd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_LITTERAL:
// {{{
			{
				rpn_litteral				*_litteral, *_litteral_y;
				char						*_y, *_result_str, _x_str[32];

				_x                       	= _stk_x->value.i;
				sprintf(_x_str, "%d", _x);

				_litteral_y				= _stk_y->value.obj;
				_y						= _litteral_y->value;

				_result_str				= dt_alloc_string2(_x_str, _y, 4);
				sprintf(_result_str, "%s + %s", _y, _x_str);

				_litteral					= rpn_new_litteral();
				_litteral->value			= _result_str;
				_litteral->need_parentheses	= TRUE;

				_stk_result				= rpn_new_elt(RPN_TYPE_LITTERAL);
				_stk_result->value.obj		= _litteral;

				rpn_set_lastx(stack, _stk_x);

				rpn_push(stack, _stk_result);
			}
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			_stk_result			= dt_op_mat_X(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
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
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;

          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               _yd                       = (double) _stk_y->value.i;
               _resultd                 = _yd + _xd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_DOUBLE:
// {{{
               _yd                      = _stk_y->value.d;
               _resultd                 = _yd + _xd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			_stk_result			= dt_op_mat_X(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
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
     case RPN_TYPE_MATRIX:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_DOUBLE:
// {{{
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			if ((_retcode = dt_mat_check_dim(_stk_y, _stk_x)) == RPN_RET_OK) {
				rpn_set_lastx(stack, _stk_x);
				_stk_result			= dt_op_mat_mat(_stk_y, _stk_x, op);
				rpn_push(stack, _stk_result);
			}
			else {
				rpn_push(stack, _stk_y);
				rpn_push(stack, _stk_x);
				goto end;
			}
               break;
// }}}
		case	RPN_TYPE_LITTERAL:
// {{{
               rpn_set_lastx(stack, _stk_x);
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_push(stack, _stk_result);
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
	case	RPN_TYPE_LITTERAL:
// {{{
          switch (_Y_type) {

		case RPN_TYPE_INT:
// {{{
			{
				rpn_litteral				*_litteral, *_litteral_x;
				char						*_x, *_result_str, _y_str[32];

				_y                       	= _stk_y->value.i;
				sprintf(_y_str, "%d", _y);

				_litteral_x				= _stk_x->value.obj;
				_x						= _litteral_x->value;

				_result_str				= dt_alloc_string2(_x, _y_str, 4);
				sprintf(_result_str, "%s + %s", _y_str, _x);

				_litteral					= rpn_new_litteral();
				_litteral->value			= _result_str;
				_litteral->need_parentheses	= TRUE;

				_stk_result				= rpn_new_elt(RPN_TYPE_LITTERAL);
				_stk_result->value.obj		= _litteral;
				_litteral_x				= _stk_x->value.obj;

				rpn_set_lastx(stack, _stk_x);

				rpn_push(stack, _stk_result);
			}
               break;
// }}}
          case RPN_TYPE_LITTERAL:
// {{{
			{
				rpn_litteral				*_litteral, *_litteral_x, *_litteral_y;
				char						*_x, *_y, *_result_str;

				_litteral_y				= _stk_y->value.obj;
				_litteral_x				= _stk_x->value.obj;

				_x						= _litteral_x->value;
				_y						= _litteral_y->value;

				_result_str				= dt_alloc_string2(_x, _y, 4);
				sprintf(_result_str, "%s + %s", _y, _x);

				_litteral					= rpn_new_litteral();
				_litteral->value			= _result_str;
				_litteral->need_parentheses	= TRUE;

				_stk_result				= rpn_new_elt(RPN_TYPE_LITTERAL);
				_stk_result->value.obj		= _litteral;
			}
               rpn_push(stack, _stk_result);
			break;
// }}}
		case	RPN_TYPE_MATRIX:
// {{{
               rpn_set_lastx(stack, _stk_x);
			_stk_result			= dt_op_mat_X(_stk_y, _stk_x, op);
               rpn_push(stack, _stk_result);
               break;
// }}}
		default:
// {{{
			// XXX Return : INCOMPATIBLE TYPES
			RPN_INTERNAL_ERROR;
			break;
// }}}
		}
		break;
// }}}
     case RPN_TYPE_NIL:
// {{{
          rpn_push(stack, _stk_y);
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

     rpn_free_elt(&_stk_x);
	rpn_free_elt(&_stk_y);

end:
     return _retcode;
}

/* dt_op_math_add() }}} */
/* dt_op_math_sub() {{{ */

/******************************************************************************

                         DT_OP_MATH_SUB

******************************************************************************/
RPN_DEF_OP(dt_op_math_sub)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _X_type, _Y_type;
     int                       _x, _y, _result;
     double                    _xd, _yd, _resultd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

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
               _result                  = _y - _x;

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
               _resultd                 = _yd - _xd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			_stk_result			= dt_op_mat_X(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
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
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;

          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               _yd                       = (double) _stk_y->value.i;
               _resultd                 = _yd - _xd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_DOUBLE:
// {{{
               _yd                      = _stk_y->value.d;
               _resultd                 = _yd - _xd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			_stk_result			= dt_op_mat_X(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
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
     case RPN_TYPE_MATRIX:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
               break;

// }}}
          case RPN_TYPE_DOUBLE:
// {{{
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			if ((_retcode = dt_mat_check_dim(_stk_y, _stk_x)) == RPN_RET_OK) {
				rpn_set_lastx(stack, _stk_x);
				_stk_result			= dt_op_mat_mat(_stk_y, _stk_x, op);
				rpn_push(stack, _stk_result);
			}
			else {
				rpn_push(stack, _stk_y);
				rpn_push(stack, _stk_x);
				goto end;
			}
               break;
// }}}
		case	RPN_TYPE_LITTERAL:
// {{{
               rpn_set_lastx(stack, _stk_x);
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_push(stack, _stk_result);
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
	case	RPN_TYPE_LITTERAL:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_LITTERAL:
// {{{
			{
				rpn_litteral				*_litteral, *_litteral_a, *_litteral_b;
				char						*_a, *_b, *_result_str;

				_litteral_a				= _stk_y->value.obj;
				_litteral_b				= _stk_x->value.obj;

				if (_litteral_a->need_parentheses) {
					_a						= dt_parentheses(_litteral_a->value);
				}
				else {
					_a						= _litteral_a->value;
				}
				if (_litteral_b->need_parentheses) {
					_b						= dt_parentheses(_litteral_b->value);
				}
				else {
					_b						= _litteral_b->value;
				}

				_result_str				= dt_alloc_string2(_a, _b, 4);
				sprintf(_result_str, "%s - %s", _a, _b);

				_litteral					= rpn_new_litteral();
				_litteral->value			= _result_str;
				_litteral->need_parentheses	= TRUE;

				_stk_result				= rpn_new_elt(RPN_TYPE_LITTERAL);
				_stk_result->value.obj		= _litteral;
			}
               rpn_push(stack, _stk_result);
			break;
// }}}
		case	RPN_TYPE_MATRIX:
// {{{
               rpn_set_lastx(stack, _stk_x);
			_stk_result			= dt_op_mat_X(_stk_y, _stk_x, op);
               rpn_push(stack, _stk_result);
               break;
// }}}
		default:
// {{{
			RPN_INTERNAL_ERROR;
			break;
// }}}
		}
		break;
// }}}
     case RPN_TYPE_NIL:
// {{{
          rpn_push(stack, _stk_y);
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

     rpn_free_elt(&_stk_x);
	rpn_free_elt(&_stk_y);

end:
     return _retcode;
}

/* dt_op_math_sub() }}} */
/* dt_op_math_mul() {{{ */

/******************************************************************************

                         DT_OP_MATH_MUL

******************************************************************************/
RPN_DEF_OP(dt_op_math_mul)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _X_type, _Y_type;
     int                       _x, _y, _result;
     double                    _xd, _yd, _resultd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

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
               _result                  = _y * _x;

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
               _resultd                 = _yd * _xd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			_stk_result			= dt_op_mat_X(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
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
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;

          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               _yd                       = (double) _stk_y->value.i;
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
			_stk_result			= dt_op_mat_X(_stk_y, _stk_x, op);
			rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
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
     case RPN_TYPE_MATRIX:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_DOUBLE:
// {{{
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			if ((_retcode = dt_mat_check_dim(_stk_y, _stk_x)) == RPN_RET_OK) {
				rpn_set_lastx(stack, _stk_x);
				_stk_result			= dt_op_mat_mat(_stk_y, _stk_x, op);
				rpn_push(stack, _stk_result);
			}
			else {
				rpn_push(stack, _stk_y);
				rpn_push(stack, _stk_x);
				goto end;
			}
               break;
// }}}
		case	RPN_TYPE_LITTERAL:
// {{{
               rpn_set_lastx(stack, _stk_x);
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_push(stack, _stk_result);
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
	case	RPN_TYPE_LITTERAL:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_LITTERAL:
// {{{
			{
				rpn_litteral				*_litteral, *_litteral_a, *_litteral_b;
				char						*_a, *_b, *_result_str;

				_litteral_a				= _stk_y->value.obj;
				_litteral_b				= _stk_x->value.obj;

				if (_litteral_a->need_parentheses) {
					_a						= dt_parentheses(_litteral_a->value);
				}
				else {
					_a						= _litteral_a->value;
				}
				if (_litteral_b->need_parentheses) {
					_b						= dt_parentheses(_litteral_b->value);
				}
				else {
					_b						= _litteral_b->value;
				}

				_result_str				= dt_alloc_string2(_a, _b, 4);
				sprintf(_result_str, "%s*%s", _a, _b);

				_litteral					= rpn_new_litteral();
				_litteral->value			= _result_str;
				_litteral->need_parentheses	= FALSE;

				_stk_result				= rpn_new_elt(RPN_TYPE_LITTERAL);
				_stk_result->value.obj		= _litteral;
			}
               rpn_push(stack, _stk_result);
			break;
// }}}
		case	RPN_TYPE_MATRIX:
// {{{
               rpn_set_lastx(stack, _stk_x);
			_stk_result			= dt_op_mat_X(_stk_y, _stk_x, op);
               rpn_push(stack, _stk_result);
               break;
// }}}
		default:
// {{{
			RPN_INTERNAL_ERROR;
			break;
// }}}
		}
		break;
// }}}
     case RPN_TYPE_NIL:
// {{{
          rpn_push(stack, _stk_y);
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

     rpn_free_elt(&_stk_x);
	rpn_free_elt(&_stk_y);

end:
     return _retcode;
}

/* dt_op_math_mul() }}} */
/* dt_op_math_div() {{{ */

/******************************************************************************

                         DT_OP_MATH_DIV

******************************************************************************/
RPN_DEF_OP(dt_op_math_div)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _X_type, _Y_type;
     int                       _x, _y, _result;
     double                    _xd, _yd, _resultd;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

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
               _result                  = _y / _x;

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
               _resultd                 = _yd / _xd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			_stk_result			= dt_op_mat_X(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
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
     case RPN_TYPE_DOUBLE:
// {{{
          _xd                      = _stk_x->value.d;

          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               _yd                       = (double) _stk_y->value.i;
               _resultd                 = _yd / _xd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_DOUBLE:
// {{{
               _yd                      = _stk_y->value.d;
               _resultd                 = _yd / _xd;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt(RPN_TYPE_DOUBLE);
               _stk_result->value.d     = _resultd;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			_stk_result			= dt_op_mat_X(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
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
     case RPN_TYPE_MATRIX:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_DOUBLE:
// {{{
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_set_lastx(stack, _stk_x);
               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
			if ((_retcode = dt_mat_check_dim(_stk_y, _stk_x)) == RPN_RET_OK) {
				rpn_set_lastx(stack, _stk_x);
				_stk_result			= dt_op_mat_mat(_stk_y, _stk_x, op);
				rpn_push(stack, _stk_result);
			}
			else {
				rpn_push(stack, _stk_y);
				rpn_push(stack, _stk_x);
				goto end;
			}
               break;
// }}}
		case	RPN_TYPE_LITTERAL:
// {{{
               rpn_set_lastx(stack, _stk_x);
			_stk_result			= dt_op_Y_mat(_stk_y, _stk_x, op);
               rpn_push(stack, _stk_result);
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
	case	RPN_TYPE_LITTERAL:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_LITTERAL:
// {{{
			{
				rpn_litteral				*_litteral, *_litteral_a, *_litteral_b;
				char						*_a, *_b, *_result_str;

				_litteral_a				= _stk_y->value.obj;
				_litteral_b				= _stk_x->value.obj;

				if (_litteral_a->need_parentheses) {
					_a						= dt_parentheses(_litteral_a->value);
				}
				else {
					_a						= _litteral_a->value;
				}
				if (_litteral_b->need_parentheses) {
					_b						= dt_parentheses(_litteral_b->value);
				}
				else {
					_b						= _litteral_b->value;
				}

				_result_str				= dt_alloc_string2(_a, _b, 4);
				sprintf(_result_str, "%s/%s", _a, _b);

				_litteral					= rpn_new_litteral();
				_litteral->value			= _result_str;
				_litteral->need_parentheses	= FALSE;

				_stk_result				= rpn_new_elt(RPN_TYPE_LITTERAL);
				_stk_result->value.obj		= _litteral;
			}
               rpn_push(stack, _stk_result);
			break;
// }}}
		case	RPN_TYPE_MATRIX:
// {{{
               rpn_set_lastx(stack, _stk_x);
			_stk_result			= dt_op_mat_X(_stk_y, _stk_x, op);
               rpn_push(stack, _stk_result);
               break;
// }}}
		default:
// {{{
			RPN_INTERNAL_ERROR;
			break;
// }}}
		}
		break;
// }}}
     case RPN_TYPE_NIL:
// {{{
          rpn_push(stack, _stk_y);
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

     rpn_free_elt(&_stk_x);
	rpn_free_elt(&_stk_y);

end:
     return _retcode;
}

/* dt_op_math_div() }}} */
/* dt_op_math_math_mul() {{{ */

/******************************************************************************

                         DT_OP_MATH_MATH_MUL

******************************************************************************/
RPN_DEF_OP(dt_op_math_math_mul)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _X_type, _Y_type;
     int                       _x, _y, _result, _lg, _n, _i;
	size_t				 _total_size;
     double                    _xd, _yd, _resultd;
     char                     *_s, *_result_s;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

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
                    int                       _n, _p, _i, _j, _idx;
				size_t				 _size;
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

                              _elt      = dt_mat_get_elt_ref(_mat_A, _i, _j);
                              rpn_push(_stack, _elt);

                              if ((_retcode = (op->func)(_stack, op)) != 0) {
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
                    int                       _n, _p, _i, _j, _idx;
				size_t				 _size;
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

                              _elt      = dt_mat_get_elt_ref(_mat_A, _i, _j);
                              rpn_push(_stack, _elt);

                              if ((_retcode = (op->func)(_stack, op)) != 0) {
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
     case RPN_TYPE_MATRIX:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_INT:
          case RPN_TYPE_DOUBLE:
// {{{
               {
                    struct rpn_matrix        *_mat_B, *_mat_C;
                    int                       _n, _p, _i, _j, _idx;
				size_t				 _size;
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
                              _elt      = dt_mat_get_elt_ref(_mat_B, _i, _j);
                              rpn_push(_stack, _elt);

                              _scalar   = rpn_clone_elt(_stk_y);
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
          case RPN_TYPE_MATRIX:
// {{{
               {
                    struct rpn_matrix        *_mat_A, *_mat_B, *_mat_C;
                    int                       _n, _m, _p, _i, _j, _k, _idx;
				size_t				 _size;
                    struct rpn_elt           *_elt, *_clone;
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

                    for (_i = 1; likely(_i <= _n); _i++) {
                         for (_j = 1; likely(_j <= _p); _j++) {
                              for (_k = 1; likely(_k <= _m); _k++) {
                                   _elt                = dt_mat_get_elt_ref(_mat_A, _i, _k);
                                   _clone              = rpn_clone_elt(_elt);
                                   rpn_push(_stack, _clone);

                                   _elt                = dt_mat_get_elt_ref(_mat_B, _k, _j);
                                   _clone              = rpn_clone_elt(_elt);
                                   rpn_push(_stack, _clone);

// printf("AVANT MULT:\n");
// rpn_disp_stack(_stack);
                                   if (likely((_retcode = (op->func)(_stack, op)) != 0)) {
                                        rpn_err_msg_op_error(op->op_name, _retcode);
                                        exit(RPN_EXIT_OP_ERROR);
                                   }

// printf("APRES MULT, AVANT ADD :\n");
// rpn_disp_stack(_stack);
                                   if (_k > 1) {
                                        if ((_retcode = dl_exec_op(_stack, "+")) != 0) {
									RPN_INTERNAL_ERROR;
//                                             rpn_err_msg_op_error(RPN_OP_DESC(ADD)->op_name, _retcode);
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
                         _stk_result              = dt_mat_get_elt_ref(_mat_C, 1, 1);
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
	case	RPN_TYPE_LITTERAL:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_LITTERAL:
// {{{
			{
				rpn_litteral				*_litteral, *_litteral_a, *_litteral_b;
				char						*_a, *_b, *_result_str;

				_litteral_a				= _stk_y->value.obj;
				_litteral_b				= _stk_x->value.obj;

				if (_litteral_a->need_parentheses) {
					_a						= dt_parentheses(_litteral_a->value);
				}
				else {
					_a						= _litteral_a->value;
				}
				if (_litteral_b->need_parentheses) {
					_b						= dt_parentheses(_litteral_b->value);
				}
				else {
					_b						= _litteral_b->value;
				}

				_result_str				= dt_alloc_string2(_a, _b, 4);
				sprintf(_result_str, "%s.%s", _a, _b);

				_litteral					= rpn_new_litteral();
				_litteral->value			= _result_str;
				_litteral->need_parentheses	= FALSE;

				_stk_result				= rpn_new_elt(RPN_TYPE_LITTERAL);
				_stk_result->value.obj		= _litteral;
			}
               rpn_push(stack, _stk_result);
			break;
// }}}
		case	RPN_TYPE_MATRIX:
// {{{
               rpn_set_lastx(stack, _stk_x);
			_stk_result			= dt_op_mat_X(_stk_y, _stk_x, op);
               rpn_push(stack, _stk_result);
               break;
// }}}
		default:
// {{{
			RPN_INTERNAL_ERROR;
			break;
// }}}
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

/* dt_op_math_math_mul() }}} */
/* dt_op_math_seq() {{{ */

/******************************************************************************

                              DT_OP_MATH_SEQ

******************************************************************************/
RPN_DEF_OP(dt_op_math_seq)
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
/* dt_op_math_seq() }}} */
/* dt_op_math_seq2() {{{ */

/******************************************************************************

                              DT_OP_MATH_SEQ2

******************************************************************************/
RPN_DEF_OP(dt_op_math_seq2)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_z, *_stk_result;
     int                       _x_min, _x_max, _y_min, _y_max, _i, _j;
     int                       _retcode, _Z_type;
	char					*_str;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);
	_stk_z				= rpn_pop(stack);

	_Z_type				= rpn_get_type(_stk_z);

     if (rpn_get_type(_stk_x) != RPN_TYPE_MIN_MAX) {
		RPN_INTERNAL_ERROR;
	}
     if (rpn_get_type(_stk_y) != RPN_TYPE_MIN_MAX) {
		RPN_INTERNAL_ERROR;
	}
	_x_min                   = rpn_get_min(_stk_x);
	_x_max                   = rpn_get_max(_stk_x);
	_y_min                   = rpn_get_min(_stk_y);
	_y_max                   = rpn_get_max(_stk_y);

	switch (_Z_type) {

	case	RPN_TYPE_STRING:
		_str					= _stk_z->value.s;
		break;

	case	RPN_TYPE_LITTERAL:
		_str					= rpn_litteral_value(_stk_z);
		break;

	default:
		RPN_INTERNAL_ERROR;
		break;
	}

	if (_x_min <= _x_max) {
		for (_i = _x_min; _i <= _x_max; _i++) {
			if (_y_min <= _y_max) {
				for (_j = _y_min; _j <= _y_max; _j++) {
					char					 _buf[1024], *_result;
					rpn_litteral			*_litteral;

					sprintf(_buf, _str, _i, _j);
					_stk_result              = rpn_new_elt(_stk_z->type);

					switch (_Z_type) {

					case	RPN_TYPE_STRING:
						_result				= strdup(_buf);
						_stk_result->value.s     = _result;
						break;

					case	RPN_TYPE_LITTERAL:
						_litteral				= rpn_new_litteral();
						_stk_result->value.obj	= _litteral;
						rpn_litteral_set_value(_stk_result, _buf);
						break;

					default:
						RPN_INTERNAL_ERROR;
						break;
					}


					rpn_push(stack, _stk_result);
				}
			}
		}
	}
	else {
		printf("X : min %d > max %d\n", _x_min, _x_max);
		_retcode				= RPN_RET_INVALID_X;
	}

	if (_retcode == RPN_RET_OK) {
		rpn_set_lastx(stack, _stk_x);

		rpn_free_elt(&_stk_x);
		rpn_free_elt(&_stk_y);
		rpn_free_elt(&_stk_z);
     }
	else {
		rpn_push(stack, _stk_z);
		rpn_push(stack, _stk_y);
		rpn_push(stack, _stk_x);
	}

     return _retcode;
}

/* dt_op_math_seq2() }}} */
/* dt_op_math_dim() {{{ */

/******************************************************************************

                              DT_OP_CORE_DIM

******************************************************************************/
RPN_DEF_OP(dt_op_math_dim)
{
     rpn_elt                  *_stk_x, *_dim_row, *_dim_col;
     int                       _row, _col;
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

/* dt_op_math_dim() }}} */
/* dt_op_math_matrix() {{{ */

/******************************************************************************

                         DT_OP_MATH_MATRIX

******************************************************************************/
RPN_DEF_OP(dt_op_math_matrix)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_matrix, *_stk_elt;
     int                       _n, _p, _i, _j, _idx;
	size_t				 _size;
     rpn_matrix			*_matrix;
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

     default:
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
          break;
     }

     rpn_push(stack, _stk_matrix);

end:
     return _retcode;
}

/* dt_op_math_matrix() }}} */
/* dt_op_math_explode() {{{ */

/******************************************************************************

					DT_OP_MATH_EXPLODE

******************************************************************************/
RPN_DEF_OP(dt_op_math_explode)
{
     rpn_elt                  *_stk_x, *_elt;
     int                       _n, _p, _i, _j, _idx;
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

     default:
          rpn_push(stack, _stk_x);
          _retcode                 = RPN_RET_INVALID_X;
          break;
     }

     return _retcode;
}

/* dt_op_math_explode() }}} */
/* dt_op_math_vec2() {{{ */

/******************************************************************************

                         DT_OP_MAT_VEC2

******************************************************************************/
RPN_DEF_OP(dt_op_math_vec2)
{
     rpn_elt                  *_stk_n, *_stk_p;

     /* Vectors of dimension 2 are defined as matrices 2x1
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _stk_n                   = rpn_new_elt(RPN_TYPE_INT);
     _stk_n->value.i          = 2;      // 2 lines
     rpn_push(stack, _stk_n);

     _stk_p                   = rpn_new_elt(RPN_TYPE_INT);
     _stk_p->value.i          = 1;      // 1 column
     rpn_push(stack, _stk_p);

     return dl_exec_op(stack, "mat");
}
/* dt_op_math_vec2() }}} */
/* dt_op_math_vec3() {{{ */

/******************************************************************************

                         DT_OP_MATH_VEC3

******************************************************************************/
RPN_DEF_OP(dt_op_math_vec3)
{
     rpn_elt                  *_stk_n, *_stk_p;

     /* Vectors of dimension 3 are defined as matrices 3x1
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _stk_n                   = rpn_new_elt(RPN_TYPE_INT);
     _stk_n->value.i          = 3;      // 3 lines
     rpn_push(stack, _stk_n);

     _stk_p                   = rpn_new_elt(RPN_TYPE_INT);
     _stk_p->value.i          = 1;      // 1 column
     rpn_push(stack, _stk_p);

     return dl_exec_op(stack, "mat");
}
/* dt_op_math_vec3() }}} */
/* dt_op_math_transpose() {{{ */

/******************************************************************************

                         DT_OP_MATH_TRANSPOSE

******************************************************************************/
RPN_DEF_OP(dt_op_math_transpose)
{
     rpn_elt                  *_stk_x;
     int                       _n, _p, _X_type, _i, _j, _idx_src, _idx_dst;
     int                       _retcode;
	size_t				 _size;
     struct rpn_matrix        *_matrix, *_transpose;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_MATRIX:
		rpn_set_lastx(stack, _stk_x);

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

/* dt_op_math_transpose() }}} */
/* dt_op_math_cross_product() {{{ */

/******************************************************************************

                         DT_OP_MATH_CROSS_PRODUCT

******************************************************************************/
RPN_DEF_OP(dt_op_math_cross_product)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _retcode;
     struct rpn_matrix        *_mat_x, *_mat_y;
     struct rpn_elt           *_a1, *_a2, *_a3,
                              *_b1, *_b2, *_b3;
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

     rpn_set_lastx(stack, _stk_x);

     _stack                   = rpn_new_stack(__func__);

     _a2  = dt_mat_clone_elt(_mat_y, 2, 1);
     _b3  = dt_mat_clone_elt(_mat_x, 3, 1);
     rpn_push(_stack, _a2);
     rpn_push(_stack, _b3);
     dl_exec_op(_stack, "*");

     _a3  = dt_mat_clone_elt(_mat_y, 3, 1);
     _b2  = dt_mat_clone_elt(_mat_x, 2, 1);
     rpn_push(_stack, _a3);
     rpn_push(_stack, _b2);
     dl_exec_op(_stack, "*");
     dl_exec_op(_stack, "-");

     _a3  = dt_mat_clone_elt(_mat_y, 3, 1);
     _b1  = dt_mat_clone_elt(_mat_x, 1, 1);
     rpn_push(_stack, _a3);
     rpn_push(_stack, _b1);
     dl_exec_op(_stack, "*");

     _a1  = dt_mat_clone_elt(_mat_y, 1, 1);
     _b3  = dt_mat_clone_elt(_mat_x, 3, 1);
     rpn_push(_stack, _a1);
     rpn_push(_stack, _b3);
     dl_exec_op(_stack, "*");
     dl_exec_op(_stack, "-");

     _a1  = dt_mat_clone_elt(_mat_y, 1, 1);
     _b2  = dt_mat_clone_elt(_mat_x, 2, 1);
     rpn_push(_stack, _a1);
     rpn_push(_stack, _b2);
     dl_exec_op(_stack, "*");

     _a2  = dt_mat_clone_elt(_mat_y, 2, 1);
     _b1  = dt_mat_clone_elt(_mat_x, 1, 1);
     rpn_push(_stack, _a2);
     rpn_push(_stack, _b1);
     dl_exec_op(_stack, "*");
     dl_exec_op(_stack, "-");

     dl_exec_op(_stack, "vec3");

     rpn_free_elt(&_stk_x);
     rpn_free_elt(&_stk_y);

     _stk_result              = rpn_pop(_stack);

     rpn_free_stack(_stack);
     rpn_push(stack, _stk_result);

end:
     return _retcode;
}

/* dt_op_math_cross_product() }}} */
/* dt_op_math_hrev() {{{ */

/******************************************************************************

                         DT_OP_MATH_HREV

******************************************************************************/
RPN_DEF_OP(dt_op_math_hrev)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type, _n, _p, _idx_src, _idx_dst;
     int                       _retcode, _row, _col;
	size_t				 _size;
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

	rpn_set_lastx(stack, _stk_x);

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

/* dt_op_math_hrev() }}} */
/* dt_op_math_vrev() {{{ */

/******************************************************************************

                         DT_OP_MATH_VREV

******************************************************************************/
RPN_DEF_OP(dt_op_math_vrev)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _X_type, _n, _p, _idx_src, _idx_dst;
     int                       _retcode, _row, _col;
	size_t				 _size;
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

	rpn_set_lastx(stack, _stk_x);

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
               _idx_dst                 = RPN_MATRIX_IDX(_row, _p - _col + 1, _n, _p);

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

/* dt_op_math_vrev() }}} */
/* dt_op_math_diagmat() {{{ */

/******************************************************************************

                         DT_OP_MATH_DIAGMAT

******************************************************************************/
RPN_DEF_OP(dt_op_math_diagmat)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_z, *_stk_matrix, *_elt_mat;
     int                       _n, _i, _j, _idx;
	size_t				 _size;
     rpn_matrix			*_matrix;
     int                       _X_type;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
          _n                       = _stk_x->value.i;
          if (_n <= 0) {
               /* n is invalid
                  ~~~~~~~~~~~~ */
               rpn_push(stack, _stk_x);

               _retcode                 = RPN_RET_INVALID_X;
               goto end;
          }

          _stk_y                   = rpn_pop(stack);
		_stk_z				= rpn_pop(stack);

		/* Create matrix element
		   ~~~~~~~~~~~~~~~~~~~~~ */
		_stk_matrix              = rpn_new_elt(RPN_TYPE_MATRIX);

		_size                    = sizeof(*_matrix) + (((_n * _n) - 1) * sizeof (void *));
		_matrix                  = (struct rpn_matrix *) RPN_MALLOC(_size);
		_matrix->n               = _n;
		_matrix->p               = _n;
		_stk_matrix->value.obj   = _matrix;

		for (_i = _n; _i > 0; _i--) {
			for (_j = _n; _j > 0; _j--) {
				if (_i == _j) {
					_elt_mat			= rpn_clone_elt(_stk_z);
				}
				else {
					_elt_mat			= rpn_clone_elt(_stk_y);
				}
				_idx                = RPN_MATRIX_IDX(_i, _j, _n, _n);
				_matrix->base[_idx] = (void *) _elt_mat;
			}
		}

		rpn_set_lastx(stack, _stk_x);

		RPN_FREE(_stk_x);
		RPN_FREE(_stk_y);
		RPN_FREE(_stk_z);
     }

     rpn_push(stack, _stk_matrix);

end:
     return _retcode;
}

/* dt_op_math_diagmat() }}} */
/* dt_op_math_hcat() {{{ */

/******************************************************************************

                         DT_OP_MATH_HCAT

******************************************************************************/
RPN_DEF_OP(dt_op_math_hcat)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     rpn_matrix               *_mat_A, *_mat_B, *_mat_C;
     int                       _n, _pA, _pB, _i, _j, _idx;
	size_t				 _size;
     rpn_stack                *_stack;
     int                       _retcode, _X_type, _Y_type;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     _X_type                  = rpn_get_type(_stk_x);
     _Y_type                  = rpn_get_type(_stk_y);

     if (_X_type != RPN_TYPE_MATRIX
     ||  _Y_type != RPN_TYPE_MATRIX) {
          RPN_INTERNAL_ERROR;
     }

     _mat_A                   = (rpn_matrix *) _stk_y->value.obj;
     _mat_B                   = (rpn_matrix *) _stk_x->value.obj;

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
               _mat_C->base[_idx++]     = dt_mat_extract_elt(_mat_A, _i, _j);
          }

          for (_j = 1; _j <= _pB; _j++) {
               _mat_C->base[_idx++]     = dt_mat_extract_elt(_mat_B, _i, _j);
          }
     }

     rpn_free_stack(_stack);
     rpn_push(stack, _stk_result);

end:
     return _retcode;
}

/* dt_op_math_hcat() }}} */
/* dt_op_math_vcat() {{{ */

/******************************************************************************

                         DT_OP_MATH_VCAT

******************************************************************************/
RPN_DEF_OP(dt_op_math_vcat)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     rpn_matrix               *_mat_A, *_mat_B, *_mat_C;
     int                       _nA, _nB, _p, _i, _j, _idx;
	size_t				 _size;
     rpn_stack                *_stack;
     int                       _retcode, _X_type, _Y_type;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     _X_type                  = rpn_get_type(_stk_x);
     _Y_type                  = rpn_get_type(_stk_y);

     if (_X_type != RPN_TYPE_MATRIX
     ||  _Y_type != RPN_TYPE_MATRIX) {
          RPN_INTERNAL_ERROR;
     }

     _mat_A                   = (rpn_matrix *) _stk_y->value.obj;
     _mat_B                   = (rpn_matrix *) _stk_x->value.obj;

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
               _mat_C->base[_idx++]     = dt_mat_extract_elt(_mat_A, _i, _j);
          }
     }

     for (_i = 1; _i <= _nB; _i++) {
          for (_j = 1; _j <= _p; _j++) {
               _mat_C->base[_idx++]     = dt_mat_extract_elt(_mat_B, _i, _j);
          }
     }

     rpn_free_stack(_stack);
     rpn_push(stack, _stk_result);

end:
     return _retcode;
}

/* dt_op_math_vcat() }}} */
/* dt_op_math_zmat() {{{ */

/******************************************************************************

                              DT_OP_MATH_ZMAT

******************************************************************************/
RPN_DEF_OP(dt_op_math_zmat)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_z = NULL;
     rpn_elt                  *_stk_matrix,
                              *_stk_elt;
     int                       _n, _p, _i, _j, _idx;
	size_t				 _size;
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

     _stk_z				= rpn_pop(stack);

     for (_i = _n; _i > 0; _i--) {
          for (_j = _p; _j > 0; _j--) {
               _stk_elt            = rpn_clone_elt(_stk_z);
               _idx                = RPN_MATRIX_IDX(_i, _j, _n, _p);
               _matrix->base[_idx] = (void *) _stk_elt;
          }
     }

     rpn_push(stack, _stk_matrix);

end:
     return _retcode;
}

/* dt_op_math_zmat() }}} */
/* dt_mat_get_elt_ref() {{{ */

/******************************************************************************

                              DT_MAT_GET_ELT_REF

******************************************************************************/
rpn_elt *dt_mat_get_elt_ref(rpn_matrix *matrix, int i, int j)
{
     rpn_elt		      *_elt;
     int                  _idx;

     _idx                = RPN_MATRIX_IDX(i, j, matrix->n, matrix->p);
     _elt                = (rpn_elt *) matrix->base[_idx];

     return _elt;
}
/* dt_mat_get_elt_ref() }}} */
/* dt_mat_extract_elt() {{{ */

/******************************************************************************

                              DT_MAT_EXTRACT_ELT

******************************************************************************/
rpn_elt *dt_mat_extract_elt(rpn_matrix *matrix, int i, int j)
{
     rpn_elt		      *_elt;
     int                  _idx;

     _idx                = RPN_MATRIX_IDX(i, j, matrix->n, matrix->p);
     _elt                = (rpn_elt *) matrix->base[_idx];

     /* Reset original pointer since the element is extracted
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     matrix->base[_idx]  = NULL;

     return _elt;
}

/* dt_mat_extract_elt() }}} */
/* dt_mat_clone_elt() {{{ */

/******************************************************************************

                              DT_MAT_CLONE_ELT

******************************************************************************/
rpn_elt *dt_mat_clone_elt(rpn_matrix *matrix, int i, int j)
{
     rpn_elt		      *_elt, *_clone;
     int                  _idx;

     _idx                = RPN_MATRIX_IDX(i, j, matrix->n, matrix->p);
     _elt                = (rpn_elt *) matrix->base[_idx];
	_clone			= rpn_clone_elt(_elt);

     return _clone;
}

/* dt_mat_clone_elt() }}} */
/* dt_op_mat() {{{ */

/******************************************************************************

                         DT_OP_MAT

******************************************************************************/
rpn_elt *dt_op_mat(rpn_elt *elt_mat, dl_operator *op)
{
	/* Matrix element "mat" is MODIFIED and cannot be reused !!!
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_matrix			*_mat_A, *_mat_C;
     int                       _retcode, _n, _p, _i, _j, _idx;
	size_t				 _size;
     rpn_elt				*_elt, *_nil, *_stk_result;
     rpn_stack                *_stack;

     _mat_A                   = (rpn_matrix *) elt_mat->value.obj;

     _n                       = _mat_A->n;
     _p                       = _mat_A->p;

     _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
     _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
     _mat_C                   = (rpn_matrix *) RPN_MALLOC(_size);
     _mat_C->n                = _n;
     _mat_C->p                = _p;
     _stk_result->value.obj   = _mat_C;

     _stack                   = rpn_new_stack(__func__);

     for (_i = 1; _i <= _n; _i++) {
          for (_j = 1; _j <= _p; _j++) {
               _nil      = rpn_new_elt(RPN_TYPE_NIL);
               rpn_push(_stack, _nil);

               _elt      = dt_mat_extract_elt(_mat_A, _i, _j);
//               _elt      = dt_mat_get_elt_ref(_mat_A, _i, _j);
               rpn_push(_stack, _elt);
               if ((_retcode = (*op->func)(_stack, op)) != 0) {
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
          _stk_result              = dt_mat_extract_elt(_mat_C, 1, 1);
     }

     rpn_free_stack(_stack);

     return _stk_result;
}

/* dt_op_mat() }}} */
/* dt_op_mat_X() {{{ */

/******************************************************************************

                         DT_OP_MAT_X

******************************************************************************/
rpn_elt *dt_op_mat_X(rpn_elt *elt_mat, rpn_elt *elt_X, dl_operator *op)
{
	/* Matrix element "mat" is MODIFIED and cannot be reused !!!
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_matrix	          *_mat_A, *_mat_C;
     int                       _retcode, _n, _p, _i, _j, _idx;
	size_t				 _size;
     rpn_elt                  *_elt, *_stk_result, *_elt_X;
     rpn_stack                *_stack;

     _mat_A                   = (rpn_matrix *) elt_mat->value.obj;

     _n                       = _mat_A->n;
     _p                       = _mat_A->p;

     _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
     _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
     _mat_C                   = (rpn_matrix *) RPN_MALLOC(_size);
     _mat_C->n                = _n;
     _mat_C->p                = _p;
     _stk_result->value.obj   = _mat_C;

     _stack                   = rpn_new_stack(__func__);

     for (_i = 1; _i <= _n; _i++) {
          for (_j = 1; _j <= _p; _j++) {
               _elt      = dt_mat_extract_elt(_mat_A, _i, _j);
               rpn_push(_stack, _elt);

			_elt_X	= rpn_clone_elt(elt_X);
               rpn_push(_stack, _elt_X);

               if ((_retcode = (*op->func)(_stack, op)) != 0) {
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
          _stk_result              = dt_mat_get_elt_ref(_mat_C, 1, 1);
     }

     rpn_free_stack(_stack);

     return _stk_result;
}

/* dt_op_mat_X() }}} */
/* dt_op_keep_mat_X() {{{ */

/******************************************************************************

                         DT_OP_KEEP_MAT_X

******************************************************************************/
rpn_elt *dt_op_keep_mat_X(rpn_elt *elt_mat, rpn_elt *elt_X, dl_operator *op)
{
	/* Matrix element "mat" is left unchanged so that
	   the caller program can keep on using it.
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_matrix	          *_mat_A, *_mat_C;
     int                       _retcode, _n, _p, _i, _j, _idx;
	size_t				 _size;
     rpn_elt                  *_elt, *_stk_result, *_elt_X, *_clone;
     rpn_stack                *_stack;

     _mat_A                   = (rpn_matrix *) elt_mat->value.obj;

     _n                       = _mat_A->n;
     _p                       = _mat_A->p;

     _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
     _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
     _mat_C                   = (rpn_matrix *) RPN_MALLOC(_size);
     _mat_C->n                = _n;
     _mat_C->p                = _p;
     _stk_result->value.obj   = _mat_C;

     _stack                   = rpn_new_stack(__func__);

     for (_i = 1; _i <= _n; _i++) {
          for (_j = 1; _j <= _p; _j++) {
               _elt      			= dt_mat_get_elt_ref(_mat_A, _i, _j);
			_clone				= rpn_clone_elt(_elt);
               rpn_push(_stack, _clone);

			_elt_X	= rpn_clone_elt(elt_X);
               rpn_push(_stack, _elt_X);

               if ((_retcode = (*op->func)(_stack, op)) != 0) {
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
          _stk_result              = dt_mat_get_elt_ref(_mat_C, 1, 1);
     }

     rpn_free_stack(_stack);

     return _stk_result;
}

/* dt_op_keep_mat_X() }}} */
/* dt_op_Y_mat() {{{ */

/******************************************************************************

                         DT_OP_Y_MAT

******************************************************************************/
rpn_elt *dt_op_Y_mat(rpn_elt *elt_Y, rpn_elt *elt_mat, dl_operator *op)
{
	/* Matrix element "mat" is left unchanged so that
	   the caller program can push it in the Lastx register.
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     rpn_matrix	          *_mat_B, *_mat_C;
     int                       _retcode, _n, _p, _i, _j, _idx;
	size_t				 _size;
     rpn_elt                  *_elt, *_stk_result, *_elt_Y, *_clone;
     rpn_stack                *_stack;

     _mat_B                   = (rpn_matrix *) elt_mat->value.obj;

     _n                       = _mat_B->n;
     _p                       = _mat_B->p;

     _stk_result              = rpn_new_elt(RPN_TYPE_MATRIX);
     _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
     _mat_C                   = (rpn_matrix *) RPN_MALLOC(_size);
     _mat_C->n                = _n;
     _mat_C->p                = _p;
     _stk_result->value.obj   = _mat_C;

     _stack                   = rpn_new_stack(__func__);

     for (_i = 1; _i <= _n; _i++) {
          for (_j = 1; _j <= _p; _j++) {
			_elt_Y				= rpn_clone_elt(elt_Y);
               rpn_push(_stack, _elt_Y);

               _elt      			= dt_mat_get_elt_ref(_mat_B, _i, _j);
			_clone				= rpn_clone_elt(_elt);
               rpn_push(_stack, _clone);

               if ((_retcode = (*op->func)(_stack, op)) != 0) {
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
          _stk_result              = dt_mat_get_elt_ref(_mat_C, 1, 1);
     }

     rpn_free_stack(_stack);

     return _stk_result;
}

/* dt_op_Y_mat() }}} */
/* dt_op_mat_mat() {{{ */

/******************************************************************************

                         DT_OP_MAT_MAT

******************************************************************************/
rpn_elt *dt_op_mat_mat(rpn_elt *mat_Y, rpn_elt *mat_X, dl_operator *op)
{
	rpn_matrix			*_mat_A, *_mat_B, *_mat_C;
	int                       _n, _p, _i, _j, _idx, _retcode;
	size_t				 _size;
	rpn_elt				*_elt, *_stk_result;
	rpn_stack				*_stack;

	_mat_A                   = (struct rpn_matrix *) mat_Y->value.obj;
	_mat_B                   = (struct rpn_matrix *) mat_X->value.obj;

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
			_elt      = dt_mat_extract_elt(_mat_A, _i, _j);
			rpn_push(_stack, _elt);

			_elt      = dt_mat_extract_elt(_mat_B, _i, _j);
			rpn_push(_stack, _elt);

			if ((_retcode = (*op->func)(_stack, op)) != 0) {
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
		_stk_result              = dt_mat_get_elt_ref(_mat_C, 1, 1);
	}

	rpn_free_stack(_stack);

	return _stk_result;
}

/* dt_op_mat_mat() }}} */
/* dt_op_keep_mat_mat() {{{ */

/******************************************************************************

                         DT_OP_KEEP_MAT_MAT

******************************************************************************/
rpn_elt *dt_op_keep_mat_mat(rpn_elt *mat_Y, rpn_elt *mat_X, dl_operator *op)
{
	rpn_matrix			*_mat_A, *_mat_B, *_mat_C;
	int                       _n, _p, _i, _j, _idx, _retcode;
	size_t				 _size;
	rpn_elt				*_elt, *_stk_result, *_clone;
	rpn_stack				*_stack;

	_mat_A                   = (struct rpn_matrix *) mat_Y->value.obj;
	_mat_B                   = (struct rpn_matrix *) mat_X->value.obj;

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
			_elt      			= dt_mat_get_elt_ref(_mat_A, _i, _j);
			_clone				= rpn_clone_elt(_elt);
			rpn_push(_stack, _clone);

			_elt      			= dt_mat_get_elt_ref(_mat_B, _i, _j);
			_clone				= rpn_clone_elt(_elt);
			rpn_push(_stack, _clone);

			if ((_retcode = (*op->func)(_stack, op)) != 0) {
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
		_stk_result              = dt_mat_get_elt_ref(_mat_C, 1, 1);
	}

	rpn_free_stack(_stack);

	return _stk_result;
}

/* dt_op_keep_mat_mat() }}} */
/* dt_mat_check_dim() {{{ */

/******************************************************************************

					DT_MAT_CHECK_DIM

******************************************************************************/
int dt_mat_check_dim(rpn_elt *mat_Y, rpn_elt *mat_X)
{
	rpn_matrix			*_mat_A, *_mat_B;
	int                       _retcode;

	_mat_A                   = (struct rpn_matrix *) mat_Y->value.obj;
	_mat_B                   = (struct rpn_matrix *) mat_X->value.obj;

	/* Check dimensions : A : n x p;   B : n x p
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if ((_mat_A->n != _mat_B->n)
	||  (_mat_A->p != _mat_B->p)) {
		/* Incompatible dimensions
		   ~~~~~~~~~~~~~~~~~~~~~~~ */
		_retcode                 = RPN_RET_INCOMPATIBLE_ELTS;
	}
	else {
		_retcode				= RPN_RET_OK;
	}

	return _retcode;
}

/* dt_mat_check_dim() }}} */
/* dt_alloc_string() {{{ */

/******************************************************************************

					DT_ALLOC_STRING

******************************************************************************/
char *dt_alloc_string(char *str, int delta)
{
	char					*_str;
	size_t				 _size;

	_size				= strlen(str) + delta;

	if ((_str = RPN_MALLOC(_size)) == 0) {
		rpn_err_msg_no_mem();
		exit(RPN_EXIT_NO_MEM);
	}

	return _str;
}

/* dt_alloc_string() }}} */
/* dt_alloc_string2() {{{ */

/******************************************************************************

					DT_ALLOC_STRING2

******************************************************************************/
char *dt_alloc_string2(char *str1, char *str2, int delta)
{
	char					*_str;
	size_t				 _size;

//Z
//printf("str1 = [%s] str2 = [%s] delta = %d\n", str1, str2, delta);
	_size				= strlen(str1) + strlen(str2) + delta;

//printf("%s(): _size = %d\n", __func__, _size);
	if ((_str = RPN_MALLOC(_size)) == 0) {
		rpn_err_msg_no_mem();
		exit(RPN_EXIT_NO_MEM);
	}

	return _str;
}

/* dt_alloc_string2() }}} */
/* dt_parentheses() {{{ */

/******************************************************************************

					DT_PARENTHESES

******************************************************************************/
char *dt_parentheses(char *str)
{
	char					*_buf;

	_buf					= dt_alloc_string(str, 3);
	sprintf(_buf, "(%s)", str);

	return _buf;
}

/* dt_parentheses() }}} */
// GROUP : Math }}}
