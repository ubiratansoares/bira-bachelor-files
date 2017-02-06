// para compilar: mpicc slave.c -o slave -Wall
// para rodar: sera inciado pelo mestre com MPI_Comm_spawn

/*
!! a simple/Master slave (dynamically spawnd) examplewith 
!!  MPI_Comm_spawn, MPI_Comm_get_parent
!!  both Master and slave executables must be build, see Master  

*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"mpi.h"

#define N 3

int
main(int argc, char **argv)
{
        int             tag = 1;
        int             my_rank;
        int             num_proc;
        char            message_0[50];
        char            message_1[]="hello master" ;
	char            master_data[50];
	char            slaves_data[50];

	int 		i, vet[N], buf_rcv;

        MPI_Status      status;
        MPI_Comm        inter_comm; 

	MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
        MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

        MPI_Comm_get_parent(&inter_comm);
							/* the master gets a message from the
							parent (Mater) and broadcasts it to the 
							other processes (slave) */
	if ( my_rank == 0 )				
	  {

	    MPI_Recv(message_0, 50, MPI_CHAR, 0, tag, inter_comm, &status);
	    printf("SLAVE (master) : message received : %s\n", message_0);
 
	    MPI_Send(message_1, 50, MPI_CHAR,0 , tag, inter_comm);

	    MPI_Recv(master_data, 50, MPI_CHAR, 0, tag, inter_comm, &status);
	    printf("SLAVE (master) : master data received : %s\n", master_data);
 
	    strcpy(slaves_data, master_data);
	    
	  }
	
	MPI_Bcast(slaves_data, 50, MPI_CHAR, 0, MPI_COMM_WORLD);
	
	printf("SLAVE - %d : slaves data  received from Bcast : %s\n", my_rank, slaves_data);

	MPI_Scatter(vet, 1, MPI_INT, &buf_rcv, 1, MPI_INT, 0, inter_comm);

	for (i = 0; i < N; i++) 
	{
		printf("SLAVE Scatter %d: vet[ %d ] = %d, buf_rcv = %d \n", my_rank, i, vet[i], buf_rcv);
	}
	MPI_Gather(&buf_rcv, 1, MPI_INT, vet, 1, MPI_INT, 0, inter_comm);

	MPI_Finalize();
	exit(0);
}
