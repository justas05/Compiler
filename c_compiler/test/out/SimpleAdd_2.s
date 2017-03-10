	.text
	.globl	main
main:
	addiu	$sp,$sp,-12
	sw	$fp,8($sp)
	move	$fp,$sp
	li	$2,4
	sw	$2,4($fp)
	li	$2,1
	move	$3,$2
	lw	$2,4($fp)
	add	$2,$2,$3
	sw	$2,4($fp)
	lw	$2,4($fp)
	move	$sp,$fp
	lw	$fp,8($sp)
	addiu	$sp,$sp,12
	jr	$31
	nop
