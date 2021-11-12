Powerful RPN interpreter
========================

Reverse Polish Notation interpreter

This RPN interpreter is written in C.

It has been modularized so that it can use dynamic libraries (plug-ins) such as :
- core (core functions)
- math (mathematical functions)
- stats (statistical functions)
- strings (strings manipulation functions)
- system (system functions)
- json (JSON output functions)
- ai (artificial intelligence functions : MNIST manipulation currently)

These modules currently contain 129 different operators and 356 different definitions of these operators (see catalog below).
And there are more to come ...

This program manipulates various element types :
- integer,
- floating point numbers,
- strings,
- simple litterals,
- vectors,
- matrices,
- lists,
- texts,                       [work in progress]
- the MNIST database of Yann Le Cun
- Multi-Layer Perceptron (MLP) [work in progress]
- and soon many others.

Operators can be overloaded : for example, the + operator is an addition with integer and double operands (in the math module), but is a concatenation with string operands (in the strings module), and the * operator is a multiplication with integer and double operands, but means duplicate with a string and an integer.

Here is an example of some hand written digits from the MNIST database :
![MNIST_sample](https://raw.githubusercontent.com/mbornet-hl/MNIST/master/IMAGES/GROUPS/mnist_v5_MNIST_29001-30000_25x40.png)

The commands used to generate the 60 images of 1,000 digits each for the training set are :
```
./rpn <<- EOF
	read_mnist
	1 60000
	mnist_pics
EOF
```

which roughly takes 1m35s on my PC (1m49s with a label on each graph).

And here is an image which contains only the digit '5' :
![MNIST_sample_5](https://raw.githubusercontent.com/mbornet-hl/MNIST/master/IMAGES/GROUPS/mnist_v5_MNIST-5_04001-05000_25x40.png)

For those who prefer to classify fashion items rather than digits, RPN can manipulate FASHION-MNIST as well (see https://github.com/zalandoresearch/fashion-mnist).
A manual replacement of MNIST data files with FASHION-MNIST data files allows us to generate images of the contents of the base.
Here are two examples of FASHION-MNIST images :

Example 1 :
![FASHION-MNIST_example_1](https://raw.githubusercontent.com/mbornet-hl/MNIST/master/IMAGES-FASHION/GROUPS/mnist_v5_MNIST_TESTS_01001-02000_25x40.png)

Example 2 :
![FASHION_MNIST_example_2](https://raw.githubusercontent.com/mbornet-hl/MNIST/master/IMAGES-FASHION/GROUPS/mnist_v5_MNIST_TESTS-0_00001-01000_25x40.png)

You can also use RPN to compute with litteral matrices :
```
RPN> import core
RPN> import math
RPN> 'a11' 'a12' 'a13' 'a21' 'a22' 'a23' 'a31' 'a32' 'a33' 3 3 mat
         3mat
MATRIX    [3 x 3]
|  'a11'   'a12'   'a13'  |
|  'a21'   'a22'   'a23'  |
|  'a31'   'a32'   'a33'  |
            ***
RPN> 'b11' 'b12' 'b13' 'b21' 'b22' 'b23' 'b31' 'b32' 'b33' 3 3 mat
         3mat
MATRIX    [3 x 3]
|  'b11'   'b12'   'b13'  |
|  'b21'   'b22'   'b23'  |
|  'b31'   'b32'   'b33'  |
            ***
RPN> m*
MATRIX    [3 x 3]
|  'a11.b11 + a12.b21 + a13.b31'   'a11.b12 + a12.b22 + a13.b32'   'a11.b13 + a12.b23 + a13.b33'  |
|  'a21.b11 + a22.b21 + a23.b31'   'a21.b12 + a22.b22 + a23.b32'   'a21.b13 + a22.b23 + a23.b33'  |
|  'a31.b11 + a32.b21 + a33.b31'   'a31.b12 + a32.b22 + a33.b32'   'a31.b13 + a32.b23 + a33.b33'  |
            ***
RPN>

RPN> 'a1' 'a2' 'a3' vec3
 'a3' vec3
MATRIX    [3 x 1]
|  'a1'  |
|  'a2'  |
|  'a3'  |
            ***
RPN> t
MATRIX    [1 x 3]
|  'a1'   'a2'   'a3'  |
            ***
RPN> 'b1' 'b2' 'b3' vec3
 'b3' vec3
MATRIX    [3 x 1]
|  'b1'  |
|  'b2'  |
|  'b3'  |
            ***
RPN> m*
 'a1.b1 + a2.b2 + a3.b3'             ***
RPN>

```

Here is the current catalog of operators :
```
RPN> catalog
===== 10 imported modules : =====
[ai]             2.0   /  439    Artificial intelligence
[core]           2.0   /  508    Core functions
[hosts]          2.0   /  384    Hosts files manipulation
[json]           2.0   /  408    JSON conversions
[math]           2.0   /  686    Mathematical functions
[regex]          2.0   /  277    Regular expressions management
[stats]          2.0   /  378    Statistics functions
[strings]        2.0   /  341    Strings functions
[system]         2.0   /  988    System tools
[webstats]       2.0   /  218    Web server tools

=====  129 imported operators (356 definitions) : =====
[%]                     9 definitions
     INT           INT           UNUSED        UNUSED          (1) [math]
     DOUBLE        INT           UNUSED        UNUSED          (4) [math]
     MATRIX        INT           UNUSED        UNUSED          (7) [math]
     INT           DOUBLE        UNUSED        UNUSED          (2) [math]
     DOUBLE        DOUBLE        UNUSED        UNUSED          (5) [math]
     MATRIX        DOUBLE        UNUSED        UNUSED          (8) [math]
     INT           MATRIX        UNUSED        UNUSED          (3) [math]
     DOUBLE        MATRIX        UNUSED        UNUSED          (6) [math]
     MATRIX        MATRIX        UNUSED        UNUSED          (9) [math]

[*]                    13 definitions
     INT           INT           UNUSED        UNUSED          (1) [math]     Multiply Y by X
     DOUBLE        INT           UNUSED        UNUSED          (4) [math]     Multiply Y by X
     MATRIX        INT           UNUSED        UNUSED          (7) [math]
     INT           DOUBLE        UNUSED        UNUSED          (2) [math]     Multiply Y by X
     DOUBLE        DOUBLE        UNUSED        UNUSED          (5) [math]     Multiply Y by X
     MATRIX        DOUBLE        UNUSED        UNUSED          (8) [math]
     INT           STRING        UNUSED        UNUSED          (1) [strings]  Concatenate string Y X times
     INT           MATRIX        UNUSED        UNUSED          (3) [math]
     DOUBLE        MATRIX        UNUSED        UNUSED          (6) [math]
     MATRIX        MATRIX        UNUSED        UNUSED          (9) [math]     Hadamard : multiply matrix Y by matrix X
     LITTERAL      MATRIX        UNUSED        UNUSED         (11) [math]
     MATRIX        LITTERAL      UNUSED        UNUSED         (12) [math]
     LITTERAL      LITTERAL      UNUSED        UNUSED         (10) [math]

[+]                    16 definitions
     INT           INT           UNUSED        UNUSED          (1) [math]     Add X to Y
     DOUBLE        INT           UNUSED        UNUSED          (5) [math]     Add X to Y
     MATRIX        INT           UNUSED        UNUSED          (8) [math]
     LITTERAL      INT           UNUSED        UNUSED         (11) [math]     Add X to Y
     INT           DOUBLE        UNUSED        UNUSED          (2) [math]     Add X to Y
     DOUBLE        DOUBLE        UNUSED        UNUSED          (6) [math]     Add X to Y
     MATRIX        DOUBLE        UNUSED        UNUSED          (9) [math]
     STRING        STRING        UNUSED        UNUSED          (1) [strings]  Concatenate string X to string Y
     INT           MATRIX        UNUSED        UNUSED          (3) [math]
     DOUBLE        MATRIX        UNUSED        UNUSED          (7) [math]
     MATRIX        MATRIX        UNUSED        UNUSED         (10) [math]     Hadamard : add matrix X to matrix Y
     LITTERAL      MATRIX        UNUSED        UNUSED         (13) [math]
     INT           LITTERAL      UNUSED        UNUSED          (4) [math]     Add X to Y
     MATRIX        LITTERAL      UNUSED        UNUSED         (14) [math]
     LITTERAL      LITTERAL      UNUSED        UNUSED         (12) [math]
     LIST          LIST          UNUSED        UNUSED          (1) [core]     Concatenate list X to list Y

[-]                    12 definitions
     INT           INT           UNUSED        UNUSED          (1) [math]     Substract X from Y
     DOUBLE        INT           UNUSED        UNUSED          (4) [math]     Substract X from Y
     MATRIX        INT           UNUSED        UNUSED          (7) [math]
     INT           DOUBLE        UNUSED        UNUSED          (2) [math]     Substract X from Y
     DOUBLE        DOUBLE        UNUSED        UNUSED          (5) [math]     Substract X from Y
     MATRIX        DOUBLE        UNUSED        UNUSED          (8) [math]
     INT           MATRIX        UNUSED        UNUSED          (3) [math]
     DOUBLE        MATRIX        UNUSED        UNUSED          (6) [math]
     MATRIX        MATRIX        UNUSED        UNUSED          (9) [math]     Hadamard : substract matrix X from matrix Y
     LITTERAL      MATRIX        UNUSED        UNUSED         (11) [math]
     MATRIX        LITTERAL      UNUSED        UNUSED         (12) [math]
     LITTERAL      LITTERAL      UNUSED        UNUSED         (10) [math]

[/]                    12 definitions
     INT           INT           UNUSED        UNUSED          (1) [math]
     DOUBLE        INT           UNUSED        UNUSED          (4) [math]
     MATRIX        INT           UNUSED        UNUSED          (7) [math]
     INT           DOUBLE        UNUSED        UNUSED          (2) [math]
     DOUBLE        DOUBLE        UNUSED        UNUSED          (5) [math]
     MATRIX        DOUBLE        UNUSED        UNUSED          (8) [math]
     INT           MATRIX        UNUSED        UNUSED          (3) [math]
     DOUBLE        MATRIX        UNUSED        UNUSED          (6) [math]
     MATRIX        MATRIX        UNUSED        UNUSED          (9) [math]     Hadamard : divide matrix Y by matrix X
     LITTERAL      MATRIX        UNUSED        UNUSED         (11) [math]
     MATRIX        LITTERAL      UNUSED        UNUSED         (12) [math]
     LITTERAL      LITTERAL      UNUSED        UNUSED         (10) [math]

[1/x]                   4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]

[10^x]                  4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]

[^]                     1 definition
     MATRIX        MATRIX        UNUSED        UNUSED          (1) [math]

[acos]                  4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]     Arc cosine of X
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]     Arc cosine of X
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]     Arc cosine of X

[acosh]                 4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]

[asin]                  4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]     Arc sine of X
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]     Arc sine of X
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]     Arc sine of X

[asinh]                 4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]

[atan]                  4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]     Arc tangent of X
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]     Arc tangent of X
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]     Arc tangent of X

[atanh]                 4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]

[better_units]          1 definition
     MATRIX        UNUSED        UNUSED        UNUSED          (1) [system]   Transforms bps into kbps, Mbps, Gbps

[cat]                   2 definitions
     STRING        STRING        UNUSED        UNUSED          (1) [strings]  Concatenate string X to string Y
     LIST          LIST          UNUSED        UNUSED          (1) [core]     Concatenate list X to list Y

[chs]                   2 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [core]     Change sign
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [core]     Change sign

[clone]                 5 definitions
     TRAINING_ELT  UNUSED        UNUSED        UNUSED          (1) [ai]
     TRAINING_SET  UNUSED        UNUSED        UNUSED          (3) [ai]
     TEST_ELT      UNUSED        UNUSED        UNUSED          (2) [ai]
     TEST_SET      UNUSED        UNUSED        UNUSED          (4) [ai]
     MIN_MAX       UNUSED        UNUSED        UNUSED          (1) [core]     Clone min_max element

[clst]                  1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Clear stack

[clx]                   1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Clear X register

[coef_a_b]              1 definition
     DOUBLE        DOUBLE        UNUSED        UNUSED          (1) [core]     Convert Y and X into a coef_a_b element

[cos]                   4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]     Cosine of X
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]     Cosine of X
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]     Cosine of X

[cosh]                  4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]

[create_mlp]            1 definition
     LITTERAL      INT           UNUSED        UNUSED          (1) [ai]

[del_l]                 1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Delete lastx element

[delta%]                9 definitions
     INT           INT           UNUSED        UNUSED          (1) [math]
     DOUBLE        INT           UNUSED        UNUSED          (4) [math]
     MATRIX        INT           UNUSED        UNUSED          (7) [math]
     INT           DOUBLE        UNUSED        UNUSED          (2) [math]
     DOUBLE        DOUBLE        UNUSED        UNUSED          (5) [math]
     MATRIX        DOUBLE        UNUSED        UNUSED          (8) [math]
     INT           MATRIX        UNUSED        UNUSED          (3) [math]
     DOUBLE        MATRIX        UNUSED        UNUSED          (6) [math]
     MATRIX        MATRIX        UNUSED        UNUSED          (9) [math]

[delx]                  1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Delete X register content

[diagmat]               1 definition
     INT           ANY_TYPE      ANY_TYPE      UNUSED          (1) [math]     Create an X x X matrix, Z on the diagonal, Y elsewhere

[diff]                  2 definitions
     HOSTSFILE     HOSTSFILE     UNUSED        UNUSED          (1) [hosts]    Display differences between two hosts files
     HOSTS         HOSTS         UNUSED        UNUSED          (2) [hosts]    Display differences between two hosts files

[dim]                   4 definitions
     NIL           UNUSED        UNUSED        UNUSED          (1) [math]
     INT           UNUSED        UNUSED        UNUSED          (2) [math]
     DOUBLE        UNUSED        UNUSED        UNUSED          (3) [math]
     MATRIX        UNUSED        UNUSED        UNUSED          (4) [math]

[disp]                  1 definition
     HOSTS         UNUSED        UNUSED        UNUSED          (1) [hosts]    Display contents of an HOSTS element

[disp_name]             1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Display the name of X

[dispatch]              2 definitions
     TRAINING_SET  UNUSED        UNUSED        UNUSED          (1) [ai]
     TEST_SET      UNUSED        UNUSED        UNUSED          (2) [ai]

[double]                1 definition
     INT           UNUSED        UNUSED        UNUSED          (1) [core]     Cast int to double

[dump]                  1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Dump element X

[dupl]                  1 definition
     INT           STRING        UNUSED        UNUSED          (1) [strings]  Concatenate string Y X times

[dupx]                  1 definition
     INT           ANY_TYPE      UNUSED        UNUSED          (1) [core]     Creates X copies of Y in the stack

[enter]                 1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Copy element X in Y and extend the stack upward

[ere]                   4 definitions
     NIL           UNUSED        UNUSED        UNUSED          (1) [regex]    Returns extended regex global flag
     REGEX         UNUSED        UNUSED        UNUSED          (4) [regex]    Recompile regex using extended regex global flag
     INT           NIL           UNUSED        UNUSED          (2) [regex]    Initialize extended regex global flag
     INT           REGEX         UNUSED        UNUSED          (3) [regex]    Recompile regex with modified extended regex flag

[exp]                   4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]

[exp_data_set]          1 definition
     INT           DOUBLE        MIN_MAX       COEF_A_B        (1) [stats]

[expl]                  6 definitions
     MATRIX        UNUSED        UNUSED        UNUSED          (1) [math]
     LIST          UNUSED        UNUSED        UNUSED          (1) [core]     Explode list element
     TUPLE         UNUSED        UNUSED        UNUSED          (2) [core]     Explode tuple element
     OPAIR         UNUSED        UNUSED        UNUSED          (3) [core]     Explode opair element
     COEF_A_B      UNUSED        UNUSED        UNUSED          (4) [core]     Explode coef_a_b element
     MIN_MAX       UNUSED        UNUSED        UNUSED          (5) [core]     Explode min_max element

[filename]              2 definitions
     STRING        UNUSED        UNUSED        UNUSED          (1) [core]     Convert string to filename
     LITTERAL      UNUSED        UNUSED        UNUSED          (2) [core]     Convert litteral to filename

[get]                   4 definitions
     TRAINING_SET  UNUSED        UNUSED        UNUSED          (1) [ai]
     TEST_SET      UNUSED        UNUSED        UNUSED          (2) [ai]
     INT           TRAINING_SET  UNUSED        UNUSED          (3) [ai]
     INT           TEST_SET      UNUSED        UNUSED          (4) [ai]

[get_debug]             1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [webstats] Get debug level for webstats module

[hcat]                  1 definition
     MATRIX        MATRIX        UNUSED        UNUSED          (1) [math]

[hostsfile]             1 definition
     FILENAME      UNUSED        UNUSED        UNUSED          (1) [hosts]    Convert a filename into a hosts filename

[hrev]                  1 definition
     MATRIX        UNUSED        UNUSED        UNUSED          (1) [math]

[icase]                 4 definitions
     NIL           UNUSED        UNUSED        UNUSED          (1) [regex]    Returns icase global flag
     REGEX         UNUSED        UNUSED        UNUSED          (4) [regex]    Recompile regex using icase global flag
     INT           NIL           UNUSED        UNUSED          (2) [regex]    Initialize icase global flag
     INT           REGEX         UNUSED        UNUSED          (3) [regex]    Recompile regex with modified icase flag

[ignore]                2 definitions
     TRAINING_ELT  UNUSED        UNUSED        UNUSED          (1) [ai]       Ignore specified element
     INT           TRAINING_SET  UNUSED        UNUSED          (2) [ai]       Ignore specified element

[index]                 4 definitions
     TRAINING_ELT  UNUSED        UNUSED        UNUSED          (1) [ai]
     TRAINING_SET  UNUSED        UNUSED        UNUSED          (3) [ai]
     TEST_ELT      UNUSED        UNUSED        UNUSED          (2) [ai]
     TEST_SET      UNUSED        UNUSED        UNUSED          (4) [ai]

[int]                   1 definition
     DOUBLE        UNUSED        UNUSED        UNUSED          (1) [core]     Cast double to int

[ip]                    1 definition
     INT           UNUSED        UNUSED        UNUSED          (1) [core]     Cast int to IPv4

[iperf]                 4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [system]
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [system]
     FILENAME      UNUSED        UNUSED        UNUSED          (2) [system]
     OPAIR         UNUSED        UNUSED        UNUSED          (4) [system]

[label]                 2 definitions
     TRAINING_ELT  UNUSED        UNUSED        UNUSED          (1) [ai]
     TEST_ELT      UNUSED        UNUSED        UNUSED          (2) [ai]

[lastx]                 1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Recall lastx element

[length]                1 definition
     STRING        UNUSED        UNUSED        UNUSED          (1) [strings]  Replace X with length of string in X

[line_data_set]         1 definition
     INT           DOUBLE        MIN_MAX       COEF_A_B        (1) [stats]

[list]                  1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Convert elements between BEGIN and X into a list

[litteral]              2 definitions
     STRING        UNUSED        UNUSED        UNUSED          (1) [core]     Convert string to litteral
     FILENAME      UNUSED        UNUSED        UNUSED          (2) [core]     Convert filename to litteral

[ln]                    4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]

[load]                  2 definitions
     STRING        UNUSED        UNUSED        UNUSED          (1) [core]     Load commands from file X
     FILENAME      UNUSED        UNUSED        UNUSED          (2) [core]     Load commands from file X

[log]                   4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]

[log2mat]               1 definition
     FILENAME      UNUSED        UNUSED        UNUSED          (1) [webstats] Convert a WWW logfile into a matrix

[log_data_set]          1 definition
     INT           DOUBLE        MIN_MAX       COEF_A_B        (1) [stats]

[m*]                    7 definitions
     INT           INT           UNUSED        UNUSED          (1) [math]
     DOUBLE        INT           UNUSED        UNUSED          (4) [math]
     INT           DOUBLE        UNUSED        UNUSED          (2) [math]
     DOUBLE        DOUBLE        UNUSED        UNUSED          (5) [math]
     INT           MATRIX        UNUSED        UNUSED          (3) [math]
     DOUBLE        MATRIX        UNUSED        UNUSED          (6) [math]
     MATRIX        MATRIX        UNUSED        UNUSED          (7) [math]

[mat]                   3 definitions
     TRAINING_ELT  UNUSED        UNUSED        UNUSED          (1) [ai]
     TEST_ELT      UNUSED        UNUSED        UNUSED          (2) [ai]
     INT           INT           UNUSED        UNUSED          (1) [math]

[mat7seg]               1 definition
     INT           UNUSED        UNUSED        UNUSED          (1) [ai]

[match]                 4 definitions
     REGEX         INT           UNUSED        UNUSED          (1) [regex]    Select int matching regex (NIL when not matching)
     REGEX         STRING        UNUSED        UNUSED          (2) [regex]    Select string matching regex (NIL when not matching)
     REGEX         LIST          UNUSED        UNUSED          (3) [regex]    Select elements from list matching regex
     REGEX         TEXT          UNUSED        UNUSED          (4) [regex]    Select elements from text matching regex

[mean]                  1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [stats]

[meanx]                 2 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [stats]
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [stats]

[mem]                   1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [system]

[min_max]               1 definition
     DOUBLE        DOUBLE        UNUSED        UNUSED          (1) [core]     Convert Y and X into a min_max element

[mmm]                   1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [stats]

[mnist_pic]             2 definitions
     TRAINING_ELT  UNUSED        UNUSED        UNUSED          (1) [ai]
     TEST_ELT      UNUSED        UNUSED        UNUSED          (2) [ai]

[mnist_pics]            4 definitions
     TRAINING_SET  UNUSED        UNUSED        UNUSED          (1) [ai]
     TEST_SET      UNUSED        UNUSED        UNUSED          (2) [ai]
     INT           INT           TRAINING_SET  UNUSED          (3) [ai]
     INT           INT           TEST_SET      UNUSED          (4) [ai]

[name]                  1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Push the name of X on the stack

[newline]               4 definitions
     NIL           UNUSED        UNUSED        UNUSED          (1) [regex]    Returns newline global flag
     REGEX         UNUSED        UNUSED        UNUSED          (4) [regex]    Recompile regex using newline global flag
     INT           NIL           UNUSED        UNUSED          (2) [regex]    Initialize newline global flag
     INT           REGEX         UNUSED        UNUSED          (3) [regex]    Recompile regex with modified newline flag

[nosub]                 4 definitions
     NIL           UNUSED        UNUSED        UNUSED          (1) [regex]    Returns nosub global flag
     REGEX         UNUSED        UNUSED        UNUSED          (4) [regex]    Recompile regex using nosub global flag
     INT           NIL           UNUSED        UNUSED          (2) [regex]    Initialize nosub global flag
     INT           REGEX         UNUSED        UNUSED          (3) [regex]    Recompile regex with modified nosub flag

[notbol]                2 definitions
     NIL           UNUSED        UNUSED        UNUSED          (1) [regex]    Returns notbol global flag
     INT           UNUSED        UNUSED        UNUSED          (2) [regex]    Initialize notbol global flag

[noteol]                2 definitions
     NIL           UNUSED        UNUSED        UNUSED          (1) [regex]    Returns noteol global flag
     INT           UNUSED        UNUSED        UNUSED          (2) [regex]    Initialize noteol global flag

[orig_index]            4 definitions
     TRAINING_ELT  UNUSED        UNUSED        UNUSED          (1) [ai]
     TRAINING_SET  UNUSED        UNUSED        UNUSED          (3) [ai]
     TEST_ELT      UNUSED        UNUSED        UNUSED          (2) [ai]
     TEST_SET      UNUSED        UNUSED        UNUSED          (4) [ai]

[pair]                  1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Create a pair with Y and X

[pause]                 1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [system]

[ping]                  1 definition
     MATRIX        UNUSED        UNUSED        UNUSED          (1) [system]

[pop]                   1 definition
     LIST          UNUSED        UNUSED        UNUSED          (1) [core]     Pop first element of a list

[pow_data_set]          1 definition
     INT           DOUBLE        MIN_MAX       COEF_A_B        (1) [stats]

[prstk]                 1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Display elements of the stack (without types)

[prx]                   1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Display X

[push]                  1 definition
     ANY_TYPE      LIST          UNUSED        UNUSED          (1) [core]     Push X at the end of a list

[rcl_x]                 1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Recall X

[rdn]                   1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Roll the stack down

[read_mnist]            1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [ai]

[regex]                 3 definitions
     NIL           UNUSED        UNUSED        UNUSED          (1) [regex]    Display global flags
     STRING        UNUSED        UNUSED        UNUSED          (2) [regex]    Convert string to regular expression
     REGEX         UNUSED        UNUSED        UNUSED          (3) [regex]    Recompile regex using global flags

[rup]                   1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Roll the stack up

[sdev]                  1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [stats]    Standard deviation

[seq]                   1 definition
     INT           INT           UNUSED        UNUSED          (1) [math]

[seq2]                  2 definitions
     MIN_MAX       MIN_MAX       STRING        UNUSED          (1) [math]
     MIN_MAX       MIN_MAX       LITTERAL      UNUSED          (2) [math]

[set_debug]             1 definition
     INT           UNUSED        UNUSED        UNUSED          (1) [webstats] Set debug level for webstats module

[set_name]              2 definitions
     NIL           ANY_TYPE      UNUSED        UNUSED          (1) [core]     Clear name of element Y
     STRING        ANY_TYPE      UNUSED        UNUSED          (2) [core]     Set name of element Y

[sigma+]                4 definitions
     INT           INT           UNUSED        UNUSED          (1) [stats]
     DOUBLE        INT           UNUSED        UNUSED          (3) [stats]
     INT           DOUBLE        UNUSED        UNUSED          (2) [stats]
     DOUBLE        DOUBLE        UNUSED        UNUSED          (4) [stats]

[sigma-]                4 definitions
     INT           INT           UNUSED        UNUSED          (1) [stats]
     DOUBLE        INT           UNUSED        UNUSED          (3) [stats]
     INT           DOUBLE        UNUSED        UNUSED          (2) [stats]
     DOUBLE        DOUBLE        UNUSED        UNUSED          (4) [stats]

[sigmoid]               4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]

[sin]                   4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]     Sine of X
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]     Sine of X
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]     Sine of X

[sinh]                  4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]

[sqrt]                  4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]

[stk]                   1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Display elements of the stack (with types)

[string]                3 definitions
     LITTERAL      UNUSED        UNUSED        UNUSED          (2) [core]     Convert litteral to string
     FILENAME      UNUSED        UNUSED        UNUSED          (1) [core]     Convert filename to string
     REGEX         UNUSED        UNUSED        UNUSED          (1) [regex]    Convert regular expression to string

[sw_off]                1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Disable stopwatch (operators timing)

[sw_on]                 1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Enable stopwatch (operators timing)

[t]                     1 definition
     MATRIX        UNUSED        UNUSED        UNUSED          (1) [math]

[tan]                   4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]     Tangent of X
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]     Tangent of X
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]     Tangent of X

[tanh]                  4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]

[tuple]                 1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Convert elements between BEGIN and X into a tuple

[types]                 1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Display types

[use]                   2 definitions
     TRAINING_ELT  UNUSED        UNUSED        UNUSED          (1) [ai]       Ignore specified element
     INT           TRAINING_SET  UNUSED        UNUSED          (2) [ai]       Ignore specified element

[vcat]                  1 definition
     MATRIX        MATRIX        UNUSED        UNUSED          (1) [math]

[vec2]                  4 definitions
     INT           INT           UNUSED        UNUSED          (1) [math]
     DOUBLE        INT           UNUSED        UNUSED          (2) [math]
     INT           DOUBLE        UNUSED        UNUSED          (3) [math]
     DOUBLE        DOUBLE        UNUSED        UNUSED          (4) [math]

[vec3]                  9 definitions
     INT           INT           INT           UNUSED          (1) [math]
     DOUBLE        INT           INT           UNUSED          (5) [math]
     INT           DOUBLE        INT           UNUSED          (3) [math]
     DOUBLE        DOUBLE        INT           UNUSED          (7) [math]
     INT           INT           DOUBLE        UNUSED          (2) [math]
     DOUBLE        INT           DOUBLE        UNUSED          (6) [math]
     INT           DOUBLE        DOUBLE        UNUSED          (4) [math]
     DOUBLE        DOUBLE        DOUBLE        UNUSED          (8) [math]
     LITTERAL      LITTERAL      LITTERAL      UNUSED          (9) [math]

[vrev]                  1 definition
     MATRIX        UNUSED        UNUSED        UNUSED          (1) [math]

[write]                 2 definitions
     FILENAME      UNUSED        UNUSED        UNUSED          (1) [core]     Write Y to file X
     TEXT_FILE     UNUSED        UNUSED        UNUSED          (2) [core]     Write text file

[write_json]            7 definitions
     NIL           UNUSED        UNUSED        UNUSED          (4) [json]     Write NIL in JSON format
     DOUBLE        UNUSED        UNUSED        UNUSED          (1) [json]     Write double X in JSON format
     STRING        UNUSED        UNUSED        UNUSED          (7) [json]     Write string X in JSON format
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [json]     Write matrix X in JSON format
     FILENAME      UNUSED        UNUSED        UNUSED          (2) [json]     Write Y to file specified by X
     LIST          UNUSED        UNUSED        UNUSED          (5) [json]     Write list X in JSON format
     OPAIR         UNUSED        UNUSED        UNUSED          (6) [json]     Write opair X in JSON format

[x<>l]                  1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Swap X and LASTX

[x<>t]                  1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Swap X and T

[x<>y]                  1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Swap X and Y

[x<>z]                  1 definition
     UNUSED        UNUSED        UNUSED        UNUSED          (1) [core]     Swap X and Z

[x^2]                   4 definitions
     INT           UNUSED        UNUSED        UNUSED          (1) [math]
     DOUBLE        UNUSED        UNUSED        UNUSED          (2) [math]
     MATRIX        UNUSED        UNUSED        UNUSED          (3) [math]
     LITTERAL      UNUSED        UNUSED        UNUSED          (4) [math]

[y^x]                   9 definitions
     INT           INT           UNUSED        UNUSED          (1) [math]
     DOUBLE        INT           UNUSED        UNUSED          (4) [math]
     MATRIX        INT           UNUSED        UNUSED          (7) [math]
     INT           DOUBLE        UNUSED        UNUSED          (2) [math]
     DOUBLE        DOUBLE        UNUSED        UNUSED          (5) [math]
     MATRIX        DOUBLE        UNUSED        UNUSED          (8) [math]
     INT           MATRIX        UNUSED        UNUSED          (3) [math]
     DOUBLE        MATRIX        UNUSED        UNUSED          (6) [math]
     MATRIX        MATRIX        UNUSED        UNUSED          (9) [math]

[zmat]                  1 definition
     INT           INT           ANY_TYPE      UNUSED          (1) [math]

```
