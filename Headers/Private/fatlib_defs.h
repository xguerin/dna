#ifndef __FAT_DEFS_H__
#define __FAT_DEFS_H__

#include <stdint.h>

#include <Private/fatlib_opts.h>

/*----------------------------------------------------------------------------- */
/*			FAT32 Offsets */
/*		Name				Offset */
/*----------------------------------------------------------------------------- */

/* Boot Sector */
#define BS_JMPBOOT				0	/* Length = 3 */
#define BS_OEMNAME				3	/* Length = 8 */
#define BPB_BYTSPERSEC			11	/* Length = 2 */
#define BPB_SECPERCLUS			13	/* Length = 1 */
#define BPB_RSVDSECCNT			14	/* Length = 2 */
#define BPB_NUMFATS				16	/* Length = 1 */
#define BPB_ROOTENTCNT			17	/* Length = 2 */
#define BPB_TOTSEC16			19	/* Length = 2 */
#define BPB_MEDIA				21	/* Length = 1 */
#define	BPB_FATSZ16				22	/* Length = 2 */
#define BPB_SECPERTRK			24	/* Length = 2 */
#define BPB_NUMHEADS			26	/* Length = 2 */
#define BPB_HIDDSEC				28	/* Length = 4 */
#define BPB_TOTSEC32			32	/* Length = 4 */

/* FAT 12/16 */
#define BS_FAT_DRVNUM			36	/* Length = 1 */
#define BS_FAT_BOOTSIG			38	/* Length = 1 */
#define BS_FAT_VOLID			39	/* Length = 4 */
#define BS_FAT_VOLLAB			43	/* Length = 11 */
#define BS_FAT_FILSYSTYPE		54	/* Length = 8 */

/* FAT 32 */
#define BPB_FAT32_FATSZ32		36	/* Length = 4 */
#define BPB_FAT32_EXTFLAGS		40	/* Length = 2 */
#define BPB_FAT32_FSVER			42	/* Length = 2 */
#define BPB_FAT32_ROOTCLUS		44	/* Length = 4 */
#define BPB_FAT32_FSINFO		48	/* Length = 2 */
#define BPB_FAT32_BKBOOTSEC		50	/* Length = 2 */
#define BS_FAT32_DRVNUM			64	/* Length = 1 */
#define BS_FAT32_BOOTSIG		66	/* Length = 1 */
#define BS_FAT32_VOLID			67	/* Length = 4 */
#define BS_FAT32_VOLLAB			71	/* Length = 11 */
#define BS_FAT32_FILSYSTYPE		82	/* Length = 8 */

/*----------------------------------------------------------------------------- */
/* FAT Types */
/*----------------------------------------------------------------------------- */
#define	FAT_TYPE_FAT12			1
#define	FAT_TYPE_FAT16			2
#define	FAT_TYPE_FAT32			3

/*----------------------------------------------------------------------------- */
/* FAT32 Specific Statics */
/*----------------------------------------------------------------------------- */
#define SIGNATURE_POSITION						 510
#define SIGNATURE_VALUE							 0xAA55
#define PARTITION1_TYPECODE_LOCATION	 		 450
#define FAT32_TYPECODE1						 	 0x0B
#define FAT32_TYPECODE2						 	 0x0C
#define PARTITION1_LBA_BEGIN_LOCATION	 		 454
#define PARTITION1_SIZE_LOCATION	 			 458

/*----------------------------------------------------------------------------- */
/* FAT32 File Attributes and Types */
/*----------------------------------------------------------------------------- */
#define FILE_ATTR_READ_ONLY   	0x01
#define FILE_ATTR_HIDDEN 		0x02
#define FILE_ATTR_SYSTEM 		0x04
#define FILE_ATTR_SYSHID		0x06
#define FILE_ATTR_VOLUME_ID 	0x08
#define FILE_ATTR_DIRECTORY		0x10
#define FILE_ATTR_ARCHIVE  		0x20
#define FILE_ATTR_LFN_TEXT		0x0F
#define FILE_HEADER_BLANK		0x00
#define FILE_HEADER_DELETED		0xE5
#define FILE_TYPE_DIR			0x10
#define FILE_TYPE_FILE			0x20

/*----------------------------------------------------------------------------- */
/* Other Defines */
/*----------------------------------------------------------------------------- */
#define FAT32_LAST_CLUSTER		0xFFFFFFFF
#define FAT32_INVALID_CLUSTER	0xFFFFFFFF


/*----------------------------------------------------------------------------- */
/* Defines */
/*----------------------------------------------------------------------------- */
#define FAT_INIT_OK							0
#define FAT_INIT_MEDIA_ACCESS_ERROR			(-1)
#define FAT_INIT_INVALID_SECTOR_SIZE		(-2)
#define FAT_INIT_INVALID_SIGNATURE			(-3)
#define FAT_INIT_ENDIAN_ERROR				(-4)
#define FAT_INIT_WRONG_FILESYS_TYPE			(-5)
#define FAT_INIT_WRONG_PARTITION_TYPE		(-6)

#define FILE_READ	(1 << 0)
#define FILE_WRITE	(1 << 1)
#define FILE_APPEND	(1 << 2)
#define FILE_BINARY	(1 << 3)
#define FILE_ERASE	(1 << 4)
#define FILE_CREATE	(1 << 5)

/*
typedef struct
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
} FAT32_ShortEntry;
*/
#endif

