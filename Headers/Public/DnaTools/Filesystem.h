/****h* libos/filesystem
 * SUMMARY
 * libOS filesystem definition.
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

#ifndef DNA_LIBOS_FILESYSTEM_H
#define DNA_LIBOS_FILESYSTEM_H

#include <stdint.h>
#include <DnaTools/Status.h>

/****t* vfs/filesystem_cmd_t
 * SUMMARY
 * Filesystem commands type.
 *
 * SOURCE
 */

typedef struct filesystem_cmd
{
  status_t (* walk) (void * ns, void * base, char * restrict path, char ** new_path, int64_t * vnid);
  status_t (* mount) (int32_t vid, const char * dev_path, uint32_t flags, void * params, void ** data, int64_t * vnid);
  status_t (* read_vnode) (void * ns, int64_t vnid, void ** data);
  status_t (* write_vnode) (void * ns, void * node);
  status_t (* destroy_vnode) (void * ns, void * node);
  status_t (* open) (void * ns, void * node, int32_t mode, void ** data);
  status_t (* close) (void * ns, void * node, void * data);
  status_t (* create) (void * ns, void * node, char * restrict path, int32_t mode, int32_t perms, int64_t * vnid, void ** data);
  status_t (* read) (void * ns, void * node, void * file, void * data, int64_t offset, int32_t * p_count);
  status_t (* write) (void * ns, void * node, void * file, void * data, int64_t offset, int32_t * p_count);
  status_t (* mkdir) (void * ns, void * node, char * restrict name, int32_t mode);
  status_t (* readdir) (void * ns, void * node, void * data, void * entry_array, int64_t * offset, int32_t * p_count);
  status_t (* ioctl) (void * ns, void * node, void * data, int32_t function, void * arguments, int32_t * p_ret);
}
filesystem_cmd_t;

/*
 ****/

/****t* vfs/filesystem_t
 * SUMMARY
 * Filesystem type.
 *
 * SOURCE
 */

typedef struct filesystem
{
  char * name;
  filesystem_cmd_t * cmd;
}
filesystem_t;

/*
 ****/

extern int32_t OS_N_FILESYSTEMS;
extern filesystem_t * OS_FILESYSTEMS_LIST [];

extern status_t filesystem_load (char * name, filesystem_t ** fs);

#endif

