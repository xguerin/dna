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
#include <MemoryManager/MemoryManager.h>
#include <Processor/Processor.h>

/****f* Core/thread_destroy
 * SUMMARY
 * Destroy a thread.
 *
 * SYNOPSIS
 */

status_t thread_destroy (thread_t thread)

/*
 * ARGUMENTS
 * * thread : the thread to destroy
 *
 * RESULT
 *
 * SOURCE
 */

{
  watch (status_t)
  {
    ensure (thread != NULL, DNA_BAD_ARGUMENT);

    kernel_free (thread -> signature . stack_base);
    kernel_free (thread);

    return DNA_OK;
  }
}

/*
 ****/

