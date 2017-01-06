	.data
s0:	.asciiz "\nToo large!Upper bound now is:"
s1:	.asciiz "\nToo small!Lower bound now is:"
s2:	.asciiz "\nCongrats!Correct number:"
s3:	.asciiz "fib para error! return ERROR CODE = -1\t"
s4:	.asciiz "\ncall void in state successfully!"
s5:	.asciiz "\ncall int in state successfully!"
s6:	.asciiz "\ncase * ascii"
s7:	.asciiz "\ncase *"
s8:	.asciiz "\ncase * ascii"
s9:	.asciiz "\ncase + ascii"
s10:	.asciiz "\ncase +"
s11:	.asciiz "\ncase 4"
s12:	.asciiz "\ncase 5"
s13:	.asciiz "\nWrong option!"
s14:	.asciiz "\n"
s15:	.asciiz "\n"
s16:	.asciiz "\t"
s17:	.asciiz "\t"
s18:	.asciiz "\t\t"
s19:	.asciiz "\nFib sum is :"
s20:	.asciiz "\nFib sum is :"
s21:	.asciiz "\nArray[8]:"
s22:	.asciiz "\nArray[0]:"
s23:	.asciiz "\nArray[0]+Array[8]="
s24:	.asciiz "\t"

	.text
la	$t0,s0
addi	$sp,$t0,396
move	$gp,$sp
addi	$sp,$sp,36
move	$fp,$sp
addi	$sp,$sp,204
addi	$sp,$sp,12
j	Main
nop

Label_1:
lw	$t1,0($fp)
lw	$t2,4($fp)
add	$t1,$t1,$t2
sw	$t1,8($fp)
lw	$v0,8($fp)
move	$k0,$sp
move	$sp,$fp
lw	$fp,-8($k0)
jr	$ra
nop

