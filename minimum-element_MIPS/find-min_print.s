# Tell assembler to not insert instructions to fill branch delay slots.
# This is necessary when branch delay slots are disabled.
.set noreorder

.global _start
_start:
	li $sp, 0x1000              # initialize SP to something sane
	jal main					# jump and link to main
	li $v0, 10
	syscall						# use syscall 10 to stop simulation

print_intArray:
	addi $sp, $sp, -24			# init space on the stack for 5 words
	sw $s0, 20($sp)				# save register $s0 on the stack
	sw $s1, 16($sp)				# save register $s1 on the stack
	sw $t0, 12($sp)				# save register $t0 on the stack
	sw $t1, 8($sp)				# save register $t1 on the stack
	sw $t2, 4($sp)				# save register $t2 on the stack
	sw $t3, 0($sp)				# save register $t3 on the stack
	
	move $s0, $a0  				# array => move address of the array from param
	move $s1, $a1  				# arr_length => move address of arr_length from param
	move $t0, $s0				# copy a pointer to traverse the array 
	li $t1, 0					# i 
	addi $t2, $s1, -1			# arr_length - 1
	
	li $v0, 4					# system call code => print_string
	la $a0, arrayStr			# load address of => "array = "
	syscall						# call system
	la $a0, openingBracketStr	# load address of => "{ "
	syscall						# call system

	LOOP:
		beq $t1, $s1, END_LOOP		# forLoop => if i == arr_length branch to EXIT
		lw $t3, 0($t0)				# load word array[i]
		li $v0, 1					# system call code => print_int
		move $a0, $t3				# copy the number to print into $a0
		syscall 					# call system
		
		beq $t1, $t2, LAST   		# if i == arr_length -1 branch to print for LAST (element)
		li $v0, 4 					# system call code => print_string
		la $a0, commaStr			# load address of => ", "
		syscall 					# system call
		j L2						# jump over else
		LAST:
		  li $v0, 4 				# system call code => print_string
		  la $a0, spaceStr			# load address of => " "
		  syscall 					# system call
		L2:
		addi $t0, $t0, 4			# increment pointer to next word
		addi $t1, $t1, 1		    # i++
		j LOOP
	
	END_LOOP:
		la $a0, closingBracketStr 	# load address of => " }"
		syscall
		la $a0, newLineStr 		  	# load address of => "\n"
		syscall
		lw $s0, 20($sp)			  	# restore all registers from the stack
		lw $s1, 16($sp)				
		lw $t0, 12($sp)				
		lw $t1, 8($sp)				
		lw $t2, 4($sp)				
		lw $t3, 0($sp)	
		addi $sp, $sp, 24			# adjust stack to delete 6 words
		jr $ra					 	# jump back to calling routine

main:
	la $s0, arrA				# load address of arrA
	la $s1, ANSWER  			# load address of ANSWER => where to store the final answer 
	li $s2, 8					# arr_length => 8 words in the array 
	lw $t6, 0($s0) 				# min => load word to init minimum element from arrA (min = arrA[0])
	li $t0, 1					# i => load immediate the loop counter, i = 1
	add $t1, $s0, 4 			# temp pointer for arrA[i] => increment the address of arrA to point to A[1]

	FIND_MIN_LOOP:		
		beq $t0, $s2, END	 	# forLoop => i == arr_length end loop
		lw $t2, 0($t1) 		 	# current => load word from arrA => A[i]
		slt $t3, $t2, $t6		# condition c1: set less than if (A[i] < min), returns 1 if true
		beq $t3, $zero, L1		# if ($t3 = 0) c1 is false, branch to L1
		lw $t6, 0($t1)			# otherwise, min == current
		L1:
		addi $t1, $t1, 4		# advance pointer to the next word in the arrA
		addi $t0, $t0, 1		# i++
		j FIND_MIN_LOOP			# jump to repeat the loop
			
	END:
		sw $t6, 0($s1)		    # ANSWER == min => store final answer into memory address ANSWER
		
		move $a0, $s0			# set parameter1: the address of arrA
		move $a1, $s2			# set parameter2: arr_length
		addi $sp, $sp, -4		# initialize space on the stack to store $ra
		sw   $ra, 0($sp)		# save $ra on the stack
		jal print_intArray      # jump and link to procedure print_intArray
		lw	$ra, 0($sp)			# restore $ra
		addi $sp, $sp, 4		# adjust stack to delete the item
		
		li $v0, 4				# system call code => print_string
		la $a0, minimumStr		# load address of => "the minimum element is: "
		syscall 				# call system
		li $v0, 1				# system call code => print_int
		lw $a0, 0($s1) 			# load word from ANSWER
		syscall 				# call system
		li $v0, 4				# system call code => print_string
		la $a0, newLineStr		# load address of => "\n"
		syscall 				# call system
		jr $ra
			
.data
	arrA: .word 3,5,3,2,1,6,9,4 
	ANSWER: .space 4			
	arrayStr: .asciiz "array = "
	openingBracketStr: .asciiz "{ "
	closingBracketStr: .asciiz "} "
	minimumStr: .asciiz "the minimum integer is = "
	spaceStr: .asciiz " "
	commaStr: .asciiz ", "
	newLineStr: .asciiz "\n"
	