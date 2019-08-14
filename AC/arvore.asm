#Lendo n
addi $v0, $zero, 5 
syscall
add $s0, $zero, $v0

#Reservando o espaço de memória
addi $s1, $zero, 12 # equivalente ao espaço de 3 variáveis 
mul $t1, $s1, $s0 # N * 12 (Três variáveis)
addi $t2, $zero, -1
mul $t1, $t1, $t2 # Invertendo o valor de N * 12

add $sp, $sp, $t1 #Tirando de SP o equivalente a N * 12

#Lendo valores e criando a árvore
main:
	
	# Salvando o primeiro valor diretamente 
	addi $v0, $zero, 5 
	syscall
	add $s0, $zero, $v0
	
	sw $s0, 0($sp)
	j inserir

salvarDados:
	addi $t0, $zero, 0
	for_l:
		mul $t1, $t0, $s1 # index  *  12
		add $t2, $sp, $t1 # sp + (index  *  12)

		lw $s0, 0($t2)
		bne $s0, $a1, salvar
		jr $ra
		
		salvar:
			slt $t3, $s0, $a1
			beq $t3, $zero, salvar_esq
			j salvar_dir 
			
			salvar_esq:
				addi $t4, $zero, 4
				mul $t1, $t0, $t4 # index  *  4
				addi $t2, $sp, $t1 # sp + (index  *  12)
				sw $s0, 0($t2)
	
inserir:
	addi $a0, $zero, 1 # index
	
	mul $t1, $t0, $s1 # index  *  12
	add $t2, $sp, $t1 # sp + (index  *  12)

	sw $s0, 0($t2)