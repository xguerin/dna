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

#include <stdint.h>
#include <DnaLibrary/DnaLibrary.h>

/****f* libc/dna_printf
 * SUMMARY
 * Basic kernel printf support.
 *
 * SYNOPSIS
 */

void dna_printf (const char * restrict format, ...)

  /*
   * FUNCTION
   * Print formatted text.
   *
   * SOURCE
   */

{
  int32_t d_val = 0, ascii_index = 0, len = 0;
  uint32_t u_val = 0, rem = 0;
  char * fmt_ptr = (char *)format, ascii[32];
  char buffer[1024], * b_ptr = buffer;
  char c_val = 0, * ptr = NULL;

  va_list arg;
  va_start (arg, format);

  while(*fmt_ptr)
  {
    switch(*fmt_ptr)
    {
      case '\\':
        fmt_ptr += 1;

        switch(*fmt_ptr)
        {
          case 'n':
            *b_ptr++ = '\n';
            break;

          case 't':
            *b_ptr++ = '\t';
            break;

          default:
            *b_ptr++ = *fmt_ptr;
            break;
        }

        fmt_ptr++;
        break;

      case '%':
        fmt_ptr++;

        switch (*fmt_ptr)
        {
          case '%' :
            * b_ptr ++ = * fmt_ptr;
            break;

          case 'd' :
            d_val = va_arg(arg, long int);
            ascii_index = 0;

            if(d_val < 0) u_val = -1 * d_val;
            else u_val = d_val;

            do
            {
              rem = u_val % 10;
              u_val = u_val / 10;
              ascii[ascii_index ++] = rem + '0';
            }
            while(u_val > 0);

            if (d_val < 0) ascii[ascii_index ++] = '-';

            for(int32_t i = ascii_index - 1; i >= 0; i--) *b_ptr++ = ascii[i];

            break;

          case 'u' :
            u_val = va_arg(arg, unsigned long int);
            ascii_index = 0;

            do
            {
              rem = u_val % 10;
              u_val = u_val / 10;
              ascii[ascii_index ++] = rem + '0';
            }
            while (u_val > 0);

            for(int32_t i = ascii_index - 1; i >= 0; i--) *b_ptr++ = ascii[i];

            break;

          case 'x' :
            u_val = va_arg(arg, unsigned long int);
            ascii_index = 0;

            do
            {
              rem = u_val & 0xf;
              u_val >>= 4;
              if(rem < 10) ascii[ascii_index ++] = rem + '0'; 
              else ascii[ascii_index ++] = rem - 10 + 'a';
            }
            while(u_val > 0);

            for(int32_t i = ascii_index - 1; i >= 0; i--) *b_ptr++ = ascii[i];

            break;

          case 'c' :
            u_val = va_arg(arg, unsigned long int);
            c_val = u_val;
            * b_ptr ++ = c_val;
            break;

          case 's' :
            ptr = va_arg(arg, char *);
            len = dna_strlen (ptr);

            for (int32_t i = 0; i < len; i ++) * b_ptr ++ = ptr[i];
            break;

          default : break;
        }

        fmt_ptr += 1;
        break;

      default :
        *b_ptr++ = *fmt_ptr;
        fmt_ptr += 1;
        break;
    }
  }

  *b_ptr ++ = '\0';

  va_end (arg);
}

/*
 ****/

