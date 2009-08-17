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

/****f* Core/semaphore_get_info
 * SUMMARY
 * Get info from a semaphore.
 *
 * SYNOPSIS
 */

status_t semaphore_get_info (int32_t sid, semaphore_info_t * info)

/*
 * ARGUMENTS
 * * sid : the ID of the semaphore to get_info.
 * * sem_info : a pointer to a sem_info structure.
 *
 * RESULT
 * * DNA_BAD_SEM_ID if the sid parameter is invalid.
 * * DNA_OK if the operation succeded.
 * * DNA_ERROR if the sem_info pointer is NULL.
 *
 * SOURCE
 */

{
  semaphore_t sem = NULL;
  interrupt_status_t it_status = 0;
  status_t status = DNA_OK;

  watch (status_t)
  {
    ensure (sid >= 0 && sid < DNA_MAX_SEM, DNA_BAD_SEM_ID);
    ensure (info != NULL, DNA_BAD_ARGUMENT);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& semaphore_pool . lock);

    /*
     * Look for the semaphore with ID sid
     */

    sem = semaphore_pool . semaphore[sid];
    check (invalid_semaphore, sem != NULL, DNA_BAD_SEM_ID);

    lock_acquire (& sem -> lock);
    lock_release (& semaphore_pool . lock);

    /*
     * Copy information from the semaphore to the
     * information structure
     */

    info -> id = sem -> id;
    dna_strcpy (info -> name, sem -> name);
    info -> tokens = sem -> tokens;

    if (sem -> latest_holder != NULL)
    {
      info -> latest_holder = sem -> latest_holder -> id;
    }
    else
    {
      info -> latest_holder = -1;
    }

    lock_release (& sem -> lock);
    cpu_trap_restore(it_status);

    return status;
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

