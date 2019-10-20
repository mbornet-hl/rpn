/* ============================================================================
 *	(C) Copyright CS Telecom, 1995.
 *
 *	Auteur       :     Martial BORNET (MB) - 22 Decembre 1995
 *
 *	Description  :     Gestion des FIFOs
 *
 *	Fichier      :     rm_fifo.c
 *
 *	@(#)	rm_fifo.c	2.7	07/03/26	V2	(C) Copyright RCE, 1985-1993.
 *
 *	Liste des fonctions de ce fichier :
 *	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *	- rm_init_link			[Initialisation d'un lien de FIFO			]
 *	- rm_init_fifo			[Initialisation d'une FIFO				]
 *	- rm_get_fifo			[Extraction d'un element d'une FIFO		]
 *	- rm_put_fifo			[Placement d'un element dans une FIFO		]
 *	- rm_read_elt_fifo		[Lecture de l'element "n" d'une FIFO		]
 *	- rm_read_fifo			[Parcours d'une FIFO					]
 * ============================================================================
 */

#include	"../by/by_type.h"

#include	<stdio.h>

#include	"../on/on_cpub.h"
#include	"../on/on_epub.h"

#include	"../bz/bz_cpub.h"
#include	"../bz/bz_epub.h"
#include	"rm_cpub.h"
#include	"rm_epub.h"

/*******************************************************************************

						RM_INIT_LINK

*******************************************************************************/
#if ! defined(NO_TYPE_CHECK)
void rm_init_link(rm_ref_elt elt)
#else
void rm_init_link(elt)
rm_ref_elt		 elt;
#endif
{
	elt->data		= 0;
	elt->next		= 0;
}

/*******************************************************************************

						RM_INIT_FIFO

*******************************************************************************/
#if ! defined(NO_TYPE_CHECK)
void rm_init_fifo(rm_ref_fifo fifo, char *name, void (*init_elt)(rm_ref_elt))
#else
void rm_init_fifo(fifo, name, init_elt)
rm_ref_fifo	 fifo;
char			*name;
void			(*init_elt)();
#endif
{
	if (rm_debug & RM_TRACE_FIFO) {
		Printf("Init_fifo(\"%s\" 0x%x)\n", name, fifo);
	}
	fifo->extract	=
	fifo->insert	= 0;
	fifo->nb		= 0;
	fifo->name	= name;
	fifo->init_elt	= init_elt;
}

/*******************************************************************************

						RM_GET_FIFO

*******************************************************************************/
#if ! defined(NO_TYPE_CHECK)
rm_ref_elt rm_get_fifo(rm_ref_fifo fifo, rm_ref_elt elt, bool trace)
#else
rm_ref_elt rm_get_fifo(fifo, elt, trace)
rm_ref_fifo		 fifo;
rm_ref_elt		 elt;
bool				 trace;
#endif
{
	/**************************************************************************
	 Cette fonction retourne l'adresse du prochain element de la FIFO, ou 0 si
	 la FIFO est vide.
	**************************************************************************/
	rm_ref_elt		 _elt;
	rm_ref_elt		 _previous;

#if ! defined(RM_NO_IT)
	int				 _priority;
#endif

	if ((rm_debug & RM_TRACE_FIFO) && trace) {
		Printf(">> Get_fifo(\"%s\" 0x%x, 0x%x): [%3d]\n",
									fifo->name, fifo, elt, fifo->nb);
	}

	/* Initialisation du pointeur vers l'element precedent
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	_previous	=  0;

#if ! defined(RM_NO_IT)
	/* Masquage des IT
	   ~~~~~~~~~~~~~~~ */
	_priority = jb_masquage_it();
#endif

	if (elt == 0) {
		/* Extraction du 1er element de la FIFO
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		if ((_elt = fifo->extract) == 0) {
#if ! defined(RM_NO_IT)
			/* FIFO vide: demasquage des interruptions
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			jb_demasquage_it(_priority);
#endif
			_elt		= 0;
			goto fin;
		}

		fifo->extract	= _elt->next;
		_elt->next	= 0;
		fifo->nb--;

		if (fifo->extract == 0) {
			/* C'etait le seul element de la FIFO
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			fifo->insert = 0;
		}

#if ! defined(RM_NO_IT)
		/* Demasquage des interruptions
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		jb_demasquage_it(_priority);
#endif

		/* Envoi de l'adresse de l'element comme code retour
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		goto fin;
	}
	else {
		/* Extraction de l'element specifie
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		_elt		= fifo->extract;
		while (_elt != 0) {
			/* Recherche de l'element a supprimer de la FIFO
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			if (_elt != elt) {
				/* Passage au suivant
				   ~~~~~~~~~~~~~~~~~~ */
				_previous	= _elt;
				_elt		= _elt->next;
				continue;
			}

			/* Element trouve: on le supprime de la FIFO
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			if (_previous == 0) {
				/* C'est le 1er element de la FIFO
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				fifo->extract	= _elt->next;

				if (fifo->extract == 0) {
					/* C'etait le seul element de la FIFO
					   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
					fifo->insert	= 0;
				}
			}
			else {
				/* Ce n'etait pas le premier element de la FIFO
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				_previous->next	= _elt->next;
				if (fifo->insert == _elt) {
					/* C'etait le dernier element de la FIFO
					   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
					fifo->insert	= _previous;
				}
			}
			_elt->next	= 0;
			fifo->nb--;

#if ! defined(RM_NO_IT)
			/* Demasquage des IT
			   ~~~~~~~~~~~~~~~~~ */
			jb_demasquage_it(_priority);
#endif
			goto fin;
		}

