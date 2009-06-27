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

void queue_extract (queue_t * queue, queue_item_t * item)
{
	queue_item_t * item = queue -> head;

  /*
   * TODO: we need to check the case
   * where item is not in the queue
   */

	if (queue -> head == item)
  {
    queue -> head = item -> next;
  }
	else
  {
		while (item -> next != item)
    {
      item = item -> next;
    }

		item -> next = item -> next;

		if (item -> next == NULL)
    {
      queue -> tail = item;
    }
	}

  item -> next = NULL;
	queue -> status -= 1;
}

