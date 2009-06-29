#ifndef PROCESSOR_CACHE_H
#define PROCESSOR_CACHE_H

#if 0 

/*
 * FIXME: This is the way it should be. It is not
 * supported by the SoCLib arm ISS yet.
 */

#define cpu_cache_wbflush()                   \
{                                             \
	volatile register int32_t dummy = 0;        \
                                              \
  __asm__ volatile (                          \
      "mcr p15,0,%0,c7,c10,4"                 \
      :                                       \
      : "r"(dummy)                            \
      );                                      \
}

#else

#define cpu_cache_wbflush()                   \
{                                             \
                                              \
}

#endif

#endif

