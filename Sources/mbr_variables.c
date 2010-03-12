/*
 * Copyright (C) 2007 TIMA Laboratory                                    
 * Original copyright:
 *
 * GNU fdisk - a clone of Linux fdisk.
 *
 * Copyright (C) 2006
 * Free Software Foundation, Inc.
 *                                                                       
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, either version 3 of the License, or     
 * (at your option any later version.                                   
 *                                                                       
 * This program is distributed in the hope that it will be useful,       
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
 * GNU General Public License for more details.                          
 *                                                                       
 * You should have received a copy of the GNU General Public License     
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */

#include <stddef.h>
#include <Private/MasterBootRecord.h>
#include <MasterBootRecord/MasterBootRecord.h>

mbr_module_t mbr_module = 
{
  { "mbr", 0 },
  mbr_load,
  mbr_dump
};

partition_type_t partition_type[] =
{
  { 0x00, "Empty" },
  { 0x01, "FAT12" }, 
  { 0x02, "XENIX root" }, 
  { 0x03, "XENIX usr" }, 
  { 0x04, "Small FAT16" }, 
  { 0x05, "Extended" },
  { 0x06, "FAT16" }, 
  { 0x07, "HPFS/NTFS/exFAT" }, 
  { 0x08, "AIX boot" }, 
  { 0x09, "AIX data" }, 
  { 0x0a, "OS/2 boot manager" }, 
  { 0x0b, "FAT32" }, 
  { 0x0c, "FAT32 LBA" }, 

  MBR_TYPE_UNUSED(0x0d),

  { 0x0e, "FAT16 LBA" }, 
  { 0x0f, "Extended LBA" }, 
  { 0x10, "OPUS" }, 
  { 0x11, "Hidden FAT12" }, 
  { 0x12, "Compaq diagnostic" }, 
  { 0x14, "Hidden Small FAT16" }, 

  MBR_TYPE_UNUSED(0x15),

  { 0x16, "Hidden FAT16" }, 
  { 0x17, "Hidden HPFS/NTFS" }, 
  { 0x18, "AST SmartSleep" }, 

  MBR_TYPE_UNUSED(0x15), MBR_TYPE_UNUSED(0x16),
  MBR_TYPE_UNUSED(0x17), MBR_TYPE_UNUSED(0x18),
  MBR_TYPE_UNUSED(0x19), MBR_TYPE_UNUSED(0x1a),

  { 0x1b, "Hidden FAT32" }, 
  { 0x1c, "Hidden FAT32 LBA" }, 

  MBR_TYPE_UNUSED(0x1d),

  { 0x1e, "Hidden FAT16 LBA" }, 

  MBR_TYPE_UNUSED(0x1f), MBR_TYPE_UNUSED(0x20),
  MBR_TYPE_RESERVED(0x21), MBR_TYPE_UNUSED(0x22),
  MBR_TYPE_RESERVED(0x23),

  { 0x24, "NEC DOS" }, 

  MBR_TYPE_UNUSED(0x25), MBR_TYPE_RESERVED(0x26),
  MBR_TYPE_UNUSED(0x27), MBR_TYPE_UNUSED(0x28),
  MBR_TYPE_UNUSED(0x29), MBR_TYPE_UNUSED(0x2a),
  MBR_TYPE_UNUSED(0x2b), MBR_TYPE_UNUSED(0x2c),
  MBR_TYPE_UNUSED(0x2d), MBR_TYPE_UNUSED(0x2e),
  MBR_TYPE_UNUSED(0x2f), MBR_TYPE_UNUSED(0x30),
  MBR_TYPE_RESERVED(0x31),

  { 0x32, "NOS" },

  MBR_TYPE_UNUSED(0x33), MBR_TYPE_UNUSED(0x34),

  { 0x35, "JFS OS2/eCS" },

  MBR_TYPE_RESERVED(0x36), MBR_TYPE_UNUSED(0x37),
  
  { 0x38, "THEOS v3.2 2gb" },
  { 0x39, "Plan 9" }, 

  { 0x3a, "THEOS v4 4gb" },
  { 0x3b, "THEOS v4 extended" },
  { 0x3c, "PMagic recovery" }, 
  { 0x3d, "Hidden NetWare" }, 

  MBR_TYPE_UNUSED(0x3e), MBR_TYPE_UNUSED(0x3f),

  { 0x40, "Venix 80286" }, 
  { 0x41, "PPC PReP Boot" }, 
  { 0x42, "SFS/Linux swap" }, 
  { 0x43, "Linux native" }, 
  { 0x44, "GoBack" }, 
  { 0x45, "Boot-US boot manager" }, 
  { 0x46, "EUMEL/Elan" }, 
  { 0x47, "EUMEL/Elan" }, 
  { 0x48, "EUMEL/Elan" }, 

  MBR_TYPE_UNUSED(0x49),

  { 0x4a, "AdaOS Aquila" }, 

  MBR_TYPE_UNUSED(0x4b),

  { 0x4c, "Oberon" }, 
  { 0x4d, "QNX4.x" }, 
  { 0x4e, "QNX4.x 2nd part" }, 
  { 0x4f, "QNX4.x 3rd part" }, 
  { 0x50, "OnTrack DM" }, 
  { 0x51, "OnTrackDM6 Aux1" }, 
  { 0x52, "CP/M" }, 
  { 0x53, "OnTrackDM6 Aux3" }, 
  { 0x54, "OnTrack DM6" }, 
  { 0x55, "EZ Drive" }, 
  { 0x56, "Golden Bow" }, 
  { 0x57, "DrivePro" }, 

  MBR_TYPE_UNUSED(0x58), MBR_TYPE_UNUSED(0x59),
  MBR_TYPE_UNUSED(0x5a), MBR_TYPE_UNUSED(0x5b),

  { 0x5c, "Priam Edisk" }, 

  MBR_TYPE_UNUSED(0x5d), MBR_TYPE_UNUSED(0x5e),
  MBR_TYPE_UNUSED(0x5f), MBR_TYPE_UNUSED(0x60),

  { 0x61, "SpeedStor" }, 

  MBR_TYPE_UNUSED(0x62),

  { 0x63, "GNU HURD/SysV" }, 
  { 0x64, "Netware 286" }, 
  { 0x65, "Netware 386" }, 
  { 0x66, "Netware SMS" }, 
  { 0x67, "Novell" }, 
  { 0x68, "Novell" }, 
  { 0x69, "Netware NSS" }, 

  MBR_TYPE_UNUSED(0x6a), MBR_TYPE_UNUSED(0x6b),
  MBR_TYPE_UNUSED(0x6c), MBR_TYPE_UNUSED(0x6d),
  MBR_TYPE_UNUSED(0x6e), MBR_TYPE_UNUSED(0x6f),

  { 0x70, "DiskSec MltBoot" }, 

  MBR_TYPE_RESERVED(0x71), MBR_TYPE_UNUSED(0x72),
  MBR_TYPE_RESERVED(0x73),

  { 0x74, "Scramdisk" }, 
  { 0x75, "PC/IX" }, 

  MBR_TYPE_RESERVED(0x76),

  { 0x77, "M2FS/M2CS" }, 
  { 0x78, "XOSL" }, 

  MBR_TYPE_UNUSED(0x79), MBR_TYPE_UNUSED(0x7a),
  MBR_TYPE_UNUSED(0x7b), MBR_TYPE_UNUSED(0x7c),
  MBR_TYPE_UNUSED(0x7d), MBR_TYPE_UNUSED(0x7e),
  MBR_TYPE_UNUSED(0x7f),

  { 0x80, "Minix <1.4a" }, 
  { 0x81, "Minix >1.4b" }, 
  { 0x82, "Linux swap / Solaris" },
  { 0x83, "Linux" }, 
  { 0x84, "OS/2 hidden C:" }, 
  { 0x85, "Linux extended" }, 
  { 0x86, "NTFS volume set" },
  { 0x87, "NTFS volume set" }, 
  { 0x88, "Linux plaintext" }, 

  MBR_TYPE_UNUSED(0x89),

  { 0x8a, "Linux kernel" }, 
  { 0x8b, "Fault-tolerant FAT32" }, 
  { 0x8c, "Fault-tolerant FAT32 (Int13h)" }, 
  { 0x8d, "FDISK hidden FAT16" }, 
  { 0x8e, "Linux LVM" }, 

  MBR_TYPE_UNUSED(0x8f),

  { 0x90, "Free FDISK hidden FAT16" }, 
  { 0x91, "Free FDISK hidden extended" }, 
  { 0x92, "Free FDISK hidden large FAT16" }, 
  { 0x93, "Amoeba" }, 
  { 0x94, "Amoeba BBT" },  
  { 0x95, "MIT EXOPC native" },  

  MBR_TYPE_UNUSED(0x96),

  { 0x97, "Free FDISK hidden FAT32" }, 
  { 0x98, "Free FDISK hidden FAT32 (LBA)" }, 
  { 0x99, "DCE376 logical drive" }, 
  { 0x9a, "Free FDISK hidden FAT16 (LBA)" }, 
  { 0x9b, "Free FDISK hidden extended (LBA)" }, 

  MBR_TYPE_UNUSED(0x9c), MBR_TYPE_UNUSED(0x9d),
  MBR_TYPE_UNUSED(0x9e),

  { 0x9f, "BSD/OS" }, 
  { 0xa0, "Laptop hibernation" }, 
  { 0xa1, "Laptop hibernation" }, 

  MBR_TYPE_UNUSED(0xa2), MBR_TYPE_RESERVED(0xa3),
  MBR_TYPE_RESERVED(0xa4),

  { 0xa5, "FreeBSD" }, 
  { 0xa6, "OpenBSD" }, 
  { 0xa7, "NeXTSTEP" }, 
  { 0xa8, "Darwin UFS" }, 
  { 0xa9, "NetBSD" }, 
  { 0xaa, "Olivetti service" }, 
  { 0xab, "Darwin boot" }, 

  MBR_TYPE_UNUSED(0xac),
  MBR_TYPE_UNUSED(0xad),

  { 0xae, "ShagOS FS" }, 
  { 0xaf, "Apple HFS/ShagOS swap" }, 
  { 0xb0, "Bootstar dummy" }, 

  MBR_TYPE_RESERVED(0xb1), MBR_TYPE_UNUSED(0xb2),
  MBR_TYPE_RESERVED(0xb3), MBR_TYPE_RESERVED(0xb4),
  MBR_TYPE_UNUSED(0xb5), MBR_TYPE_RESERVED(0xb6),

  { 0xb7, "BSDI fs" }, 
  { 0xb8, "BSDI swap" }, 

  MBR_TYPE_UNUSED(0xb9), MBR_TYPE_UNUSED(0xba),

  { 0xbb, "Boot Wizard Hidden" },

  MBR_TYPE_UNUSED(0xbc), MBR_TYPE_UNUSED(0xbd),

  { 0xbe, "Solaris boot" }, 
  { 0xbf, "Solaris" }, 

  MBR_TYPE_UNUSED(0xc0),

  { 0xc1, "DRDOS/2 FAT12" }, 
  { 0xc2, "Hidden Linux" }, 
  { 0xc3, "Hidden Linux swap" }, 
  { 0xc4, "DRDOS/2 small FAT16" }, 
  { 0xc5, "DRDOS/2 extended" }, 
  { 0xc6, "DRDOS/2 FAT16" }, 
  { 0xc7, "Syrinx" },  
  { 0xc8, "Reserved for DRDOS/2" },  
  { 0xc9, "Reserved for DRDOS/2" },  
  { 0xca, "Reserved for DRDOS/2" },  
  { 0xcb, "Reserved for DRDOS/2" },  
  { 0xcc, "Reserved for DRDOS/2" },  
  { 0xcd, "CTOS Memdump" },  
  { 0xce, "Reserved for DRDOS/2" },  

  MBR_TYPE_UNUSED(0xcf),

  { 0xd0, "REAL/32" },

  MBR_TYPE_UNUSED(0xd1), MBR_TYPE_UNUSED(0xd2),
  MBR_TYPE_UNUSED(0xd3), MBR_TYPE_UNUSED(0xd4),
  MBR_TYPE_UNUSED(0xd5), MBR_TYPE_UNUSED(0xd6),
  MBR_TYPE_UNUSED(0xd7),

  { 0xd8, "CP/M-86" },

  MBR_TYPE_UNUSED(0xd9),

  { 0xda, "Non-FS data" },
  { 0xdb, "CP/M / CTOS" }, 

  MBR_TYPE_UNUSED(0xdc),

  { 0xdd, "Hidden CTOS Memdump" },  
  { 0xde, "Dell Utility" }, 
  { 0xdf, "BootIt" },  
  { 0xe0, "ST AVFS" },  
  { 0xe1, "DOS access" }, 

  MBR_TYPE_UNUSED(0xe2),

  { 0xe3, "DOS R/O" }, 
  { 0xe4, "SpeedStor" },  

  MBR_TYPE_RESERVED(0xe5), MBR_TYPE_RESERVED(0xe6),
  MBR_TYPE_UNUSED(0xe7), MBR_TYPE_UNUSED(0xe8),
  MBR_TYPE_UNUSED(0xe9), MBR_TYPE_UNUSED(0xea),

  { 0xeb, "BeOS fs" }, 

  MBR_TYPE_UNUSED(0xec),

  { 0xed, "Sprytix" }, 
  { 0xee, "GPT" }, 
  { 0xef, "EFI FAT" }, 
  { 0xf0, "Lnx/PA-RISC bt" }, 
  { 0xf1, "SpeedStor" }, 
  { 0xf2, "DOS secondary" }, 
  { 0xf3, "SpeedStor" }, 
  { 0xf4, "SpeedStor" },  

  MBR_TYPE_UNUSED(0xf5),

  { 0xf6, "SpeedStor" },  

  MBR_TYPE_UNUSED(0xf7), MBR_TYPE_UNUSED(0xf8),
  MBR_TYPE_UNUSED(0xf9),

  { 0xfa, "Bochs" }, 
  { 0xfb, "VmWare" }, 
  { 0xfc, "VmWare swap" }, 
  { 0xfd, "Lnx RAID auto" }, 
  { 0xfe, "LANstep" }, 
  { 0xff, "XENIX BBT" },
  { 0, NULL }
};
