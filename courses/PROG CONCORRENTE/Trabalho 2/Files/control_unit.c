/******************************************************************************************************************************

* Arquivo : control_unit.c

* Data : 18/06/2010
* Professor : Paulo SL Souza
* Autores :
		Ubiratan Soares (5634292)
		Paulo Cesar Jr. (5634890)
		Paul Kibe (5729158)
		Petrus Gomes (5634966)

******************************************************************************************************************************/
#include "control_unit.h"
#include <stdio.h>

char opcode;

static char state = 0;

void Next_State(char ns0, char ns1, char ns2, char ns3) {
    state = ns0;
    state = state + (ns1 << 1);
    state = state + (ns2 << 2);
    state = state + (ns3 << 3);
}

//Gera int global "signals" com sinais de controle da UC
void Signals_Generate(char s0, char s1, char s2, char s3) {

	// Incializando variavel de retorno de 32 bits, os 18 bits mais significativos representam os sinais de controle,
	// os bits de 32 a 19 sao zero.

	// Sinais de controle, cada um representado como uma variavel de 8 bits, todos inicializados como zero

    char RegDst = 0,
		RegWrite = 0,
		ALUSrcA = 0,
		ALUSrcB[2],
		ALUOp[2],
		PCWrite = 0,
		IorD = 0,
		MemRead = 0,
		MemWrite = 0,
		MemtoReg = 0,
		IRWrite = 0;

		ALUSrcB[0] = 0;
		ALUSrcB[1] = 0;
		ALUOp[0] = 0;
		ALUOp[1] = 0;


	// Equacoes logicas para cada sinal de controle de acordo com a tabela anterior

	RegDst =      (!s3 && s2 &&  s1 &&  s0) || (!s3 &&  s2 &&  s1 &&  s0) || ( s3 &&  s2 &&  s1 && !s0);
	MPI_Send(&RegDst, 1, MPI_CHAR, PR_MUX_WR, 1, MPI_COMM_WORLD);

	RegWrite =     (!s3 &&  s2 && !s1 && !s0) || (!s3 &&  s2 && !s1 && !s0) || (!s3 &&  s2 &&  s1 &&  s0) || ( s3 && !s2 &&  s1 &&  s0);
	MPI_Send(&RegWrite, 1, MPI_CHAR, PR_REGISTERS, 1, MPI_COMM_WORLD);

	ALUSrcA =      (!s3 && !s2 &&  s1 && !s0) || (!s3 && !s2 &&  s1 && !s0) || (!s3 &&  s2 &&  s1 && !s0) || ( s3 && !s2 && !s1 && !s0) || ( s3 &&  s2 && !s1 && !s0) || ( s3 &&  s2 &&  s1 &&  s0);
	MPI_Send(&ALUSrcA, 1, MPI_CHAR, PR_MUX_ALU_A, 1, MPI_COMM_WORLD);

	ALUSrcB[0] =     (!s3 & !s2 & !s1 & !s0) || (!s3 & !s2 & !s1 &  s0);
	ALUSrcB[1] =     (!s3 & !s2 & !s1 &  s0) || (!s3 & !s2 &  s1 & !s0) || ( s3 &  s2 &  s1 &  s0);

	MPI_Send(ALUSrcB, 2, MPI_CHAR, PR_MUX_ALU_B, 1, MPI_COMM_WORLD);

	ALUOp[0] =       ( s3 && !s2 && !s1 && !s0) || ( s3 &&  s2 && !s1 && !s0) || ( s3 &&  s2 &&  s1 &&  s0);
	ALUOp[1] =       (!s3 &&  s2 &&  s1 && !s0) || ( s3 &&  s2 &&  s1 &&  s0);
	MPI_Send(ALUOp, 2, MPI_CHAR, PR_ALU_CTRL, 1, MPI_COMM_WORLD);

	PCWrite =      (!s3 && !s2 && !s1 && !s0) || ( s3 && !s2 && !s1 &&  s0) || ( s3 && !s2 &&  s1 && !s0) || ( s3 &&  s2 && !s1 &&  s0);
	MPI_Send(&PCWrite, 1, MPI_CHAR, PR_PC, 1, MPI_COMM_WORLD);

	IorD =         (!s3 && !s2 &&  s1 &&  s0) || (!s3 &&  s2 && !s1 &&  s0);
	MPI_Send(&IorD, 1, MPI_CHAR, PR_MUX_MEM, 1, MPI_COMM_WORLD);

	MemRead =      (!s3 && !s2 && !s1 && !s0) || (!s3 && !s2 &&  s1 &&  s0);
	MPI_Send(&MemRead, 1, MPI_CHAR, PR_MEMORY, 1, MPI_COMM_WORLD);

	MemWrite =     (!s3 &&  s2 && !s1 &&  s0);
	MPI_Send(&MemWrite, 1, MPI_CHAR, PR_MEMORY, 1, MPI_COMM_WORLD);

	MemtoReg =     (!s3 &&  s2 && !s1 && !s0);
	MPI_Send(&MemtoReg, 1, MPI_CHAR, PR_MUX_WD, 1, MPI_COMM_WORLD);

	IRWrite =      (!s3 && !s2 && !s1 && !s0);
	MPI_Send(&IRWrite, 1, MPI_CHAR, PR_IR, 1, MPI_COMM_WORLD);

}


