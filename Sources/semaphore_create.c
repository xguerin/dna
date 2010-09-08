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

/****f* Semaphore/semaphore_create
 * SUMMARY
 * Create a new semaphore_t.
 *
 * SYNOPSIS
 */

status_t semaphore_create (char * name, int32_t tokens, int32_t * id)

/*
 * ARGUMENTS
 * * name : the name of the semaphore.
 * * tokens : the number of tokens of the semaphore.
 *
 * RESULT
 * * DNA_NO_MORE_SEM: no more semaphore available
 * * DNA_OUT_OF_MEM: cannot allocate memory to create a semaphore
 * * DNA_OK: the operation succeeded
 *
 * SOURCE
 */

{
  semaphore_t semaphore = NULL;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (name != NULL && id != NULL, DNA_BAD_ARGUMENT);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& semaphore_pool . lock);

    /*
     * Get an empty semaphore slot.
     */

    semaphore = queue_rem (& semaphore_pool . semaphore);
    check (pool_error, semaphore != NULL, DNA_NO_MORE_SEM);

    /*
     * Fill in the information
     */

    dna_strcpy (semaphore -> info . name, name);
    semaphore -> info . tokens = tokens;

    semaphore -> id . s . value = semaphore_pool . counter;
    semaphore_pool . counter += 1;

    /*
     * Release the pool and return the semaphore ID.
     */

    lock_release (& semaphore_pool . lock);
    cpu_trap_restore(it_status);

    log (INFO_LEVEL, "ID(%d:%d) TOKEN(%d)",
        semaphore -> id . s . value, semaphore -> id . s . index,
        semaphore -> info . tokens);

    *id = semaphore -> id . raw;
    return DNA_OK;
  }

  rescue (pool_error)
  {
    cpu_trap_restore(it_status);
    kernel_free (semaphore);
    leave;
  }
}

/*
 ****/

