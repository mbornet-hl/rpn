/*
 * Copyright (C) 2010-2019, Martial Bornet
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
 *   @(#)  [MB] cd_cpub.h Version 1.4 du 19/09/29 - 
 */

/* Exit codes {{{ */
#define   CD_EXIT_OK                      (0)
#define   CD_EXIT_USAGE                   (1)
#define   CD_EXIT_NO_MEM                  (2)
#define   CD_EXIT_OP_UNKNOWN              (3)
#define   CD_EXIT_OP_ERROR                (4)
#define   CD_EXIT_INVALID_ELT             (5)
#define   CD_EXIT_SYNTAX                  (6)
#define   CD_EXIT_UNIMPLEMENTED           (7)
#define   CD_EXIT_READ_ERROR              (8)
#define   CD_EXIT_SIGNAL_ERROR            (9)
#define   CD_EXIT_MKDIR_ERROR            (10)
#define   CD_EXIT_WGET_ERROR             (11)
#define   CD_EXIT_GETCWD_ERROR           (12)
#define   CD_EXIT_CHDIR_ERROR            (13)
#define   CD_EXIT_SYSTEM_ERROR           (14)
#define   CD_EXIT_GET_TIME               (15)
#define   CD_EXIT_OPEN_ERROR             (16)
#define   CD_EXIT_SEGV                   (17)
#define   CD_EXIT_PIPE_ERROR             (18)
#define   CD_EXIT_FORK_ERROR             (19)
#define   CD_EXIT_EXEC_ERROR             (20)
#define   CD_EXIT_LINK_ERROR             (21)
#define   CD_EXIT_NONEXISTENT            (22)
#define   CD_EXIT_DUPLICATE              (23)
#define   CD_EXIT_MISSING_PARAM          (24)
#define   CD_EXIT_CLOSE                  (25)
#define   CD_EXIT_WRITE                  (26)
#define   CD_EXIT_MALLOC                 (27)
#define   CD_EXIT_INTERNAL               (28)
#define   CD_EXIT_BAD_MAGIC              (29)
#define   CD_EXIT_MYSQL_CNX              (30)
#define   CD_EXIT_BAD_PARAM              (31)
#define   CD_EXIT_STAT                   (32)
#define   CD_EXIT_MMAP                   (33)
#define   CD_EXIT_PIPE                   (34)
#define   CD_EXIT_FORK                   (35)
#define   CD_EXIT_EXEC                   (36)
#define   CD_EXIT_FOPEN                  (37)
#define   CD_EXIT_SETVBUF                (38)
#define   CD_EXIT_GETHOSTNAME            (39)
#define   CD_EXIT_GETHOSTBYNAME          (40)
#define   CD_EXIT_CONNECT                (41)
#define   CD_EXIT_CORE_DUMPED            (42)
#define   CD_EXIT_SIGNAL                 (43)
#define   CD_EXIT_CREATE                 (44)
#define   CD_EXIT_MYSQL_CONNECT          (45)
#define   CD_EXIT_MYSQL_QUERY            (46)
#define   CD_EXIT_MYSQL_STORE_RESULT     (47)
#define   CD_EXIT_THREAD_CREATE          (48)
#define   CD_EXIT_THREAD_JOIN            (49)
#define   CD_EXIT_DISP_VERSION           (50)
#define   CD_EXIT_MUNMAP                 (51)
#define   CD_EXIT_DUPLICATE_VALUE        (52)
#define   CD_EXIT_GETCWD                 (53)
#define   CD_EXIT_CONF                   (54)
#define   CD_EXIT_FCLOSE                 (55)
#define   CD_EXIT_READLINK               (56)
#define   CD_EXIT_TESTS                  (57)
#define   CD_EXIT_OPT                    (58)
#define   CD_EXIT_UNKNOWN_OPT            (59)
#define   CD_EXIT_OVERFLOW               (60)
#define   CD_EXIT_LINK                   (61)

#define   CD_EXIT_WRITE_01              (101)
#define   CD_EXIT_WRITE_02              (102)
#define   CD_EXIT_WRITE_03              (103)

#define   CD_EXIT_INTERNAL_ERR		(126)

/* Exit codes }}} */
/* Return codes {{{ */
#define   CD_RET_INVALID_ELT             (-2)
#define   CD_RET_ERROR                   (-1)
#define   CD_RET_OK                       (0)
#define   CD_RET_INVALID_STACK            (1)
#define   CD_RET_INVALID_STR              (2)
#define   CD_RET_INVALID_X                (3)
#define   CD_RET_INVALID_Y                (4)
#define   CD_RET_INVALID_Z                (5)
#define   CD_RET_INVALID_X_TYPE           (6)
#define   CD_RET_INVALID_Y_TYPE           (7)
#define   CD_RET_INVALID_Z_TYPE           (8)
#define   CD_RET_INVALID_TYPES            (9)
#define   CD_RET_TOO_BIG                 (10)
#define   CD_RET_NOT_ENOUGH_ELTS         (11)
#define   CD_RET_INCOMPATIBLE_ELTS       (12)
#define   CD_RET_INCOMPATIBLE_DIM        (13)
#define   CD_RET_NO_START_MARKER         (14)
#define   CD_RET_NO_MORE_ELT             (15)
#define   CD_RET_INVALID_INDEX           (16)
#define   CD_RET_INVALID_LIST            (17)
#define   CD_RET_NOT_HOMOGENEOUS         (18)
#define   CD_RET_DUPLICATE               (19)
#define   CD_RET_NONEXISTENT             (20)
#define   CD_RET_NOT_READABLE            (21)
#define   CD_RET_CANNOT_LINK             (22)
#define   CD_RET_NOT_FOUND               (23)

#define   CD_RET_INTERNAL_ERROR		(127)

/* Return codes }}} */
