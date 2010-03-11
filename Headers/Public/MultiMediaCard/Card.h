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

#ifndef MULTIMEDIACARD_CARD_H
#define MULTIMEDIACARD_CARD_H

#include <stdint.h>
#include <Core/Core.h>
#include <DnaTools/DnaTools.h>

typedef enum _mmc_card_type
{
  MMC_SD_CARD,
  MMC_SDHC_CARD,
  MMC_MMC_CARD
}
mmc_card_type_t;

typedef struct _mmc_callbacks
{
  status_t (* send_command) (uint32_t, uint32_t response[4]);
  bool (* is_connected) (void);
}
mmc_callbacks_t;

typedef struct _mmc_card
{
  mmc_card_type_t;
  device_info_t info;
}
* mmc_card_t;

extern status_t mmc_card_create (mmc_card_t * card, mmc_callbacks_t callbacks);
extern status_t mmc_card_destroy (mmc_card_t card);
  
#endif
