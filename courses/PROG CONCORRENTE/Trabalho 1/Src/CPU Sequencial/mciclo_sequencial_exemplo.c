#include <stdio.h>
#include <stdlib.h>
#include "mascara.h"

/****************************
 inicio das variaveis globais
 ****************************/
int memoria[MAX];			// Memoria RAM
int reg[NUMREG];			// Banco de Registradores
int PC = 0, IR=-1, MDR, A, B, ALUOUT;	// Registradores especiais

int SC = 0;				// Sinais de Controle (sao usados 16 sinais)
					// cada bit de SC representa um sinal de controle. Vide mascara.h

char SC_ULA = 0;			// Controle da ULA e o bit "zero"
					//    sao usados apenas 4 bits dos 8 disp. Vide mascara.h

int PCnew, IRnew, MDRnew, Anew, Bnew,
	  ALUOUTnew;	// Registradores auxiliares.
			// Guardam temporariamente o resultado que sera
			// atualizado apenas no final do ciclo (para que o mesmo
			// esteja disponivel apenas no incio do ciclo seguinte.

/***********************
 inicio da  prototipacao
 ***********************/
int erro(char []);

void main (void);

int ULA (int, int, char);

int mulplx_mem(char);
char mulplx_areg(char);
int mulplx_dreg(char);
int mulplx_op1(char);
int mulplx_op2(char);
int mulplx_PC(char);

void UC_ULA(char, char);
int shiftleft(int);
int signalextend(int);
int concat(int, int);

void UnidadeControle(void);
void Busca_Instrucao(void);
void Decodifica_BuscaRegistrador(void);
void Execucao_CalcEnd_Desvio(void);
void EscreveTipoR_AcessaMemoria(void);
void EscreveRefMem(void);

void Atualiza_Registradores(void);

/******************
 inicio das funcoes
 ******************/
int erro(char mensagem[])
{
	printf("Programa finalizado! \n");
	printf("Mensagem de erro: %s \n", mensagem);

	printf("[21] %ld \n", memoria[21]);
	printf("[22] %ld \n", memoria[22]);
	printf("[23] %ld \n", memoria[23]);
	printf("[24] %ld \n", memoria[24]);
	printf("[25] %ld \n", memoria[25]);
	printf("[26] %ld \n", memoria[26]);

	printf("Tecle algo para finalizar... \n");
	getchar();

	exit(-1);

	return (-1);

} // fim de erro


int ULA(int op1, int op2, char controle)
{
	int saida;

	controle = controle & separa_controle_ula;

	if (controle == 0)		// 000
		saida = op1 & op2;
	if (controle == 1)		// 001
		saida = op1 | op2;
	if (controle == 2)		// 010
		saida = op1 + op2;
	if (controle == 6)		// 110
		saida = op1 - op2;
	if (controle == 7)		// 111
		if (op1 < op2)
			saida = 1;
		else
			saida = 0;

	if(saida == 0)
		SC_ULA = SC_ULA | ativa_bit_zero;
	else
		SC_ULA = SC_ULA & desativa_bit_zero;

	return(saida);

}//fim de ULA

/* endereco de memoria */
int mulplx_mem(char controle)
{

	if (controle < 0 || controle > 1)
		erro("mulplx_mem() recebeu controle errado");

	if (controle == 0)
		return(PC);

	if (controle == 1)
		return(ALUOUT);

	return(-1);

} // fim de mulplx_mem


/* endereco registrador a ser escrito */
char mulplx_areg(char controle)
{
	if (controle < 0 || controle > 1)
		erro("mulplx_areg() recebeu controle errado");

	if (controle == 0)
		return( (char) ((IR & separa_rt)>>16) );  // campo RT da Instrucao: IR[20-16]

	if (controle == 1)
		return( (char) ((IR & separa_rd)>>11) );  // campo RD da instrucao: IR[15-11];

	return (-1);

} // fim de mulplx_areg

