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
 *	@(#)	[MB] cg_cpub.h	Version 1.4 du 19/10/01 - 
 */

#if ! defined(CG_CPUB_H)
#define CG_CPUB_H

#include "cg_epub.h"

#define	CG_FATAL()				cg_fatal(__func__, __FILE__, __LINE__)
#define   CG_INTERNAL_ERROR			cg_fatal(__func__, __FILE__, __LINE__)

#endif	/* CG_CPUB_H */
