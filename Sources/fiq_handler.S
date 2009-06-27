.section .excep,#alloc,#execinstr
	.align 4
	.global fiq_handler

fiq_handler:
  b fiq_handler
