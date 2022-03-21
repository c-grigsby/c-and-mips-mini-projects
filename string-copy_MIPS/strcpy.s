# Tell assembler to not insert instructions to fill branch delay slots.
# This is necessary when branch delay slots are disabled.
.set noreorder

.global _start
_start:
	la	$sp, 0x7FFFFFFC				# initialize stack pointer 
	jal main
	li $v0, 10
	syscall							# Use syscall 10 to stop simulation

main:
	# Place the return address on the stack
	addi $sp, $sp, -4		    	# initialize space on the stack to store $ra
	sw   $ra, 0($sp)		    	# save $ra on the stack
	
	# Print the SRC string
	li $v0, 4 			  	 	 	# system call code => print_string
	la $a0, SRC						# load address of the SRC string 
	syscall							# system call
	la $a0, newLineStr				# load address of the SRC string 
	syscall							# system call
	
	# Print the DES string
	la $a0, DES						# load address of the DES string 
	syscall							# system call
	la $a0, newLineStr				# load address of the SRC string 
	syscall							# system call
	
	# Print a space for after the copy
	la $a0, newLineStr				# load address of the SRC string 
	syscall							# system call
	
	# Call strcpy => strcpy(dest (a0), src (a1)
	la $a0, DES						# load address of DES to a temp reg
	la $a1, SRC						# load address of SRC to a temp reg
	jal strcpy
	
	# Print the SRC string
	li $v0, 4 			  	 	 	# system call code => print_string
	la $a0, SRC						# load address of the SRC string 
	syscall							# system call
	la $a0, newLineStr				# load address of the SRC string 
	syscall							# system call
	
	# Print the DES string
	la $a0, DES						# load address of the DES string 
	syscall							# system call
	la $a0, newLineStr				# load address of the SRC string 
	syscall							# system call
	
	# Print a space for after the copy
	la $a0, newLineStr				# load address of the SRC string 
	syscall							# system call
	
	END:
		# Restore the return address
		lw	$ra, 0($sp)			    # restore $ra
		addi $sp, $sp, 4		    # adjust stack to delete the item
		
		jr $ra						# exit main
		
strcpy:
    addi $sp, $sp, -4      			# adjust stack for 1 item
    sw   $s0, 0($sp)       			# save $s0
    add  $s0, $zero, $zero 			# i = 0
	
	L1: 
		add  $t1, $s0, $a1     		# addr of y[i] in $t1
    	lbu  $t2, 0($t1)       		# $t2 = y[i]
    	add  $t3, $s0, $a0     		# addr of x[i] in $t3
    	sb   $t2, 0($t3)       		# x[i] = y[i]
    	beq  $t2, $zero, L2    		# exit loop if y[i] == 0  
    	addi $s0, $s0, 1       		# i = i + 1
    	j    L1                		# next iteration of loop
	L2: 
		lw   $s0, 0($sp)       		# restore saved $s0
    	addi $sp, $sp, 4       		# pop 1 item from stack
   	 	jr   $ra               		# and return

.data
SRC: .string "ABCD"
DES: .string "EFGH"
newLineStr: .string "\n"
	