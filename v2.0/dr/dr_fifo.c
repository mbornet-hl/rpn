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
 *   @(#)  [MB] dr_fifo.c Version 1.7 du 19/10/04 - 
 */

#include	<stdio.h>
#include  "../cc/cc_types.h"
#include	"../cg/cg_cpub.h"
#include  "dr_cpub.h"

#define	Z printf("%s(): %s(%d)\n", __func__, __FILE__, __LINE__);

/* dr_init_fifo() {{{ */
/*******************************************************************************

                              DR_INIT_FIFO

*******************************************************************************/
void dr_init_fifo(dr_fifo *fifo)
{
     fifo->num_elts           = 0;
     fifo->head               = 0;
     fifo->tail               = 0;
}

/* dr_init_fifo() }}} */
/* dr_init_elt() {{{ */
/*******************************************************************************

                              DR_INIT_ELT

*******************************************************************************/
void dr_init_elt(dr_elt *elt)
{
     elt->previous            = 0;
     elt->next                = 0;
     elt->data                = 0;
}

/* dr_init_elt() }}} */
/* dr_reset_links() {{{ */
/*******************************************************************************

                              DR_RESET_LINKS

*******************************************************************************/
void dr_reset_links(dr_elt *elt)
{
     elt->previous            = 0;
     elt->next                = 0;
}

/* dr_reset_links() }}} */
/* dr_fifo_insert() {{{ */
/*******************************************************************************

                              DR_FIFO_INSERT

*******************************************************************************/
void dr_fifo_insert(dr_fifo *fifo, dr_elt *elt)
{
     if (elt->next     != 0
     ||  elt->previous != 0) {
          CG_INTERNAL_ERROR;
     }

     if (fifo->head == 0) {
          /* FIFO is empty
             ~~~~~~~~~~~~~ */
          fifo->head               =
          fifo->tail               = elt;
     }
     else {
          /* FIFO is not empty
             ~~~~~~~~~~~~~~~~~ */
          elt->next                = fifo->head;
          fifo->head->previous     = elt;
          fifo->head               = elt;
     }

     fifo->num_elts++;
}

/* dr_fifo_insert() }}} */
/* dr_fifo_add() {{{ */
/*******************************************************************************

                              DR_FIFO_ADD

*******************************************************************************/
void dr_fifo_add(dr_fifo *fifo, dr_elt *elt)
{
     if (elt->next     != 0
     ||  elt->previous != 0) {
          CG_INTERNAL_ERROR;
     }

     if (fifo->tail == 0) {
          /* FIFO is empty
             ~~~~~~~~~~~~~ */
//Z
          fifo->head               =
          fifo->tail               = elt;
     }
     else {
          /* FIFO is not empty
             ~~~~~~~~~~~~~~~~~ */
//Z
          elt->previous            = fifo->tail;
          fifo->tail->next         = elt;
          fifo->tail               = elt;
     }

     fifo->num_elts++;
}

/* dr_fifo_add() }}} */
/* dr_fifo_extract_head() {{{ */

/******************************************************************************

                         DR_FIFO_EXTRACT_HEAD

******************************************************************************/
dr_elt *dr_fifo_extract_head(dr_fifo *fifo, dr_elt *wanted_elt)
{
	dr_elt              		*_elt;

	if (wanted_elt == 0) {
		/* Extract the first element of the FIFO
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		if ((_elt = fifo->head) != 0) {
               fifo->head               	= fifo->head->next;
               fifo->num_elts--;

               if (fifo->head == 0) {
                    /* FIFO is now empty
                       ~~~~~~~~~~~~~~~~~ */
                    fifo->tail                     = 0;
               }
               else {
                    fifo->head->previous          = 0;
               }
               dr_reset_links(_elt);
		}
	}
	else {
          for (_elt = fifo->head; _elt != 0; _elt = _elt->next) {
			if (_elt != wanted_elt) {
				/* This element is not the wanted element
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				continue;
			}

			/* This element IS the wanted element
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			if (_elt->previous == 0) {
				/* It was the first element of the FIFO
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				fifo->head          	= _elt->next;

				if (fifo->head == 0) {
					/* It was the only element of the FIFO
					   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
					fifo->tail     	= 0;
				}
                    else {
                         fifo->head->previous = 0;
                    }
			}
			else {
				/* It was NOT the first element of the FIFO
				   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
				_elt->previous->next     = _elt->next;

				if (_elt->next == 0) {
					/* It was the last element of the FIFO
					   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
					fifo->tail     	= _elt->previous;
				}
                    else {
                         _elt->next->previous = _elt->previous;
                    }
			}

               dr_reset_links(_elt);
			fifo->num_elts--;
		}
	}

	return _elt;
}
/* dr_fifo_extract_head() }}} */
/* dr_fifo_read_elt() {{{ */
/*******************************************************************************

						DR_FIFO_READ_ELT

*******************************************************************************/
dr_elt *dr_fifo_read_elt(dr_fifo *fifo, cc_uint32 n)
{
	cc_uint32					 _n;
	dr_elt					*_elt;

	_n						= n;
	if (_n > fifo->num_elts) {
		return 0;
	}

	for (_elt = fifo->head; _elt != 0; _elt = _elt->next) {
		if (--_n == 0) {
			break;
		}
	}

	return _elt;
}

/* dr_fifo_read_elt() }}} */
