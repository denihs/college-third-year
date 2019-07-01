addi $a0, $zero, 8
jal calc
add $a0, $v0, $zero
addi $v0, $zero, 1
syscall
j exit

calc:	
	addi $sp, $sp, -8
	sw $ra, 0($sp)
	sw $a0, 4($sp)
	
	slti $t0, $a0, 3
	beq $t0, $zero, else
	addi $v0, $zero, 1
	jr $ra
	
	else:
		lw $a0, 4($sp)
		addi $t0, $zero, 2
		div $a0, $t0
		mflo $a0
		
		jal calc
		
		lw $ra, 0($sp)
		lw $a0, 4($sp)
		
		addi $t0, $zero, 2
		mul $v0, $v0, $t0
		add $v0, $v0, $a0
	
		addi $sp, $sp, 8
		jr $ra
exit: