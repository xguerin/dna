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
  interrupt_status_t it_status = 0;
  isr_t isr = NULL;

  watch (status_t)
  {
    ensure (id < CPU_TRAP_COUNT, DNA_BAD_ARGUMENT);

    isr = kernel_malloc (sizeof (struct _isr), true);
    ensure (isr != NULL, DNA_OUT_OF_MEM);

    isr -> handler = handler;

    it_status = cpu_trap_mask_and_backup();
    lock_acquire (& interrupt_manager . lock);

    interrupt_manager . counter[cpuid][id] += 1;
    queue_add (& interrupt_manager . isr_list[cpuid][id], isr);

    if (interrupt_manager . counter[cpuid][id] == 1)
    {
      cpu_trap_attach_isr (cpuid, id, mode, handler);

      if (cpuid == cpu_mp_id ())
      {
        cpu_trap_enable (id);
      }
      else
      {
        cpu_mp_send_ipi (cpuid, DNA_IPI_TRAP_ENABLE, (void *) id);
      }
    }
    else if (interrupt_manager . counter[cpuid][id] == 2)
    {
      cpu_trap_attach_isr (cpuid, id, mode, interrupt_demultiplexer);
    }

    lock_release (& interrupt_manager . lock);
    cpu_trap_restore(it_status);

    return DNA_OK;
  }
}

/*
 ****/

