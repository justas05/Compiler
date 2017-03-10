	.text
	.globl	main
main:
	addiu	$sp,$sp,-16
	sw	$fp,12($sp)
	move	$fp,$sp
	li	$2,12
	sw	$2,4($fp)
	li	$2,39
	sw	$2,8($fp)
	lw	$2,4($fp)
	move	$sp,$fp
	lw	$fp,12($sp)
	addiu	$sp,$sp,16
	jr	$31
	nop