/* dado a ser escrito no bco de registradores */
int mulplx_dreg(char controle)
{
	if (controle < 0 || controle > 1)
		erro("mulplx_dreg() recebeu controle errado");

	if (controle == 0)
		return(ALUOUT);

	if (controle == 1)
		return(MDR);

	return (-1);

} // fim de mulplx_dreg

/* primeiro operando ULA  */
int mulplx_op1(char controle)
{
	if (controle < 0 || controle > 1)
		erro("mulplx_op1() recebeu controle errado");

	if (controle == 0)
		return(PC);

	if (controle == 1)
		return(A);

	return (-1);

} // fim de mulplx_op1

/* segundo operando ULA  */
int mulplx_op2(char controle)
{
	if (controle < 0 || controle > 3)
		erro("mulplx_op2() recebeu controle errado");

	if (controle == 0)
		return(B);

	if (controle == 1)
		return((int)4);

	if (controle == 2)
		return( signalextend( (int)(IR & separa_imediato) ) );

	if (controle == 3)
		return( shiftleft( signalextend( (int)(IR & separa_imediato) ) ) );

	return (-1);

} // fim de mulplx_op2

/* dado a ser escrito em PC  */
int mulplx_PC(char controle)
{
	if (controle < 0 || controle > 2)
		erro("mulplx_PC() recebeu controle errado");

	if (controle == 0)
		return(ALUOUTnew);  // nao pega resultado de ALUOUT e sim direto da ULA
	if (controle == 1)
		return(ALUOUT);

	if (controle == 2)
		return( concat( shiftleft( (int) (IR & separa_endereco_jump) ), PC ) );

	return (-1);

} // fim de mulplx_PC

void UC_ULA(char ALUOp, char campo_funcao)
{
	// zera os 2 bits + signif. do campo de funcao. Nao sao necessarios.
	campo_funcao = campo_funcao & zera_2bits_cfuncao;

	// zera os 3 bits menos sig de SC_ULA
	// necessario para usar | abaixo e nao atuar no bit "zero" (posicao 3)
	SC_ULA = SC_ULA & zera_controle_ULA;

	if(ALUOp == 0) //00 em binario
		SC_ULA = SC_ULA | ativa_soma;

	if (ALUOp == 1) //01 em binario
		SC_ULA = SC_ULA | ativa_subtracao;

	if (ALUOp == 2) { //10 em binario
		if (campo_funcao == 0)  // 0000
			SC_ULA = SC_ULA | ativa_soma;
		if (campo_funcao == 2)  // 0010
			SC_ULA = SC_ULA | ativa_subtracao;
		if (campo_funcao == 4)  // 0100
			SC_ULA = SC_ULA | ativa_and;
		if (campo_funcao == 5)  // 0101
			SC_ULA = SC_ULA | ativa_or;
		if (campo_funcao == 10)  // 1010
			SC_ULA = SC_ULA | ativa_slt;
	}
} // fim de UC_ULA


int shiftleft(int valor)
{
	return( valor << 2 );

} // shiftleft


int signalextend(int valor)
{
	return( (int) valor);

} // fim de signalextend


int concat(int IR28_0, int pc)
{
	pc = pc & separa_4bits_PC;  // separa os 4 bits mais sig. do PC
	IR28_0 = IR28_0 | pc;	// concatena os 4 bits separados com o endereco
					// obtido na instrucao (IR[26_0] + deslocamento
					// de 2 bits.
	return( IR28_0 );

} // fim de concat


