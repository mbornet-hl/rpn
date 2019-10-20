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
 *	@(#)	[MB] df_convert.c	Version 1.1 du 19/09/13 - 
 */

#include <stdint.h>

union df_u16 {
	uint16_t			u16;
	uint8_t			u8[2];
};

union df_u32 {
	uint32_t			u32;
	uint16_t			u16[2];
	uint8_t			u8[4];
};

/*******************************************************************************

						DF_CONVERT_UINT32

*******************************************************************************/
uint32_t df_convert_uint32(uint32_t u32)
{
	union df_u32		 _u32;
	uint8_t			 _tmp_u8;

	_u32.u32			= u32;

	_tmp_u8			= _u32.u8[0];
	_u32.u8[0]		= _u32.u8[3];
	_u32.u8[3]		= _tmp_u8;

	_tmp_u8			= _u32.u8[1];
	_u32.u8[1]		= _u32.u8[2];
	_u32.u8[2]		= _tmp_u8;

	return _u32.u32;
}
