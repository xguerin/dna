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
  cpu_t * cpu = NULL;
  int32_t thread_id = -1;
  void * area = NULL;

  watch (status_t)
  {
    /*
     * Initialize the thread pool
     */

    dna_memset (& thread_pool, 0, sizeof (thread_pool_t));

    area = kernel_malloc (DNA_MAX_GROUP * sizeof (thread_t *), true);
    thread_pool . thread = area;

    for (int32_t i = 0; i < DNA_MAX_GROUP; i += 1)
    {
      area = kernel_malloc (DNA_MAX_THREAD * sizeof (thread_t), true);
      thread_pool . thread[i] = area;
    }

    /*
     * Initialize the scheduler
     */

    dna_memset (& scheduler, 0, sizeof (scheduler_t));

    area = kernel_malloc ((DNA_MAX_CPU + 1) * sizeof (queue_t), true);
    scheduler . queue = area;

    /*
     * Initialize the CPU pool
     */

    dna_memset (& cpu_pool, 0, sizeof (cpu_pool_t));

    area = kernel_malloc (DNA_MAX_CPU * sizeof (cpu_t), true);
    cpu_pool . cpu = area;

    /*
     * Initialize the alarm manager
     */

    dna_memset (& alarm_manager, 0, sizeof (alarm_manager_t));

    /*
     * Initialize the semaphore pool
     */

    dna_memset (& semaphore_pool, 0, sizeof (semaphore_pool_t));

    area = kernel_malloc (DNA_MAX_SEM * sizeof (semaphore_t), true);
    semaphore_pool . semaphore = area;

    /*
     * Initialize the CPUs
     */

    for (int32_t cpu_i = 0; cpu_i < cpu_mp_count (); cpu_i++)
    {
      cpu = & cpu_pool . cpu[cpu_i];

      /*
       * Create the ISR lists
       */

      cpu -> isr_list = kernel_malloc (sizeof (queue_t) *
          cpu_trap_count (), true);

      /*
       * Create the Idle stack
       */

      cpu -> stack = kernel_malloc (DNA_IDLE_STACK_SIZE, true);

      /*
       * Create the Idle thread
       */

      status = thread_create (thread_idle, NULL,
          "IdleThread", cpu_i, cpu -> stack,
          DNA_IDLE_STACK_SIZE, & thread_id);
      check (create_threads, status == DNA_OK, DNA_ERROR);

      scheduler . thread[thread_id] -> info . status = DNA_THREAD_READY;

      /*
       * Deal with the new thread
       */

      cpu -> id = cpu_i;
      cpu -> status = DNA_CPU_DISABLED;
      cpu -> idle_thread = scheduler . thread[thread_id];
      cpu -> current_thread = scheduler . thread[thread_id];
    }

    /*
     * Create the main application thread
     */

    status = thread_create (APP_ENTRY_POINT, NULL,
        "ApplicationMain", DNA_NO_AFFINITY, NULL,
        DNA_THREAD_STACK_SIZE, & thread_id);
    check (create_threads, status == DNA_OK, DNA_ERROR);

    cpu_pool . cpu[0] . current_thread = scheduler . thread[thread_id];

    return DNA_OK;
  }

  rescue(create_threads)
  {
    thread_t idle_thread = NULL;

    /*
     * We free each allocated thread and its stack
     */

    for (int32_t cpu_i = 0; cpu_i < cpu_mp_count (); cpu_i++)
    {
      idle_thread = cpu_pool . cpu[cpu_i] . idle_thread;

      if (idle_thread != NULL)
      {
        thread_destroy (idle_thread);

        cpu_pool . cpu[cpu_i] . idle_thread = NULL;
        cpu_pool . cpu[cpu_i] . current_thread = NULL;
      }
    }

    leave;
  }
}

/*
 ****/

