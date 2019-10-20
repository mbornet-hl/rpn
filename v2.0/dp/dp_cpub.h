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
 *   @(#)  [MB] dp_cpub.h Version 1.2 du 19/10/19 - 
 */

/* Activation functions
   ~~~~~~~~~~~~~~~~~~~~ */
#define	DP_AF_SIGMOID				"AF_SIGMOID"
#define	DP_AF_TANH				"AF_TANH"
#define	DP_AF_ARC_TAN				"AF_ARC_TAN"
#define	DP_AF_RELU				"AF_RELU"

/* Types
   ~~~~~ */
typedef double                           dp_weight;
typedef double                           dp_input;
typedef double                           dp_output;
typedef struct dp_input_neuron           dp_input_neuron;
typedef struct dp_input_layer            dp_input_layer;
typedef struct dp_neuron                 dp_neuron;
typedef struct dp_perceptron             dp_perceptron;
typedef struct dp_mlp                    dp_mlp;
typedef struct dp_mnist_desc             dp_mnist_desc;
typedef struct dp_training_data          dp_training_data;
typedef struct dp_mnist_set              dp_mnist_set;
typedef struct dp_training_label         dp_training_label;
typedef struct dp_training_elt            dp_training_elt;
typedef struct dp_training_set           dp_training_set;
typedef struct dp_7_seg_digit            dp_7_seg_digit;

/* Input neuron {{{ */
struct dp_input_neuron {
     dp_input                            input;
};

/* Input neuron }}} */
/* Input layer {{{ */
struct dp_input_layer {
     int                                 nb_inputs;
     struct dp_input_neuron             *inputs[0];
};

/* Input layer }}} */
/* Neuron {{{ */
struct dp_neuron {
     dp_output                           output;
     int                                 nb_weights;
     dp_weight                           bias;
     dp_weight                           weights[0];
};

/* Neuron }}} */
/* Perceptron {{{ */
struct dp_perceptron {
     int                                 nb_neurons;
     dp_weight                         (*activation)(dp_weight);
     dp_weight                         (*derivative)(dp_weight);
     struct dp_neuron                   *neurons[0];
};

/* Perceptron }}} */
/* Multi-layer perceptron {{{ */
struct dp_mlp {
     int                                 nb_layers;
     int                                 nb_neurons;
     int                                 nb_weights;
     char                               *str_af;
     dp_weight                         (*activation)(dp_weight);     // TODO : A SUPPRIMER !!!!!!!!!
     char                               *comment;
     char                               *version;
     struct dp_input_layer              *input;
     struct dp_perceptron               *layers[0];
};

/* Multi-layer perceptron }}} */
/* Mnist_module descriptor {{{ */
struct dp_mnist_desc {
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

/* Mnist_module descriptor }}} */
/* Training data {{{ */
struct dp_training_data {
     int                                 index;        // Index of the element in the set
     int                                 orig_index;
     int                                 ignore;
     int                                 nb_elts;
     int                                 elt_size;
     int                                 num_rows,
                                         num_cols;
     void                               *vector;
};

struct dp_mnist_set {
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

/* Training data }}} */
/* Training label {{{ */
struct dp_training_label {
     int                                 index;        // Index of the element in the set
     int                                 value;
};

/* Training label }}} */
/* Training element {{{ */
struct dp_training_elt {
     struct dp_training_data             *data;
     struct dp_training_label            *label;
};

/* Training element }}} */
/* Training set {{{ */
struct dp_training_set {
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

/* Training set }}} */
/* 7 segments digit {{{ */
struct dp_7_seg_digit {
     int                                  digit[RPN_MNIST_ROWS * RPN_MNIST_COLS];
};

/* 7 segments digit }}} */
