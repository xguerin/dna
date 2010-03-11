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

#include <Private/Card.h>
#include <Private/Command.h>
#include <MultiMediaCard/Operation.h>
#include <DnaTools/DnaTools.h>

status_t mmc_execute (mmc_card_t a_card, mmc_operation_t operation,
    void * buffer, int64_t block_start, int32_t block_count)
{
  bool loop = true, card_selected = false;
  uint32_t response[4], argument = 0;
  uint32_t block_offset = 0, multiplier = 0;
  mmc_card_private_t card = (mmc_card_private_t)a_card;
  mmc_card_status_t card_status;
  status_t status;
  uint32_t command = SELECT_DESELECT_CARD;

  watch (status_t)
  {
    while (loop) switch (command)
    {
      case SELECT_DESELECT_CARD :
        {
          log (VERBOSE_LEVEL, "[SELECT_DESELECT_CARD]");

          argument = card_selected ? 0 : card -> rca . raw;
          status = card -> callbacks . send_command (command,
              argument, response);

          if (! card_selected)
          {
            card_selected = true;
            command = SD_SET_BUS_WIDTH;
          }
          else
          {
            card_selected = false;
            loop = false;
          }

          break;
        }

      case SD_SET_BUS_WIDTH :
        {
          log (VERBOSE_LEVEL, "[SD_SET_BUS_WIDTH]");

          command = APP_CMD;
          status = card -> callbacks . send_command (command,
              card -> rca . raw, response);
          check (error, status == DNA_OK, status);

          command = SD_SET_BUS_WIDTH;
          status = card -> callbacks . send_command (command, 2, response);
          check (error, status == DNA_OK, status);

          command = SET_BLOCK_LENGTH;
          break;
        }

      case SET_BLOCK_LENGTH :
        {
          log (VERBOSE_LEVEL, "[SET_BLOCK_LENGTH]");

          status = card -> callbacks . send_command (command, 512, response);
          check (error, status == DNA_OK, status);

          command = READ_SINGLE_BLOCK;
          break;
        }

      case READ_SINGLE_BLOCK :
        {
          log (VERBOSE_LEVEL, "[READ_SINGLE_BLOCK]");

          multiplier = (card -> card . type == MMC_SD_CARD) ? 9 : 0;
          block_offset = block_start << multiplier;

          status = card -> callbacks . send_command (command,
              block_offset, response);
          check (error, status == DNA_OK, status);

          status = card -> callbacks . read (buffer, block_count >> 2);
          check (error, status == DNA_OK, status);

          command = SELECT_DESELECT_CARD;
          break;
        }

      default :
        {
          log (PANIC_LEVEL, "Unsupported MMC command.");
          return DNA_ERROR;
        }
    }

    return DNA_OK;
  }

  rescue (error)
  {
    card_status . raw = response[0];
    mmc_show_card_status (card_status);

    leave;
  }
}

