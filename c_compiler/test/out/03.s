	.text
	.globl	main
main:
	addiu	$sp,$sp,-12
	sw	$fp,8($sp)
	move	$fp,$sp
	move	$sp,$fp
	lw	$fp,8($sp)
	addiu	$sp,$sp,12
	jr	$31
	nop
