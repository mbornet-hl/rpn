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
 *	@(#)	[MB] cy_rpn_print.c	Version 1.1 du 19/02/15 - 
 */

void rpn_ctx_prt(const char *funct, int line, int type, char *fmt, ...)
{
	/**************************************************************************
	 Affichage formatte de messages.
	**************************************************************************/
	va_list		 _ap;
	char			*_buf;
	int			 _nb;
	b_uint32		 _debug_level_copy;

	/* Faut-il afficher le message ?
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (ah_no_print) {
		switch (type) {

		case	AH_ERR:
		case	AH_INTERNAL:
			/* Message d'erreur : on continue => affichage
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			break;

		default:
			/* Autre type de message : on n'affiche pas
			   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
			return;
		}
	}

	va_start(_ap, fmt);

	/* Allocation memoire pour le buffer de formattage
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if ((_buf = R_MALLOC(AH_BUFSIZE)) == 0) {
//		ah_clean_print(stderr, file, line, AH_ERR, ah_msg_004[ah_ind], 0);
		exit(M_ERR_MALLOC);
	}

	/* Formattage du message
	   ~~~~~~~~~~~~~~~~~~~~~ */
	_nb	= vsprintf(_buf, fmt, _ap);

	/* Affichage du message avec cesure propre
	   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	ah_clean_ctx_print(ctx, file, funct, line, type, _buf, _nb);

	/* Liberation de la memoire
	   ~~~~~~~~~~~~~~~~~~~~~~~~ */
	_debug_level_copy	= r_debug;
	r_debug			= 0;
	R_FREE(&_buf);
	r_debug			= _debug_level_copy;

	va_end(_ap);
}

