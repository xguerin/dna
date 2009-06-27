#include <stdint.h>
#include <Processor/Processor.h>


void cpu_trap_attach_isr (interrupt_id_t id, uint32_t mode,
    interrupt_handler_t isr)
{
	handler_table[id] = isr;
}

