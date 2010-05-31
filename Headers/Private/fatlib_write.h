#ifndef __FAT_WRITE_H__
#define __FAT_WRITE_H__

#include <Private/fatlib_defs.h>
#include <Private/fatlib_opts.h>

/*----------------------------------------------------------------------------- */
/* Prototypes */
/*----------------------------------------------------------------------------- */
int		fatfs_add_file_entry(struct fatfs *fs, uint32_t dirCluster, char *filename, char *shortfilename, uint32_t startCluster, uint32_t size, int dir);
int		fatfs_add_free_space(struct fatfs *fs, uint32_t *startCluster);
int		fatfs_allocate_free_space(struct fatfs *fs, int newFile, uint32_t *startCluster, uint32_t size);

#endif

