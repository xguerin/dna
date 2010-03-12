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

#include <stdbool.h>
#include <Private/MultiMediaCard.h>
#include <Core/Core.h>
#include <MemoryManager/MemoryManager.h>
#include <DnaTools/DnaTools.h>

status_t mmc_card_create (mmc_card_t * p_card, mmc_callbacks_t callbacks)
{
  status_t status = DNA_OK;
  bool is_sdhc = is_sdhc, has_ccs = false, loop = true;
  uint32_t argument = 0, block_size = 0, block_count = 0, multiplier;
  uint32_t response[4];
  uint32_t command = GO_IDLE_STATE;
  mmc_card_private_t card = NULL;
  mmc_ocr_t ocr = { .raw = 0 };

  watch (status_t)
  {
    ensure (p_card != NULL, DNA_BAD_ARGUMENT);

    /*
     * Allocate the p_card object.
     */

    card = kernel_malloc (sizeof (struct _mmc_card_private), true);
    card -> callbacks = callbacks;

    /*
     * Execute the initialization state machine.
     */

    while (loop) switch (command)
    {
      case GO_IDLE_STATE :
        {
          log (VERBOSE_LEVEL, "[GO_IDLE_STATE]");
          status = callbacks . send_command (command, 0, response);
          check (error, status == DNA_OK, status);

          command = is_sdhc ? SEND_IF_COND : SD_CMD_APP_SEND_OP_COND;
          break;
        }

      case SEND_IF_COND :
        {
          log (VERBOSE_LEVEL, "[SEND_IF_COND]");

          status = callbacks . send_command (command, 0x1AA, response);

          is_sdhc = (response[0] == 0x1AA) ? true : false;
          command = is_sdhc ? SD_CMD_APP_SEND_OP_COND : GO_IDLE_STATE;

          break;
        }

      case SD_CMD_APP_SEND_OP_COND :
        {
          log (VERBOSE_LEVEL, "[SD_CMD_APP_SEND_OP_COND]");

          command = APP_CMD;
          status = callbacks . send_command (command, 0, response);
          check (error, status == DNA_OK, status);

          command = SD_CMD_APP_SEND_OP_COND;
          argument = MMC_VOLTAGE_RANGE | (is_sdhc ? MMC_SD_HCS : 0);

          status = callbacks . send_command (command, argument, response);
          check (error, status == DNA_OK, status);

          ocr . raw = response[0];

          if (ocr . bits . not_busy != 1)
          {
            command = SD_CMD_APP_SEND_OP_COND;
          }
          else
          {
            has_ccs = (ocr . bits . capacity_status == 1);
            card -> card . type = is_sdhc && has_ccs ?
              MMC_SDHC_CARD : MMC_SD_CARD;
            command = ALL_SEND_CID;
          }

          break;
        }

      case ALL_SEND_CID :
        {
          log (VERBOSE_LEVEL, "[ALL_SEND_CID]");

          status = callbacks . send_command (command, 0, response);
          check (error, status == DNA_OK, status);

          mmc_build_cid (response, & card -> cid);
          command = SEND_RELATIVE_ADDR;

          break;
        }

      case SEND_RELATIVE_ADDR :
        {
          log (VERBOSE_LEVEL, "[SEND_RELATIVE_ADDR]");

          status = callbacks . send_command (command, 0, response);
          check (error, status == DNA_OK, status);

          card -> rca . raw = response[0];
          command = SEND_CSD;

          break;
        }

      case SEND_CSD :
        {
          log (VERBOSE_LEVEL, "[SEND_CSD]");

          status = callbacks . send_command (command,
              card -> rca . raw, response);
          check (error, status == DNA_OK, status);

          mmc_build_csd (response, & card -> csd);

          switch (card -> csd . csd_structure)
          {
            case CSD_V1_STRUCTURE :
              multiplier = 1 << (card -> csd . size_multiplier + 2); 
              block_size = 1 << card -> csd . read_block_length;
              block_count = (card -> csd . device_size + 1) * multiplier;
              break;

            case CSD_V2_STRUCTURE :
              block_size = 512;
              block_count = card -> csd . device_size + 1;
              break;
          }

          card -> card . info . block_size = block_size;
          card -> card . info . block_count = block_count;

          loop = false;
          break;
        }

      default :
        {
          log (PANIC_LEVEL, "Unsupported MMC command.");
          return DNA_ERROR;
        }
    }

    /*
     * Fill-in some remaining parameters.
     */

    card -> card . info . removable = true;
    card -> card . info . read_only = true;
    card -> card . info . write_once = false;

    *p_card = & card -> card;
    return DNA_OK;
  }

  rescue (error)
  {
    kernel_free (card);
    leave;
  }
}
