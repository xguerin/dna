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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Private/VirtualFileSystem.h>
#include <DnaTools/DnaTools.h>

/****f* framework/vfs_start
 * SUMMARY
 * Start the VFS subsystem.
 *
 * SYNOPSIS
 */

status_t vfs_start (void)

/*
 * FUNCTION
 * Mount the base file systems.
 *
 * SOURCE
 */

{
  status_t status = DNA_OK;

  watch (status_t)
  {
    status = vfs_mount ("", "/", "rootfs", 0, NULL);
    ensure (status == DNA_OK, status);

    status = vfs_mount ("", "/devices", "devfs", 0, NULL);
    ensure (status == DNA_OK, status);

    return DNA_OK;
  }
}

/*
 ****/

