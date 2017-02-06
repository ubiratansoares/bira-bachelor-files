//para compilar: mpicc teste_mpi.c -o teste_mpi
//para rodar: mpirun -np N teste_mpi ->onde N é o número de processos

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "mpi.h"

int main(int argc, char *argv[]){
	
	int rank, size;
	char text_send1[10], text_send2[10], text_recv1[10], text_recv2[10];
	MPI_Status status;
	

	strcpy(text_send1, "goodbye");
	strcpy(text_send2, "hello");
	
	MPI_Init(&argc, &argv);
	
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		//MPI_Comm_size(MPI_COMM_WORLD, &size);
		//printf("Hello World from process %d of %d\n", rank, size);
		
		if(rank == 0){
			MPI_Ssend(text_send1, 10, MPI_CHAR, 1, 21, MPI_COMM_WORLD);
			printf("[Process %d] and I say...\n", rank);
			MPI_Recv(text_recv2, 10, MPI_CHAR, 1, 21, MPI_COMM_WORLD, &status);
			printf("[Process %d] ...%s!\n", rank, text_recv2);
		}
		
		else 
			if(rank == 1){
				printf("[Process %d] You say...\n", rank);
				MPI_Recv(text_recv1, 10, MPI_CHAR, 0, 21, MPI_COMM_WORLD, &status);
				printf("[Process %d] ...%s!\n", rank, text_recv1);
				MPI_Ssend(text_send2, 10, MPI_CHAR, 0, 21, MPI_COMM_WORLD);
			}
		
		else
			printf("[Process %d] I don't know this song.\n", rank);
		
	MPI_Finalize();
	return 0;
}