.section .excep,#alloc,#execinstr
	.align 4
	.global cpu_context_init

cpu_context_init:
	stmdb sp!, {r4,r5,r6}
	ldr r4, [sp, #12]
	str r4, [r0], #4
	stmia r0!, {r1-r10, fp, ip}
	add r5, r1, r2 
	sub r5, r5, #4
	str r5, [r0], #4
	str r3, [r0], #4
	mrs r5, cpsr
	mrs r6, spsr
	stmia r0!, {r5,r6}
	ldmia sp!, {r4, r5,r6}
	mov pc, lr