void Busca_Instrucao(void)
{
	int endereco_leitura;
	char MemRead, IRWrite, PCWrite;

	MemRead = (char) ((SC & separa_MemRead) >> 12);
	IRWrite = (char) ((SC & separa_IRWrite) >> 15);
	PCWrite = (char) ((SC & separa_PCWrite) >> 10);

	if(MemRead && IRWrite) {
		char IorD;

		IorD    = (char) ((SC & separa_IorD)    >> 11);

		endereco_leitura = mulplx_mem(IorD); // escolhe entre PC e ALUOUT
		IRnew = memoria[(int)((int)endereco_leitura / 4)];
	}

	if(PCWrite)      //PC = PC + 4;
	{
		char ALUSrcA, ALUSrcB0, ALUSrcB1, ALUOp0, ALUOp1, PCSource0, PCSource1;
		int op1, op2;

		ALUSrcA	= (char) ((SC & separa_ALUSrcA) >> 2);

		ALUSrcB0	= (char) ((SC & separa_ALUSrcB0) >> 3);
		ALUSrcB1	= (char) ((SC & separa_ALUSrcB1) >> 3);

		ALUOp0	= (char) ((SC & separa_ALUOp0) >> 5);
		ALUOp1	= (char) ((SC & separa_ALUOp1) >> 5);

		PCSource0	= (char) ((SC & separa_PCSource0) >> 7);
		PCSource1	= (char) ((SC & separa_PCSource1) >> 7);

		op1 = mulplx_op1(ALUSrcA);
		op2 = mulplx_op2( (ALUSrcB0 | ALUSrcB1) );

		UC_ULA((ALUOp0 | ALUOp1), (char) (IR & separa_cfuncao));

		ALUOUTnew = ULA(op1, op2, SC_ULA);

		PCnew = mulplx_PC((PCSource0 | PCSource1));
	}
}// fim de Busca_Instrucao


void Decodifica_BuscaRegistrador(void)
{
	char ALUSrcA, ALUSrcB0, ALUSrcB1, ALUOp0, ALUOp1;
	int op1, op2;

	Anew = reg[(int)((IR & separa_rs)>>21)];  // campo RS da Instrucao: IR[25-21]
	Bnew = reg[(int)((IR & separa_rt)>>16)];  // campo RT da Instrucao: IR[20-16]

	ALUSrcA = (char) ((SC & separa_ALUSrcA) >> 2);

	ALUSrcB0 = (char) ((SC & separa_ALUSrcB0) >> 3);
	ALUSrcB1 = (char) ((SC & separa_ALUSrcB1) >> 3);

	ALUOp0 = (char) ((SC & separa_ALUOp0) >> 5);
	ALUOp1 = (char) ((SC & separa_ALUOp1) >> 5);

	op1 = mulplx_op1(ALUSrcA);
	op2 = mulplx_op2( (ALUSrcB0 | ALUSrcB1) );

	UC_ULA((ALUOp0 | ALUOp1), (char) (IR & separa_cfuncao));

	ALUOUTnew = ULA(op1, op2, SC_ULA);

} // fim de Decodifica_BuscaRegistrador

void Execucao_CalcEnd_Desvio(void)
{
	char ALUSrcA, ALUSrcB0, ALUSrcB1, ALUOp0, ALUOp1, PCSource0, PCSource1;
	char PCWriteCond, BitZero;

	int op1, op2;

	ALUSrcA  = (char) ((SC & separa_ALUSrcA) >> 2);

	ALUSrcB0 = (char) ((SC & separa_ALUSrcB0) >> 3);
	ALUSrcB1 = (char) ((SC & separa_ALUSrcB1) >> 3);

	ALUOp0 = (char) ((SC & separa_ALUOp0) >> 5);
	ALUOp1 = (char) ((SC & separa_ALUOp1) >> 5);

	PCSource0 = (char) ((SC & separa_PCSource0) >> 7);
	PCSource1 = (char) ((SC & separa_PCSource1) >> 7);

	PCWriteCond = (char) ((SC & separa_PCWriteCond) >> 9);

	op1 = mulplx_op1(ALUSrcA);
	op2 = mulplx_op2( (ALUSrcB0 | ALUSrcB1) );

	UC_ULA((ALUOp0 | ALUOp1), (char) (IR & separa_cfuncao));

	ALUOUTnew = ULA(op1, op2, SC_ULA);
	BitZero = (char) ((SC_ULA & separa_bit_zero) >> 3);

	if (PCWriteCond && BitZero)
		PCnew = mulplx_PC((PCSource0 | PCSource1));

} // fim de Execucao_CalcEnd_Desvio

