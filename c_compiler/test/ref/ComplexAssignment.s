	.file	1 "ComplexAssignment.c"
	.section .mdebug.abi32
	.previous
	.nan	legacy
	.module	fp=xx
	.module	nooddspreg
	.abicalls
	.text
	.align	2
	.globl	main
	.set	nomips16
	.set	nomicromips
	.ent	main
	.type	main, @function
main:
	.frame	$fp,32,$31		# vars= 24, regs= 1/0, args= 0, gp= 0
	.mask	0x40000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$fp,28($sp)
	move	$fp,$sp
	li	$2,5			# 0x5
	sw	$2,0($fp)
	li	$2,49			# 0x31
	sw	$2,4($fp)
	li	$2,239			# 0xef
	sw	$2,8($fp)
	li	$2,23			# 0x17
	sw	$2,12($fp)
	li	$2,234			# 0xea
	sw	$2,16($fp)
	li	$2,12			# 0xc
	sw	$2,20($fp)
	lw	$2,12($fp)
	sw	$2,0($fp)
	lw	$2,16($fp)
	sw	$2,4($fp)
	lw	$2,16($fp)
	sw	$2,20($fp)
	lw	$2,20($fp)
	sw	$2,0($fp)
	lw	$2,20($fp)
	sw	$2,12($fp)
	lw	$2,12($fp)
	sw	$2,0($fp)
	lw	$2,8($fp)
	sw	$2,20($fp)
	lw	$2,0($fp)
	sw	$2,8($fp)
	lw	$2,16($fp)
	sw	$2,0($fp)
	lw	$2,0($fp)
	move	$sp,$fp
	lw	$fp,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	main
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.1) 5.4.0 20160609"
