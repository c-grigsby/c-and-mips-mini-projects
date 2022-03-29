# Tell assembler to not insert instructions to fill branch delay slots.
# This is necessary when branch delay slots are disabled.
.set noreorder

.global _start
_start:
	la	$sp, 0x7FFFFFFC				# initialize stack pointer 
	jal main
	li $v0, 10
	syscall										# Use syscall 10 to stop simulation

main:	
	# Place the return address and s register on the stack
	addi $sp, $sp, -4					# initialize space on the stack to store $ra
	sw $ra, 0($sp)						# save $ra on the stack
	
	# Call printArray 
	la $a0, A									# load address of array A into a param reg => int* A
	lw $a1, len								# load word, the size of array A into a param reg => len
	jal printArray						# call printArray(int* A, len)
	
	# Call sort 
	la $a0, A									# load address of array A into a param reg => int v[]
	lw $a1, len								# load word, the size of array A into a param reg => int n
	jal sort									# call sort(int v[], int n)
	
	# Call printArray 
	la $a0, A									# load address of array A into a param reg => int* A
	lw $a1, len								# load word, the size of array A into a param reg => len
	jal printArray						# call printArray(int* A, len)
	
	# Restore the return address from the stack 
	lw   $ra, 0($sp)					# restore the return address
	addi $sp, $sp, 4		    	# adjust the stack to delete 1 word
	
	# Exit Main	
	jr $ra										# exit main

sort:   	
		addi $sp,$sp, -20 		   		# make room on stack for 5 registers
  	sw $ra, 16($sp)        			# save $ra on stack
    sw $s3,12($sp)         			# save $s3 on stack
    sw $s2, 8($sp)         			# save $s2 on stack
    sw $s1, 4($sp)         			# save $s1 on stack
    sw $s0, 0($sp)         			# save $s0 on stack
         												# procedure body
    move $s2, $a0           		# save $a0 into $s2
    move $s3, $a1           		# save $a1 into $s3
    move $s0, $zero         		# i = 0
 for1tst: 
	slt  $t0, $s0, $s3      			# $t0 = 0 if $s0 ≥ $s3 (i ≥ n)
    beq  $t0, $zero, exit1  		# go to exit1 if $s0 ≥ $s3 (i ≥ n)
    addi $s1, $s0, -1       		# j = i – 1
 for2tst: 
  	slti $t0, $s1, 0        		# $t0 = 1 if $s1 < 0 (j < 0)
    bne  $t0, $zero, exit2  		# go to exit2 if $s1 < 0 (j < 0)
    sll  $t1, $s1, 2        		# $t1 = j * 4
    add  $t2, $s2, $t1      		# $t2 = v + (j * 4)
    lw   $t3, 0($t2)        		# $t3 = v[j]
    lw   $t4, 4($t2)        		# $t4 = v[j + 1]
    slt  $t0, $t4, $t3      		# $t0 = 0 if $t4 ≥ $t3
    beq  $t0, $zero, exit2  		# go to exit2 if $t4 ≥ $t3
    move $a0, $s2           		# 1st param of swap is v (old $a0)
    move $a1, $s1           		# 2nd param of swap is j
    jal  swap               		# call swap procedure
    addi $s1, $s1, -1       		# j –= 1
    j    for2tst            		# jump to test of inner loop
 exit2:   
    addi $s0, $s0, 1        		# i += 1
    j    for1tst            		# jump to test of outer loop

   exit1: 
	  	lw $s0, 0($sp)  					# restore $s0 from stack
      lw $s1, 4($sp)         		# restore $s1 from stack
      lw $s2, 8($sp)         		# restore $s2 from stack
      lw $s3,12($sp)         		# restore $s3 from stack
      lw $ra,16($sp)         		# restore $ra from stack
      addi $sp,$sp, 20       		# restore stack pointer
      jr $ra                 		# return to calling routine

swap: 
    sll $t1, $a1, 2   				# $t1 = k * 4
    add $t1, $a0, $t1 				# $t1 = v+(k*4)
                        			#   (address of v[k])
    lw $t0, 0($t1)    				# $t0 (temp) = v[k]
    lw $t2, 4($t1)    				# $t2 = v[k+1]
    sw $t2, 0($t1)    				# v[k] = $t2 (v[k+1])
    sw $t0, 4($t1)    				# v[k+1] = $t0 (temp)
    jr $ra            				# return to calling routine

printArray: 
	# Prologue
	move $t0, $a0								# array(A) => move address of the array from param at $a0
	move $t1, $a1  				    	# arrayLength(len) => move value of arr_length from param at $a1
	li $t2, 0					    			# i
	addi $t3, $t1, -1			    	# len - 1
	
	# Procedure Body 
	li $v0, 4					    			# system call code => print_string
	la $a0, arrayStr			    	# load address of => "array = "
	syscall						    			# call system
	la $a0, openingBracketStr	  # load address of => "{ "
	syscall											# call system
	
	LOOP:
		beq $t2, $t1, END_LOOP	  # forLoop => if i == (len) branch to EXIT
		
		# Print A[i]
		lw $t4, 0($t0)			    	# load word array[i] 
		li $v0, 1				    			# system call code => print_int
		move $a0, $t4			    		# copy the number to print into $a0
		syscall 				    			# call system
		
		# Print ", " or " " after A[i]
		li $v0, 4 				    			# system call code => print_string
		beq $t2, $t3, LAST_ELEMENT  # if i == arrayLength -1 branch to print for LAST_ELEMENT
		la $a0, commaStr		    		# load address of => ", "
		syscall 				    				# system call
		j L2					    					# jump over else block to L2
		LAST_ELEMENT:
		  la $a0, spaceStr		    	# load address of => " "
		  syscall 				    			# system call
		L2:
		# Increment data for next loop
		addi $t0, $t0, 4		    		# increment pointer to next word
		addi $t2, $t2, 1		    		# i++
		j LOOP
	
	END_LOOP:
		la $a0, closingBracketStr 	# load address of => " }"
		syscall
		la $a0, newLineStr 					# load address of => "\n"
		syscall
		# Epilogue
		jr $ra											# jump back to calling routine

.data
A:	.word 3, 2, 5, 4, 7, 4, 8, 6
len: .word 8
arrayStr: .string "array = "
openingBracketStr: .string "{ "
closingBracketStr: .string "} "
spaceStr: .string " "
commaStr: .string ", "
newLineStr: .string "\n"

	