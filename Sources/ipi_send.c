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
#include <DnaLibrary/DnaLibrary.h>
#include <MemoryManager/MemoryManager.h>
#include <Processor/Processor.h>

/****f* Core/ipi_attach
 * SUMMARY
 * Send an interrupt to a processor.
 *
 * SYNOPSIS
 */

status_t ipi_send (int32_t target, int32_t command, void * cookie)

/*
 * ARGUMENTS
 * * target : the processor's ID
 * * command : the command to pass
 * * cookie : the data attached to the command
 *
 * RESULT
 * * DNA_ERROR if the target ID is wrong
 * * DNA_OK if everything went well
 *
 * SOURCE
 */

{
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (target >= 0 && target <= cpu_mp_count, DNA_ERROR);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& it_manager . lock);

    if (target == DNA_IPI_ALL)
    {
      for (int32_t i = 0; i < cpu_mp_count; i += 1)
      {
        it_manager . ipi_manager . send (i, command, cookie);
      }
    }
    else
    {
      it_manager . ipi_manager . send (target, command, cookie);
    }

    lock_release (& it_manager . lock);
    cpu_trap_restore(it_status);

    return DNA_OK;
  }
}

/*
 ****/

