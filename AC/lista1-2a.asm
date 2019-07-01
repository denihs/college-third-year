#Lendo n
addi $v0, $zero, 5 
syscall
add $s0, $zero, $v0

#Lendo r
addi $v0, $zero, 5 
syscall
add $s1, $zero, $v0

#Lendo a1
addi $v0, $zero, 5 
syscall
add $s2, $zero, $v0

addi $t0, $zero, 0 # index

for:
	add $a0, $zero, $s2
	addi $v0, $zero, 1 # setando o comando de impressão de inteiros
	syscall
	
	addi $a0, $zero, 32 # ascii para espaço
	addi $v0, $zero, 11 # setando o comando de impressão de caracter
	syscall
	
	addi $t0, $t0, 1
	add $s2, $s2, $s1
	
	slt $t2, $t0, $s0
	bne $t2, $zero, for
	
