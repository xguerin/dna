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

#include <Private/Timer.h>
#include <Processor/Processor.h>

extern void system_kickstart (void);

void cpu_start (void)
{
  soclib_timer_port_t timer;

  timer = & SOCLIB_TIMER_DEVICES[0] . port[cpu_mp_id ()];
  cpu_write(UINT32, & (timer -> mode), 1); 

  system_kickstart ();
}

