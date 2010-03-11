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

#ifndef MULTIMEDIACARD_OCR_PRIVATE_H
#define MULTIMEDIACARD_OCR_PRIVATE_H

#include <stdint.h>

typedef union _mmc_ocr
{
  uint32_t raw;

  struct _mmc_ocr_bits
  {
    uint32_t reserved_0      :15;
    uint32_t voltage_27_28   :1;
    uint32_t voltage_28_29   :1;
    uint32_t voltage_29_30   :1;
    uint32_t voltage_30_31   :1;
    uint32_t voltage_31_32   :1;
    uint32_t voltage_32_33   :1;
    uint32_t voltage_33_34   :1;
    uint32_t voltage_34_35   :1;
    uint32_t voltage_35_36   :1;
    uint32_t reserved_1      :6;
    uint32_t capacity_status :1;
    uint32_t not_busy        :1;
  }
  bits;
}
mmc_ocr_t;

extern void mmc_show_ocr (mmc_ocr_t ocr);

#endif

