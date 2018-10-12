# C0compiler
A compiler for C Language

## pipeline
C program -> Lexer -> Parser -> Quarter (Intermediate expression) -> DAG optimization (optional) -> MIPS instructions


## An example
### C program (Input)
``` C
const int NUMBER=2
const char cha = '*';
int targetvar;					
int ADD(int a,int b){						
	return (+a++b);	
}
void guess(int a,int b){	
	int guessnum;	
	guessnum=(a+b)/2;
	if(guessnum>targetvar){				
		printf("\nToo large!Upper bound now is:",guessnum);
		guess (a,guessnum);
		return;		
	}
	if(guessnum<targetvar){
		printf("\nToo small!Lower bound now is:",guessnum);
		guess(guessnum,b);
		return;
	}
	if(guessnum==targetvar){
		printf("\nCongrats!Correct number:",guessnum);
		return;
	}
}
int fib(x){
	if x<=0){
		printf("fib para error! return ERROR CODE = -1\t");
		return (-1);
	}
	if(x==1)
		return (1);
	if(x==2)
		return (1);
	return ((fib(x-1)+fib(x-2)));
}

void main(){	
	const int TEN = 10;						
	int option,i;						
	int x,y;
	int chaint;
	int array[10];
	char cha_;				
	i=1;							
	if(i<NUMBER){												
	    scanf(option,x,y);						
            switch(option){						
            	case 1:{targetvar=10;guess(0,20);}		
            	case 2:{targetvar=8;guess(0,20);}		
            	case 3:{targetvar=11;guess(0,20);}
            	case 4:{targetvar=16;guess(0,20);}
            	default:printf("\nWrong option!");		
            }         	  						
	}
	printf("\n",ADD(x,y));	
	printf("\n");
	while(i<TEN){
		chaint=cha+i;
		cha_=cha+i;
		printf("\t", cha_);
		printf("\t",chaint);
		printf("\t\t");
		i=i+1;
	}
	printf("\nFib sum is :",fib(ADD(x,y)+1)+10);
	printf("\nFib sum is :",fib(-ADD(x,y+x)+1)+1);
	array[3*6-10] = ADD(x,y+1);
	array[0] = array[8]+1;
	printf("\nArray[0]:",array[10*9*8*0]);
	return;
}

```   