//Ao final da execucao, os sinais sao gravados no int global "signals"
char ControlUnit(int ciclo) {

		if (ciclo == 1){
			opcode = 0;
		}

		// Variaveis para bits de representacao do estado atual

	    char s3 = 0, s2 = 0, s1 = 0, s0 = 0;

		// Variaveis para bits de representacao do proximo estado

	    char ns3 = 0, ns2 = 0, ns1 = 0, ns0 = 0;

	    // Inicialmente, isolam-se os bits de cada posicao da variavel "estado"

	    s0 = (state & 1);      // 0001
	    s1 = (state & 2) >> 1; // 0010
	    s2 = (state & 4) >> 2; // 0100
	    s3 = (state & 8) >> 3; // 1000

	    // Implementacao para a Maquina de Estados Finitos (MEF), de acordo com a imagem enviada juntamente com esse codigo.

	    // A MEF esta no estado 0

	    if (!s3 && !s2 && !s1 && !s0) {

	        // Alterando os bits de proximo estado para o estado 1
	        ns0 = 1;
	        ns1 = 0;
	        ns2 = 0;
	        ns3 = 0;

	    }

	    // A MEF está no estado 1
	    else
	        if (!s3 && !s2 && !s1 && s0) {

	            if (opcode == LW || opcode == SW) {
	        		// Alterando os bits de proximo estado para o estado 2
	                ns0 = 0;
	                ns1 = 1;
	                ns2 = 0;
	                ns3 = 0;

	            } else if (opcode == RTYPE) {

					// Alterando os bits de proximo estado para o estado 6
	                ns0 = 0;
	                ns1 = 1;
	                ns2 = 1;
	                ns3 = 0;
	            }
	        }

	    // A MEF está no estado 2
	    else
	        if (!s3 && !s2 && s1 && !s0) {
	            if (opcode == LW) {

	                // Alterando os bits de proximo estado para o estado 3
	                ns0 = 1;
	                ns1 = 1;
	                ns2 = 0;
	                ns3 = 0;

	            } else if (opcode == SW) {

	                // Alterando os bits de proximo estado para o estado 5
	                ns0 = 1;
	                ns1 = 0;
	                ns2 = 1;
	                ns3 = 0;

	            }
	        }

	    // A MEF está no estado 3
	    else
	        if (!s3 && !s2 &&  s1 &&  s0) {

	        // Alterando os bits de proximo estado para o estado 4
	        ns0 = 0;
	        ns1 = 0;
	        ns2 = 1;
	        ns3 = 0;

	    }

	    // a MEF está no estado 4
	    else
	        if (!s3 && s2 && !s1 && !s0) {

	        // Alterando os bits de proximo estado para o estado 0
	        ns0 = 0;
	        ns1 = 0;
	        ns2 = 0;
	        ns3 = 0;

	    }

	    // A MEF está no estado 5
	    else
	        if (!s3 &&  s2 && !s1 && s0) {
	        // alterando os bits de proximo estado para o estado 0
	        ns0 = 0;
	        ns1 = 0;
	        ns2 = 0;
	        ns3 = 0;

	    }

	    // A MEF está no estado 6
	    else
	        if (!s3 &&  s2 &&  s1 && !s0) {
	        // Alterando os bits de proximo estado para o estado 7
	        ns0 = 1;
	        ns1 = 1;
	        ns2 = 1;
	        ns3 = 0;

	    }

	    // A MEF está no estado 7
	    else
	        if (!s3 &&  s2 &&  s1 &&  s0) {
	        // Alterando os bits de proximo estado para o estado 0
	        ns0 = 0;
	        ns1 = 0;
	        ns2 = 0;
	        ns3 = 0;

	    }

		// Gerando o estado futuro de acordo com o estado atual.

		Next_State(ns0, ns1, ns2, ns3);

		// Gerando os sinais de controle para o estado atual

		Signals_Generate(s0, s1, s2, s3);

		MPI_Recv(&opcode, 1, MPI_CHAR, PR_IR, 1, MPI_COMM_WORLD, &status); // recebe opcode do IR

		// printf("\t\tPROCESS = CU ; Recebido de IR (opcode = %u)\n",opcode);
		// fflush(NULL);

		return opcode;
}

