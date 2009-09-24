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
#include <DnaTools/DnaTools.h>

#define DNA_IPI_EXECUTE       0xFFFF
#define DNA_IPI_SUSPEND       0xFFFE
#define DNA_IPI_TRAP_ENABLE   0xFFFD

/****t* interrupt/isr_t
 * SUMMARY
 * Describes an interrupt service routine element.
 *
 * SOURCE
 */

typedef struct _isr
{
  queue_item_t link;
  interrupt_handler_t handler;
}
* isr_t;

/*
 ****/

extern int32_t interrupt_demultiplexer (int32_t itn);

extern bool interrupt_handler_inspector (void * p_isr,
    void * p_handler, void * dummy);

#endif

