.section .excep,#alloc,#execinstr
	.align 4
	.global cpu_context_save

cpu_context_save:
	stmia r0!, {r0 - r10, fp, ip, sp}
  str r1, [r0], #4
	stmdb sp!, {r5,r6}
	mrs r5, cpsr
	mrs r6, spsr
	stmia	r0, {r5, r6}
	ldmia sp!, {r5,r6}
	mov pc, lr

