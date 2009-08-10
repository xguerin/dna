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

/****f* Core/semaphore_create
 * SUMMARY
 * Create a new semaphore_t.
 *
 * SYNOPSIS
 */

status_t semaphore_create (char * name, int32_t tokens, int32_t * sid)

/*
 * ARGUMENTS
 * * name : the name of the semaphore.
 * * tokens : the number of tokens of the semaphore.
 *
 * RESULT
 * * DNA_OK and a valid sid in case of success.
 * * DNA_NO_MORE_SEM if no more semaphore are available.
 * * DNA_OUT_OF_MEM if the system ran out of memory.
 *
 * SOURCE
 */

{
  semaphore_t semaphore = NULL;
  interrupt_status_t it_status = 0;
  int32_t index = 0;

  watch (status_t)
  {
    semaphore = kernel_malloc (sizeof (struct _semaphore), true);
    ensure (semaphore != NULL, DNA_OUT_OF_MEM);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& semaphore_pool . lock);

    for (index = 0; index < DNA_MAX_SEM; index ++)
    {
      if (semaphore_pool . semaphore[index] == NULL) break;
    }

    check (pool_error, index < DNA_MAX_SEM, DNA_NO_MORE_SEM);

    semaphore -> id = index;
    dna_strcpy (semaphore -> name, name);
    semaphore -> tokens = tokens;
    semaphore -> team = scheduler . cpu[cpu_mp_id()] . current_team;

    semaphore_pool . semaphore[index] = semaphore;

    lock_release (& semaphore_pool . lock);
    cpu_trap_restore(it_status);

    *sid = semaphore -> id;
    return DNA_OK;
  }

  rescue (pool_error)
  {
    lock_release (& semaphore_pool . lock);
    cpu_trap_restore(it_status);
    kernel_free (semaphore);
    leave;
  }
}

/*
 ****/

