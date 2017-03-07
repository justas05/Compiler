	.text
	.globl	main
main:
	addiu	$sp,$sp,-16
	sw	$fp,12($sp)
	move	$fp,$sp
	move	$sp,$fp
	lw	$fp,12($sp)
	addiu	$sp,$sp,16
	jr	$31
	nop
	.text
	.globl	f
f:
	addiu	$sp,$sp,-8
	sw	$fp,4($sp)
	move	$fp,$sp
	move	$sp,$fp
	lw	$fp,4($sp)
	addiu	$sp,$sp,8
	jr	$31
	nop
