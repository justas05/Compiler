	.text
	.globl	main
main:
	addiu	$sp,$sp,-20
	sw	$fp,16($sp)
	move	$fp,$sp
	li	$2,4
	sw	$2,4($fp)
	li	$2,35
	sw	$2,8($fp)
	li	$2,3
	move	$3,$2
	li	$2,3
	add	$2,$2,$3
	sw	$2,12($fp)
	li	$2,5
	move	$3,$2
	li	$2,4
	add	$2,$2,$3
	sw	$2,4($fp)
	lw	$2,4($fp)
	move	$sp,$fp
	lw	$fp,16($sp)
	addiu	$sp,$sp,20
	jr	$31
	nop
