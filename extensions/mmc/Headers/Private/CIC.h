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

#ifndef MULTIMEDIACARD_CIC_PRIVATE_H
#define MULTIMEDIACARD_CIC_PRIVATE_H

#include <stdint.h>

typedef union _mmc_cic
{
  uint32_t raw;

  struct _mmc_cic_bits
  {
    uint32_t check_pattern    :8;
    uint32_t voltage_accepted :4;
    uint32_t reserved         :20;
  }
  bits;
}
mmc_cic_t;

extern void mmc_show_cic (mmc_cic_t cic);

#endif

