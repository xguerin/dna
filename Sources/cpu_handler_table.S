.section .data
	.align 4
	.global cpu_handler_table

cpu_handler_table:
  .word _handler_table_0 
  .word _handler_table_1 

_handler_table_0:
  .fill 8, 4, 0

_handler_table_1:
  .fill 8, 4, 0

