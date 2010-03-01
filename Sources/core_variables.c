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

/****v* framework/core_component
 * SUMMARY
 * The core component instance.
 *
 * SOURCE
 */

component_t core_component =
{
  .name = "core_component",
  .create = core_create,
  .destroy = core_destroy,
  .start = core_start,
  .stop = core_stop
};

/*
 ****/

/****v* thread_private/thread_pool
 * SUMMARY
 * The thread pool instance.
 *
 * SOURCE
 */

thread_pool_t thread_pool;

/*
 ****/

/****v* scheduler_private/scheduler
 * SUMMARY
 * The scheduler manager instance.
 *
 * SOURCE
 */

scheduler_t scheduler;

/*
 ****/

/****v* cpu_private/cpu_pool
 * SUMMARY
 * The CPU pool instance.
 *
 * SOURCE
 */

cpu_pool_t cpu_pool;

/*
 ****/

/****v* alarm_private/alarm_manager
 * SUMMARY
 * The time manager instance.
 *
 * SOURCE
 */

alarm_manager_t alarm_manager;

/*
 ****/

/****v* semaphore_private/semaphore_pool
 * SUMMARY
 * The semaphore pool instance.
 *
 * SOURCE
 */

semaphore_pool_t semaphore_pool;

/*
 ****/

