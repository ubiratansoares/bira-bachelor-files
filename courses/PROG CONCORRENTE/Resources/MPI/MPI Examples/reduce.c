/*
Author: André Fernandes - 60/06/2010

Using "MPI_Reduce"

This simple program tests the routine especified above

It reads an integer "n", broadcasts this number to other process and each one makes an simple operation, where the results are reduced and stored in variable "result" to be showed by the process 0 

*/
#include <mpi.h>
#include <math.h>
#include <stdio.h>

int main (int argc, char *argv[ ]) {

	int n, i;
	int myid;
	int numprocs;
	double my_result=0.0;
	double result=2;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	
	if (myid == 0) {
	    printf ("Entre com o número n: \n");
	    scanf("%d", &n);
	}
	
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	

	for (i=0; i<n; i++) { //para cada i em n
		my_result=my_result+1;	//realiza uma soma
	}

	printf("Sou o processo -%d- meu resultado -%f- \n", myid, my_result); //mostra o resultado do calculo do processo especificado

	
	//Armazena na variavel "result" do processo 0, a soma de "my_result" de todos os processos. MPI_SUM -> soma 
	MPI_Reduce(&my_result, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (myid == 0) {
	    printf ("Resultado: %.5f \n", result);
	}

	MPI_Finalize( );
}



