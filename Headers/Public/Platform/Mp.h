#ifndef PLATFORM_MP_H
#define PLATFORM_MP_H

#include <stdint.h>

extern uint32_t PLATFORM_N_ARMV6;
extern uint32_t PLATFORM_N_SPARCV8;
extern uint32_t PLATFORM_N_MIPSR3000;
extern uint32_t PLATFORM_N_MIPSMIPS32;
extern uint32_t PLATFORM_N_NIOS2FAST;
extern uint32_t PLATFORM_N_UBLAZE81i;

#define platform_mp_cpu_count(what) PLATFORM_N_##what

#endif
