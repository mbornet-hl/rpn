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
 *	@(#)	[MB] ci_test.c	Version 1.10 du 19/09/30 - 
 */


#include	<stdio.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<string.h>

#include	"../cc/cc_types.h"
#include	"../ce/ce_magic.h"
#include	"../cd/cd_cpub.h"
#include	"../ch/ch_cpub.h"
#include	"../ch/ch_epub.h"
#include	"../cf/cf_epub.h"
#include	"ci_cpub.h"
#include	"ci_epub.h"
#include	"ci_cpri.h"
#include	"ci_epri.h"

int		 ci_tab_int[] = {
	10,  7,  9,  6,  8,  5,  2,  4,  3, 11, 16, 14, 15, 12, 13,
	 7,  6, 11, 10,  9,  8,  5,  4, 13, 12,  3, 14, 15,  2, 16,

	10,  7,  9,  6,  8,  5,  2,  4,  3, 11, 16, 14, 15, 12, 13,
	10,  7,  9,  6,  8,  5,  2,  4,  3, 11, 16, 14, 15, 12, 13,

	10,  7,  9,  6,  8,  5,  2,  4,  3, 11, 16, 14, 15, 12, 13,
	 2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16,

	10,  7,  9,  6,  8,  5,  2,  4,  3, 11, 16, 14, 15, 12, 13,
	16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,
};

cc_uint16				 ci_cpt_drop		= 0,
					 ci_H_left		= 0,
					 ci_H_right		= 0;
int					 ci_maximum		= 0;

cc_uint16				 ci_type_parcours	= CI_T_LNR;
int					(*ci_pparc)(ci_ref_root, void (*)(ci_ref_node), cc_uint16);

bool					 ci_liste			= FALSE,
					 ci_type_fifo		= FALSE;

int					 main(int, char *[]);
void					 ci_usage(char *);

/*******************************************************************************

						MAIN

*******************************************************************************/
int main(int argc, char *argv[])
{
	struct ci_root	 _tree;
	ci_ref_node	 _p, _node;
	ci_ref_int	 _elt;
	struct ci_int	 _elt0;
	int			 _i, _c, _tableau, _err, _nb;
	int			 _seed, _seed_set, _random, _max_set, _display_all, _search,
				 _recursif;
	char			*_opts;

	setvbuf(stdout, (char *) 0, _IONBF, 0);

	ci_init_root(&_tree);

	_err			= 0;
	_opts		= "1ade:fhlNn:RrS:s:tu";
	_seed		= 25557;
	_nb			= 512;
	_random		= FALSE;
	_seed_set		= FALSE;
	_search		= FALSE;
	_tableau		= FALSE;
	_max_set		= FALSE;
	_display_all	= FALSE;
	_recursif		= TRUE;
	ci_liste		= FALSE;
	ci_type_fifo	= FALSE;
	while ((_c = getopt(argc, argv, _opts)) != EOF) {
		switch (_c) {

		case	'1':
			ci_liste			= TRUE;
			break;

		case	'a':
			_display_all		= TRUE;
			break;

		case	'e':
			_elt0.i			= atoi(optarg);
			_elt0.node.data	= (fpvoid) &_elt0;
			_search			= TRUE;
			break;

		case	'f':
			_recursif			= FALSE;
			ci_type_fifo		= TRUE;
			break;

		case	'N':
			_recursif			= FALSE;
			break;

		case	'n':
			_nb				= atoi(optarg);
			_max_set			= TRUE;
			break;

		case	'R':
			ci_type_parcours	= CI_T_RNL;
			break;

		case	'r':
			_seed			= getpid();
			_random			= TRUE;
			break;

		case	'S':
			ci_type_parcours	= CI_T_LRN;
			if (optarg[0] == 'r') {
				ci_type_parcours	= CI_T_RLN;
			}
			break;

		case	's':
			_seed			= atoi(optarg);
			_seed_set			= TRUE;
			break;

		case	't':
			_tableau			= TRUE;
			break;

		case	'u':
			_err++;
			break;

		case	'?':
			_err++;
			break;
		}
	}

	if (_tableau && _random) {
		fprintf(stderr, "Options 't' et 'r' incompatibles\n");
		_err++;
	}

	if (_tableau && _seed_set) {
		fprintf(stderr, "Options 't' et 's' incompatibles\n");
		_err++;
	}

	if (_tableau &&_max_set) {
		fprintf(stderr, "Options 't' et 'n' incompatibles\n");
		_err++;
	}

	if (_seed_set && _random) {
		fprintf(stderr, "Options 's' et 'r' incompatibles\n");
		_err++;
	}

	if (_err) {
		ci_usage(argv[0]);
		exit(CD_EXIT_USAGE);
	}

	if (_recursif) {
		ci_pparc		= ci_parcours;
	}
	else if (ci_type_fifo) {
		ci_pparc		= ci_fifo;
	}
	else {
		ci_pparc		= ci__parcours;
	}

	if (_tableau) {
		for (_i = 0; _i < sizeof(ci_tab_int)/sizeof(ci_tab_int[0]); _i++) {
			if ((_elt = (ci_ref_int) CH_MALLOC(sizeof(struct ci_int))) == 0) {
				fprintf(stderr, "Cannot malloc\n");
				exit(CD_EXIT_MALLOC);
			}

			_elt->node.data	= (fpvoid) _elt;
			_elt->indice		= _i;
			_elt->i			= ci_tab_int[_i];

			cf_line(stdout, '*', 79, 1);
			printf("          Recherche de [%10d] ...\n", _elt->i);
			if ((_node = ci_search(&_tree, &_elt->node, ci_cmp)) == 0) {
				if (_display_all) {
					printf("          Pas trouve.\n");
					printf("          Ajout de [%10d] (%10d).\n",
											_elt->i, _elt->indice);
				}
				_p = ci_add_node(&_tree, &_elt->node, ci_cmp, ci_drop);
				if (_p) {
					printf("Element %d deja present\n",
										((ci_ref_int) _p->data)->i);
				}
			}
			else {
				if (_display_all) {
					_elt		= (ci_ref_int) _node->data;
					printf("          Trouve.\n");
					printf("          Suppression de [%10d] (%10d).\n",
											_elt->i, _elt->indice);
					_p = ci_del_node(&_tree, _node);
				}
			}
			cf_line(stdout, '*', 79, 1);

			if (_display_all) {
				printf("\n");
				ci_disp_tree(&_tree, ci_disp_node);
				cf_line(stdout, '=', 79, 2);
			}
		}
	}
	else {
		srand(_seed);
		printf("SEED = %d\n", _seed);
		printf("MAX  = %d\n",   _nb);
		printf("\n");

		for (_i = 0; _i < _nb; _i++) {
			if ((_elt = (ci_ref_int) CH_MALLOC(sizeof(struct ci_int))) == 0) {
				fprintf(stderr, "Cannot malloc\n");
				exit(CD_EXIT_MALLOC);
			}

			_elt->node.data	= (fpvoid) _elt;
			_elt->indice		= _i;
			_elt->i			= rand();

			if (_display_all) {
				printf("********** Ajout de [%10d] (%10d) **********\n",
											_elt->i, _elt->indice);
			}
			_p = ci_add_node(&_tree, &_elt->node, ci_cmp, ci_drop);

			if (_display_all) {
				printf("\n");
				ci_disp_tree(&_tree, ci_disp_node);
			}
		}
	}

	if (!_display_all) ci_disp_tree(&_tree, ci_disp_node);

	if (_search) {
		printf("Elt0 = %p\n", &_elt0);
		_p	= ci_search(&_tree, &_elt0.node, ci_cmp);
		if (_p) {
			printf("Element trouve: %p [%10d] CIndice = %d\n",
			_p, ((ci_ref_int) _p->data)->i, ((ci_ref_int) _p->data)->indice);
		}
		else {
			printf("Element %d non trouve\n", _elt0.i);
		}
	}

	return 0;
}

/*******************************************************************************

						CI_CMP

*******************************************************************************/
cc_uint16 ci_cmp(ci_ref_node e1, ci_ref_node e2)
{
	ci_ref_int	 _i1, _i2;

	_i1		= (ci_ref_int) e1->data;
	_i2		= (ci_ref_int) e2->data;

	if (_i2->i < _i1->i) {
		return CI_CMP_LT;
	}
	if (_i2->i > _i1->i) {
		return CI_CMP_GT;
	}
	ci_cpt_drop++;

	return CI_CMP_EQ;
}

/*******************************************************************************

						CI_DROP

*******************************************************************************/
void ci_drop(ci_ref_node e1, ci_ref_node e2)
{
}

/*******************************************************************************

						CI_DISP_NODE

*******************************************************************************/
void ci_disp_node(ci_ref_node elt)
{
	ci_ref_int		 _i1, _i2;
	char				 _left[20], _right[20], _root, _up[10];
	int				 _diff;

	_i1			= (ci_ref_int) elt->data;
	_left[0]		= 0;
	_right[0]		= 0;
	_root		= ' ';

	if (elt->left) {
		_i2		= (ci_ref_int) elt->left->data;
		sprintf(_left,  "%10d <-", _i2->i);
	}
	if (elt->right) {
		_i2		= (ci_ref_int) elt->right->data;
		sprintf(_right, "-> %10d", _i2->i);
	}
	if (elt->up == 0) {
		_root	= '*';
		strcpy(_up, "");
	}

	switch (elt->where) {

	case	CI_LEFT:
		sprintf(_up, ".%10d ", ((ci_ref_int) elt->up->data)->i);
		break;

	case	CI_RIGHT:
		sprintf(_up, " %10d.", ((ci_ref_int) elt->up->data)->i);
		break;

	default:
		sprintf(_up, "       ");
		break;
	}

	if (elt->H_left > elt->H_right) {
		_diff	= elt->H_left - elt->H_right;
	}
	else {
		_diff	= elt->H_right - elt->H_left;
	}
	printf("[%10d] %10d    (%2d, %2d)  %-14s%c%14s | %10s  %s\n",
				_i1->indice, _i1->i,
				elt->H_left, elt->H_right, _left, _root, _right, _up,
				_diff >= 2 ? "<=====" : "");

	ci_H_left		= MAX(ci_H_left,  elt->H_left);
	ci_H_right	= MAX(ci_H_right, elt->H_right);
}

/*******************************************************************************

						CI_DISP_TREE

*******************************************************************************/
void ci_disp_tree(ci_ref_root tree, void (*disp)(ci_ref_node))
{
	int				 _nb;
	ci_ref_node		 _node;
	ci_ref_int		 _elt;
	struct ci_trek		 _p;
	char				*_global, *_recursif, *_type;

	if (ci_liste) {
		_global	= "elementary";
		_recursif	= "non recursive";
	}
	else {
		_global	= "global";

		if (ci_type_fifo) {
			_recursif	= "FIFO type";
		}
		else {
			_recursif	= "non FIFO type";
		}
	}

	switch (ci_type_parcours) {

	case	CI_T_LNR:
		_type	= "left - node - right";
		break;

	case	CI_T_RNL:
		_type	= "right - node - left";
		break;

	case	CI_T_LRN:
		_type	= "left - right - node";
		break;

	case	CI_T_RLN:
		_type	= "right - left - node";
		break;

	default:
		_type	= "???";
		break;
	}

	printf("\n");
	printf("   Index       Value       (Dl, Dr)      Left              Right          Up\n");
	printf(
"---------------------------------------------------------------------------------\n");
	printf("Parcours %s %s, traitement %s\n", _global, _recursif, _type);

	if (ci_liste) {
		_nb		= 0;
		ci_reset(&_p, tree, ci_type_parcours);
		while ((_node = ci_get_next(&_p))) {
			(*disp)(_node);
			_nb++;
		}
	}
	else {
		_nb	= (*ci_pparc)(tree, disp, ci_type_parcours);
	}

	printf("\n");
	if (_nb >= ci_maximum) {
		printf("Nb elts  = %10d\n", _nb);
	}
	else {
		printf("Nb elts  = %10d   *** Disparition de %d element%s\n",
				_nb, ci_maximum - _nb, ci_maximum - _nb > 1 ? "s" : "");
	}
	ci_maximum	= _nb;
	printf("Drop     = %10d\n", ci_cpt_drop);
	printf("H_left   = %10d\n", ci_H_left);
	printf("H_right  = %10d\n", ci_H_right);

	_elt		= (ci_ref_int) tree->root->data;
	printf("Root     = %10d\n", _elt->i);

	_elt		= (ci_ref_int) tree->first->data;
	printf("First    = %10d\n", _elt->i);

	_elt		= (ci_ref_int) tree->last->data;
	printf("Last     = %10d\n", _elt->i);

	printf("\n");
}

/*******************************************************************************

						CI_USAGE

*******************************************************************************/
void ci_usage(char *progname)
{
	fprintf(stderr,
	"Usage: %s [-1adfhNR][-S a|r][-e valeur][ [-n nb_elts][-r|s] ][-t]\n",
														progname);
	fprintf(stderr, "\t-1 : parcours des elements 1 par 1\n");
	fprintf(stderr, "\t-a : affichage de tous les ajouts\n");
	fprintf(stderr, "\t-e : recherche de l'element specifie\n");
	fprintf(stderr, "\t-f : parcours de type FIFO\n");
	fprintf(stderr, "\t-N : parcours non recursif\n");
	fprintf(stderr, "\t-n : nombre d'elements\n");
	fprintf(stderr, "\t-R : classement inverse\n");
	fprintf(stderr, "\t-r : utilisation de valeurs aleatoires\n");
	fprintf(stderr, "\t-S : parcours symetrique (a = avant, r = arriere)\n");
	fprintf(stderr, "\t-s : graine (entiere) pour les nombres aleatoires\n");
	fprintf(stderr, "\t-t : utilisation d'un tableau de valeurs constantes\n");
}
