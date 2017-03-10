	.text
	.globl	main
main:
	addiu	$sp,$sp,-16
	sw	$fp,12($sp)
	move	$fp,$sp
	li	$2,68
	sw	$2,4($fp)
	li	$2,38
	sw	$2,8($fp)
	lw	$2,8($fp)
	move	$3,$2
	lw	$2,4($fp)
	add	$2,$2,$3
	move	$sp,$fp
	lw	$fp,12($sp)
	addiu	$sp,$sp,16
	jr	$31
	nop
