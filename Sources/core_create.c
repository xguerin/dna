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

/****f* Core/core_create
 * SUMMARY
 * Create the core component.
 *
 * SYNOPSIS
 */

status_t core_create (void)

/*
 * FUNCTION
 * * Initialize the scheduler manager
 * * Initialize the it multiplexer
 * * Initialize the time manager
 * * Initialize the semaphore pool
 * * Initialize the IDLE threads
 *
 * SOURCE
 */

{
  status_t status;
  int32_t thread_id = -1;

  watch (status_t)
  {
    /*
     * Initialize the scheduler manager
     */

    dna_memset (& scheduler, 0, sizeof (scheduler_t));

    /*
     * Initialize the IT multiplexer
     */

    dna_memset (& interrupt_manager, 0, sizeof (interrupt_manager_t));

    /*
     * Initialize the time manager
     */

    dna_memset (& time_manager, 0, sizeof (time_manager_t));

    /*
     * Initialize the semaphore pool
     */

    dna_memset (& semaphore_pool, 0, sizeof (semaphore_pool_t));

    /*
     * Create the idle threads
     */

    for (int32_t cpu_i = 0; cpu_i < cpu_mp_count; cpu_i++)
    {
      /*
       * Create the Idle thread
       */

      status = thread_create (thread_idle, NULL,
          "IdleThread", cpu_i, & scheduler . cpu[cpu_i] . stack,
          DNA_IDLE_STACK_SIZE, & thread_id);
      check (create_threads, status == DNA_OK, DNA_ERROR);

      /*
       * Deal with the new thread
       */

      scheduler . cpu[cpu_i] . id = cpu_i;
      scheduler . cpu[cpu_i] . status = DNA_CPU_DISABLED;
      scheduler . cpu[cpu_i] . idle_thread = scheduler . thread[thread_id];
      scheduler . cpu[cpu_i] . current_thread = scheduler . thread[thread_id];
    }

    /*
     * Create the root thread
     */

    status = thread_create (thread_root, NULL,
        "RootThread", DNA_NO_AFFINITY, NULL,
        DNA_THREAD_STACK_SIZE, & thread_id);
    check (create_threads, status == DNA_OK, DNA_ERROR);

    scheduler . cpu[0] . current_thread = scheduler . thread[thread_id];
    return DNA_OK;
  }

  rescue(create_threads)
  {
    thread_t idle_thread = NULL;

    /*
     * We free each allocated thread and its stack
     */

    for (int32_t cpu_i = 0; cpu_i < cpu_mp_count; cpu_i++)
    {
      idle_thread = scheduler . cpu[cpu_i] . idle_thread;

      if (idle_thread != NULL)
      {
        thread_destroy (idle_thread);

        scheduler . cpu[cpu_i] . idle_thread = NULL;
        scheduler . cpu[cpu_i] . current_thread = NULL;
      }
    }

    leave;
  }
}

/*
 ****/

