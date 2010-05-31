#ifndef __FAT_ACCESS_H__
#define __FAT_ACCESS_H__

#include <Private/FATFileSystem.h>

/*----------------------------------------------------------------------------- */
/* Prototypes */
/*----------------------------------------------------------------------------- */
int		fatfs_init(struct fatfs *fs);
uint32_t	fatfs_lba_of_cluster(struct fatfs *fs, uint32_t Cluster_Number);
int		fatfs_sector_reader(struct fatfs *fs, uint32_t Startcluster, uint32_t offset, unsigned char *target);
int		fatfs_sector_writer(struct fatfs *fs, uint32_t Startcluster, uint32_t offset, unsigned char *target);
void	fatfs_show_details(struct fatfs *fs);
uint32_t	fatfs_get_root_cluster(struct fatfs *fs);
uint32_t	fatfs_get_file_entry(struct fatfs *fs, uint32_t Cluster, char *nametofind, FAT32_ShortEntry *sfEntry);
int		fatfs_sfn_exists(struct fatfs *fs, uint32_t Cluster, char *shortname);
int		fatfs_update_file_length(struct fatfs *fs, uint32_t Cluster, char *shortname, uint32_t fileLength);
int		fatfs_mark_file_deleted(struct fatfs *fs, uint32_t Cluster, char *shortname);
void	fatfs_list_directory_start(struct fatfs *fs, struct fs_dir_list_status *dirls, uint32_t StartCluster);
int		fatfs_list_directory_next(struct fatfs *fs, struct fs_dir_list_status *dirls, struct fs_dir_ent *entry);

#endif

