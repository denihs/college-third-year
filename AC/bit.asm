addi $a0, $zero, 137
jal calc

# Mostrando quantidade de notas de 50
add $a0, $zero, $s0
addi $v0, $zero, 1
syscall
addi $a0, $zero, 32
addi $v0, $zero, 11
syscall

# Mostrando quantidade de notas de 10
add $a0, $zero, $s1
addi $v0, $zero, 1
syscall
addi $a0, $zero, 32
addi $v0, $zero, 11
syscall

# Mostrando quantidade de notas de 5
add $a0, $zero, $s2
addi $v0, $zero, 1
syscall
addi $a0, $zero, 32
addi $v0, $zero, 11
syscall

# Mostrando quantidade de notas de 1
add $a0, $zero, $s3
addi $v0, $zero, 1
syscall
addi $a0, $zero, 32
addi $v0, $zero, 11
syscall

j exit

calc:
	slti $t0, $a0, 1
	beq $t0, $zero, else
	jr $ra
	
	else:
	check50:
 		addi $t0, $zero, 50
		sub $t1, $a0, $t0
		bltz $t1, check10
		addi $s0, $s0, 1
		addi $a0, $a0, -50
		j calc
	
	
	check10:
 		addi $t0, $zero, 10
		sub $t1, $a0, $t0
		bltz $t1, check5
		addi $s1, $s1, 1
		addi $a0, $a0, -10
		j calc
		
	
	check5:
 		addi $t0, $zero, 5
		sub $t1, $a0, $t0
		bltz $t1, check1
		addi $s2, $s2, 1
		addi $a0, $a0, -5
		j calc
		
	check1:
 		addi $t0, $zero, 1
		sub $t1, $a0, $t0
		bltz $t1, check1
		addi $s3, $s3, 1
		addi $a0, $a0, -1
		j calc
	
exit: