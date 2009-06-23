.section .excep,#alloc,#execinstr
	.align 4
	.global irq_handler

irq_handler:
	subs pc, lr, #0x4
