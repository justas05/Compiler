	.file	1 "test.c"
	.section .mdebug.abi32
	.previous
	.nan	legacy
	.module	fp=xx
	.module	nooddspreg
	.abicalls
	.rdata
	.align	2
$LC2:
	.ascii	"%.4f\012\000"
	.text
	.align	2
	.globl	main
	.set	nomips16
	.set	nomicromips
	.ent	main
	.type	main, @function
main:
	.frame	$fp,56,$31		# vars= 24, regs= 2/0, args= 16, gp= 8
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$fp,48($sp)
	move	$fp,$sp
	lui	$28,%hi(__gnu_local_gp)
	addiu	$28,$28,%lo(__gnu_local_gp)
	.cprestore	16
	li	$2,4			# 0x4
	sw	$2,24($fp)
	li	$2,5			# 0x5
	sw	$2,28($fp)
	li	$2,6			# 0x6
	sw	$2,32($fp)
	lui	$2,%hi($LC0)
	lwc1	$f0,%lo($LC0)($2)
	swc1	$f0,36($fp)
	lwc1	$f2,36($fp)
	lui	$2,%hi($LC1)
	lwc1	$f0,%lo($LC1)($2)
	div.s	$f0,$f2,$f0
	swc1	$f0,40($fp)
	lw	$3,24($fp)
	lw	$2,28($fp)
	addu	$3,$3,$2
	lw	$2,32($fp)
	addu	$2,$3,$2
	sw	$2,44($fp)
	lwc1	$f0,40($fp)
	cvt.d.s	$f0,$f0
	mfc1	$7,$f0
	mfhc1	$6,$f0
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	lw	$2,%call16(printf)($28)
	move	$25,$2
	.reloc	1f,R_MIPS_JALR,printf
1:	jalr	$25
	nop

	lw	$28,16($fp)
	lwc1	$f0,40($fp)
	trunc.w.s $f0,$f0
	mfc1	$2,$f0
	move	$sp,$fp
	lw	$31,52($sp)
	lw	$fp,48($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	main
	.size	main, .-main
	.rdata
	.align	2
$LC0:
	.word	1073741824
	.align	2
$LC1:
	.word	1077936128
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.1) 5.4.0 20160609"
