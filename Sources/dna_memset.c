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

void * dna_memset (void * s, int32_t c, size_t n){
	int32_t rem = n % 4;
	int32_t quo = n >> 2;
	int32_t step = 0;
	
	for (step = 0; step < quo; step++)
		*((int32_t *)s + step) = c;

	for (step = 0; step < rem; step++)
		*((char *)s + step) = (char)c;
	
	return s;
}

