# Tell assembler to not insert instructions to fill branch delay slots.
# This is necessary when branch delay slots are disabled.
.set noreorder

.global _start
_start:
	# init vars
	la $s0, arrA	# load address of arrA (pointer to arrA)
	la $s1, arrB	# load address of arrb (pointer to arrB)
	la $s2, arrC	# load address of arrC (pointer to arrC, sums arr)
	la $t7, 0 		# load immediate the running sum var to 0 => C
	li $t0, 5		# load immediate length of the arr's
	li $t1, 0		# load immediate loop index to 0 => i

	LOOP: 
		beq 	$t0, $t1, ENDLOOP	# branch to ENDLOOP if $t0 == $t1
		lw 		$s3, 0($s0)			# grab an element out of arrA => A[i]
		lw 		$s4, 0($s1)			# grab an element out of arrB => B[i]
	
		add		$t7, $t7, $s3		# add the value from arrA to the sum
		add		$t7, $t7, $s4		# add the value from arrB to the sum
		sw		$t7, 0($s2)			# add the value into the sum arr => arrC
	
		addi	$s0, $s0, 4			# increment the pointer to arrA (by 4 because of word size) to the next element 
		addi	$s1, $s1, 4			# increment the pointer to arrB (by 4 because of word size) to the next element 
		addi	$s2, $s2, 4			# increment the pointer to arrC (by 4 because of word size) to the next element 
		li    	$t7, 0				# reset the sum to zero (arrC should hold individual sums at each index)
		
		addi	$t1, $t1, 1			# i++

		j	LOOP					# go back to the top of the loop
		
	ENDLOOP:
		# end program
		li	$v0, 10					# system call code for exit = 10
		syscall						# call operating system

.data 
	arrA: .word 1, 2, 3, 4, 5 		# init the arr's
	arrB: .word 6, 7, 8, 9, 10
	arrC: .word 0, 0, 0, 0, 0

	