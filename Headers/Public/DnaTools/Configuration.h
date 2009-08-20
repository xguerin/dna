/****h* DnaInterface/Configuration
 * SUMMARY
 * Configuration.
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

#ifndef DNA_LIBRARY_CONFIGURATION_H
#define DNA_LIBRARY_CONFIGURATION_H

#define DNA_NAME_LENGTH           32
#define DNA_FILENAME_LENGTH       256
#define DNA_PATH_LENGTH           1024

#define DNA_IDLE_STACK_SIZE       0x1000
#define DNA_THREAD_STACK_SIZE     0x4000

#define DNA_MAX_CPU               8
#define DNA_MAX_SEM               16
#define DNA_MAX_FILE              16
#define DNA_MAX_FDARRAY           4
#define DNA_MAX_THREAD            128
#define DNA_MAX_ALARM             128

#endif
