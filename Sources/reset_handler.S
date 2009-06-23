#define FIQ_MODE 0xD1
#define IRQ_MODE 0xD2
#define SVC_MODE 0xD3
#define USR_MODE 0xD0

.section .excep,#alloc,#execinstr
	.align 4
	.global reset_handler
	.extern system_kickstart
	.extern CPU_SVC_STACK_ADDR

reset_handler:
	@FIQ - SP = .FIQ_STACK
	mov	r0, #FIQ_MODE
	msr	cpsr_c, r0
	ldr	sp, = CPU_SVC_STACK_ADDR

	@IRQ - SP = .IRQ_STACK
	mov	r0, #IRQ_MODE
	msr	cpsr_c, r0
	ldr	sp, = CPU_SVC_STACK_ADDR

	@SVC - SP = .SVC_STACK
	mov	r0,  #SVC_MODE
	msr cpsr_c, r0
	ldr	sp, = CPU_SVC_STACK_ADDR
	mov fp, sp

	@ Disable CPU interrupts
	mrs r0, cpsr
	orr r0, r0, #0xC0
	msr cpsr_c, r0

	@ Start the Operating system
	ldr	lr, = system_kickstart
	mov pc, lr

