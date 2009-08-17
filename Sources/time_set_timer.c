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

/****f* Core/time_set_timer
 * SUMMARY
 * Set the system timer.
 *
 * SYNOPSIS
 */

status_t time_set_timer (system_timer_t timer, bool force)

/*
 * ARGUMENTS
 * * timer : a timer
 * * force : force this timer, even if a timer has already been set
 *
 * SOURCE
 */

{
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (time_manager . has_timer || force, DNA_BAD_ARGUMENT);

    /*
     * Set the timer to the time manager
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& time_manager . lock);

    time_manager . has_timer = true;
    time_manager . system_timer = timer;

    lock_release (& time_manager . lock);
    cpu_trap_restore(it_status);

    return DNA_OK;
  }
}

/*
 ****/

