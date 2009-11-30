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

/****f* Core/interrupt_attach
 * SUMMARY
 * Attach a interrupt to an ID.
 *
 * SYNOPSIS
 */

status_t interrupt_attach (int32_t cpuid, interrupt_id_t id,
    int32_t mode, interrupt_handler_t handler)

/*
 * ARGUMENTS
 * * id : the ID of the interrupt to attach
 * * mode : the mode of the attach
 * * handler : handler of the interrupt
 *
 * RESULT
 * * DNA_OUT_OF_MEM if no more memory is available
 * * DNA_OK if everything went well
 *
 * SOURCE
 */

{
  isr_t isr = NULL;
  interrupt_status_t it_status = 0;
  queue_t * queue = & scheduler . cpu[cpu_mp_id ()] . isr_list[id];

  watch (status_t)
  {
    ensure (id < cpu_trap_count (), DNA_BAD_ARGUMENT);

    isr = kernel_malloc (sizeof (struct _isr), true);
    ensure (isr != NULL, DNA_OUT_OF_MEM);

    isr -> handler = handler;

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& queue -> lock);

    queue_add (queue, isr);

    if (queue -> status == 1)
    {
      cpu_trap_attach_isr (cpuid, id, mode, interrupt_demultiplexer);

      if (cpuid == cpu_mp_id ()) cpu_trap_enable (id);
      else
      {
#if 0
        lock_acquire (& scheduler . cpu[cpuid] . ipi_lock);
#endif
        cpu_mp_send_ipi (cpuid, DNA_IPI_TRAP_ENABLE, (void *) id);
      }
    }

    lock_release (& queue -> lock);
    cpu_trap_restore(it_status);

    return DNA_OK;
  }
}

/*
 ****/

