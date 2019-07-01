addi $a0, $zero, 5
jal fac
add $a0, $zero, $v0
addi $v0, $zero, 1
syscall
j exit 

fac:
	addi $sp, $sp, -8
	sw $ra, 0($sp)
	sw $a0, 4($sp)
	
	slti $t0, $a0, 2
	beq $t0, $zero, else
	addi $v0, $zero, 1
	jr $ra
	
	else :
		addi $a0, $a0, -1 
		jal fac
		
		lw $a0, 4($sp)
		lw $ra, 0($sp)

		mul $v0, $v0, $a0
		addi $sp, $sp, 8
		jr $ra 
	
exit: