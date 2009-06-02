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

#ifndef _DNALIBC_LIBOS_LIBC_H_
#define _DNALIBC_LIBOS_LIBC_H_

#include <stddef.h>
#include <stdint.h>

extern void * dna_memcpy (void * restrict s1, const void * restrict s2, size_t n);
extern void * dna_memset (void *s, int32_t c, size_t n);

extern char * dna_strcpy (char * restrict s1, const char * restrict s2);
extern char * dna_strcat (char * restrict dest, const char * restrict src);
extern int32_t dna_strcmp (const char *s1, const char *s2);
extern size_t dna_strlen (const char *s);

extern int32_t dna_atoi (char * alpha);
extern int32_t dna_itoa (int32_t integer, char * buffer);

extern void dna_printf (const char * restrict format, ...);

#endif
