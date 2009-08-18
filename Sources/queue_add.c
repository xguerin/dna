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

status_t queue_add (queue_t * queue, void * data)
{
  queue_item_t * item = data;

  watch (status_t)
  {
    check (queue_error, item -> next == NULL, DNA_ERROR);

    if (queue -> status == 0)
    {
      queue -> head = item;
      queue -> tail = item;
    }
    else
    {
      queue -> tail -> next = item;
      queue -> tail = item;
    }

    queue -> status += 1;

    return DNA_OK;
  }

  rescue (queue_error)
  {
    log (1, "Q(0x%x): status %d, item 0x%x",
        queue, queue -> status, item);

    leave;
  }

}

