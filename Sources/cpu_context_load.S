.section .excep,#alloc,#execinstr
	.align 4
	.global cpu_context_load

cpu_context_load:
	ldr	r5, [r0, #60]
	ldr	r6, [r0, #64]
	msr	cpsr, r5
	msr	spsr, r6
	ldmia	r0, {r0 - r10, fp, ip, sp, pc}

