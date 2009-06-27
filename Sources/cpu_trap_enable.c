#include <Processor/Processor.h>

void cpu_trap_enable (interrupt_id_t id)
{
  register uint32_t mask = 1 << (id + 6);

  __asm__ volatile (
      "mrs r0, cpsr\n"
      "bic r0, r0, %0\n"
      "msr cpsr, r0\n"
      : 
      : "r"(mask)
      : "r0"
      );
}

