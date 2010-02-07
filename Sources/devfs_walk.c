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

#include <Private/DeviceFileSystem.h>
#include <DnaTools/DnaTools.h>

status_t devfs_walk (void * ns, void * base, char * restrict path,
    char ** new_path, int64_t * p_vnid)
{
  devfs_t devfs = ns;
  devfs_inode_t base_inode = base;
  devfs_entry_t entry = NULL;
  int64_t vnid = -1;

  watch (status_t)
  {
    ensure (ns != NULL, DNA_ERROR);

    log (VERBOSE_LEVEL, "path = %s", path);

    entry = queue_lookup (& base_inode -> entry_list,
        devfs_entry_name_inspector, path);
    ensure (entry != NULL, DNA_NO_ENTRY);

    log (VERBOSE_LEVEL, "inode found");

    vnid = entry -> id;
    ensure (vnid != devfs -> root_vnid, DNA_ALREADY_AT_ROOT);
    ensure (dna_strcmp (path, "..") != 0, DNA_ALREADY_AT_ROOT);

    *p_vnid = vnid;
    return DNA_OK;
  }
}

