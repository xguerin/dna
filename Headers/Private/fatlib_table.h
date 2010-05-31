#ifndef __FAT_TABLE_H__
#define __FAT_TABLE_H__

#include <Private/fatlib_opts.h>
#include <Private/fatlib_misc.h>

/*----------------------------------------------------------------------------- */
/* Prototypes */
/*----------------------------------------------------------------------------- */
void	fatfs_fat_init(struct fatfs *fs);
int		fatfs_fat_purge(struct fatfs *fs);
uint32_t	fatfs_find_next_cluster(struct fatfs *fs, uint32_t current_cluster);
void	fatfs_set_fs_info_next_free_cluster(struct fatfs *fs, uint32_t newValue);
int		fatfs_find_blank_cluster(struct fatfs *fs, uint32_t start_cluster, uint32_t *free_cluster);
int		fatfs_fat_set_cluster(struct fatfs *fs, uint32_t cluster, uint32_t next_cluster);
int		fatfs_fat_add_cluster_to_chain(struct fatfs *fs, uint32_t start_cluster, uint32_t newEntry);
int		fatfs_free_cluster_chain(struct fatfs *fs, uint32_t start_cluster);

#endif
