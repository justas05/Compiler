	.text
	.globl	main
main:
	addiu	$sp,$sp,-12
	sw	$fp,8($sp)
	move	$fp,$sp
	li	$2,23
	sw	$2,4($fp)
	li	$2,123
	sw	$2,4($fp)
	lw	$2,4($fp)
	move	$sp,$fp
	lw	$fp,8($sp)
	addiu	$sp,$sp,12
	jr	$31
	nop
