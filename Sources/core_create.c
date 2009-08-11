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
 * * Initialize the team manager
 * * Initialize the scheduler manager
 * * Initialize the it multiplexer
 * * Initialize the time manager
 * * Initialize the semaphore pool
 * * Initialize the system team and the IDLE threads
 * * Create the system team.
 *
 * SOURCE
 */

{
  status_t status;
  team_t team = NULL;
  thread_t thread = NULL;
  int32_t thread_id = -1;

  watch (status_t)
  {
    /*
     * Initialize the team manager
     */

    dna_memset (& team_manager, 0, sizeof (team_manager_t));

    /*
     * Initialize the scheduler manager
     */

    dna_memset (& scheduler, 0, sizeof (scheduler_t));
    scheduler . xt_index = cpu_mp_count;

    scheduler . cpu = kernel_malloc (sizeof (cpu_t) * DNA_MAX_CPU, true);
    ensure (scheduler . cpu != NULL, DNA_OUT_OF_MEM);

    scheduler . xt = kernel_malloc (sizeof (queue_t) * (DNA_MAX_CPU + 1), true);
    check (sched_xt_alloc, scheduler . xt != NULL, DNA_OUT_OF_MEM);

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
     * Allocate the system team structure.
     */

    team = kernel_malloc (sizeof (struct _team), true);
    check (team_alloc, team != NULL, DNA_OUT_OF_MEM);

    /*
     * Fill-in the necessary fields.
     */

    team -> id = atomic_add (& team_manager . team_index, 1);
    dna_strcpy (team -> name, DNA_SYSTEM_TEAM);

    queue_item_init (& team -> sched_link, team);

    /*
     * Add the new team to the scheduler.
     */

    queue_add (& team_manager . team_list, & team -> sched_link);

    /*
     * Create the idle threads
     */

    for (int32_t cpu_i = 0; cpu_i < cpu_mp_count; cpu_i++)
    {
      /*
       * Set the current team as the system team.
       * We do that now because current_team is required
       * by thread_create ().
       */

      scheduler . cpu[cpu_i] . current_team = team;

      /*
       * Create the Idle thread
       */

      status = thread_create (thread_idle, NULL, "IdleThread",
          cpu_i, 0x400, & thread_id);
      check (create_threads, status == DNA_OK, DNA_ERROR);

      thread = queue_lookup (& team -> thread_list,
          thread_id_inspector, (void *) & thread_id, NULL);

      /*
       * Deal with the new thread
       */

      scheduler . cpu[cpu_i] . id = cpu_i;
      scheduler . cpu[cpu_i] . status = DNA_CPU_DISABLED;

      queue_item_init (& scheduler . cpu[cpu_i] . link,
          & scheduler . cpu[cpu_i]);

      scheduler . cpu[cpu_i] . idle_thread = thread;
      scheduler . cpu[cpu_i] . current_thread = thread;
    }

    /*
     * Create the root thread
     */

    status = thread_create (thread_root, NULL, "RootThread",
        DNA_NO_AFFINITY, 0x2000, & thread_id);
    check (create_threads, status == DNA_OK, DNA_ERROR);

    thread = queue_lookup (& team -> thread_list,
        thread_id_inspector, (void *) & thread_id, NULL);

    scheduler . cpu[0] . current_thread = thread;

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

        scheduler . cpu[cpu_i] . current_team = NULL;
        scheduler . cpu[cpu_i] . idle_thread = NULL;
        scheduler . cpu[cpu_i] . current_thread = NULL;
      }
    }

    /*
     * We free the team
     */

    kernel_free (team);
  }
  
  rescue (team_alloc)
  {
    kernel_free (semaphore_pool . semaphore);
  }

  rescue (sched_xt_alloc)
  {
    kernel_free (scheduler . cpu);
    leave;
  }
}

/*
 ****/

