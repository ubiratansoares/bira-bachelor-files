##################################################################
# Trabalho 1 - Organizacao de Computadores
# Somador IEEE754 usando inteiros
# 
# Trabalho desenvolvido pelos alunos
# Ubiratan Soares
# Fernando Neves
# Lohan Ebrener
#
# Data:  05/10/2009
##################################################################

##################################################################
# SEGMENTO DE DADOS
##################################################################

			.data

			.align 2
NUM1:		.float -0.75	# Operando 1
NUM2:		.float 	2.5		# Operando 2

			.align 0
SIGMASK:	.byte 0x00,0x00,0x00,0x10	# Mascara para isolar sinal
EXPMASK:	.byte 0x00,0x00,0x80,0x7f	# Mascara para expoente
FRAMASK:	.byte 0xff,0xff,0x7f,0x00	# Mascara para mantissa

			.align 2

##################################################################
# SEGMENTO DE TEXTO
##################################################################

			.text
			.globl MAIN

MAIN:	

	##############################################################

	# Descrevendo as variaveis
	# $s1 - sinal do operando 1
	# $s2 - sinal do operando 2
	# $s3 - expoente do operando 1
	# $s4 - mantissa do operando 1
	# $s5 - expoente do operando 2
	# $s6 - mantissa do operando 2		

	##############################################################


	##############   Parser do operando 1   ######################
 	

			la $t0,NUM1			# $t0 = &NUM1
			lw $t0,0($t0)		# $t0 = NUM1

			# Isolando o sinal de NUM1

			la $s0,SIGMASK		# $s0 = &SIGMASK
			lw $s0,0($s0)		# $s0 = SIGMASK
			and $s1,$t0,$s0		# $s1 = $t0 AND $s0

			# Isolando o expoente de NUM1

			la $s0,EXPMASK		# $s0 = &EXPMASK
			lw $s0,0($s0)		# $s0 = EXPMASK
			and $s3,$t0,$s0		# $s3 = $t0 AND $s0


			# Isolando a mantissa de NUM1

			la $s0,FRAMASK		# $s0 = &FRAMASK
			lw $s0,0($s0)		# $s0 = FRAMASK
			and $s5,$t0,$s0		# $s5 = $t0 AND $s0

	##############   Parser do operando 2   ######################


			la $t0,NUM2			# $t0 = &NUM2
			lw $t0,0($t0)		# $t0 = NUM2

			# Isolando o sinal de NUM2

			la $s0,SIGMASK		# $s0 = &SIGMASK
			lw $s0,0($s0)		# $s0 = SIGMASK
			and $s2,$t0,$s0		# $s2 = $t0 AND $s0

			# Isolando o expoente de NUM1

			la $s0,EXPMASK		# $s0 = &EXPMASK
			lw $s0,0($s0)		# $s0 = EXPMASK
			and $s4,$t0,$s0		# $s4 = $t0 AND $s0


			# Isolando a mantissa de NUM1

			la $s0,FRAMASK		# $s0 = &FRAMASK
			lw $s0,0($s0)		# $s0 = FRAMASK
			and $s6,$t0,$s0		# $s6 = $t0 AND $s0



	########  Determinando o operando de menor expoente  ##########

			# Subtraimos o deslocamenteo para precisao simples

			subi $s3,$s3,127
			subi $s4,$s4,127

			# Comparamos os expoentes
			# Inicialmente, testamos a igualdade. Caso afirmativo,
			# desviamos para o label EQ_EXP

			beq $s3,$s5,EQ_EXP


			# Se os expoentes sao diferentes, testamos para ver quem
			# eh o menor. Usando $s0 = 1 como flag para indicar que
			# $s3 < $s5, ou seja, exp(NUM1) < exp(NUM2)


			slt $s0,$s3,$s5		# se ($s3 < $s5), entao $s0 = 1
								# senao $s0 = 0


	###############      Os expoente sao iguais     ##############
			

EQ_EXP:


END:		# Saida do programa

			li $v0,10			# Prepara syscall para sair do programa
			syscall
