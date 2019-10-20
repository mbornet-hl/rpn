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
 *	@(#)	[MB] dn_cpub.h	Version 1.1 du 19/08/30 - 
 */


// Statistics
#define   RPN_OP_BASE_STATS             (RPN_OP_END_MATH + 1)
#define   RPN_OP_END_STATS               RPN_OP_ID_POW_DATA_SET

#define   RPN_OP_ID_SUM_ADD             (RPN_OP_BASE_STATS + 0)
#define   RPN_OP_ID_SUM_SUB             (RPN_OP_BASE_STATS + 1)
#define   RPN_OP_ID_MEAN                (RPN_OP_BASE_STATS + 2)
#define   RPN_OP_ID_SDEV                (RPN_OP_BASE_STATS + 3)
#define   RPN_OP_ID_MEANX               (RPN_OP_BASE_STATS + 4)
#define   RPN_OP_ID_MMM                 (RPN_OP_BASE_STATS + 5)
#define   RPN_OP_ID_LINE_DATA_SET       (RPN_OP_BASE_STATS + 6)
#define   RPN_OP_ID_EXP_DATA_SET        (RPN_OP_BASE_STATS + 7)
#define   RPN_OP_ID_LOG_DATA_SET        (RPN_OP_BASE_STATS + 8)
#define   RPN_OP_ID_POW_DATA_SET        (RPN_OP_BASE_STATS + 9)

