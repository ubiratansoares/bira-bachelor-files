		.data	# Inicio dos dados
		.align 0	# Alinhamento 0 - os dados a seguir serao colocados na memoria em posicoes multiplas de 2^0 = 1 byte
		.asciiz "Hello Word!"	# Z indica que tem um '/0' no final da string
STR:	.align 2 	# Alinhamento 2 -> os dados abaixo serao colocados em posicoes multiplas de 2^2= 4 bytes na memoria
OP2:	.word 120	# Valor 120 em complemento de 2 (32 bits)


		.text	# Inicio do segmento de texto
				#.align 2 - Forcar alinhamento 2 eh boa pratica de programacao
		.globl MAIN		# Ponto de entrada do programa
		
MAIN:			
		la $a0,STR	# Carrega o endereco apontado por STR no registrador A0
		li $v0,4	# Carrega o valor 4 no registrador V0
		syscall 	# Chama o SO, usando a chamada de numero 4 (registrado V0) - print string
		
		la $t0,OP2	# Carrega o endereco de OP2 em $T0 
					# $T0 = &OP2
								
		lw $t0,0($t0)	# Carrega o conteudo apontado por $T0, com base em zero, para o proprio $T0 
						# $TO = RAM[$T0 + 0]
		
		addi $a0,$t0,5	# $A0 = $T0 + 5

		li $v0,1	# Carrega o valor 1 no registrador V0
		syscall		# Chama o SO, usando a chamada de numero 1 (registrado V0) - print int
		
		li $v0,10	# Carrega o valor 10 no registrador V0
		syscall		# Chama o SO, usando a chamada de numero 10 (registrado V0) - exit
				
		
