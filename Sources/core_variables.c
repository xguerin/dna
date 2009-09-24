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

/****v* Core/core_component
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

/****v* Core/scheduler
 * SUMMARY
 * The scheduler manager instance.
 *
 * SOURCE
 */

scheduler_t scheduler;

/*
 ****/

/****v* Core/interrupt_manager
 * SUMMARY
 * The interrupt multiplexer instance.
 *
 * SOURCE
 */

interrupt_manager_t interrupt_manager;

/*
 ****/

/****v* Core/alarm_manager
 * SUMMARY
 * The time manager instance.
 *
 * SOURCE
 */

alarm_manager_t alarm_manager;

/*
 ****/

/****v* Core/semaphore_pool
 * SUMMARY
 * The semaphore manager instance.
 *
 * SOURCE
 */

semaphore_pool_t semaphore_pool;

/*
 ****/

