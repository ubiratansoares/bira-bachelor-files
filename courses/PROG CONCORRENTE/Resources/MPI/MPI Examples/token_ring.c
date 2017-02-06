/*
Token Ring !
Autor: André Fernandes - 06/06/10

Descricao: 

Os N processos devem formar uma token-ring lógica, onde o processo (zero) dispara um token que é
repassado para o processo 1. Todos os processos i (i > 0) recebem o token do processo i-1 e o
repassam para o processo i+1. A aplicação acaba quando o processo 0 recebe novamente o token.

Deve-se modificar o valor da constante Nprocs, a qual define o n° N de processos da token ring 


Lebrem-se, para compilar: mpicc token_ring.c -o token_ring
Para rodar: mpirun -np "Nprocs" ./token_ring
*/

#include <mpi.h>
#include <stdio.h>

#define count 1 	// nro de unidades do tipo de dado enviado - usado em "send" e "receive"
#define TAG 0	 	// rótulo da mensagem - usado em "send" e "receive"

#define NProcs 5 	// ******* AQUI determina-se o n° de processos do token ring *********

int main(int argc, char *argv[]) {

	int numprocs; 	//recebe o argumento nro de processos

	int token; 	//token a ser enviado/recebido

	int myid;  	//armazena o rank do processo

	int i;

	MPI_Status stat; 

	MPI_Init(&argc,&argv); 			// inicializacao do ambiente MPI

	MPI_Comm_size(MPI_COMM_WORLD,&numprocs); // determina o nro de processos que pertencem ao mundo MPI

	MPI_Comm_rank(MPI_COMM_WORLD,&myid); 	//retorna o rank do processo

       //mensagem de erro personalizada qdo argumento n° de processos é diferente do necessario 
	if (numprocs!= NProcs) { 
	    printf("O argumento nro de processos deve ser igual a %d. \n", NProcs);
	    MPI_Finalize();
	    return 0;
	} 

	if (myid == 0) {	//se o processo for o mestre (rank 0 ) dispara o token
		puts("Entre com o token (inteiro) a ser enviado: ");
		scanf("%d", &token);
		MPI_Send(&token, count, MPI_INT, (myid+1), TAG, MPI_COMM_WORLD);	 	//envia mensagem para o myid+1
		token=0;  //zera token para checar se o valor recebido eh o mesmo que foi enviado
		MPI_Recv(&token, count, MPI_INT, (NProcs-1), TAG, MPI_COMM_WORLD, &stat);	//espera receber do processo N para finalizar a operação
		printf("Sou o mestre (0) e recebi o token %d do processo %d \n", token, (NProcs-1)); 
	}
	else {
		for (i=1;i<NProcs-1;i++) { //determina o rank/id do processo, recebendo do anterior e enviando o token ao proximo
			if (myid==i) {
				MPI_Recv(&token, count, MPI_INT, (myid-1) , TAG, MPI_COMM_WORLD, &stat);	//recebe o token do processo myid-1	
				printf("Sou o processo -%d- recebi o token -%d- do processo %d. \n", myid, token, myid-1);
				MPI_Send(&token, count, MPI_INT, (myid+1), TAG, MPI_COMM_WORLD);		 //envia mensagem para o myid+1	
			}
		}
		if (myid==(NProcs-1)) { //se for o ultimo processo, envia token para o primeiro
			MPI_Recv(&token, count, MPI_INT, (myid-1) , TAG, MPI_COMM_WORLD, &stat);	//recebe o token do processo myid-1
			printf("Sou o processo -%d- recebi o token -%d- do processo %d. \n", myid, token, myid-1);
			MPI_Send(&token, count, MPI_INT, 0, TAG, MPI_COMM_WORLD);		 //envia mensagem para o mestre
		}
	}

	MPI_Finalize(); /* MPI Programs end with MPI Finalize; this is a weak synchronization point */

	return 0;
	
}
