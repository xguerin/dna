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

/****f* Port/port_create
 * SUMMARY
 * Create a new port_t.
 *
 * SYNOPSIS
 */

status_t port_create (char * name, int32_t queue_length, int32_t * p_id)

/*
 * ARGUMENTS
 * * name : the name of the port.
 * * queue_length : the length of its queue.
 *
 * RESULT
 * * DNA_NO_MORE_PORT: no more port available
 * * DNA_OUT_OF_MEM: cannot allocate memory to create a port
 * * DNA_OK: the operation succeeded
 *
 * SOURCE
 */

{
  int32_t index = 0;
  port_t port = NULL;
  status_t status;
  interrupt_status_t it_status = 0;

  watch (status_t)
  {
    ensure (name != NULL && p_id != NULL, DNA_BAD_ARGUMENT);

    /*
     * Create the port and fill in its information
     */

    port = kernel_malloc (sizeof (struct _port), true);
    ensure (port != NULL, DNA_OUT_OF_MEM);

    /*
     * Fill in the information
     */

    status = semaphore_create (name, queue_length, & port -> semaphore);
    check (rsrc_error, status == DNA_OK, status);

    dna_strcpy (port -> info . name, name);
    port -> info . capacity = queue_length;

    /*
     * Insert the port if a room is available
     */

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& port_pool . lock);

    for (index = 0; index < DNA_MAX_PORT; index ++)
    {
      if (port_pool . port[index] == NULL)
      {
        port -> id . s . value = port_pool . counter;
        port -> id . s . index = index;

        port_pool . counter += 1;
        port_pool . port[index] = port;

        break;
      }
    }

    lock_release (& port_pool . lock);
    check (rsrc_error, index < DNA_MAX_PORT, DNA_NO_MORE_PORT);

    cpu_trap_restore(it_status);

    *p_id = port -> id . raw;
    return DNA_OK;
  }

  rescue (rsrc_error)
  {
    cpu_trap_restore(it_status);
    kernel_free (port);
    leave;
  }
}

/*
 ****/