void EscreveTipoR_AcessaMemoria(void)
{
	char MemRead, MemWrite, RegWrite;

	MemRead  = (char) ((SC & separa_MemRead) >> 12);
	MemWrite = (char) ((SC & separa_MemWrite) >> 12);
	RegWrite = (char) ((SC & separa_RegWrite) >> 1);

	if (MemRead || MemWrite) {
		char IorD;
		int endereco_acesso;

		IorD    = (char) ((SC & separa_IorD) >> 11);
		endereco_acesso = mulplx_mem(IorD); // escolhe entre PC e ALUOUT

		if (MemRead) // load word
			MDRnew = memoria[(int)((int)endereco_acesso / 4)];
		if (MemWrite) // store word
			memoria[(int)((int)endereco_acesso / 4)] = B;
	}

	if (RegWrite) {
		char RegDst, MemtoReg;

		RegDst   = (char) (SC & separa_RegDst);
		MemtoReg = (char) ((SC & separa_MemtoReg) >> 14);

		reg[mulplx_areg(RegDst)] = mulplx_dreg(MemtoReg);
	}
} // fim de EscreveTipoR_AcessaMemoria

void EscreveRefMem(void)
{
	char RegWrite;

	RegWrite = (char) ((SC & separa_RegWrite) >> 1);

	if (RegWrite) {
		char RegDst, MemtoReg;

		RegDst   = (char) (SC & separa_RegDst);
		MemtoReg = (char) ((SC & separa_MemtoReg) >> 14);

		reg[mulplx_areg(RegDst)] = mulplx_dreg(MemtoReg);
	}
} // fim de EscreveRefMem


