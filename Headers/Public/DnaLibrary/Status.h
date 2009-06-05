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

typedef enum _status
{
  /*
   * This code are not errors, hence that are
   * given positive numbers
   */

  DNA_HANDLED_INTERRUPT =	0x0004,
  DNA_UNHANDLED_INTERRUPT	= 0x0003,
  DNA_INVOKE_SCHEDULER = 0x0002,
  DNA_ALREADY_AT_ROOT	= 0x0001,

  /*
   * Standard error codes
   */

  DNA_OK = 0x0000,
  DNA_ERROR	= 0xFFFF,
  DNA_NOT_IMPLEMENTED = 0xFFFE,
  DNA_OUT_OF_MEM = 0xFFFD,

  /*
   * VFS error codes
   */

  DNA_NO_ENTRY = 0xFFFC,
  DNA_NO_VOLUME = 0xFFFB,
  DNA_NO_VNODE = 0xFFFA,
  DNA_DEVICE_MOUNTED = 0xFFF9,
  DNA_VNODE_MOUNTED = 0xFFF8,
  DNA_UNKNOWN_FILESYSTEM = 0xFFF7,
  DNA_MAX_OPENED_FILES = 0xFFF6,
  DNA_INVALID_FD = 0xFFF5,
  DNA_BAD_INODE_TYPE = 0xFFF4,
  DNA_INVALID_WHENCE = 0xFFF3,
  DNA_VOLUME_IN_USE	= 0xFFF2,

  /*
   * Semaphore error codes
   */

  DNA_BAD_SEM_ID = 0xFFF1,
  DNA_NO_MORE_SEM = 0xFFF0,
  DNA_WOULD_BLOCK	= 0xFFEF,
  DNA_TIMED_OUT	= 0xFFEE,

  /*
   * Team and threads error codes
   */

  DNA_UNKNOWN_TEAM = 0xFFED,
  DNA_INVALID_TEAM_ID = 0xFFEC,
  DNA_UNKNOWN_THREAD = 0xFFEB,
  DNA_INVALID_THREAD_ID = 0xFFEA,

  /*
   * Timer error code
   */

  DNA_NO_TIMER = 0xFFE9,

  /*
   * Ports and messages codes
   */

  DNA_BAD_PORT_ID = 0xFFE8,
  DNA_NO_MORE_PORT = 0xFFE7
}
status_t;

/*
 ****/

#endif
