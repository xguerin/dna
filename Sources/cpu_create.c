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

#include <Private/Ipi.h>
#include <Private/Timer.h>
#include <Processor/Processor.h>

void cpu_create (void)
{
  int32_t current_cpuid = cpu_mp_id ();
  cpu_handler_table[2][current_cpuid] = timer_handler;
  cpu_handler_table[7][current_cpuid] = ipi_handler;
}

