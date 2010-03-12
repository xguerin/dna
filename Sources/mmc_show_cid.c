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

#include <Private/CID.h>
#include <DnaTools/DnaTools.h>

void mmc_show_cid (mmc_cid_t cid)
{
  log (INFO_LEVEL, "Manufacturer ID: 0x%x", cid . manufacturer_id);
  log (INFO_LEVEL, "OEM ID: 0x%x", cid . oem_id);
  log (INFO_LEVEL, "Product name : %s", cid . name);
  log (INFO_LEVEL, "Revision: 0x%x", cid . revision);
  log (INFO_LEVEL, "S/N: 0x%x", cid . serial_number);
  log (INFO_LEVEL, "Date: 0x%x", cid . manufacturing_date);
}

