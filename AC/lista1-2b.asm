#Lendo n
addi $v0, $zero, 5 
syscall
add $s0, $zero, $v0

#Lendo q
addi $v0, $zero, 5 
syscall
add $s1, $zero, $v0

#Lendo a1
addi $v0, $zero, 5 
syscall
add $s2, $zero, $v0

addi $t0, $zero, 1 # index
addi $t1, $zero, 1 # Será a potência

# Adiantando o primeiro resultado
add $a0, $zero, $s2
addi $v0, $zero, 1
syscall

addi $a0, $zero, 32 # ascii para espaço
addi $v0, $zero, 11 # setando o comando de impressão de caracter
syscall

for:
	# q ^ n
	mul $t3, $s1, $t1
	# a1 * q ^ n
	mul $t4, $s2, $t3
	
	# Monstrando resultado
	add $a0, $zero, $t4
	addi $v0, $zero, 1
	syscall
	
	addi $a0, $zero, 32 # ascii para espaço
	addi $v0, $zero, 11 # setando o comando de impressão de caracter
	syscall
	
	addi $t0, $t0, 1 # incrementando o index
	
	mul $t1, $t1, $s1 # elevando a potência
	
	slt $t2, $t0, $s0
	bne $t2, $zero, for
	
