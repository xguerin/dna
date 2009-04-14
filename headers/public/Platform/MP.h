#ifndef PLATFORM_MP_H
#define PLATFORM_MP_H

extern unsigned int PLATFORM_N_ARM7TDMI;
extern unsigned int PLATFORM_N_SPARCV8;
extern unsigned int PLATFORM_N_MIPSR3000;
extern unsigned int PLATFORM_N_MIPSMIPS32;
extern unsigned int PLATFORM_N_NIOS2FAST;
extern unsigned int PLATFORM_N_UBLAZE81i;

#define platform_mp_cpu_count(what) PLATFORM_N_##what

#endif
