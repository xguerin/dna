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

#include <Private/Core.h>
#include <MemoryManager/MemoryManager.h>
#include <DnaTools/DnaTools.h>
#include <Processor/Processor.h>

/****f* Core/time_get_system
 * SUMMARY
 * Get the system time since boot-up.
 *
 * SYNOPSIS
 */

status_t time_get_system (bigtime_t * value)

/*
 * SOURCE
 */

{
  watch (status_t)
  {
    ensure (time_manager . has_timer, DNA_NO_TIMER);

    time_manager . system_timer . get (value);
    return DNA_OK;
  }
}

/*
 ****/

