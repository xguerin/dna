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
#include <DnaLibrary/DnaLibrary.h>

static filesystem_cmd_t devfs_cmd = {
	devfs_walk,
	devfs_mount,
	devfs_read_vnode,
	devfs_write_vnode,
	NULL,
	devfs_open,
	devfs_close,
	NULL,
	devfs_read,
	devfs_write,
	NULL,
	devfs_readdir,
	devfs_ioctl
};

filesystem_t devfs_module = {
	"devfs",
	& devfs_cmd
};

