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
#include <DnaTools/DnaTools.h>
#include <Processor/Processor.h>

/****f* Core/semaphore_destroy
 * SUMMARY
 * Destroy a semaphore_t.
 *
 * SYNOPSIS
 */

status_t semaphore_destroy (int32_t sid)

/*
 * ARGUMENTS
 * * sid : the semaphore id.
 *
 * RESULT
 * Not implemented.
 *
 * SOURCE
 */

{
  semaphore_t sem = NULL;
  semaphore_id_t sem_id = { .raw = sid };
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& semaphore_pool . lock);

    /*
     * Look for the semaphore with ID sid
     */

    sem = semaphore_pool . semaphore[sem_id . s . index];
    check (invalid_semaphore, sem != NULL, DNA_BAD_SEM_ID);
    check (invalid_semaphore, sem -> id . raw == sem_id . raw, DNA_BAD_SEM_ID);

    lock_release (& semaphore_pool . lock);
    return DNA_NOT_IMPLEMENTED;
  }

  rescue (invalid_semaphore)
  {
    lock_release (& semaphore_pool . lock);
    cpu_trap_restore(it_status);
    leave;
  }
}

/*
 ****/

