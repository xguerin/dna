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

#ifndef FATFS_H
#define FATFS_H

#include <stdarg.h>
#include <DnaTools/DnaTools.h>
#include <VirtualFileSystem/VirtualFileSystem.h>

#include <Private/fatlib_defs.h>

#define FATFS_NAME_LENGTH 256

/*----------------------------------------------------------------------------- */
/* Function Pointers */
/*----------------------------------------------------------------------------- */
typedef int (*fn_diskio_read) (const uint32_t sector, unsigned char *buffer, const int32_t fs_fd);
typedef int (*fn_diskio_write)(const uint32_t sector, unsigned char *buffer, const int32_t fs_fd);

/*----------------------------------------------------------------------------- */
/* Structures */
/*----------------------------------------------------------------------------- */
struct disk_if
{
	/* User supplied function pointers for disk IO */
	fn_diskio_read		read_sector;
	fn_diskio_write		write_sector;
};

struct sector_buffer
{
	unsigned char			sector[FAT_SECTOR_SIZE];
	uint32_t				address;
	int						dirty;

	/* Next in chain of sector buffers */
	struct sector_buffer  *next;
};

typedef enum eFatType
{
	FAT_TYPE_16,
	FAT_TYPE_32
} tFatType;

/* pour remplir cette structure, la lib fait un memcpy */
typedef struct fatfs_entry
{
  	unsigned char Name[11];
	unsigned char Attr;
	unsigned char NTRes;
	unsigned char CrtTimeTenth;
	unsigned char CrtTime[2];
	unsigned char CrtDate[2];
	unsigned char LstAccDate[2];
	uint16_t FstClusHI;
	unsigned char WrtTime[2];
	unsigned char WrtDate[2];
	uint16_t FstClusLO;
    uint32_t FileSize;
}
* fatfs_entry_t;

typedef struct fatfs_entry FAT32_ShortEntry;

typedef struct fatfs_inode
{
  int64_t id;
  uint32_t cc_dirid;
  unsigned char *cluster_chain_directory;
  uint32_t nb_sector;
  struct fatfs_entry entry;
}
* fatfs_inode_t;

typedef struct fatfs
{
	int32_t vid; /* volume id */
	int64_t root_vnid;
  	int32_t fs_fd; /* file descriptor */
  
  	/* Filesystem globals */
	unsigned char			sectors_per_cluster;
	uint32_t				cluster_begin_lba;
	uint32_t				rootdir_first_cluster;
	uint32_t				rootdir_first_sector;
	uint32_t				rootdir_sectors;
	uint32_t				fat_begin_lba;
	uint16_t				fs_info_sector;
	uint32_t				lba_begin;
	uint32_t				fat_sectors;
	uint32_t				next_free_cluster;
	uint16_t				root_entry_count;
	tFatType				fat_type;

	/* Disk/Media API */
	struct disk_if			disk_io;

	/* [Optional] Thread Safety */
	void					(*fl_lock)(void);  /* unused */
	void					(*fl_unlock)(void); /* unused */

	/* Working buffer */
	struct sector_buffer	currentsector;

	/* FAT Buffer */
	struct sector_buffer	*fat_buffer_head;
	struct sector_buffer	fat_buffers[FAT_BUFFERED_SECTORS];
  
}
* fatfs_t;

typedef struct sFL_FILE
{
	uint32_t				parentcluster;
	uint32_t				startcluster;
	uint32_t				bytenum;
	uint32_t				filelength;
	int						filelength_changed;
	char					path[FATFS_MAX_LONG_FILENAME];
	char					filename[FATFS_MAX_LONG_FILENAME];
	unsigned char			shortfilename[11];

	/* Read/Write sector buffer */
	struct sector_buffer	file_data;

	/* File fopen flags */
	unsigned char			flags;

	struct sFL_FILE			*next;
} FL_FILE;

struct fs_dir_list_status
{
	uint32_t				sector;
	uint32_t				cluster;
	unsigned char			offset;
};

struct fs_dir_ent
{
	char					filename[FATFS_MAX_LONG_FILENAME];
	unsigned char			is_dir;
	uint32_t				cluster;
	uint32_t				size;
};

extern status_t fatfs_walk (void * ns, void * base,
    char * restrict path, char ** new_path, int64_t * vnid);
extern status_t fatfs_mount (int32_t vid, const char * dev_path,
    uint32_t flags, void * params, void ** data, int64_t * p_vnid);

extern status_t fatfs_read_vnode (void * ns, int64_t vnid, void ** data);
extern status_t fatfs_write_vnode (void * ns, void * node);

extern status_t fatfs_open (void * ns, void * node, int32_t mode, void ** data);
extern status_t fatfs_close (void * ns, void * node, void * data);

extern status_t fatfs_create (void * ns, void * node, char * restrict path,
    int32_t mode, int32_t perms, int64_t * vnid, void ** data);
extern status_t fatfs_read (void * ns, void * node, void * file,
    void * data, int64_t offset, int32_t * p_count);
extern status_t fatfs_write (void * ns, void * node, void * file,
    void * data, int64_t offset, int32_t * p_count);

extern status_t fatfs_mkdir (void * ns, void * node,
    char * restrict name, int32_t mode);
extern status_t fatfs_readdir (void * ns, void * node, void * data,
    void * entry_array, int64_t * offset, int32_t * p_count);

extern bool fatfs_entry_name_inspector (void * entry, va_list list);
extern bool fatfs_entry_index_inspector (void * entry, va_list list);
extern bool fatfs_inode_inspector (void * inode, va_list list);

#endif

