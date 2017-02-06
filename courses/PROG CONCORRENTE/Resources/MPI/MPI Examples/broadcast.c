/*
Author: André Fernandes - 60/06/2010

Using Broadcast

This simple program tests the routine especified above

It reads an integer , broadcasts this number to other process, wich printfs the value received

*/
#include <mpi.h>
#include <stdio.h>

#define count 1 	// nro de unidades do tipo de dado enviado - usado em "send" e "receive"
#define TAG 0	 	// rótulo da mensagem - usado em "send" e "receive"

#define NProcs 10 	// ******* AQUI determina-se o n° de processos  *********

int main(int argc, char *argv[]) {

	int numprocs; 	//recebe o argumento nro de processos

	int dado=0; 	//token a ser enviado/recebido

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


	if (myid == 0) {	//se o processo for o mestre (rank 0 ) realiza o broadcast
		puts("Entre com o dado (inteiro) a ser enviado por broadcast: ");
		scanf("%d", &dado);
	}
	
	MPI_Bcast(&dado, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	for (i=1;i<NProcs;i++) { //determina o rank/id do processo, recebendo do anterior e enviando o token ao proximo
		if (myid==i) {
			printf("Sou o processo -%d- recebi o dado -%d- por broadcast. \n", myid, dado);
		}
	}

	MPI_Finalize(); /* MPI Programs end with MPI Finalize; this is a weak synchronization point */

	return 0;
	
}
