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

#ifndef DEVFS_H
#define DEVFS_H

#include <DnaTools/DnaTools.h>
#include <VirtualFileSystem/VirtualFileSystem.h>

#define DEVFS_NAME_LENGTH 256

typedef enum devfs_inode_class
{
  DNA_DEVFS_FILE,
  DNA_DEVFS_DIRECTORY,
  DNA_DEVFS_SYMLINK
}
devfs_inode_class_t;

typedef struct devfs_entry
{
  queue_item_t link;
  int64_t id;
  char name[DEVFS_NAME_LENGTH];
}
* devfs_entry_t;

typedef struct devfs_inode
{
  queue_item_t link;

  int64_t id;
  char name[DEVFS_NAME_LENGTH];
  devfs_inode_class_t class;
  int64_t size;
  int32_t mode;
  int32_t perms;
  device_cmd_t * dev_cmd;

  queue_t entry_list;
}
* devfs_inode_t;

typedef struct devfs
{
  int64_t inode_index;
  int64_t root_vnid;
  int32_t vid;
  queue_t inode_list;
}
* devfs_t;

extern status_t devfs_walk (void * ns, void * base, char * restrict path, char ** new_path, int64_t * vnid);
extern status_t devfs_mount (int32_t vid, const char * dev_path, uint32_t flags, void * params, void ** data, int64_t * p_vnid);

extern status_t devfs_read_vnode (void * ns, int64_t vnid, void ** data);
extern status_t devfs_write_vnode (void * ns, void * node);

extern status_t devfs_open (void * ns, void * node, int32_t mode, void ** data);
extern status_t devfs_close (void * ns, void * node, void * data);

extern status_t devfs_read (void * ns, void * node, void * file, void * data, int64_t offset, int32_t * p_count);
extern status_t devfs_write (void * ns, void * node, void * file, void * data, int64_t offset, int32_t * p_count);

extern status_t devfs_readdir (void * ns, void * node, void * data, void * entry_array, int64_t * offset, int32_t * p_count);
extern status_t devfs_ioctl (void * ns, void * node, void * data, int32_t function, void * arguments, int32_t * p_ret);

extern bool devfs_entry_name_inspector (void * entry, void * entry_name, void * dummy);
extern bool devfs_entry_index_inspector (void * entry, void * entry_index, void * dummy);
extern bool devfs_inode_inspector (void * inode, void * iid, void * dummy);

#endif

