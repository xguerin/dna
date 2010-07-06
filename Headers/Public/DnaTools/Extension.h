/****h* Tools/Extension
 * SUMMARY
 * Extension management.
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

#ifndef DNA_TOOLS_EXTENSION_H
#define DNA_TOOLS_EXTENSION_H

#include <stdint.h>
#include <DnaTools/Status.h>

/****t* Extension/extension_cmd_t
 * SUMMARY
 * Module commands type.
 *
 * SOURCE
 */

typedef enum extension_cmd
{
  DNA_EXTENSION_INIT,
  DNA_EXTENSION_UNINIT
}
extension_cmd_t;

/*
 ****/

/****t* Extension/extension_t
 * SUMMARY
 * Module type.
 *
 * SOURCE
 */

typedef struct extension_info
{
  char * name;
  int32_t flags;
  status_t (* init_extension) (void);
  void (* uninit_extension) (void);
}
extension_info_t;

/*
 ****/

extern int32_t OS_N_EXTENSIONS;
extern extension_info_t * OS_EXTENSIONS_LIST[];

extern status_t extension_load (char * name, extension_info_t ** p_info);

#endif

