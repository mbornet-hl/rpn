Powerful RPN interpreter
========================

Reverse Polish Notation interpreter

This RPN interpreter is written in C.

This program manipulates various element types :
- integer,
- floating point numbers,
- strings,
- simple litterals,
- vectors,
- matrices,
- lists,
- texts,                       [work in progress]
- the MNIST database of Yann Le Cun,
- Multi-Layer Perceptron (MLP) [work in progress]
- and soon many others.

Operators can be overloaded.

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
