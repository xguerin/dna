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

#include <DnaTools/DnaTools.h>

void * queue_lookup (queue_t * queue, queue_inspector_t inspector,
    void * a0, void * a1)
{
  queue_item_t * item = NULL;

  if (queue -> status != 0)
  {
    for (item = queue -> head; item != NULL; item = item -> next)
    {
      if (inspector (item, a0, a1)) break;
    }
  }

  return item;
}

