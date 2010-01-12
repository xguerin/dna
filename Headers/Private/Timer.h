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

#ifndef CPU_TIMER__PRIVATE_H
#define CPU_TIMER__PRIVATE_H

#include <stdint.h>

#define SOCLIB_TIMER_RES 10

typedef struct _soclib_timer_port
{
  uint32_t value;
  uint32_t mode;
  uint32_t period;
  uint32_t irq_ack;
}
* soclib_timer_port_t;

typedef struct _soclib_timer_config
{
  uint32_t irq;
  soclib_timer_port_t port;
}
soclib_timer_config_t;

typedef struct _soclib_timer
{
  char * name;
  uint32_t irq;
  soclib_timer_port_t port;
}
soclib_timer_t;

extern uint32_t SOCLIB_TIMER_NDEV;
extern soclib_timer_config_t SOCLIB_TIMER_DEVICES[];

extern void timer_callback (void);
extern int32_t timer_handler (int32_t itn);

#endif