#if ! defined(RM_NO_IT)
		/* Demasquage des IT
		   ~~~~~~~~~~~~~~~~~ */
		jb_demasquage_it(_priority);
#endif

		/* L'element n'a pas ete trouve
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		_elt		= 0;
		goto fin;
	}

fin:
	if ((rm_debug & RM_TRACE_FIFO) && trace) {
		Printf("<< Get_fifo(\"%s\" 0x%x, 0x%x): [%3d] 0x%x\n\n",
								fifo->name, fifo, elt, fifo->nb, _elt);
	}
	return _elt;
}

/*******************************************************************************

						RM_PUT_FIFO

*******************************************************************************/
#if ! defined(NO_TYPE_CHECK)
void rm_put_fifo(rm_ref_fifo fifo, rm_ref_elt elt)
#else
void rm_put_fifo(fifo, elt)
rm_ref_fifo		 fifo;
rm_ref_elt		 elt;
#endif
{
#if ! defined(RM_NO_IT)
	int			 _priority;
#endif

	if (rm_debug & RM_TRACE_FIFO) {
		Printf(">> Put_fifo(\"%s\" 0x%x, 0x%x): [%3d]\n",
									fifo->name, fifo, elt, fifo->nb);
	}

	/* Initialisation eventuelle de l'element avant sa mise en FIFO
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (fifo->init_elt) {
		(*fifo->init_elt)(elt);
	}

#if ! defined(RM_NO_IT)
	/* Masquage des interruptions
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	_priority = jb_masquage_it();
#endif

	/* Ajout de l'element dans la FIFO
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (fifo->extract == 0) {
		/* La FIFO est vide
		   ~~~~~~~~~~~~~~~~ */
		fifo->extract		=
		fifo->insert		= elt;
	}
	else {
		/* La FIFO n'est pas vide
		 * Rq: ne pas remplacer les 2 instructions par une seule !!!
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		fifo->insert->next	= elt;
		fifo->insert		= elt;
	}
	fifo->insert->next	= 0;
	fifo->nb++;

#if ! defined(RM_NO_IT)
	/* Demasquage des interruptions
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	jb_demasquage_it(_priority);
#endif

	if (rm_debug & RM_TRACE_FIFO) {
		Printf("<< Put_fifo(\"%s\" 0x%x, 0x%x): [%3d]\n\n",
									fifo->name, fifo, elt, fifo->nb);
	}
}

/*******************************************************************************

						RM_READ_ELT_FIFO

*******************************************************************************/
#if ! defined(NO_TYPE_CHECK)
rm_ref_elt rm_read_elt_fifo(rm_ref_fifo fifo, int n)
#else
rm_ref_elt rm_read_elt_fifo(fifo, n)
rm_ref_fifo		 fifo;
int				 n;
#endif
{
	int				 _n;
	rm_ref_elt		 _elt;

	_n		= n;
	if (_n > fifo->nb) {
		return 0;
	}
	_elt		= fifo->extract;
	while (_n-- > 0) {
		if (_elt == 0) {
			Printf("Read_elt_fifo(\"%s\" 0x%x, %d)\n", fifo->name,
														fifo, n);
			bz_fatal();
		}
		_elt		= _elt->next;
	}
	return _elt;
}

/*******************************************************************************

						RM_READ_FIFO

*******************************************************************************/
#if ! defined(NO_TYPE_CHECK)
int rm_read_fifo(rm_ref_fifo fifo, bool (*fct)(rm_ref_elt), bool trace)
#else
int rm_read_fifo(fifo, fct, trace)
rm_ref_fifo		 fifo;
bool				(*fct)();
bool				 trace;
#endif
{
	/**************************************************************************
	 Cette fonction parcourt une FIFO, et pour chaque element appelle la
	 fonction "fct" avec la reference de l'element en parametre, tant que le
	 code retour de la fonction est VRAI.
	 Le code retour de cette fonction est le nombre d'elements traites.
	**************************************************************************/
	rm_ref_elt		 _elt;
	int				 _nb			= 0;
#if ! defined(RM_NO_IT)
	int				 _priority;
#endif

	if ((rm_debug & RM_TRACE_FIFO) && trace) {
		Printf(">> Read_fifo(\"%s\" 0x%x): [%3d]\n", fifo->name,
												fifo, fifo->nb);
	}

#if ! defined(RM_NO_IT)
	/* Masquage des IT
	   ~~~~~~~~~~~~~~~ */
	_priority = jb_masquage_it();
#endif

	_elt		= fifo->extract;
	while (_elt) {

		/* Incrementation du nombre d'elements
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		_nb++;
		switch ((*fct)(_elt)) {

		case	FAUX:
			_elt		= 0;
			break;

		default:
			_elt		= _elt->next;
			break;
		}
	}

#if ! defined(RM_NO_IT)
	/* Demasquage des interruptions
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	jb_demasquage_it(_priority);
#endif

	if ((rm_debug & RM_TRACE_FIFO) && trace) {
		Printf("<< Read_fifo(\"%s\" 0x%x): [%3d]\n\n",
										fifo->name, fifo, fifo->nb);
	}
	return _nb;
}
