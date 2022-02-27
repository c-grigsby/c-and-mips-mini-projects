# Tell assembler to not insert instructions to fill branch delay slots.
# This is necessary when branch delay slots are disabled.
.set noreorder

.global _start
_start:

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
		li $v0, 10  			# system call code for exit = 10
		syscall 			    # call operating system
		
.data
	arrA: .word 3,5,3,2,1,6,9,4 
	ANSWER: .space 4	