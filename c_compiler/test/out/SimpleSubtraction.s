	.text
	.globl	main
main:
	addiu	$sp,$sp,-20
	sw	$fp,16($sp)
	move	$fp,$sp
	li	$2,23
	sw	$2,4($fp)
	li	$2,19
	sw	$2,8($fp)
	lw	$2,8($fp)
	move	$3,$2
	lw	$2,4($fp)
	sub	$2,$2,$3
	sw	$2,12($fp)
	lw	$2,12($fp)
	move	$sp,$fp
	lw	$fp,16($sp)
	addiu	$sp,$sp,20
	jr	$31
	nop
