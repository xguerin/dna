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

/****f* Core/ipi_set_manager
 * SUMMARY
 * Set the IPI manager.
 *
 * SYNOPSIS
 */

status_t ipi_set_manager (ipi_manager_t manager, bool force)

/*
 * ARGUMENTS
 * * manager : a manager
 * * force : force this manager, even if a manager has already been set
 *
 * SOURCE
 */

{
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (interrupt_manager . has_ipi || force, DNA_BAD_ARGUMENT);

    /*
     * Set the timer to the time manager
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& interrupt_manager . lock);

    interrupt_manager . has_ipi = true;
    interrupt_manager . ipi_manager = manager;

    lock_release (& interrupt_manager . lock);
    cpu_trap_restore(it_status);

    return DNA_OK;
  }
}

/*
 ****/

