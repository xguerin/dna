/****h* core/time
 * SUMMARY
 * Time management.
 ****
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

#ifndef DNA_CORE_TIME_PRIVATE_H
#define DNA_CORE_TIME_PRIVATE_H

#include <stdint.h>
#include <stdbool.h>
#include <DnaTools/DnaTools.h>
#include <Core/Core.h>

#define DNA_TIMER_JIFFY 1000

/****t* time/alarm_t
 * SUMMARY
 * Alarm type.
 *
 * SOURCE
 */

typedef struct _alarm
{
  queue_item_t link;

  int32_t id;
  int32_t cpu_id;
  int32_t mode;

  bigtime_t quantum;
  bigtime_t deadline;
  alarm_callback_t callback;

  void * data;
}
* alarm_t;

/*
 ****/

/****t* time/time_manager_t
 * SUMMARY
 * The time manager type.
 *
 * SOURCE
 */

typedef struct _time_manager
{
  spinlock_t lock;

  alarm_t alarm[DNA_MAX_ALARM];

  bool has_timer;
  system_timer_t system_timer;
}
time_manager_t;

/*
 ****/

extern time_manager_t time_manager;

extern bool alarm_comparator (void * item1, void * item2);
extern bool alarm_id_inspector (void * item, void * a0, void * a1);

#endif