Label_2:
lw	$t1,0($fp)
lw	$t2,4($fp)
add	$t1,$t1,$t2
sw	$t1,12($fp)
lw	$t1,12($fp)
li	$t2,2
div	$t1,$t1,$t2
sw	$t1,16($fp)
lw	$t1,16($fp)
sw	$t1,8($fp)
lw	$t1,8($fp)
lw	$t2,8($gp)
ble	$t1,$t2,Label_3
nop
li	$v0,4
la	$a0,s0
syscall
li	$v0,1
lw	$a0,8($fp)
syscall
sw	$ra,-4($sp)
lw	$t1,0($fp)
sw	$t1,0($sp)
lw	$t1,8($fp)
sw	$t1,4($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,20
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_2
nop
lw	$ra,-4($sp)
move	$k0,$sp
move	$sp,$fp
lw	$fp,-8($k0)
jr	$ra
nop

Label_3:
lw	$t1,8($fp)
lw	$t2,8($gp)
bge	$t1,$t2,Label_4
nop
li	$v0,4
la	$a0,s1
syscall
li	$v0,1
lw	$a0,8($fp)
syscall
sw	$ra,-4($sp)
lw	$t1,8($fp)
sw	$t1,0($sp)
lw	$t1,4($fp)
sw	$t1,4($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,20
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_2
nop
lw	$ra,-4($sp)
move	$k0,$sp
move	$sp,$fp
lw	$fp,-8($k0)
jr	$ra
nop

Label_4:
lw	$t1,8($fp)
lw	$t2,8($gp)
bne	$t1,$t2,Label_5
nop
li	$v0,4
la	$a0,s2
syscall
li	$v0,1
lw	$a0,8($fp)
syscall
move	$k0,$sp
move	$sp,$fp
lw	$fp,-8($k0)
jr	$ra
nop

Label_5:
move	$k0,$sp
move	$sp,$fp
lw	$fp,-8($k0)
jr	$ra
nop

Label_6:
lw	$t1,0($fp)
li	$t2,0
bgt	$t1,$t2,Label_7
nop
li	$v0,4
la	$a0,s3
syscall
li	$v0,-1
move	$k0,$sp
move	$sp,$fp
lw	$fp,-8($k0)
jr	$ra
nop

Label_7:
lw	$t1,0($fp)
li	$t2,1
bne	$t1,$t2,Label_8
nop
li	$v0,1
move	$k0,$sp
move	$sp,$fp
lw	$fp,-8($k0)
jr	$ra
nop

Label_8:
lw	$t1,0($fp)
li	$t2,2
bne	$t1,$t2,Label_9
nop
li	$v0,1
move	$k0,$sp
move	$sp,$fp
lw	$fp,-8($k0)
jr	$ra
nop

Label_9:
lw	$t1,0($fp)
li	$t2,1
sub	$t1,$t1,$t2
sw	$t1,4($fp)
sw	$ra,-4($sp)
lw	$t1,4($fp)
sw	$t1,0($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,24
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_6
nop
lw	$ra,-4($sp)
sw	$v0,8($fp)
lw	$t1,0($fp)
li	$t2,2
sub	$t1,$t1,$t2
sw	$t1,12($fp)
sw	$ra,-4($sp)
lw	$t1,12($fp)
sw	$t1,0($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,24
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_6
nop
lw	$ra,-4($sp)
sw	$v0,16($fp)
lw	$t1,8($fp)
lw	$t2,16($fp)
add	$t1,$t1,$t2
sw	$t1,20($fp)
lw	$v0,20($fp)
move	$k0,$sp
move	$sp,$fp
lw	$fp,-8($k0)
jr	$ra
nop

Label_10:
li	$v0,4
la	$a0,s4
syscall
li	$v0,1
lw	$a0,0($fp)
syscall
move	$k0,$sp
move	$sp,$fp
lw	$fp,-8($k0)
jr	$ra
nop

Label_11:
lw	$t1,4($fp)
li	$t2,0
beq	$t1,$t2,Label_12
nop
li	$v0,4
la	$a0,s5
syscall
li	$v0,1
lw	$a0,0($fp)
syscall

Label_12:
lw	$t1,0($fp)
li	$t2,1
add	$t1,$t1,$t2
sw	$t1,8($fp)
lw	$v0,8($fp)
move	$k0,$sp
move	$sp,$fp
lw	$fp,-8($k0)
jr	$ra
nop

#Start here
Main:
li	$t1,1
sw	$t1,8($fp)
lw	$t1,8($fp)
li	$t2,2
bge	$t1,$t2,Label_13
nop
li	$v0,5
syscall
sw	$v0,4($fp)
li	$v0,5
syscall
sw	$v0,12($fp)
li	$v0,5
syscall
sw	$v0,16($fp)
lw	$t1,4($fp)
li	$t2,1
bne	$t1,$t2,Label_15
nop
li	$t1,10
sw	$t1,8($gp)
li	$t1,42
li	$t2,42
bne	$t1,$t2,Label_17
nop
li	$v0,4
la	$a0,s6
syscall
j	Label_16
nop

Label_17:
li	$t1,42
li	$t2,42
bne	$t1,$t2,Label_18
nop
li	$t1,1
li	$t2,0
ble	$t1,$t2,Label_19
nop
li	$v0,4
la	$a0,s7
syscall

Label_19:
j	Label_16
nop

Label_18:
li	$t1,42
li	$t2,42
bne	$t1,$t2,Label_20
nop
li	$v0,4
la	$a0,s8
syscall
j	Label_16
nop

Label_20:
li	$t1,42
li	$t2,43
bne	$t1,$t2,Label_21
nop
li	$v0,4
la	$a0,s9
syscall
j	Label_16
nop

Label_21:
li	$t1,42
li	$t2,43
bne	$t1,$t2,Label_22
nop
li	$v0,4
la	$a0,s10
syscall
j	Label_16
nop

Label_22:
li	$t1,42
li	$t2,97
bne	$t1,$t2,Label_23
nop
li	$v0,4
la	$a0,s11
syscall
j	Label_16
nop

Label_23:
li	$t1,42
li	$t2,5
bne	$t1,$t2,Label_24
nop
li	$v0,4
la	$a0,s12
syscall
j	Label_16
nop

Label_24:

Label_16:
sw	$ra,-4($sp)
li	$t1,0
sw	$t1,0($sp)
li	$t1,20
sw	$t1,4($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,20
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_2
nop
lw	$ra,-4($sp)
j	Label_14
nop

Label_15:
lw	$t1,4($fp)
li	$t2,2
bne	$t1,$t2,Label_25
nop
li	$t1,8
sw	$t1,8($gp)
sw	$ra,-4($sp)
li	$t1,0
sw	$t1,0($sp)
li	$t1,20
sw	$t1,4($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,20
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_2
nop
lw	$ra,-4($sp)
j	Label_14
nop

Label_25:
lw	$t1,4($fp)
li	$t2,3
bne	$t1,$t2,Label_26
nop
li	$t1,11
sw	$t1,8($gp)
sw	$ra,-4($sp)
li	$t1,0
sw	$t1,0($sp)
li	$t1,20
sw	$t1,4($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,20
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_2
nop
lw	$ra,-4($sp)
j	Label_14
nop

Label_26:
lw	$t1,4($fp)
li	$t2,4
bne	$t1,$t2,Label_27
nop
li	$t1,16
sw	$t1,8($gp)
sw	$ra,-4($sp)
li	$t1,0
sw	$t1,0($sp)
li	$t1,20
sw	$t1,4($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,20
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_2
nop
lw	$ra,-4($sp)
j	Label_14
nop

Label_27:
li	$v0,4
la	$a0,s13
syscall

Label_14:

Label_13:
li	$v0,4
la	$a0,s14
syscall
sw	$ra,-4($sp)
lw	$t1,12($fp)
sw	$t1,0($sp)
lw	$t1,16($fp)
sw	$t1,4($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,12
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_1
nop
lw	$ra,-4($sp)
sw	$v0,68($fp)
li	$v0,1
lw	$a0,68($fp)
syscall
li	$v0,4
la	$a0,s15
syscall

Label_28:
lw	$t1,8($fp)
li	$t2,6
bge	$t1,$t2,Label_29
nop
li	$t1,42
lw	$t2,8($fp)
add	$t1,$t1,$t2
sw	$t1,72($fp)
lw	$t1,72($fp)
sw	$t1,20($fp)
li	$t1,42
lw	$t2,8($fp)
add	$t1,$t1,$t2
sw	$t1,76($fp)
lw	$t1,76($fp)
sw	$t1,64($fp)
li	$v0,4
la	$a0,s16
syscall
li	$v0,11
lw	$a0,64($fp)
syscall
li	$v0,4
la	$a0,s17
syscall
li	$v0,1
lw	$a0,20($fp)
syscall
li	$v0,4
la	$a0,s18
syscall
lw	$t1,8($fp)
li	$t2,1
add	$t1,$t1,$t2
sw	$t1,80($fp)
lw	$t1,80($fp)
sw	$t1,8($fp)
j	Label_28
nop

Label_29:
li	$v0,4
la	$a0,s19
syscall
sw	$ra,-4($sp)
lw	$t1,12($fp)
sw	$t1,0($sp)
lw	$t1,16($fp)
sw	$t1,4($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,12
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_1
nop
lw	$ra,-4($sp)
sw	$v0,84($fp)
lw	$t1,84($fp)
li	$t2,1
add	$t1,$t1,$t2
sw	$t1,88($fp)
sw	$ra,-4($sp)
lw	$t1,88($fp)
sw	$t1,0($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,24
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_6
nop
lw	$ra,-4($sp)
sw	$v0,92($fp)
lw	$t1,92($fp)
li	$t2,10
add	$t1,$t1,$t2
sw	$t1,96($fp)
li	$v0,1
lw	$a0,96($fp)
syscall
li	$v0,4
la	$a0,s20
syscall
lw	$t1,16($fp)
lw	$t2,12($fp)
add	$t1,$t1,$t2
sw	$t1,100($fp)
sw	$ra,-4($sp)
lw	$t1,12($fp)
sw	$t1,0($sp)
lw	$t1,100($fp)
sw	$t1,4($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,12
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_1
nop
lw	$ra,-4($sp)
sw	$v0,104($fp)
lw	$t1,104($fp)
neg	$t1,$t1
sw	$t1,108($fp)
lw	$t1,108($fp)
li	$t2,1
add	$t1,$t1,$t2
sw	$t1,112($fp)
sw	$ra,-4($sp)
lw	$t1,112($fp)
sw	$t1,0($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,24
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_6
nop
lw	$ra,-4($sp)
sw	$v0,116($fp)
lw	$t1,116($fp)
li	$t2,1
add	$t1,$t1,$t2
sw	$t1,120($fp)
li	$v0,1
lw	$a0,120($fp)
syscall
li	$t1,3
li	$t2,6
mul	$t1,$t1,$t2
sw	$t1,124($fp)
lw	$t1,124($fp)
li	$t2,-10
add	$t1,$t1,$t2
sw	$t1,128($fp)
la	$t1,24($fp)
lw	$t2,128($fp)
mul	$t2,$t2,4
add	$t1,$t1,$t2
sw	$t1,132($fp)
li	$t1,-1
lw	$t2,12($fp)
add	$t1,$t1,$t2
sw	$t1,136($fp)
lw	$t1,16($fp)
li	$t2,1
add	$t1,$t1,$t2
sw	$t1,140($fp)
sw	$ra,-4($sp)
lw	$t1,136($fp)
sw	$t1,0($sp)
lw	$t1,140($fp)
sw	$t1,4($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,12
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_1
nop
lw	$ra,-4($sp)
sw	$v0,144($fp)
lw	$t1,144($fp)
lw	$t0,132($fp)
sw	$t1,0($t0)
la	$t1,24($fp)
li	$t2,0
mul	$t2,$t2,4
add	$t1,$t1,$t2
sw	$t1,148($fp)
la	$t1,24($fp)
li	$t2,8
mul	$t2,$t2,4
add	$t1,$t1,$t2
lw	$t1,0($t1)
sw	$t1,152($fp)
lw	$t1,152($fp)
li	$t2,1
add	$t1,$t1,$t2
sw	$t1,156($fp)
lw	$t1,156($fp)
lw	$t0,148($fp)
sw	$t1,0($t0)
li	$v0,4
la	$a0,s21
syscall
li	$t1,6
li	$t2,2
sub	$t1,$t1,$t2
sw	$t1,160($fp)
lw	$t1,160($fp)
li	$t2,4
mul	$t1,$t1,$t2
sw	$t1,164($fp)
lw	$t1,164($fp)
li	$t2,2
div	$t1,$t1,$t2
sw	$t1,168($fp)
la	$t1,24($fp)
lw	$t2,168($fp)
mul	$t2,$t2,4
add	$t1,$t1,$t2
lw	$t1,0($t1)
sw	$t1,172($fp)
li	$v0,1
lw	$a0,172($fp)
syscall
li	$v0,4
la	$a0,s22
syscall
la	$t1,24($fp)
li	$t2,0
mul	$t2,$t2,4
add	$t1,$t1,$t2
lw	$t1,0($t1)
sw	$t1,176($fp)
li	$v0,1
lw	$a0,176($fp)
syscall
li	$v0,4
la	$a0,s23
syscall
la	$t1,24($fp)
li	$t2,0
mul	$t2,$t2,4
add	$t1,$t1,$t2
lw	$t1,0($t1)
sw	$t1,180($fp)
la	$t1,24($fp)
li	$t2,8
mul	$t2,$t2,4
add	$t1,$t1,$t2
lw	$t1,0($t1)
sw	$t1,184($fp)
sw	$ra,-4($sp)
lw	$t1,180($fp)
sw	$t1,0($sp)
lw	$t1,184($fp)
sw	$t1,4($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,12
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_1
nop
lw	$ra,-4($sp)
sw	$v0,188($fp)
li	$v0,1
lw	$a0,188($fp)
syscall
sw	$ra,-4($sp)
li	$t1,666
sw	$t1,0($sp)
li	$t1,1
sw	$t1,4($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,12
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_11
nop
lw	$ra,-4($sp)
sw	$v0,192($fp)
sw	$ra,-4($sp)
li	$t1,666
sw	$t1,0($sp)
li	$t1,0
sw	$t1,4($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,12
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_11
nop
lw	$ra,-4($sp)
sw	$v0,196($fp)
sw	$ra,-4($sp)
lw	$t1,196($fp)
sw	$t1,0($sp)
move	$k0,$fp
move	$fp,$sp
addi	$sp,$sp,4
addi	$sp,$sp,12
sw	$k0,-8($sp)
jal	Label_10
nop
lw	$ra,-4($sp)
li	$v0,4
la	$a0,s24
syscall
li	$t1,42
li	$t2,1
add	$t1,$t1,$t2
sw	$t1,200($fp)
li	$v0,11
lw	$a0,200($fp)
syscall
#RETURN MAIN