	.section "x"

	.global start

	
start:
	b	vector_copy

vector_start:
	ldr	pc,	reset_target
	ldr	pc,	undefined_target
	ldr	pc,	svc_target
	ldr	pc,	prefetch_target
	ldr	pc,	abort_target
	nop
	ldr	pc,	irq_target
	ldr	pc,	fiq_target

reset_target:		.word	reset_entry	-4
undefined_target:	.word	undefined_entry	-4
svc_target:		.word	syscall_entry	-4
prefetch_target:	.word	prefetch_entry	-4
abort_target:		.word	abort_entry	-4
irq_target:		.word	interrupt_entry	-4
fiq_target:		.word	fiq_entry	-4

vector_end:

reset_entry:	/* A */
	ldr	r5,	[pc, #8]
	mov	r4,	#0x41
	str	r4,	[r5]
	b	reset_entry
	.word	0x10009000

undefined_entry:/* B */
	ldr	r5,	[pc, #8]
	mov	r4,	#0x42
	str	r4,	[r5]	
	b	undefined_entry
	.word	0x10009000

interrupt_entry:/* C */
	ldr	r5,	[pc, #8]
	mov	r4,	#0x43
	str	r4,	[r5]
	b	interrupt_entry
	.word	0x10009000
	
syscall_entry:	/* D */
	ldr	r5,	[pc, #8]
	mov	r4,	#0x44
	str	r4,	[r5]
	b	syscall_entry
	.word	0x10009000
	
abort_entry:	/* E */
	ldr	r5,	[pc, #8]
	mov	r4,	#0x45
	str	r4,	[r5]
	b	abort_entry
	.word	0x10009000
	
fiq_entry:	/* F */
	ldr	r5,	[pc, #8]
	mov	r4,	#0x46
	str	r4,	[r5]
	b	fiq_entry
	.word	0x10009000
	
prefetch_entry:	/* G */
	ldr	r5,	[pc, #8]
	mov	r4,	#0x47
	str	r4,	[r5]
	b	prefetch_entry
	.word	0x10009000

vector_copy:
	ldr	r1,	=0x70010000
	mov	r2,	r1
	ldr	r7,	=vector_start
	ldr	r8,	=vector_copy
	add	r1,	r7
	add	r2,	r8
	ldr	r3,	=0x0
1:				
	ldr	r4,	[r1]
	str	r4,	[r3]
	add	r1,	r1,	#4
	add	r3,	r3,	#4
	teq	r1,	r2	
	bne	1b
	ldr	sp,	=stack_top
	bl	main
	b	vector_copy
