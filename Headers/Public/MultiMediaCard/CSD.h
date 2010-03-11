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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the 
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program.If not, see <http://www.gnu.org/licenses/>. 
 */

#ifndef MULTIMEDIACARD_CSD_H
#define MULTIMEDIACARD_CSD_H

#include <stdint.h>
#include <stdbool.h>

typedef enum _mmc_csd_structure
{
  CSD_V1_STRUCTURE = 0,
  CSD_V2_STRUCTURE = 1
}
mmc_csd_structure_t;

typedef struct _mmc_csd
{
  mmc_csd_structure_t csd_structure;

  uint8_t read_access_time;
  uint8_t read_access_cycles;
  uint8_t data_transfer_rate;
  uint16_t command_classes;

  uint32_t device_size;
  uint8_t size_multiplier;
  uint8_t file_format;

  uint8_t read_block_length;
  bool read_block_partial;
  bool read_block_misalign;

  uint8_t write_block_length;
  uint8_t write_speed_factor;
  bool write_block_partial;
  bool write_block_misalign;

  uint8_t read_vdd_min;
  uint8_t read_vdd_max;
  uint8_t write_vdd_min;
  uint8_t write_vdd_max;

  bool erase_block_enable;
  uint8_t erase_sector_size;

  bool wp_group_enable;
  uint8_t wp_group_size;

  bool dsr_implemented;
  bool file_format_group;
  bool copy;
  bool permanent_write_protect;
  bool temporary_write_protect;

  uint8_t crc;
  bool always_one;
}                                  
mmc_csd_t;                         
                                   
extern void d940_mmc_show_csd (mmc_csd_t csd);

#endif                             
