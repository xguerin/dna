.section .excep,#alloc,#execinstr
	.align 4
	.global swi_handler

swi_handler:
	subs	pc,	lr,	#4
