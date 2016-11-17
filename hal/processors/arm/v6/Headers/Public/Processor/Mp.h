#ifndef PROCESSOR_MP_H
#define PROCESSOR_MP_H

#include <stdint.h>
#include <Platform/Platform.h>

extern volatile int32_t cpu_mp_synchro;

#define CPU_IPI_ALL   0xFFFFFFFF 

extern int32_t cpu_mp_id (void);
extern int32_t cpu_mp_count (void);

extern void cpu_mp_wait (void);
extern void cpu_mp_proceed (void);

extern void cpu_mp_send_ipi (int32_t id, int32_t command, void * arguments);

#endif

