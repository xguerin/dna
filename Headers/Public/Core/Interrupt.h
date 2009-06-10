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

#ifndef DNA_CORE_INTERRUPT_PUBLIC_H
#define DNA_CORE_INTERRUPT_PUBLIC_H

#include <stdint.h>
#include <DnaLibrary/DnaLibrary.h>
#include <Processor/Processor.h>

/****t* interrupt/ipi_manager_t
 * SUMMARY
 * The system IPI manager type.
 *
 * SOURCE
 */

typedef struct _system_ipi_manager
{
  status_t (* send) (int32_t target, int32_t command, int32_t data);
}
ipi_manager_t;

/*
 ****/

extern status_t interrupt_attach (interrupt_id_t id, int32_t mode,
    interrupt_handler_t handler);

extern status_t interrupt_detach (interrupt_id_t id,
    interrupt_handler_t handler);

extern status_t ipi_set_manager (ipi_manager_t timer, bool force);

extern status_t ipi_callback (int32_t command, int32_t data);

#endif

