#ifndef PROCESSOR_SYNCHRONIZATION_H
#define PROCESSOR_SYNCHRONIZATION_H

static inline long int cpu_test_and_set (volatile long int * spinlock)
{
  register long int ret, temp = 1; 

  __asm__ volatile
    ("\n"
     "__start_tst:\n"
     "ldrex  %0, %3\n"
     "cmp    %0, #0\n"
     "bne    __end_tst\n"
     "strex  r1, %1, %2\n"
     "cmp    r1, #0\n"
     "bne    __start_tst\n"
     "__end_tst:\n"
     : "=&r" (ret), "=&r" (temp), "=m" (*spinlock)
     : "m" (*spinlock)
     : "memory", "r1");

  return ret;
}

static inline long int cpu_compare_and_swap (volatile long int * p_val,
    long int oldval, long int newval)
{
  register long int ret;

  __asm__ volatile
    ("\n"
     "__start_cas:\n"
     "ldrex  %0, %4\n"
     "cmp    %0, %2\n"
     "bne    __end_cas\n"
     "mov    %0, %3\n"
     "strex  r1, %0, %1\n"
     "cmp    r1, #0\n"
     "bne    __start_cas\n"
     "__end_cas:\n"
     : "=&r" (ret), "=m" (*p_val)
     : "r" (oldval), "r" (newval), "m" (*p_val)
     : "memory", "r1");

  return ret;
}

#endif

