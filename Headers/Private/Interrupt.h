/****h* core/interrupt
 * SUMMARY
 * Interrupt management.
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

#ifndef DNA_CORE_INTERRUPT_PRIVATE_H
#define DNA_CORE_INTERRUPT_PRIVATE_H

#include <stdint.h>
#include <Core/Core.h>
#include <Processor/Processor.h>
#include <Platform/Platform.h>
#include <DnaLibrary/DnaLibrary.h>

#define DNA_IPI_ALL cpu_mp_count

enum
{
  DNA_IPI_YIELD = 0xFFFF
};

typedef struct _isr
{
	interrupt_handler_t handler;
	queue_item_t link;
}
* isr_t;

typedef struct _it_manager
{
	spinlock_t lock;

	int32_t counter[cpu_n_it];
	queue_t isr_list[cpu_n_it];

  bool has_ipi;
  ipi_manager_t ipi_manager;
}
it_manager_t;

extern it_manager_t it_manager;

extern int32_t interrupt_handler (int32_t itn);
extern bool interrupt_handler_inspector (void * p_isr, void * p_handler, void * dummy);

extern status_t ipi_send (int32_t target, int32_t command, int32_t data);
extern status_t ipi_callback (int32_t command, int32_t data);

#endif

