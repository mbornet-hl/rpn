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
<<<<<<< HEAD
 *	@(#)	[MB] du_cpub.h	Version 1.3 du 21/07/23 - 
 */

#include <time.h>

/* Number of columns of a log entry in a matrix :
 * 1. IP address
 * 2. Date (YYYY-mm-dd)
 * 3. Time (HH:MM:SS)
 * 4. Timestamp (in seconds since the epoch)
 * 5. Operation (GET / PUT / POST / ...)
 * 6. Pathname
 * 7. Error code
 * 8. Last field (???)
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define	DU_LOG_ENTRY_MEMBERS				(8)

struct du_log_entry {
	union rpn_IP		 IP;
	time_t			 time;
	struct tm			 tm;
	byte				 op;
	char				*path;
	int				 err_code;
	int				 last_field;
	struct du_log_entry	*next;
};
=======
 *	@(#)	[MB] du_cpub.h	Version 1.2 du 20/07/31 - 
 */


>>>>>>> d9987656174c5b946bd2d1be7c6d1ee85f575d29
