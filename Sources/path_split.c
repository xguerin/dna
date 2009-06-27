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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
 * GNU General Public License for more details.                          
 *                                                                       
 * You should have received a copy of the GNU General Public License     
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */

#include <Private/VirtualFileSystem.h>
#include <DnaTools/DnaTools.h>

/****f* path/path_split
 * SUMMARY
 *
 * SYNOPSIS
 */

status_t path_split (char * restrict path, char * restrict token)

/*
 * ARGUMENTS
 *
 * FUNCTION
 *
 * RESULT
 *
 * SOURCE
 */

{
	char * p_start = NULL, * p_end = NULL;
	char * p_ptr, * t_ptr = NULL;

  watch (status_t)
  {
    ensure (path != NULL && token != NULL, DNA_ERROR);

    p_end = path + dna_strlen (path);

    if (*p_end == '/')
    {
      *(p_end + 1) = '.';
      p_end += 1;
    }

    while (*p_end == '/') p_end -= 1;
    p_start = p_end;

    while (*p_start != '/' && p_start != path) p_start -= 1;
    ensure (*p_start == '/', DNA_ERROR);

    p_ptr = p_start + 1;
    t_ptr = token;

    while (p_ptr != p_end) *t_ptr ++  = *p_ptr ++;
    * (p_start + 1) = '\0';
    * t_ptr = '\0';

    return DNA_OK;
  }
}

/*
 ****/

