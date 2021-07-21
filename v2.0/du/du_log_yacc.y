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
 *	@(#)	[MB] du_log_yacc.y	Version 1.1 du 20/07/31 - 
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

%token	<s_value>		DU_IP DU_DATE DU DOW DU_TIME DU_GET DU_PUT DU_POST DU_HEAD DU_OPTIONS DU_PROPFIND
%token	<d_value>		DU_YEAR DU_DOW DU_NUM
%token				DU_OSB DU_CSB DU_DQUOTE DU_DASH

%start				loglines

%%

loglines		: logline
			{
				DU_TRACE_YACC("logline\n");
			}
			| loglines logline
			{
				DU_TRACE_YACC("loglines logline\n");
			}
			;

logline		: DU_IP DU_DASH DU_DASH DU_OSB DU_DATE DU_TIME DU_NUM DU_CSB DU_DQUOTE op DU_DQUOTE err_code last_field
			{
				DU_TRACE_YACC("DU_IP DU_OSB DU_DATE DU_TIME DU_NUM DU_CSB DU_DQUOTE op DU_DQUOTE err_code DU_NUM\n");
			}
			;

op			: DU_GET
			{
				DU_TRACE_YACC("op : DU_GET\n");
			}
			| DU_PUT
			{
				DU_TRACE_YACC("op : DU_PUT\n");
			}
			| DU_POST
			{
				DU_TRACE_YACC("op : DU_POST\n");
			}
			| DU_HEAD
			{
				DU_TRACE_YACC("op : DU_HEAD\n");
			}
			| DU_OPTIONS
			{
				DU_TRACE_YACC("op : DU_OPTIONS\n");
			}
			| DU_PROPFIND
			{
				DU_TRACE_YACC("op : DU_PROPFIND\n");
			}
			;

err_code		: DU_NUM
			{
				DU_TRACE_YACC("err_code : DU_NUM\n");
			}
			;

last_field	: DU_NUM
			{
				DU_TRACE_YACC("last_field : DU_NUM\n");
			}
			| DU_DASH
			{
				DU_TRACE_YACC("last_field : DU_DASH\n");
			}
			;
%%
