		.data							#Inicio do segmento de dados
		.align 2						#Alinhamento em multiplos de 4 bytes
VET:	.word 1,2,3,4,5,6,7				#Vetor de inteiros
		.align 0						#Alinhamento a cada byte
M1:		.asciiz "Vetor Original = "		#Mensagem para o usuario
M2:		.asciiz "Novo Vetor = "			#Mensagem para o usuario
ENDL:	.ascii " \n"					#Pulo de linha
SPC:	.ascii "  " 					#Espaco
		.align 2						#Alinhamento em multiplos de 4 bytes


		.text							#Incio do segmento de texto
		.globl MAIN						#Ponto de entrada do program

MAIN:

		#Preparando a impressao de mensagem
		la $a0,M1						# $a0 = &M1
		li $v0,4						# $v0 = 4
		syscall


		


		#Saida do programa
END:	li $v0,10
		syscall		