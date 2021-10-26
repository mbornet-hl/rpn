%{
/*
 * Copyright (C) 2019-2020, Martial Bornet
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
 *	%Z%	[%Y%] %M%	Version %I% du %E% - %Q%
 */

#include "../cy/cy_rpn_header.h"
#include "../cy/cy_rpn_proto.h"
#include "du_cpub.h"
#include "du_epub.h"
#include "du_cpri.h"
#include "du_epri.h"

%}

%union {
	char				*s_value;
	int				 d_value;
}

%token	<s_value>		DU_IP DU_DATE DU DOW DU_TIME DU_ARGS DU_GET DU_PUT DU_POST DU_HEAD DU_OPTIONS DU_PROPFIND
%token	<d_value>		DU_GARBAGE DU_YEAR DU_DOW DU_NUM
%token				DU_OSB DU_CSB DU_DQUOTE DU_DASH

%start				loglines

%%

loglines		: logline
			{
				DU_TRACE_YACC("logline\n");

				/* Add descriptor to list
				   ~~~~~~~~~~~~~~~~~~~~~~ */
				du_add_to_list(&du_G.list, du_G.tmp_entry);
			}
			| loglines logline
			{
				DU_TRACE_YACC("loglines logline\n");

				/* Add descriptor to list
				   ~~~~~~~~~~~~~~~~~~~~~~ */
				du_add_to_list(&du_G.list, du_G.tmp_entry);
			}
			;

logline		: IP DU_DASH DU_DASH DU_OSB timestamp DU_NUM DU_CSB DU_DQUOTE op DU_ARGS DU_DQUOTE err_code last_field
			{
				DU_TRACE_YACC("DU_IP DU_OSB timestamp DU_NUM DU_CSB DU_DQUOTE op DU_ARGS DU_DQUOTE err_code DU_NUM\n");

				du_G.tmp_entry->path		= strdup($10);
			}
			| IP DU_DASH DU_DASH DU_OSB timestamp DU_NUM DU_CSB DU_DQUOTE DU_ARGS DU_DQUOTE err_code last_field
			{
				DU_TRACE_YACC("DU_IP DU_OSB timestamp DU_NUM DU_CSB DU_DQUOTE DU_ARGS DU_DQUOTE err_code DU_NUM\n");
				DU_TRACE_YACC("BAD REQUEST !!!\n");
			}
			;

IP			: DU_IP
			{
				DU_TRACE_YACC("DU_IP\n");

				/* Allocate entry descriptor
				   ~~~~~~~~~~~~~~~~~~~~~~~~~ */
				du_G.tmp_entry				= du_new_log_entry();
				du_G.tmp_entry->IP.i		= du_IP_to_bytes($1);
			}
			;

timestamp		: date time
			{
				DU_TRACE_YACC("date time\n");
			}
			;

date			: DU_DATE
			{
				char						*_ptr, *_month;
				int						 _day, _mm, _year;

				DU_TRACE_YACC("DU_DATE\n");

				_ptr						= strtok($1, "/");
				_day						= atoi(_ptr);

				_ptr						= strtok(NULL, "/");
				_month					= _ptr;
				if (!strcmp(_month, "Jan")) {
					_mm						= 1;
				}
				else if (!strcmp(_month, "Feb")) {
					_mm						= 2;
				}
				else if (!strcmp(_month, "Mar")) {
					_mm						= 3;
				}
				else if (!strcmp(_month, "Apr")) {
					_mm						= 4;
				}
				else if (!strcmp(_month, "May")) {
					_mm						= 5;
				}
				else if (!strcmp(_month, "Jun")) {
					_mm						= 6;
				}
				else if (!strcmp(_month, "Jul")) {
					_mm						= 7;
				}
				else if (!strcmp(_month, "Aug")) {
					_mm						= 8;
				}
				else if (!strcmp(_month, "Sep")) {
					_mm						= 9;
				}
				else if (!strcmp(_month, "Oct")) {
					_mm						= 10;
				}
				else if (!strcmp(_month, "Nov")) {
					_mm						= 11;
				}
				else if (!strcmp(_month, "Dec")) {
					_mm						= 12;
				}

				_ptr						= strtok(NULL, "/");
				_year					= atoi(_ptr);

				du_G.tmp_entry->tm.tm_mday		= _day;
				du_G.tmp_entry->tm.tm_mon		= _mm - 1;
				du_G.tmp_entry->tm.tm_year		= _year - 1900;
			}
			;

time			: DU_TIME
			{
				char						*_ptr;
				int						 _HH, _MM, _SS;

				DU_TRACE_YACC("DU_TIME\n");

				_ptr						= strtok($1, ":");
				_HH						= atoi(_ptr);

				_ptr						= strtok(NULL, ":");
				_MM						= atoi(_ptr);

				_ptr						= strtok(NULL, ":");
				_SS						= atoi(_ptr);

				du_G.tmp_entry->tm.tm_hour	= _HH;
				du_G.tmp_entry->tm.tm_min	= _MM;
				du_G.tmp_entry->tm.tm_sec	= _SS;

				du_G.tmp_entry->time		= mktime(&du_G.tmp_entry->tm);
			}
			;

op			: DU_GET
			{
				DU_TRACE_YACC("op : DU_GET\n");
				du_G.tmp_entry->op			= DU_OP_GET;
			}
			| DU_PUT
			{
				DU_TRACE_YACC("op : DU_PUT\n");
				du_G.tmp_entry->op			= DU_OP_PUT;
			}
			| DU_POST
			{
				DU_TRACE_YACC("op : DU_POST\n");
				du_G.tmp_entry->op			= DU_OP_POST;
			}
			| DU_HEAD
			{
				DU_TRACE_YACC("op : DU_HEAD\n");
				du_G.tmp_entry->op			= DU_OP_HEAD;
			}
			| DU_OPTIONS
			{
				DU_TRACE_YACC("op : DU_OPTIONS\n");
				du_G.tmp_entry->op			= DU_OP_OPTIONS;
			}
			| DU_PROPFIND
			{
				DU_TRACE_YACC("op : DU_PROPFIND\n");
				du_G.tmp_entry->op			= DU_OP_PROPFIND;
			}
			| DU_GARBAGE
			{
				DU_TRACE_YACC("op : DU_GARBAGE\n");
				du_G.tmp_entry->op			= DU_OP_GARBAGE;
			}
			;

err_code		: DU_NUM
			{
				DU_TRACE_YACC("err_code : DU_NUM\n");
				du_G.tmp_entry->err_code		= $1;
			}
			;

last_field	: DU_NUM
			{
				DU_TRACE_YACC("last_field : DU_NUM\n");
				du_G.tmp_entry->last_field	= $1;
			}
			| DU_DASH
			{
				DU_TRACE_YACC("last_field : DU_DASH\n");
			}
			;
%%
