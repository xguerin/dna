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

#ifndef DNA_TOOLS_LOG_H
#define DNA_TOOLS_LOG_H

#include <DnaTools/C.h>
#include <Processor/Processor.h>

#define PANIC_LEVEL 1
#define INFO_LEVEL 2
#define VERBOSE_LEVEL 3

#ifdef DNA_ENABLE_LOG
#define log(level, string, ...)                               \
  if (level <= DNA_ENABLE_LOG)                                \
  {                                                           \
    dna_printf ("<%d>(%s:%d): " string "\r\n", cpu_mp_id (),  \
        __FUNCTION__, __LINE__,  ## __VA_ARGS__);             \
  }
#else
#define log(level, string, ...)
#endif

#endif
