.data
message: .asciiz "Já inserido\n"

.text
#Lendo n
addi $v0, $zero, 5 
syscall
add $s7, $zero, $v0

#Reservando o espaço de memória
addi $s6, $zero, 12 # equivalente ao espaço de 3 variáveis 
mul $t1, $s6, $s7 # N * 12 (Três variáveis)
addi $t2, $zero, -1
mul $t1, $t1, $t2 # Invertendo o valor de N * 12

add $sp, $sp, $t1 #Tirando de SP o equivalente a N * 12

#Lendo valores e criando a árvore
main:
	j inserir

salvarDados:
	# $a0 =  index da função inserir
	# $a1 = Entrada que vai ser verificada com os itens da arvore
	# $a2 = Endereço de onde se quer colocar o novo valor
	
	# $s1 é a cabeça do nó que estou trabalhando
	add $s1, $sp, $zero # inicia com o valor de sp
	
	for_l:
		lw $s0, 0($s1)
		bne $s0, $a1, salvar
		
		add $v1, $zero, $a0
		
		# mostrando mensagem que o dado já foi inserido
		addi $v0, $zero, 4
    lui $a0, 0x1001
    syscall
    
		jr $ra
		
		salvar:
			sw $a1, 0($a2) # Salvando a entrada na posição desejada
			slt $t3, $s0, $a1 # se o valor no nó é menor que o valor de entrada
			beq $t3, $zero, salvar_esq
			j salvar_dir 
			
			salvar_esq:
				addi $t4, $s1, 4 # Significa que vou acessar o que representa o atributo que guarda a esqueda do nó
				lw $t5, 0($t4)
				
				add $s1, $zero, $t5 # caso tenhamos um endereço  a direita, a próxima iteração vai usar esse endereço
				bgtz $t5, for_l # se já tiver um nó vinculado a esquerda, tenta inserir neste nó
				
				sw $a2, 0($t4)
				j sair_salvar
			
			salvar_dir:
				addi $t4, $s1, 8 # Significa que vou acessar o que representa o atributo que guarda a direita do nó
				lw $t5, 0($t4)
				
				add $s1, $zero, $t5 # caso tenhamos um endereço  a direita, a próxima iteração vai usar esse endereço
				bgtz $t5, for_l # se já tiver um nó vinculado a direita, tenta inserir neste nó
				
				sw $a2, 0($t4)	
		
		sair_salvar:
			addi $a0, $a0, 1 #incrementa o valor index da função inserir
			add $v1, $zero, $a0
			jr $ra
	
inserir:
	addi $v0, $zero, 5 
	syscall
	add $a1, $zero, $v0
	sw $a1, 0($sp) # colocando o primeiro elemento diretamente
	
	addi $a0, $zero, 1 # index
	for:
		addi $v0, $zero, 5 
		syscall
		add $a1, $zero, $v0
		
		addi $t0, $zero, 12 # equivalente ao espaço de 3 variáveis 
		mul $t1, $a0, $t0 
		add $a2, $sp, $t1 #indica onde eu quero colocar o valor informado
		jal salvarDados
		add $a0, $v1, $zero
		bne $a0, $s7, for
		
		add $a1, $zero, $sp

imprimir:
		lw $t0, 0($a1)
		bne $t0, $zero, continua
		jr $ra
		
		continua:
			addi $t0, $a1, 4
			lw $a1, 0($t0)
			beq $a1, $zero, skip
			jal imprimir
			skip:
			add $a1, $zero, $t0
			# Monstrando elemento
			
			lw $t0, 0($a1)
			add $a0, $zero, $t0
			addi $v0, $zero, 1
			syscall
			
			addi $a0, $zero, 32 # ascii para espaço
			addi $v0, $zero, 11 # setando o comando de impressão de caracter
			syscall
		
			addi $t0, $a1, 4
			lw $a1, 0($t0)
			beq $a1, $zero, skip2
			jal imprimir
			skip2:

sair:
