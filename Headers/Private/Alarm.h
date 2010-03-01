/****h* core_private/alarm_private
 * SUMMARY
 * Alarm management.
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

#ifndef DNA_CORE_ALARM_PRIVATE_H
#define DNA_CORE_ALARM_PRIVATE_H

#include <stdint.h>
#include <stdbool.h>
#include <DnaTools/DnaTools.h>
#include <Core/Core.h>

/****d* alarm_private/DNA_TIMER_DELAY
 * SUMMARY
 * Arbitrary alarm processing delay. Expressed in nanoseconds.
 *
 * SOURCE
 */

#define DNA_TIMER_DELAY 1000

/*
 ****/

/****t* alarm_private/alarm_callback_t
 * SUMMARY
 * Alarm callback type.
 *
 * SOURCE
 */

typedef union _alarm_id
{
  int32_t raw;

  struct _alarm_id_s
  {
    uint16_t value;
    uint16_t index;
  }
  s;
}
alarm_id_t;

/*
 ****/

/****t* alarm_private/alarm_t
 * SUMMARY
 * Alarm type.
 *
 * SOURCE
 */

typedef struct _alarm
{
  queue_link_t link;

  alarm_id_t id;
  int32_t thread_id;
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

/****t* alarm_private/alarm_manager_t
 * SUMMARY
 * The time manager type.
 *
 * SOURCE
 */

typedef struct _alarm_manager
{
  spinlock_t lock;
  uint16_t counter;
  alarm_t alarm[DNA_MAX_ALARM];
}
alarm_manager_t;

/*
 ****/

extern alarm_manager_t alarm_manager;
extern bool alarm_comparator (void * item1, void * item2);

#endif

