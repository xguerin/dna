/****h* support/status
 * SUMMARY
 * Support status.
 ****
 * Copyright (C) 2007 TIMA Laboratory
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****/

#ifndef DNA_SUPPORT_STATUS_H
#define DNA_SUPPORT_STATUS_H

#include <stdint.h>

/****t* support/status_t
 * SUMMARY
 * Status type
 *
 * SOURCE
 */

typedef int32_t status_t;

/*
 ****/

#define DNA_HANDLED_INTERRUPT			0x0004
#define DNA_UNHANDLED_INTERRUPT		0x0003
#define DNA_INVOKE_SCHEDULER			0x0002
#define DNA_ALREADY_AT_ROOT				0x0001

#define DNA_OK									  0x0000
#define DNA_ERROR									0xFFFF
#define DNA_NOT_IMPLEMENTED				0xFFFE
#define DNA_OUT_OF_MEM						0xFFFD

#define DNA_NO_ENTRY							0xFFFC
#define DNA_NO_VOLUME							0xFFFB
#define DNA_NO_VNODE							0xFFFA
#define DNA_DEVICE_MOUNTED				0xFFF9
#define DNA_VNODE_MOUNTED					0xFFF8
#define DNA_UNKNOWN_FILESYSTEM		0xFFF7
#define DNA_MAX_OPENED_FILES			0xFFF6
#define DNA_INVALID_FD						0xFFF5
#define DNA_BAD_INODE_TYPE				0xFFF4
#define DNA_INVALID_WHENCE				0xFFF3
#define DNA_VOLUME_IN_USE					0xFFF2

#define DNA_BAD_SEM_ID						0xFFF1
#define DNA_NO_MORE_SEM						0xFFF0
#define DNA_WOULD_BLOCK						0xFFEF
#define DNA_TIMED_OUT							0xFFEE

#define DNA_UNKNOWN_TEAM					0xFFED
#define DNA_INVALID_TEAM_ID				0xFFEC
#define DNA_UNKNOWN_THREAD				0xFFEB
#define DNA_INVALID_THREAD_ID			0xFFEA

#endif
