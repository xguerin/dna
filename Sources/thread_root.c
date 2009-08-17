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
#include <VirtualFileSystem/VirtualFileSystem.h>
#include <Processor/Processor.h>

/****f* core/thread_root
 * SUMMARY
 * The root thread.
 *
 * SYNOPSIS
 */

int32_t thread_root (void * data)

/*
 * ARGUMENTS
 *
 * FUNCTION
 *
 * RESULT
 *
 * SOURCE
 */

{
  int16_t dummy = -1;
  int32_t main_thread = -1;
  status_t status = DNA_OK;

  watch (status_t)
  {
#if 0
    status = vfs_open ("/devices/console", 0, 0, & dummy);
    ensure (status == DNA_OK, status);

    status = vfs_open ("/devices/console", 0, 0, & dummy);
    ensure (status == DNA_OK, status);

    status = vfs_open ("/devices/console", 0, 0, & dummy);
    ensure (status == DNA_OK, status);
#endif

    return 0;
  }
}

/*
 ****/

