/****h* support/types
 * SUMMARY
 * Support types.
 ****
 * Copyright (C) 2007 TIMA Laboratory                                    
 *                                                                       
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, either version 3 of the License, or     
 * (at your option) any later version.                                   
 *                                                                       
 * This program is distributed in the hope that it will be useful,       
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
 * GNU General Public License for more details.                          
 *                                                                       
 * You should have received a copy of the GNU General Public License     
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */

#ifndef DNA_SUPPORT_DRIVER_H
#define DNA_SUPPORT_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#include <DnaTools/Status.h>

/****v* support/device_control_code
 * SUMMARY
 * Device control codes.
 */

enum _device_control_codes
{
  DNA_GET_DEVICE_SIZE = 0,
  DNA_SET_DEVICE_SIZE,
  DNA_SET_NONBLOCKING_IO,
  DNA_SET_BLOCKING_IO,
  DNA_GET_READ_STATUS,
  DNA_GET_WRITE_STATUS,
  DNA_GET_INFO,
  DNA_CONTROL_CODES_END = 0x0FFF
};

/*
 ****/

/****v* support/device_type
 * SUMMARY
 * Device types.
 */

enum _device_type
{
  DNA_CHARACTER_DEVICE,
  DNA_BLOCK_DEVICE
};

/*
 ****/

/****t* support/device_info_t
 * SUMMARY
 * Device information.
 *
 * SOURCE
 */

typedef struct _device_info
{
  int32_t type;
  uint32_t bytes_per_sector;
  uint32_t sectors_per_track;
  uint32_t cylinder_count;
  uint32_t head_count;
  bool removable;
  bool read_only;
  bool write_once;
}
device_info_t;

/*
 ****/

/****t* support/device_cmd_t
 * SUMMARY
 * Device commands type.
 *
 * SOURCE
 */

typedef struct _device_cmd
{
  status_t (* open) (char * name, int32_t mode, void ** cookie);
  status_t (* close) (void * cookie);
  status_t (* free) (void * cookie);
  status_t (* read) (void * handler, void * destination, int64_t offset, int32_t * p_res);
  status_t (* write) (void * handler, void * source, int64_t offset, int32_t * p_res);
  status_t (* control) (void * handler, int32_t operation, void * data, int32_t * p_res);
}
device_cmd_t;

/*
 ****/

/****t* support/driver_t
 * SUMMARY
 * Driver type.
 *
 * SOURCE
 */

typedef struct _driver
{
  char * name;
  status_t (* init_hardware) (void);
  status_t (* init_driver) (void);
  void (* uninit_driver) (void);
  const char ** (* publish_devices) (void);
  device_cmd_t * (* find_device) (const char * name);
}
driver_t;

/*
 ****/

extern int32_t OS_N_DRIVERS;
extern driver_t * OS_DRIVERS_LIST [];

extern status_t driver_load (char * name, driver_t ** drv);

#endif

