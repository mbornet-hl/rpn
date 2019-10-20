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
 *	@(#)	[MB] cc_types.h	Version 1.4 du 19/10/01 - 
 */

#if ! defined(CC_TYPES_H)
#  define CC_TYPES_H

/* Boolean values
   ~~~~~~~~~~~~~~ */
#define	FALSE						 0
#define	TRUE							 1

#define	NEVER						 0
#define	ALWAYS						 1

#if ! defined(NULL)
#define	NULL							((char *) 0)
#endif

/* Signed integers
   ~~~~~~~~~~~~~~~ */
typedef char							 cc_int8;
typedef short							 cc_int16;
typedef int							 cc_int32;
typedef long long						 cc_int64;

/* Unsigned integers
   ~~~~~~~~~~~~~~~~~ */
typedef unsigned char					 cc_uint8;
typedef unsigned short					 cc_uint16;
typedef unsigned int					 cc_uint32;
typedef unsigned long long				 cc_uint64;

typedef cc_uint8						 byte;
typedef cc_uint16						 word;
typedef cc_uint32						 dword;
typedef cc_uint64						 qword;

typedef int							 bool;

typedef void							*fpvoid;

typedef char							*cc_string;

/* Types identifiers
   ~~~~~~~~~~~~~~~~~ */
#define	CC_INT8						0x0001
#define	CC_UINT8						0x0002
#define	CC_INT16						0x0003
#define	CC_UINT16						0x0004
#define	CC_INT32						0x0005
#define	CC_UINT32						0x0006
#define	CC_INT64						0x0007
#define	CC_UINT64						0x0008
#define	CC_BYTE						0x0009
#define	CC_WORD						0x000A
#define	CC_DWORD						0x000B
#define	CC_QWORD						0x000C
#define	CC_BOOL						0x000D
#define	CC_FPVOID						0x000E
#define	CC_STRING						0x000F

/* Number of elements of an array
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define	CC_NB_ELTS(tab)  (sizeof(tab)/sizeof(tab[0]))

#endif	/* CC_TYPES_H */
