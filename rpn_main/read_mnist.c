/*
 * Copyright (C) 2018, Martial Bornet
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
 *	@(#)	[MB] cy_read_mnist.c	Version 1.4 du 18/12/30 - 
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

#include "../df/df_epub.h"

void					disp_image(unsigned char *, int, int);

int main(int argc, char *argv[])
{
	int			 _fd_images, _fd_labels;
	uint32_t		 _magic, _nb_images, _dim1, _dim2, _size,
				 _nb, _nb_labels;
	char			*_train_images, *_train_labels,
				*_test_images,  *_test_labels,
				*_image;

	_train_images		= "../data/train-images-idx3-ubyte";
	_train_labels		= "../data/train-labels-idx1-ubyte";

	_test_images		= "../data/t10k-images-idx3-ubyte";
	_test_labels		= "../data/t10k-labels-idx1-ubyte";

	if ((_fd_images = open(_train_images, O_RDONLY)) < 0) {
		fprintf(stderr, "%s : cannot open \"%s\" !\n", argv[0], _train_images);
		exit(1);
	}

	if (read(_fd_images, &_magic, sizeof(_magic)) < 0) {
		fprintf(stderr, "%s : error while reading magic number !\n", argv[0]);
		exit(1);
	}

	printf("Magic number  = 0x%08x\n", df_convert_uint32(_magic));

	if (read(_fd_images, &_nb_images, sizeof(_nb_images)) < 0) {
		fprintf(stderr, "%s : error while reading images number !\n", argv[0]);
		exit(1);
	}

	printf("Images number = %10d  (0x%08x)\n", df_convert_uint32(_nb_images), df_convert_uint32(_nb_images));

	if (read(_fd_images, &_dim1, sizeof(_dim1)) < 0) {
		fprintf(stderr, "%s : error while reading dimension 1 !\n", argv[0]);
		exit(1);
	}

	if (read(_fd_images, &_dim2, sizeof(_dim2)) < 0) {
		fprintf(stderr, "%s : error while reading dimension 2 !\n", argv[0]);
		exit(1);
	}

	_dim1			= df_convert_uint32(_dim1);
	_dim2			= df_convert_uint32(_dim2);

	printf("Dimension 1 = %10d  (0x%08x)\n", _dim1, _dim1);
	printf("Dimension 2 = %10d  (0x%08x)\n", _dim2, _dim2);

	_size			= _dim1 * _dim2;
	if ((_image = (unsigned char *) malloc(_size)) == NULL) {
		fprintf(stderr, "%s : cannot allocate %d bytes for the images !\n", argv[0], _size);
		exit(1);
	}

	if ((_fd_labels = open(_train_labels, O_RDONLY)) < 0) {
		fprintf(stderr, "%s : cannot open \"%s\" !\n", argv[0], _train_labels);
		exit(1);
	}

	if (read(_fd_labels, &_magic, sizeof(_magic)) < 0) {
		fprintf(stderr, "%s : error while reading magic number !\n", argv[0]);
		exit(1);
	}

	printf("Magic number  = 0x%08x\n", df_convert_uint32(_magic));

	if (read(_fd_labels, &_nb_labels, sizeof(_nb_labels)) < 0) {
		fprintf(stderr, "%s : error while reading labels number !\n", argv[0]);
		exit(1);
	}

	printf("Labels number = %10d  (0x%08x)\n", df_convert_uint32(_nb_labels), df_convert_uint32(_nb_labels));

	for (_nb = 0; _nb < _nb_images; _nb++) {
//		printf("Reading image %10d (size = %10d) ...\n", _nb, _size);

		if (read(_fd_images, _image, _size) <= 0) {
			fprintf(stderr, "%s : error while reading image !\n", argv[0]);
			exit(1);
		}

		printf("Image %10d read (size = %10d) ...\n", _nb, _size);
		disp_image(_image, _dim1, _dim2);
	}

	close(_fd_labels);
	close(_fd_images);

	return 0;
}

void disp_image(unsigned char *image, int rows, int cols)
{
	int				 _i, _j, _k = 0;
	unsigned char		 _val;
	char				 _str[8];

	for (_i = 0; _i < rows; _i++) {
		for (_j = 0; _j < cols; _j++) {
			_val		= image[_k++];
			if (_val != 0) {
				sprintf(_str, "%3d", _val);
			}
			else {
				sprintf(_str, "   ");
			}

			if (_j == 0) {
				printf("%s", _str);
			}
			else {
				printf(" %s", _str);
			}
		}
		printf("\n");
	}

	printf("\n");
}
