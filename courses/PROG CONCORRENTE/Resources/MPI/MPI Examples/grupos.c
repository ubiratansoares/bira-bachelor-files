/*
Author: André Fernandes - 06/06/2010

This program uses the definition of groups to divide the processes in 2 groups: pair and odd.
Then, it creates a communicator for each group and prints as output the size them

*/


#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {

	int myid;
	int numprocs;

	int tam_grupo_mundo,     //armazena tamanho dos respectivos grupos
	    tam_grupo1, 
	    tam_grupo2; 

	int *vet_grupo1,	 //armazena os 'ranks' dos processos pertencentes aos respectivos grupos
	    *vet_grupo2; 

	MPI_Group grupo_mundo,   //declara os grupos
		  grupo1, 
		  grupo2; 

	MPI_Comm comunicador1,   //declara os 2 comunicadores
	         comunicador2;   

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	
	//Aloca os vetores
	vet_grupo1=malloc(sizeof(int)*((numprocs/2)+1));
	vet_grupo2=malloc(sizeof(int)*((numprocs/2)+1));

	int i;
	int j=0; //controle na insercao dos ranks no vetor par (grupo1)
	int k=0; //controle na insercao dos ranks no vetor impar (grupo2)

	//separa os processos com rank par e impar, adicionando-os nos respectivos vetores
	for (i=0; i<numprocs; i++) { 
		if (i%2==0) { //eh par -> adiciona no vetor do grupo 1
			vet_grupo1[j++]=i;		
		}
		else { //eh impar -> add no vetor do grupo 2
			vet_grupo2[k++]=i;		
		}
	}

	//armazena em "grupo_mundo" o grupo relacionado ao comunicador "MPI_COMM_WORLD"
	MPI_Comm_group(MPI_COMM_WORLD, &grupo_mundo);

	//Gera o grupo "grupo1", a partir do grupo "grupo_mundo", com tamanho "j", onde os processos(ranks) estao em vet_grupo1  
	MPI_Group_incl(grupo_mundo, j, vet_grupo1, &grupo1);
	//Gera o grupo "grupo2", a partir do grupo "grupo_mundo", com tamanho "k", onde os processos(ranks) estao em vet_grupo2  
	MPI_Group_incl(grupo_mundo, k, vet_grupo2, &grupo2);

	//cria um novo comunicador "comunicador1" para o grupo "grupo1" (subconjunto do grupo do comunicador "MPI_COM_WORLD")
	MPI_Comm_create(MPI_COMM_WORLD, grupo1, &comunicador1); 
	//cria um novo comunicador "comunicador2" para o grupo "grupo2" (subconjunto do grupo do comunicador "MPI_COM_WORLD")
	MPI_Comm_create(MPI_COMM_WORLD, grupo2, &comunicador2); 


	if (myid==0) { //somente os processos pares tem acesso ao comunicador1 (pares) - no caso, o processo 0 imprime o tam do grupo par
		printf("Tamanho grupo_mundo: %d \n", numprocs); //imprime o tamanho do mundo - impares+pares
		//armazena em 'tam_grupo1' o tamanho do grupo1 relacionado ao comunicador1
		MPI_Comm_size(comunicador1, &tam_grupo1);
		printf ("Tamanho grupo1 (pares): %d \n", tam_grupo1);

	}

	if (myid==1) { //somente os processos impares tem acesso ao comunicador2 (impares) - no caso, o processo 1 imprime o tam do grupo impar
		//armazena em 'tam_grupo2' o tamanho do grupo2 relacionado ao comunicador2
		MPI_Comm_size(comunicador2, &tam_grupo2);
		printf ("Tamanho grupo2 (impares): %d \n", tam_grupo2);
	}

	MPI_Finalize();
	return 0;
}