// Unidade de Controle por PLA
// implementa a PLA da Fig. C.14 de Patterson; Hennessy(1998)
void UnidadeControle(void) {

	static char estado = 0;
	char S0, S1, S2, S3;
	char COp0, COp1, COp2, COp3, COp4, COp5;
	char PE0, PE1, PE2, PE3;
	char COp;

	S0 =  estado & separa_S0;
	S1 = (estado & separa_S1) >> 1;
	S2 = (estado & separa_S2) >> 2;
	S3 = (estado & separa_S3) >> 3;

	// isola codigo de operacao
	COp = (char) ((IR & separa_cop) >> 26);

	// separa bits do codigo de operacao
	COp0 =  COp & separa_COp0;
	COp1 = (COp & separa_COp1) >> 1;
	COp2 = (COp & separa_COp2) >> 2;
	COp3 = (COp & separa_COp3) >> 3;
	COp4 = (COp & separa_COp4) >> 4;
	COp5 = (COp & separa_COp5) >> 5;

	SC = 0;

	// ativa_RegDst se estado7
	SC = SC | (!S3 & S2 & S1 & S0);

	//  ativa_RegWrite se estado4 || estado7
	SC = SC | ( ( (!S3 & S2 & !S1 & !S0 )  |
			(!S3 & S2 &  S1 &  S0 )    ) << 1);

	//  ativa_ALUSrcA se estado2 || estado6 || estado8
	SC = SC | ( ( (!S3 & !S2 &  S1 & !S0 ) |
			(!S3 &  S2 &  S1 & !S0 ) |
			( S3 & !S2 & !S1 & !S0 )   ) << 2);

	//  ativa_ALUSrcB0 se estado0 || estado1
	SC = SC | ( ( (!S3 & !S2 & !S1 & !S0 ) |
			(!S3 & !S2 & !S1 &  S0 )    ) << 3);

	//  ativa_ALUSrcB1 se estado1 || estado2
	SC = SC | ( ( (!S3 & !S2 & !S1 &  S0 ) |
			(!S3 & !S2 &  S1 & !S0 )    ) << 4);

	//  ativa_ativa_ALUOp0 se estado8
	SC = SC | ( (S3 & !S2 & !S1 & !S0 ) << 5);

	//  ativa_ALUOp1 se estado6
	SC = SC | ( (!S3 & S2 & S1 & !S0 ) << 6);

	//  ativa_PCSource0 se estado8
	SC = SC | ( (S3 & !S2 & !S1 & !S0) << 7);

	//  ativa_PCSource1 se estado9
	SC = SC | ( (S3 & !S2 & !S1 & S0)  << 8);

	//  ativa_PCWriteCond se estado8
	SC = SC | ( (S3 & !S2 & !S1 & !S0 ) << 9);

	//  ativa_PCWrite se estado0 || estado9
	SC = SC | ( ( (!S3 & !S2 & !S1 & !S0 ) |
			( S3 & !S2 & !S1 &  S0  )   )  << 10);

	//  ativa_IorD se estado3 || estado5
	SC = SC | ( ( (!S3 & !S2 &  S1 & S0) |
			(!S3 &  S2 & !S1 & S0)   ) << 11);

	//  ativa_MemRead se estado0 || estado3
	SC = SC | ( ( (!S3 & !S2 & !S1 & !S0 ) |
			(!S3 & !S2 &  S1 &  S0  )   ) << 12);

	//  ativa_MemWrite se estado5
	SC = SC | ( (!S3 & S2 & !S1 & S0 ) << 13);

	//  ativa_MemToReg se estado4
	SC = SC | ( (!S3 & S2 & !S1 & !S0 ) << 14);

	//  ativa_IRWrite se estado0
	SC = SC | ( (!S3 & !S2 & !S1 & !S0 ) << 15);


	if (IR == 0)
		erro("Busca_Instrucao() obteve nop");

/*	if (estado == 0) {
		printf("IR= %x \n", IR);
		getchar();
	}*/

/*	PE0 estara ativo se:
		Pestado1 = estado0 ||
		Pestado3 = estado2 && COp=lw (100011=35) ||
		Pestado5 = estado2 && COp=sw (101011=43) ||
		Pestado7 = estado6 ||
		Pestado9 = estado1 && COp=jmp (000010=2)   */

	PE0 = ( (!S3 & !S2 & !S1 & !S0 ) |
		 (!S3 & !S2 & S1  & !S0 & COp5 & !COp4 & !COp3 & !COp2 & COp1 & COp0)  |
		 (!S3 & !S2 & S1  & !S0 & COp5 & !COp4 & COp3  & !COp2 & COp1 & COp0)  |
		 (!S3 & S2  & S1  & !S0 ) |
		 (!S3 & !S2 & !S1 &  S0 & !COp5 & !COp4 & !COp3 & !COp2 & COp1 & !COp0) );

/*	PE1 estara ativo se: 
		Pestado2 = estado1 && COp=lw (100011=35) ||
		Pestado2 = estado1 && COp=sw (101011=43) ||
		Pestado3 = estado2 && COp=lw (100011=35) ||
		Pestado6 = estado1 && COp=TipoR (000000=0) ||
		Pestado7 = estado6   */

	PE1 = ( ( !S3 & !S2 & !S1 &  S0 & COp5 & !COp4 & !COp3 & !COp2 &  COp1 & COp0   )  |
		 ( !S3 & !S2 & !S1 &  S0 & COp5 & !COp4 &  COp3 & !COp2 &  COp1 & COp0   )  |
		 ( !S3 & !S2 & S1  & !S0 & COp5 & !COp4 & !COp3 & !COp2 &  COp1 & COp0   )  |
		 ( !S3 & !S2 & !S1 & S0 & !COp5 & !COp4 & !COp3 & !COp2 & !COp1 & !COp0  )  |
		 ( !S3 & S2  & S1  & !S0  )     ) << 1;

/*	PE2 estara ativo se:
		Pestado4 = estado3 ||
		Pestado5 = estado2 && COp=sw (101011=43) ||
		Pestado6 = estado1 && COp=TipoR (000000=0) ||
		Pestado7 = estado6  */

	PE2 = ( (!S3 & !S2 &  S1 &  S0 )  |
		 (!S3 & !S2 &  S1 & !S0 &  COp5 & !COp4 &  COp3 & !COp2 &  COp1 &  COp0)  |
		 (!S3 & !S2 & !S1 &  S0 & !COp5 & !COp4 & !COp3 & !COp2 & !COp1 & !COp0)  |
		 (!S3 &  S2 &  S1 & !S0 ) )  << 2;

/*	PE3 estara ativo se: 
		Pestado8 = estado1 && COp=beq (000100=4) ||
		Pestado9 = estado1 && COp=jmp (000010=2)     */

	PE3 = ( (!S3 & !S2 & !S1 & S0 & !COp5 & !COp4 & !COp3 &  COp2 & !COp1 & !COp0)  |
		 (!S3 & !S2 & !S1 & S0 & !COp5 & !COp4 & !COp3 & !COp2 &  COp1 & !COp0)  )  << 3;


	estado = (PE0 | PE1 | PE2 | PE3);

	return;

} // fim UnidadeControle


