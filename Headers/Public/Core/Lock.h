/****h* core/lock
 * SUMMARY
 * Lock management.
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
 */

#ifndef DNA_CORE_LOCK_PUBLIC_H
#define DNA_CORE_LOCK_PUBLIC_H

#include <stdint.h>

/****t* lock/spinlock_t
 * SUMMARY
 * Spin lock type.
 *
 * SOURCE
 */

typedef volatile int32_t spinlock_t;

/*
 ****/

extern void lock_acquire (spinlock_t * lock);
extern void lock_release (spinlock_t * lock);

#endif

