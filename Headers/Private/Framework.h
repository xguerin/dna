/****h* core/framework
 * SUMMARY
 * Core component framework.
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

#ifndef DNA_CORE_FRAMEWORK_PRIVATE_H
#define DNA_CORE_FRAMEWORK_PRIVATE_H

#include <DnaLibrary/DnaLibrary.h>

extern status_t core_create (void);
extern status_t core_destroy (void);
extern status_t core_start (void);
extern status_t core_stop (void);

#endif

