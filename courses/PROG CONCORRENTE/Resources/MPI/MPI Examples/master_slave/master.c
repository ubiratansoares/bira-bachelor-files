// para compilar: mpicc master.c -o master -Wall
// para rodar: mpirun -np 1 master
/*
!! a simple Master/slave (dynamically spawnd) example with 
!!  MPI_Comm_spawn, MPI_Comm_get_parent
!!  both Master and slave executables must be build, see slave
*/
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include"mpi.h"

#define N 3

int
main(int argc, char **argv)
{

       int             tag = 1;
       int             my_rank;
       int             num_proc;
       char            message_0[] = "hello slave, i'm your master";
	char            message_1[50];
	char            master_data[] = "slaves to work";
       char 		  slave[20];
	int             array_of_errcodes[10];

	int 		  i, vet[N], buf_rcv, vet_master[N];

        MPI_Status      status;
        MPI_Comm        inter_comm;

	strcpy(slave,"slave") ;
	
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
        MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

	for (i = 0; i < N; i++) 
	{
		vet[ i ] = i;
	}

	printf("MASTER : spawning 3 slaves ... \n");
							/* spawn slave and send it a message */  
	MPI_Comm_spawn(slave, MPI_ARGV_NULL, N, MPI_INFO_NULL,0, MPI_COMM_WORLD, &inter_comm, array_of_errcodes);
	
	printf("MASTER : send a message to master of slaves ...\n");
	MPI_Send(message_0, 50, MPI_CHAR,0 , tag, inter_comm);

	MPI_Recv(message_1, 50, MPI_CHAR, 0, tag, inter_comm, &status);
	printf("MASTER : message received : %s\n", message_1);

	MPI_Send(master_data, 50, MPI_CHAR,0 , tag, inter_comm);

	MPI_Scatter(vet, 1, MPI_INT, &buf_rcv, 1, MPI_INT, MPI_ROOT, inter_comm);

	MPI_Gather(vet, 1, MPI_INT, &vet_master, 1, MPI_INT, MPI_ROOT, inter_comm);
	for (i = 0; i < N; i++) 
	{
		printf("MASTER: vet[ %d ] = %d, buf_rcv = %d, vet_master[ %d ] = %d \n", i, vet[i], buf_rcv, i, vet_master[i]);
	}

        MPI_Finalize();
	exit(0);
}
