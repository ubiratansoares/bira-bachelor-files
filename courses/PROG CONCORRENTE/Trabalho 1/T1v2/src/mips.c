

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "masks.h"
#include "muxes.h"
#include "uc.h"
#include "ula.h"

#define MAX 64
#define NUMREG 32
#define MAXTHREADS 15

int memory[MAX];	// memoria RAM

int reg[NUMREG];	// Banco de Registradores

// Registradores Especiais

int PC = 0,			// Registrador PC 
	IR = -1, 		// Registrador IR
	MDR, 			// Registrador MDR
	A, 				// Registrador A
	B, 				// Registrador B
	ALUOUT;			// Registrador ALUOUT


// Variaveis Auxiliares

int controlSignals;		// Sinais de Controle Gerados pela ULA
int opcode;				// Codigo de operacao da instrucao sendo executada
pthread_t tid[MAXTHREADS];	// Ids para as threads criadas

// Argumentos para as threads

mux0_args args0;
mux1_args args1;
mux2_args args2;
mux3_args args3;
mux4_args args4;
ULA_args ULAargs;
UC_ULA_args UCULAargs;
int sigext_args;


int main(void){

	// Criando as threads que representam cada unidade funcional do processador

	// Criando Multiplexadores

	pthread_create(&tid[0],NULL,(void*) &mux0, (void*) &args0);
	pthread_create(&tid[1],NULL,(void*) &mux1, (void*) &args1);
	pthread_create(&tid[2],NULL,(void*) &mux2, (void*) &args2);
	pthread_create(&tid[3],NULL,(void*) &mux3, (void*) &args3);
	pthread_create(&tid[4],NULL,(void*) &mux4, (void*) &args4);
	
	// Criando UC e ULA e UC_ULA e shiftleft
	
	pthread_create(&tid[5],NULL,(void*) &UnidadeControle, (void*) &opcode);
	pthread_create(&tid[6],NULL,(void*) &ULA, (void*) &ULAargs);
	pthread_create(&tid[7],NULL,(void*) &UC_ULA, (void*) &UCULAargs);
	pthread_create(&tid[8],NULL,(void*) &signalextend, (void*) &sigext_args);
	
	// Criando acesso a memoria e acesso ao banco de registradores
	
/*	
    int ciclo, opcode;

    int sc = 0; // Sinais de Controle. Cada bit determina um dos sinais de controle que saem da UC

    while (1) // loop para solicitar o opcode
    {
        printf("Entre com o opcode da instrucao (valor -1 encerra o programa): ");

        scanf("%d", &opcode);
        printf("\n");
        if (opcode == -1) {
            printf("Encerrando o trab2! \n");
            exit(0);
        }
        if (opcode != 35 && opcode != 0 && opcode != 43 ){
            printf("OPCODE NOT VALID! \n");
            continue;
        }

        ciclo = 1; //variavel que indica o ciclo da instrucao

        while (1) // aqui comeca o que seria a execucao da instrucao

        { 	// aqui comeca o que seria um novo ciclo
            // sc representa os sinais de controle vindos da UC

            sc = UnidadeControle(opcode);

            printf("INSTRUCTION = %x, CYCLE = %x, SIGNALS = %x\n", opcode, ciclo, sc);
            // criterio de parada da instrução. Como estamos simulando apenas o comportamento
            // da UC, sem o caminho de dados completo, este IF finalizará a instrução, permitindo
            // fornecer outra para execucao.
            // Determina se estamos no ultimo ciclo de cada instrucao para finalizar a execucao
            // if (lw E ciclo 5) OU
            // ((Tipo-R OU sw OU addi OU andi) E ciclo 4) OU
            // ((beq OU j OU jal OU jr OU jalr OU bne) E ciclo 3) OU
            // (esta tentando usar mais que 5 ciclos para a instrucao)
            //
            //                      então finaliza o loop da instrucao
            //
            if (    ((opcode == 35) && ciclo == 5) ||
                    ((opcode == 0 || opcode == 43)  && ciclo == 4)){
                break; // ultimo ciclo da instrução. Termina este loop interno para pedir outro opcode ao usuario.
            }

            ciclo++; // incrementa variavel que indica o ciclo da instrucao
            // aqui termina um ciclo
 
       } // fim da execução da instrução

    } // fim do loop para solicitar o opcode
*/	
	
	return 0;
}