### Quarter (Intermediate expression)
```
LAB  Label_1
ADD  (0),a,b
RET  (0)

LAB  Label_2
ADD  (1),a,b
DIV  (2),(1),2
ASG  guessnum,(2)
BLE  Label_3,guessnum,targetvar
WRT  "\nToo large!Upper bound now is:"
WRT  guessnum
SAVE  
PUSH  guess,a,1
PUSH  guess,guessnum,2
CALVF  guess
RET  

LAB  Label_3
BGE  Label_4,guessnum,targetvar
WRT  "\nToo small!Lower bound now is:"
WRT  guessnum
SAVE  
PUSH  guess,guessnum,1
PUSH  guess,b,2
CALVF  guess
RET  

LAB  Label_4
BNE  Label_5,guessnum,targetvar
WRT  "\nCongrats!Correct number:"
WRT  guessnum
RET  

LAB  Label_5
RET  

LAB  Label_6
BGT  Label_7,x,0
WRT  "fib para error! return ERROR CODE = -1\t"
RET  -1

LAB  Label_7
BNE  Label_8,x,1
RET  1

LAB  Label_8
BNE  Label_9,x,2
RET  1

LAB  Label_9
SUB  (3),x,1
SAVE  
PUSH  fib,(3),1
CALF  fib,(4)
SUB  (5),x,2
SAVE  
PUSH  fib,(5),1
CALF  fib,(6)
ADD  (7),(4),(6)
RET  (7)

LAB  Label_10
WRT  "\ncall void in state successfully!"
WRT  x
RET  

LAB  Label_11
BEQ  Label_12,is_state,0
WRT  "\ncall int in state successfully!"
WRT  y

LAB  Label_12
ADD  (8),y,1
RET  (8)

LAB  Main
ASG  i,1
BGE  Label_13,i,NUMBER
RAD  option
RAD  x
RAD  y
BNE  Label_15,option,1
ASG  targetvar,10
BNE  Label_17,cha,42
WRT  "\ncase * ascii"
JMP  Label_16

LAB  Label_17
BNE  Label_18,cha,42
BLE  Label_19,1,0
WRT  "\ncase *"

LAB  Label_19
JMP  Label_16

LAB  Label_18
BNE  Label_20,cha,42
WRT  "\ncase * ascii"
JMP  Label_16

LAB  Label_20
BNE  Label_21,cha,43
WRT  "\ncase + ascii"
JMP  Label_16

LAB  Label_21
BNE  Label_22,cha,43
WRT  "\ncase +"
JMP  Label_16

LAB  Label_22
BNE  Label_23,cha,97
WRT  "\ncase 4"
JMP  Label_16

LAB  Label_23
BNE  Label_24,cha,5
WRT  "\ncase 5"
JMP  Label_16

LAB  Label_24

LAB  Label_16
SAVE  
PUSH  guess,0,1
PUSH  guess,20,2
CALVF  guess
JMP  Label_14

LAB  Label_15
BNE  Label_25,option,2
ASG  targetvar,8
SAVE  
PUSH  guess,0,1
PUSH  guess,20,2
CALVF  guess
JMP  Label_14

LAB  Label_25
BNE  Label_26,option,3
ASG  targetvar,11
SAVE  
PUSH  guess,0,1
PUSH  guess,20,2
CALVF  guess
JMP  Label_14

LAB  Label_26
BNE  Label_27,option,4
ASG  targetvar,16
SAVE  
PUSH  guess,0,1
PUSH  guess,20,2
CALVF  guess
JMP  Label_14

LAB  Label_27
WRT  "\nWrong option!"

LAB  Label_14

LAB  Label_13
WRT  "\n"
SAVE  
PUSH  ADD,x,1
PUSH  ADD,y,2
CALF  ADD,(9)
WRT  (9)
WRT  "\n"

LAB  Label_28
BGE  Label_29,i,SIX
ADD  (10),cha,i
ASG  chaint,(10)
ADD  (11),cha,i
ASG  cha_,(11)
WRT  "\t"
WRTC  cha_
WRT  "\t"
WRT  chaint
WRT  "\t\t"
ADD  (12),i,1
ASG  i,(12)
JMP  Label_28

LAB  Label_29
WRT  "\nFib sum is :"
SAVE  
PUSH  ADD,x,1
PUSH  ADD,y,2
CALF  ADD,(13)
ADD  (14),(13),1
SAVE  
PUSH  fib,(14),1
CALF  fib,(15)
ADD  (16),(15),10
WRT  (16)
WRT  "\nFib sum is :"
ADD  (17),y,x
SAVE  
PUSH  ADD,x,1
PUSH  ADD,(17),2
CALF  ADD,(18)
NEG  (19),(18)
ADD  (20),(19),1
SAVE  
PUSH  fib,(20),1
CALF  fib,(21)
ADD  (22),(21),1
WRT  (22)
MUL  (23),3,SIX
ADD  (24),(23),-10
OFF  (25),array,(24)
ADD  (26),-1,x
ADD  (27),y,1
SAVE  
PUSH  ADD,(26),1
PUSH  ADD,(27),2
CALF  ADD,(28)
ASG  (25),(28)
OFF  (29),array,0
SEL  (30),array,8
ADD  (31),(30),1
ASG  (29),(31)
WRT  "\nArray[8]:"
SUB  (32),SIX,2
MUL  (33),(32),4
DIV  (34),(33),2
SEL  (35),array,(34)
WRT  (35)
WRT  "\nArray[0]:"
SEL  (36),array,0
WRT  (36)
WRT  "\nArray[0]+Array[8]="
SEL  (37),array,0
SEL  (38),array,8
SAVE  
PUSH  ADD,(37),1
PUSH  ADD,(38),2
CALF  ADD,(39)
WRT  (39)
SAVE  
PUSH  call_me_in__state,666,1
PUSH  call_me_in__state,1,2
CALF  call_me_in__state,(40)
SAVE  
PUSH  call_me_in__state,666,1
PUSH  call_me_in__state,0,2
CALF  call_me_in__state,(41)
SAVE  
PUSH  call_me_in_state,(41),1
CALVF  call_me_in_state
WRT  "\t"
ADD  (42),cha,1
WRTC  (42)
RETM  
```

### MIPS instructions (Output)
``` asm
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
```
