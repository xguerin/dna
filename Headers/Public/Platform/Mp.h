#ifndef PLATFORM_MP_H
#define PLATFORM_MP_H

#include <stdint.h>

typedef struct _soclib_mailbox
{
  uint32_t command;
  uint32_t data;
  uint32_t reserved;
  uint32_t reset;
}
soclib_mailbox_t;

extern soclib_mailbox_t PLATFORM_MAILBOXES[];
 
extern uint32_t PLATFORM_N_ARM7TDMI;
extern uint32_t PLATFORM_N_SPARCV8;
extern uint32_t PLATFORM_N_MIPSR3000;
extern uint32_t PLATFORM_N_MIPSMIPS32;
extern uint32_t PLATFORM_N_NIOS2FAST;
extern uint32_t PLATFORM_N_UBLAZE81i;

#define platform_mp_cpu_count(what) PLATFORM_N_##what
extern void cpu_mp_send (int32_t target, int32_t command, int32_t data);

#endif
