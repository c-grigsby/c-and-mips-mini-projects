# Tell assembler to not insert instructions to fill branch delay slots.
# This is necessary when branch delay slots are disabled.
.set noreorder

.global _start
_start:
	la $s0, arrA				# load address of arrA (pointer to arrA)
	la $s1, arrB				# load address of arrB (pointer to arrB)
	la $s2, arrC				# load address of arrC (pointer to arrC)
	li $t0, 5				# load immediate the length of the array's
	li $t1, 0				# load immediate the loop counter to 0 => i
	li $t2, 3				# load immidiate the constant to compare a[i] != 3
	li $t3, 1				# load immediate a constant to determine if c3 is true
	
	LOOP: 
		beq $t0, $t1, ENDLOOP		# branch to ENDLOOP if $t0 == $t1
		lw $s3, 0($s0)			# grab an element from arrA => arrA[i]
		lw $s4, 0($s1)			# grab an element from arrB => arrB[i]
		
		#if (a[i] < b[i] && a[i] != 3) {c[i] = a[i]}
		#else {c[i] == 0}
		
		slt $t4, $s3, $s4 		# condition c1 => set less than arrA[i] < arrB[i]
		sne $t5, $s3, $t2 		# condition c2 => set not equal arrA[i] != 3
		and $t6, $t4, $t5		# condition c3 => c3 = c1 && c2 (c1 & c2)
		
		bne $t6, $t3, L1		# if condition3 is not 'true' (c3 != 1) branch to L1
		sw  $s3, 0($s2)			# c3 is 'true' (c3 == 1) then c[i] = a[i]
		j L2				# jump to L2 (over the else block)
		L1:
		  sw $zero, 0($s2) 		# else, c3 is 'false', then c[i] = 0
		L2:
		addi $s0, $s0, 4		# increment the pointer to arrA to the next word
		addi $s1, $s1, 4		# increment the pointer to arrB to the next word
		addi $s2, $s2, 4		# increment the pointer to arrC to the next word
		addi $t1, $t1, 1		# i++
		j LOOP				# go back to the top of the loop
		
	ENDLOOP:
		li $v0, 10  			# system call code for exit = 10
		syscall 			# call operating system
	
.data
	arrA: .word 1, 2, 3, 4, 5		# init array A
	arrB: .word 5, 4, 3, 2, 1		# init array B
	arrC: .space 20				# init space for array C  
