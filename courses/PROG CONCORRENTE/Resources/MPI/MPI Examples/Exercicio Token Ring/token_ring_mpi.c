//Token ring especificado em aula (esperado no minimo 3 processos, senao nao funciona)
#include <stdio.h>
#include "mpi.h"

#define NMAX 30

int main(int argc,char **argv) {

	int size, rank, worldrank, i, dado=0, npar=1, nimp=1;
	int vetpar[NMAX], vetimp[NMAX];
	
	MPI_Group gpar, gimp, gworld;
	MPI_Comm comm_par, comm_imp;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);	
	MPI_Comm_rank(MPI_COMM_WORLD, &worldrank);
	
	//printf("Processo %d iniciando\n", worldrank);
	
	MPI_Comm_group(MPI_COMM_WORLD, &gworld);

	vetimp[0] = 0; vetpar[0] = 0;
	for(i=1;i<size;i++)
		if (i%2)
			vetimp[nimp++] = i;
		else
			vetpar[npar++] = i;
			
	//printf("Processo %d criando grupos\n", worldrank);
	MPI_Group_incl(gworld, nimp, vetimp, &gimp);
	MPI_Group_incl(gworld, npar, vetpar, &gpar);

	//printf("Processo %d criando os comunicadores\n", worldrank);
	MPI_Comm_create(MPI_COMM_WORLD, gimp, &comm_imp);
	MPI_Comm_create(MPI_COMM_WORLD, gpar, &comm_par);
	
	if (worldrank == 0) {
	
		/*	
			int MPI_Send(
			  void *buf,
			  int count,
			  MPI_Datatype datatype,
			  int dest,
			  int tag,
			  MPI_Comm comm
			);
		*/
		dado++;
		printf("Processo %d enviando o dado %d para %d\n", worldrank, dado, 2);
		MPI_Send(&dado, 1, MPI_INT, 1, 1, comm_par); //envia para o 2
		printf("Processo %d enviou o dado %d para %d\n", worldrank, dado, 2);
		
		MPI_Comm_size(comm_par, &size); //para determinar o ultimo par
		
		/*
			int MPI_Recv(
			  void *buf,
			  int count,
			  MPI_Datatype datatype,
			  int source,
			  int tag,
			  MPI_Comm comm,
			  MPI_Status *status
			);
		*/

		printf("Processo %d esperando o dado de %d\n", worldrank, size-1);
		MPI_Recv(&dado, 1, MPI_INT, size-1, 1, comm_par, &status); //recebe do ultimo par
		printf("Processo %d recebeu o dado %d de %d\n", worldrank, dado, size-1);
		
		MPI_Comm_size(comm_imp, &size); //para determinar o ultimo impar
		
		dado++;
		printf("Processo %d enviando o dado %d para %d\n", worldrank, dado, size-1);
		MPI_Send(&dado, 1, MPI_INT, size-1, 1, comm_imp); //envia para o ultimo impar
		printf("Processo %d enviou o dado %d para %d\n", worldrank, dado, size-1);
		
		printf("Processo %d esperando o dado de %d\n", worldrank, 1);
		MPI_Recv(&dado, 1, MPI_INT, 1, 1, comm_imp, &status); //recebe do primeiro impar
		printf("Processo %d recebeu o dado %d de %d\n", worldrank, dado, 1);
	
	} else if (worldrank%2 == 0) { //pares
	
		MPI_Comm_rank(comm_par, &rank);
		MPI_Comm_size(comm_par, &size); //para determinar o tamanho do comunicador

		printf("Processo %d esperando o dado de %d\n", worldrank, rank-1);
		MPI_Recv(&dado, 1, MPI_INT, rank-1, 1, comm_par, &status); //recebe do anterior (do comm_par)
		printf("Processo %d recebeu o dado %d de %d\n", worldrank, dado, rank-1);
		
		dado++;
		printf("Processo %d enviando o dado %d para %d\n", worldrank, dado, rank-1);
		MPI_Send(&dado, 1, MPI_INT, (rank+1)%size, 1, comm_par); //passa para o próximo (do comm_par)
		printf("Processo %d enviou o dado %d para %d\n", worldrank, dado, rank-1);
	
	} else { //impares
	
		MPI_Comm_rank(comm_imp, &rank);
		MPI_Comm_size(comm_imp, &size); //para determinar o tamanho do comunicador

		printf("Processo %d esperando o dado de %d\n", worldrank, (rank+1)%size);
		MPI_Recv(&dado, 1, MPI_INT, (rank+1)%size, 1, comm_imp, &status); //recebe do proximo (do comm_imp)
		printf("Processo %d recebeu o dado %d de %d\n", worldrank, dado, (rank+1)%size);
		
		dado++;
		printf("Processo %d enviando o dado %d para %d\n", worldrank, dado, rank-1);
		MPI_Send(&dado, 1, MPI_INT, rank-1, 1, comm_imp); //passa para o anterior (do comm_imp)
		printf("Processo %d enviou o dado %d para %d\n", worldrank, dado, rank-1);
	
	}

	//printf("Processo %d finalizando\n", worldrank);

	MPI_Finalize();

	return(0);

}

