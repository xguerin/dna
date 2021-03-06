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

#ifndef MASTERBOOTRECORD_OPERATION_PRIVATE_H
#define MASTERBOOTRECORD_OPERATION_PRIVATE_H

#include <stdint.h>
#include <MasterBootRecord/MasterBootRecord.h>
#include <DnaTools/DnaTools.h>

extern status_t mbr_load (uint8_t sector[512], mbr_partition_t partition[4]);
extern status_t mbr_dump (mbr_partition_t partition[4], uint8_t sector[512]);

#endif
