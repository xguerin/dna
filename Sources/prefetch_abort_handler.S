.section .excep,#alloc,#execinstr
	.align 4
	.global prefetch_abort_handler

prefetch_abort_handler:
	subs	pc,	lr,	#4
