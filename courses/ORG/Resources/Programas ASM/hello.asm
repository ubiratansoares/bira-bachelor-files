			.data				# Inicio do segmento de dados
			.align 0			# Dados a seguir alinhados a cada 1 byte
STR:		.asciiz "Hello"		# String 
			.align 2			# Dados a seguir alinhados a cada 4 bytes
			
			
			.text				# Inicio do segmento de texto
			.globl MAIN			# Main
			
MAIN:		

			# Imprimindo a string
			
			li $v0,4
			la $a0,STR
			syscall
			
			
			# Encerrando o programa
			
			li $v0,10
			syscall
