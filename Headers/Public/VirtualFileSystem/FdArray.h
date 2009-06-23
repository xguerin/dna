/*
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

#ifndef DNA_VFS_FDARRAY_PUBLIC_H
#define DNA_VFS_FDARRAY_PUBLIC_H

#include <stdint.h>
#include <DnaTools/DnaTools.h>

extern status_t fdarray_create (int32_t id, int32_t parent_id);
extern status_t fdarray_destroy (int32_t id);

#endif
