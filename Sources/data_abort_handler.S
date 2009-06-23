.section .excep,#alloc,#execinstr
	.align 4
	.global data_abort_handler

data_abort_handler:
	subs	pc,	lr,	#4

