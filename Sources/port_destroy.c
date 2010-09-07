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
#include <MemoryManager/MemoryManager.h>
#include <Processor/Processor.h>

/****f* Port/port_destroy
 * SUMMARY
 * Destroy a port_t.
 *
 * SYNOPSIS
 */

status_t port_destroy (int32_t id)

/*
 * ARGUMENTS
 * * id : the port id.
 *
 * RESULT
 * * DNA_BAD_PORT_ID: the id parameter is invalid
 * * DNA_OK: the operation succeeded
 *
 * SOURCE
 */

{
  port_t port = NULL;
  port_id_t pid = { .raw = id };
  interrupt_status_t it_status = 0;
  status_t status;

  watch (status_t)
  {
    ensure (pid . s . index < DNA_MAX_PORT, DNA_BAD_PORT_ID);

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& port_pool . lock);

    /*
     * Look for the port with ID id. If found,
     * remove its entry from the pool.
     */

    port = port_pool . port[pid . s . index];
    check (invalid_port, port != NULL, DNA_BAD_PORT_ID);
    check (invalid_port, port -> id . raw == pid . raw, DNA_BAD_PORT_ID);

    port_pool . port[pid . s . index] = NULL;

    lock_release (& port_pool . lock);
    cpu_trap_restore(it_status);

    /*
     * Destroy the port's semaphore.
     */

    status = semaphore_destroy (port -> read_sem);
    panic (status != DNA_OK);

    status = semaphore_destroy (port -> write_sem);
    panic (status != DNA_OK);

    /*
     * Delete the port's memory.
     */

    kernel_free (port);
    return status;
  }

  rescue (invalid_port)
  {
    lock_release (& port_pool . lock);
    cpu_trap_restore(it_status);
    leave;
  }
}

/*
 ****/

