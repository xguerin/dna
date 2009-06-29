#ifndef PROCESSOR_MP_H
#define PROCESSOR_MP_H

#include <stdint.h>
#include <Platform/Platform.h>

extern volatile int32_t cpu_mp_synchro;

#define cpu_mp_count platform_mp_cpu_count(ARMV6)
extern int32_t cpu_mp_id(void);

extern void cpu_mp_wait (void);
extern void cpu_mp_proceed (void);

#endif

