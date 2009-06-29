#ifndef PROCESSOR_CACHE_H
#define PROCESSOR_CACHE_H

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

#endif