void Atualiza_Registradores(void)
{

	PC     = PCnew;
	IR     = IRnew;
	MDR    = MDRnew;
	A      = Anew;
	B      = Bnew;
	ALUOUT = ALUOUTnew;

	return;

}  // fim de Atualiza_Registradores

void main (void)
{
	char loop=0;
	system("clear");
	//	clrscr();
	// este codigo le posicoes de um vetor e subtrai 4 de cada uma
	memoria[0] = 0x8c480000;  // lw $t0, 0($v0)
	memoria[1] = 0x010c182a;  // slt $v1, $t0, $t4
	memoria[2] = 0x106d0004;  // beq $v1, $t5, fim(4 palavras abaixo de PC+4)
//	memoria[3] = 0x010b4024;  // and $t0, $t0, $t3
//	memoria[3] = 0x01084020;  // add $t0, $t0, $t0
	memoria[3] = 0x010b4022;  // sub $t0, $t0, $t3
	memoria[4] = 0xac480000;  // sw $t0, 0($v0)
	memoria[5] = 0x004b1020;  // add $v0, $t3, $v0     //  sw $t0, 0($v0)
	memoria[6] = 0x08000000;  // j inicio (palavra 0)
	memoria[7] = 0;           // fim
	memoria[8] = 0;
	memoria[9] = 0;
	// Dados

	memoria[21] = 20;
	memoria[22] = 22;
	memoria[23] = 24;
	memoria[24] = 26;
	memoria[25] = 28;
	memoria[26] = -1;

	reg[2]  = (int) 84;  // $v0

	reg[11] = (int) 4;  // $t3
	reg[12] = (int) 0;  // $t4
	reg[13] = (int) 1;  // $t5

	//os loops deste while simulam os ciclos do processador. A cada ciclo a UC gera os
	//sinais de controle para que o caminho de dados possa executar.
	//ao final do ciclo os dados escritos nele sao atualizados para estarem disponiveis 
	//no proximo ciclo.
	// cada instrucao precisara de um nr variado de ciclos para executar.
	while(1){
		UnidadeControle();
		Busca_Instrucao();
		Decodifica_BuscaRegistrador();
		Execucao_CalcEnd_Desvio();
		EscreveTipoR_AcessaMemoria();
		EscreveRefMem();
		Atualiza_Registradores();

	} // fim do while(1)

} // fim de main
