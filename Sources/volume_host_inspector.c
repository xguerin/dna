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

#include <Private/VirtualFileSystem.h>
#include <DnaTools/DnaTools.h>

/****f* volume/volume_host_inspector
 * SUMMARY
 * Compare an volume and a volume's host.
 *
 * SYNOPSIS
 */

bool volume_host_inspector (void * item, void * p_host, void * p_vnid) 

/*
 * ARGUMENTS
 *
 * RESULT
 *
 * SOURCE
 */

{
	volume_t volume = item;
	volume_t host = p_host;
	int64_t * vnid = p_vnid; 

  watch (bool)
  {
    ensure (volume != NULL, false);
    ensure (p_vnid != NULL, false);

    return volume -> host_volume == host && volume -> host_vnid == *vnid;
  }
}

/*
 ****/
