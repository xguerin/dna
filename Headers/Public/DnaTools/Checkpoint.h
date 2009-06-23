/****h* support/control.h
 * SUMMARY
 * Support control.
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

#ifndef DNA_TOOLS_CHECKPOINT_H
#define DNA_TOOLS_CHECKPOINT_H

#include <DnaLibrary/C.h>

#define watch(type) type _rescue_status;
#define leave return _rescue_status

#define check(tag,condition,code)         \
  if (! (condition))                      \
  {                                       \
    log (4, "<" #condition "> failed.");  \
    _rescue_status = code;                \
    goto tag;                             \
  }

#define ensure(condition,code)            \
  if (! (condition))                      \
  {                                       \
    log (4, "<" #condition "> failed.");  \
    _rescue_status = code;                \
    return _rescue_status;                \
  }

#define rescue(tag) tag:

#endif

